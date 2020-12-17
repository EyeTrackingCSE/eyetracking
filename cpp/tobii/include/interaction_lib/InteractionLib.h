/*
COPYRIGHT 2018-2020  - PROPERTY OF TOBII AB
-------------------------------------
2018-2020 TOBII AB - KARLSROVAGEN 2D, DANDERYD 182 53, SWEDEN - All Rights Reserved.

NOTICE:  All information contained herein is, and remains, the property of Tobii AB and its suppliers, if any.
The intellectual and technical concepts contained herein are proprietary to Tobii AB and its suppliers and may be
covered by U.S.and Foreign Patents, patent applications, and are protected by trade secret or copyright law.
Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
permission is obtained from Tobii AB.
*/

#pragma once

#include "api/CallSignature.h"
#include "api/Logger.h"
#include "api/Adapter.h"
#include "api/typedefs.h"
#include "api/Result.h"
#include "math/Rectangle.h"
#include "datatypes/InteractionEventTypes.h"
#include "datatypes/StreamingDataEventTypes.h"
#include "datatypes/WeightDistributionData.h"
#include "datatypes/StreamType.h"
#include "datatypes/Capability.h"
#include "datatypes/FieldOfUse.h"


namespace IL
{
    class InteractionLib;

    /// @defgroup initsetup Setup and Teardown
    ///
    /// These factory methods are used to create and destroy an InteractionLib instance.
    ///
    /// For testing purposes, Interaction Library SDK clients can "mock" or "stub" the Interaction Library
    /// functionality by providing their own implementation of the InteractionLib interface. By using
    /// CreateInteractionLib() or its overloads, an actual Interaction Library
    /// implementation instance of the InteractionLib interface will be created.
    /// 
    /// @{
    ///

    /// Creates a new InteractionLib instance with a default adapter, if available.
    ///
    /// @see    InteractionLib::SetAdapter()
    /// 
    /// @param  fieldOfUse  This mandatory flag denotes if the Interaction Library is used for interactive or 
    ///                     analytical purposes. Please see the EULA and documentation for more information.
    /// 
    /// @return Pointer to a new InteractionLib instance. Destroying or deleting this is currently not supported.
    ///         InteractionLib destructor, but only via the DestroyInteractionLib function.
    IL_API InteractionLib* CreateInteractionLib(FieldOfUse fieldOfUse);

    /// Creates a new InteractionLib instance with a default adapter, if available.
    ///
    /// @see    InteractionLib::SetAdapter()
    ///
    /// @param  fieldOfUse  This mandatory flag denotes if the Interaction Library is used for interactive or
    ///                     analytical purposes. Please see the EULA and documentation for more information.
    /// @param  logger      Optional pointer to a structure with information about a callback function
    ///                     that will be called by the Interaction Library for logging purposes.
    ///                     If @p logger is @c nullptr, the Interaction Library does no logging related processing.
    ///                     The callback function is set in the @c IL_Logger::callback field.
    ///                     The @p level parameter denotes the highest log level at which the callback function will be called. 
    ///                     For example, if "info" is specified, then the callback will be called for "error", "warning" and "info", 
    ///                     but not for "debug" and "trace"
    ///                     The @p text parameter passed to the callback function points at a null terminated character array
    ///                     formatted in a format specified by Tobii. The parameter is only valid in the scope of the callback.
    ///                     The @p context parameter passed to the callback function is set in the @c IL_Logger::context field.
    ///
    /// @return Pointer to a new InteractionLib instance. Destroying or deleting this is currently not supported.
    ///         InteractionLib destructor, but only via the DestroyInteractionLib function.
    IL_API InteractionLib* CreateInteractionLib(FieldOfUse fieldOfUse, IL_Logger* logger);
    
    /// Creates a new InteractionLib instance with a custom adapter.
    ///
    /// @see    InteractionLib::SetAdapter()
    /// 
    /// @param  fieldOfUse        This mandatory flag denotes if the Interaction Library is used for interactive or 
    ///                           analytical purposes. Please see the EULA and documentation for more information.
    /// @param  logger            Optional pointer to a structure with information about a callback function
    ///                           that will be called by the Interaction Library for logging purposes.
    ///                           If @p logger is @c nullptr, the Interaction Library does no logging related processing.
    ///                           The callback function is set in the @c IL_Logger::callback field.
    ///                           The @p level parameter denotes the highest log level at which the callback function will be called. 
    ///                           For example, if "info" is specified, then the callback will be called for "error", "warning" and "info", 
    ///                           but not for "debug" and "trace"
    ///                           The @p text parameter passed to the callback function points at a null terminated character array
    ///                           formatted in a format specified by Tobii. The parameter is only valid in the scope of the callback.
    ///                           The @p context parameter passed to the callback function is set in the @c IL_Logger::context field.
    /// @param  adapterFunctions  Pointer to an instance of the structure that defines the custom adapter.
    ///                           @c nullptr is a valid parameter value which means the default adapter will not be set.
    ///                           Without a valid adapter the following functions will do nothing and return Result::Warning_NoAdapterSet.
    ///
    ///                           @link InteractionLib::WaitAndUpdate WaitAndUpdate() @endlink,
    ///                           @link InteractionLib::WaitForDeviceConnection WaitForDeviceConnection() @endlink,
    ///                           @link InteractionLib::Update Update() @endlink,
    ///                           @link InteractionLib::UpdateStreamingDataOutput UpdateStreamingDataOutput() @endlink,
    ///                           @link InteractionLib::UpdateInteraction UpdateInteraction() @endlink,
    ///
    ///                           However, it is still possible to add interactors and subscribe to gaze focus events and data streams.
    /// 
    /// @return Pointer to a new InteractionLib instance. Use DestroyInteractionLib() to destroy it.
    IL_API InteractionLib* CreateInteractionLib(FieldOfUse fieldOfUse, IL_Logger* logger, IL_AdapterFunctions* adapterFunctions);

    /// Destroys an InteractionLib instance created by CreateInteractionLib() or its overloads.
    ///
    /// @param  instance  Pointer to an InteractionLib instance returned by the Interaction Library (as opposed to one
    ///                   created by a client for test purposes). If @p instance is @c nullptr, then nothing is done.
    IL_API void DestroyInteractionLib(InteractionLib* instance);

    ///
    /// @}

    /// Interface to the features exposed by the Interaction Library.
    class InteractionLib
    {
    public:
        /// @name Adapters
        /// 
        /// Adapters are the abstraction layer between the Interaction Library and the eyetracking data provider. 
        /// The Interaction Library ships with a default adapter for Tobii consumer eyetracking devices so setting 
        /// an adapter is NOT required to use the Interaction Library. Setting a custom adapter can however be useful 
        /// e.g. when testing by supplying a "fake" adapter implementation that generates controlled test data.
        ///
        /// @{
        ///

        /// Sets a custom adapter for this InteractionLib instance, or removes the current adapter.
        ///
        /// @param  adapterFunctions  Pointer to an instance of the structure that defines the custom adapter.
        ///                           @c nullptr is a valid parameter value which will cause the current adapter to be removed.
        ///                           Without a valid adapter the following functions will do nothing and return Result::Warning_NoAdapterSet:
        ///                           
        ///                           WaitAndUpdate(), WaitForDeviceConnection(), Update(), UpdateStreamingDataOutput(),
        ///                           UpdateInteraction().
        ///                           
        ///                           However, it is still possible to add interactors and subscribe to gaze focus events and data streams.
        ///
        /// @return                   Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result SetAdapter(IL_AdapterFunctions* adapterFunctions) = 0;

        ///
        /// @}


        /// @name Display Properties and Coordinate Transforms
        ///
        /// In order for the interaction library to function properly, you need to setup the appropriate coordinate system for 
        /// your environment. This coordinate system will be used for all interactor operations, and all display based  
        /// coordinates in data streams.
        /// 
        /// Important:
        /// If you are using a UI binding with the Interaction Library it should set all of these values for you! 
        /// If you are, you can skip this section. If not, or if you are developing a UI binding, read on.
        /// 
        /// For most use cases:
        /// -# Use CoordinateTransformAddOrUpdateDisplayArea() to add dpi scaling and location information on all 
        ///    screens in the system.
        /// -# To use window local coordinates, use CoordinateTransformSetOriginOffset() with your window position (in system 
        ///    virtual screen coordinates).
        /// 
        /// Special cases:
        /// * If you are sure you only have one display you can use an abbreviated call to 
        ///   CoordinateTransformAddOrUpdateDisplayArea() where you don't need any knowledge about the system virtual coordinate 
        ///   system layout or display ids (see below).
        /// * The same applies if you are sure the application will stay on one display, and that display is the one that the 
        ///   tracking device is mounted on (or at least the tracking device exclusively used by the current adapter).
        /// * If you have no means to get information about displays (eg. because you are in sandboxed environment), use 
        ///   CoordinateTransformSetDisplayDensity() instead so that interaction library can perform proper gaze focus 
        ///   calculations.
        /// * If you only use data streams and wish to use adapter native units, you don't have to set any coordinate transform 
        ///   information. Note that the coordinate system being used will then be entirely dependent on the adapter being used.
        /// 
        /// Usage tips:
        /// * If your coordinate system dpi changes, remember to update.
        /// * If CoordinateTransformSetOriginOffset() is used to get window local coordinates, remember to update
        ///   when your window moves (and when dpi scaling changes)
        /// 
        /// @note If no coordinate transform is set, the default coordinate system for the current adapter is used (if available).
        ///
        /// @{
        ///


        /// Adds a display area (usually a screen) to the coordinate transform setup.
        /// Typically call this method multiple times to add information on dpi scaling and location of all screens in the 
        /// system (see sample/separate documentation for details).
        /// 
        /// If you are in a single screen environment, or a defacto single screen environment (see above), or use other 
        /// (external) mechanisms to handle multiple displays areas, you may call this only once and then omit all 
        /// parameters except the first two.
        /// 
        /// If you are unable to determine this information in your environment, please use 
        /// CoordinateTransformSetDisplayDensity() instead (see below).
        /// 
        /// @note If you are using a UI binding, it will usually take care of setting up all display areas.
        /// 
        /// @param  widthUserUnits        The width  in your preferred units (eg. with dpi scaling in your environment, or other preferred units) 
        /// @param  heightUserUnits       The height in your preferred units (eg. with dpi scaling in your environment, or other preferred units)
        /// @param  widthVirtualUnits     The width  in system virtual screen units (or custom units that you use for system display setup)
        /// @param  heightVirtualUnits    The height in system virtual screen units (or custom units that you use for system display setup)
        /// @param  xOriginVirtualUnits   The display origin x coordinate in the system virtual screen coordinate system (or your custom space for system display setup)
        /// @param  yOriginVirtualUnits   The display origin y coordinate in the system virtual screen coordinate system (or your custom space for system display setup)
        /// @param  virtualDisplayAreaId  The virtual display id as a null terminated string (eg. on Windows this is a number)
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result CoordinateTransformAddOrUpdateDisplayArea(float widthUserUnits, float heightUserUnits, 
                                                                 float widthVirtualUnits = 0, float heightVirtualUnits = 0, 
                                                                 float xOriginVirtualUnits = 0, float yOriginVirtualUnits = 0, 
                                                                 const char* virtualDisplayAreaId = nullptr) = 0;


        /// Sets your origin position, as it is in the virtual coordinate system.
        /// Typically used to set the window position to use window local coordinates for streams and interactors, 
        /// or one could set the display center to use a centered, left-handed coordinate system.
        /// 
        /// @note This command is optional. Not setting this will assume system virtual screen coordinates are being used for 
        /// all on-display coordinates (or the custom coordinate system units that you used for system display setup).
        ///
        /// @param  xOriginOffsetVirtualUnits  The x offset in the system virtual screen coordinate system (or your custom space)
        /// @param  yOriginOffsetVirtualUnits  The y offset in the system virtual screen coordinate system (or your custom space)
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result CoordinateTransformSetOriginOffset(float xOriginOffsetVirtualUnits, float yOriginOffsetVirtualUnits) = 0;


        /// An alternative to CoordinateTransformAddOrUpdateDisplayArea(), normally only used in special circumstances.
        ///
        /// This method exists as an alternative or addition to CoordinateTransformAddOrUpdateDisplayArea(). It is mainly 
        /// intended to be used in edge cases where you are in a sandboxed environment and cannot access information about 
        /// displays, or if a specialized adapter is used that you cannot access or modify for some reason and 
        /// CoordinateTransformAddOrUpdateDisplayArea() is insufficient to solve the dpi scaling problem. You can see if you 
        /// need to use CoordinateTransformSetDisplayDensity() by checking the return value from 
        /// CoordinateTransformAddOrUpdateDisplayArea(). In almost all situations, using only 
        /// CoordinateTransformAddOrUpdateDisplayArea() is sufficient.
        /// 
        /// @note Only to be used in special circumstances!
        /// 
        /// @param  xUnitsPerMm  The number of horizontal units per mm.
        /// @param  yUnitsPerMm  The number of vertical units per mm.
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result CoordinateTransformSetDisplayDensity(float xUnitsPerMm, float yUnitsPerMm) = 0;

        ///
        /// @}


        /// @anchor interactors
        /// @name Interactors
        ///
        /// Interactors is what we call regions on screen that can be interacted with using gaze. Registering interactors 
        /// that represent objects (e.g. user interface elements or game objects) will elevate gaze based interaction to be 
        /// object centric rather than pixel/position centric. 
        /// 
        /// The Interaction Library contains specialized algorithms to make an informed decision on which object is being 
        /// interacted with, taking into account factors such as physiological characteristics of eye movements, end user eye 
        /// movement behavior, momentaneous noise and errors in the raw data, relative sizes and positions of interactors 
        /// in space, and many more things.
        /// 
        /// Various interaction events are produced for declared interactors when the user looks around 
        /// (see @ref interactorevents "Interactor Events").
        /// 
        /// Add or update multiple interactors and/or set/change their properties by subsequent calls to
        /// AddOrUpdateInteractor(), UpdateInteractorBounds(), UpdateInteractorZ(),
        /// RemoveInteractor(), and ClearInteractors().
        /// 
        /// Batches of updates are performed between calls to BeginInteractorUpdates() and CommitInteractorUpdates(). 
        /// Calling both of them, and in the correct order, is required (even if updating only a single interactor).
        /// 
        /// Using BeginInteractorUpdates() and CommitInteractorUpdates() is both a performance benefit and an insurance
        /// of transaction safety: All changes take effect at the time of a successful commit, not for each individual
        /// add/update/remove/clear call. Note however that the interactor functions are *not* thread local.
        ///
        /// @{
        ///

        /// Begins an interactor update transaction, in which many add/update/remove/clear calls are batched.
        ///
        /// @note   Must be called before any add/update/remove/clear calls.
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result BeginInteractorUpdates() = 0;

        /// Commits all batched interactor add/update/remove/clear calls done since BeginInteractorUpdates() was called.
        ///
        /// @note   Must be called for any interactor or @ref interactorweights "Interactor Weights" modifications to have effect.
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result CommitInteractorUpdates() = 0;

        /// Adds an interactor or updates an existing one, specifying its @p bounds and @p z value (the value increases towards the user).
        ///
        /// @param  id
        /// @param  bounds
        /// @param  z
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result AddOrUpdateInteractor(InteractorId id, Rectangle bounds, float z) = 0;

        /// Updates the @p bounds of an existing interactor.
        ///
        /// @param  id
        /// @param  bounds
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result UpdateInteractorBounds(InteractorId id, Rectangle bounds) = 0;

        /// Updates the @p z value for an existing interactor (the value increases towards the user).
        ///
        /// @param  id
        /// @param  z
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result UpdateInteractorZ(InteractorId id, float z) = 0;

        /// Removes an existing interactor and all its related data.
        ///
        /// @note   Custom weight distribution bitmaps are not removed, since they may be shared between interactors.
        ///         However, the *binding* between the interactor and the custom weight distribution bitmaps are removed.
        /// @param  id
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result RemoveInteractor(InteractorId id) = 0;

        /// Removes all existing interactors.
        ///
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result ClearInteractors() = 0;

        ///
        /// @}

        /// @anchor interactorweights
        /// @name Interactor Weight Distributions
        ///
        /// Weight distributions can be used to describe the visual attention properties of an interactor. As an example, 
        /// UI buttons tend to have their visual content in the center, so they benefit from being center weighted.
        /// 
        /// **Standard Weight Distributions**
        ///
        /// Standard weight distribution types are built into Interaction Library and can be used to specify, for example,
        /// center weighted or flat distributions. Use UpdateInteractorStandardWeightDistributionTypes() to change these.
        ///  
        /// Setting a flat weight distribution type removes any previous weight distribution settings.
        ///
        /// **Custom Weight Distributions**
        ///
        /// Custom weight distributions can be used for three purposes:
        ///
        /// * To specify non-rectangular shapes for interactors - use as a stencil map (isStencilMap = true).
        /// * To declare a custom weight distribution (isWeightDistribution = true).
        /// * A combination of the above.
        /// 
        /// For stencil maps, values are checked for 0 or >0, with 0 being transparent.
        /// 
        /// For weight distributions, values are in the range 0 to 255, with 0 still obscuring elements behind but not
        /// adding weight to selection of focused interactors. A value of 1 denotes normal weight.
        /// 
        /// A combination of both work like weight distributions but with 0 being transparent (like in stencil maps).
        /// 
        /// Custom weight distribution bitmaps can be bound to (used for) multiple interactors. They are stretched to fit 
        /// the bounds of the bound interactor. You can add a maximum of one stencil map and one weight distribution per 
        /// interactor (or one combined).
        /// 
        /// @note Custom weight distributions are *not* copied. They need to be kept alive by you (and at the same memory 
        /// location)!
        /// 
        /// @{
        /// 
        
        /// Sets the built-in standard weight distribution of an interactor, e.g., center weighted or flat.
        ///
        /// @param  id
        /// @param  weightDistributionTypes
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result UpdateInteractorStandardWeightDistributionTypes(InteractorId id, WeightDistributionType weightDistributionTypes) = 0;

        /// Adds a new custom weight distribution bitmap or updates an existing one.
        ///
        /// @param  weightDistributionId
        /// @param  isWeightDistribution
        /// @param  isStencilMap
        /// @param  bitmapColumns
        /// @param  bitmapRows
        /// @param  bitmap
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result AddOrUpdateCustomWeightDistributionBitmap(WeightDistributionId weightDistributionId, bool isWeightDistribution, bool isStencilMap, int bitmapColumns, int bitmapRows, const unsigned char * bitmap) = 0;

        /// Removes an existing custom weight distribution bitmap.
        ///
        /// @param  weightDistributionId
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result RemoveCustomWeightDistributionBitmap(WeightDistributionId weightDistributionId) = 0;

        /// Binds a (already added) custom weight distribution to an (already added) interactor, or changes an existing binding.
        ///
        /// @param  id
        /// @param  weightDistributionId
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result AddOrUpdateBindingToCustomWeightDistribution(InteractorId id, WeightDistributionId weightDistributionId) = 0;

        /// Removes a binding to a custom weight distribution bitmap for the specified interactor.
        ///
        /// @param  id
        /// @param  weightDistributionId
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result RemoveBindingToCustomWeightDistribution(InteractorId id, WeightDistributionId weightDistributionId) = 0;

        ///
        /// @}

        /// @anchor interactorevents
        /// @name Interactor Events
        ///
        /// Gaze focus events are generated when the user looks at an interactor, as well as when
        /// the user's gaze leaves the interactor. Leave events are also produced if the user physically
        /// leaves his position in front of the tracker or the tracking stops, etc.
        ///
        /// @{
        ///

        /// Subscribes to interactor gaze focus events.
        ///
        /// @param callback
        /// @param context Will be passed to the callback when the callback is invoked.
        /// @return Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result SubscribeGazeFocusEvents(GazeFocusEventCallback callback, void* context) = 0;

        /// Unsubscribes from interactor gaze focus events.
        virtual void UnsubscribeGazeFocusEvents() = 0;

        ///
        /// @}

        /// @name Data Streams
        /// 
        /// Various device data streams are exposed in the Interaction Library.
        /// 
        /// The data streams can be subscribed to and unsubscribed from, at will, as long as only
        /// one callback is registered per data stream. Callbacks are made on the thread
        /// calling Update() or UpdateStreamingDataOutput().
        /// 
        /// @note Coordinates for data on the display are in the user specified coordinate system. Coordinates for other data 
        /// streams are in the original adapter coordinate system.
        ///
        /// @{
        ///

        /// Subscribes to gaze point data.
        ///
        /// @param callback
        /// @param context    Will be passed to the callback when the callback is invoked.
        /// @return           Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result SubscribeGazePointData(GazePointDataCallback callback, void* context) = 0;

        /// Subscribes to gaze origin data.
        ///
        /// @param callback
        /// @param context    Will be passed to the callback when the callback is invoked.
        /// @return           Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result SubscribeGazeOriginData(GazeOriginDataCallback callback, void* context) = 0;

        /// Subscribes to presence data.
        ///
        /// @param callback
        /// @param context    Will be passed to the callback when the callback is invoked.
        /// @return           Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result SubscribePresenceData(PresenceDataCallback callback, void* context) = 0;

        /// Subscribes to head pose data.
        ///
        /// @param callback
        /// @param context    Will be passed to the callback when the callback is invoked.
        /// @return           Result::Ok if the operation succeeded; otherwise a @link IL::Result Result @endlink warning or error code.
        virtual Result SubscribeHeadPoseData(HeadPoseDataCallback callback, void* context) = 0;

        /// Unsubscribes from gaze point data.
        virtual void UnsubscribeGazePointData() = 0;

        /// Unsubscribes from gaze origin data.
        virtual void UnsubscribeGazeOriginData() = 0;

        /// Unsubscribes from presence data.
        virtual void UnsubscribePresenceData() = 0;

        /// Unsubscribes from head pose data.
        virtual void UnsubscribeHeadPoseData() = 0;
        

        /// Get information on the current system capabilities for a data stream.
        /// 
        /// Data stream availability is dependent on if a device is present, what type of device it is and current system state 
        /// of the device being used. Data streams are typically not enabled unless the they are subscribed to, either internally 
        /// by the Interaction Library or from this api. 
        /// 
        /// @param streamType       The stream type to get capability information on.
        /// @param[out] capability  Will be set to the system capability for the stream type.
        virtual void GetDataStreamCapability(StreamType streamType, Capability* capability) = 0;

        /// This callback will be triggered on UpdateStreamingDataOutput()/Update()/WaitAndUpdate() if capabilities have changed.
        /// Querying capabilities with GetDataStreamCapability(...) on this callback is allowed.
        /// 
        /// @param callback
        /// @param context    Will be passed to the callback when the callback is invoked.
        virtual void SubscribeDataStreamCapabilitiesChanged(DataStreamCapabilitiesChangedCallback callback, void* context) = 0;

        /// Unsubscribes from data stream capabilities changed notifications.
        virtual void UnsubscribeDataStreamCapabilitiesChanged() = 0;

        ///
        /// @}

        /// @name Control
        ///
        /// Various ways of controlling the Interaction Library processing flow.
        ///
        /// Since the Interaction Library does not have any threads by itself, periodic update calls are needed to collect
        /// data from the device, perform all calculations, select the currently focused interactor, produce suitable
        /// interaction events and trigger any callbacks including for data streams.
        ///  
        /// WaitForDeviceConnection() is used to connect to the hardware data providers. It needs to be called until a connection 
        /// is established for any streams to work and any interaction events to be generated through the Update() calls below. 
        /// It is also needed for reconnection if device connections fail.
        /// 
        /// The Update() method performs all update operations while the UpdateStreamingDataOutput() and 
        /// UpdateInteraction() methods can be used *instead of* Update() if more fine-grained control is needed (eg if you need 
        /// data streams at native frame rates but interaction events less often).
        /// 
        /// WaitForDeviceDataAvailable() can be used to block until new device data is available. This is optional.
        /// 
        /// @note If WaitForDeviceDataAvailable() is not used, remember to call Update() or UpdateStreamingDataOutput() at least
        /// 10 times per second to ensure device connections are maintained and proper events are triggered.
        ///
        /// @{
        /// 

        /// Checks/waits for device connection, waits for data, and updates. 
        /// 
        /// This is a *convenience method* that aggregates all necessary control methods into one, and sleeps the calling thread for 
        /// the appropriate amount of time between connection attempts when no device is connected. The default connection attempt 
        /// interval is one second, passing 0 means no sleep.
        /// 
        /// If you need granular control over updates and/or non-blocking calls use a suitable combination of the various other control methods.
        ///
        /// @return Result::Ok if all goes well. See the other control methods for information about other possible return codes.
        virtual Result WaitAndUpdate(unsigned long connectionIntervalMs = 1000) = 0;


        /// Waits for a device to be available and a connection to be established.
        ///
        /// @return Result::Warning_NoDeviceAvailable if a device can't be connected to (wait time is short but varies with adapter).
        /// @return Result::Ok                        if a device is connected to. When a device connection has already been made 
        ///                                           previously and is still available, it returns Result::Ok immediately.
        virtual Result WaitForDeviceConnection() = 0;

        /// Updates the system and receives event callbacks that are called on the calling thread.
        ///
        /// @return Result::Warning_NoAdapterSet if no adapter is set (see CreateInteractionLib() and SetAdapter()); otherwise another @link IL::Result Result @endlink code.
        virtual Result Update() = 0;

        /// Waits for data from the current device.
        ///
        /// May timeout or hang indefinitely depending on adapter implementation. Waiting is optional but if you do not wait,
        /// remember to call Update() or UpdateStreamingDataOutput() at least 10 times per second. Do call Update() (or some combination 
        /// of the other update methods) after waiting regardless of return code to make sure all proper events are generated.
        ///
        /// @return Result::Warning_NoAdapterSet if no adapter is set (see CreateInteractionLib() and SetAdapter()); otherwise another @link IL::Result Result @endlink code.
        virtual Result WaitForDeviceDataAvailable() = 0;

        /// Updates stream buffers and process filters only, and publishes results on the calling thread via the registered streaming data callbacks.
        ///
        /// Use *instead of* Update() if you need more granular control or use multiple threads.
        ///
        /// @return Result::ErrorNoAdapterSet if no adapter is set (see CreateInteractionLib() and SetAdapter()); otherwise another @link IL::Result Result @endlink code.
        virtual Result UpdateStreamingDataOutput() = 0;

        /// Updates interaction buffers, performs interactor selection calculations, and publishes results on calling thread via the registered interaction event callbacks.
        ///
        /// Use *instead of* Update() if you need more granular control or use multiple threads.
        ///
        /// @note UpdateStreamingDataOutput() needs to be called before this call.
        /// @return Result::ErrorNoAdapterSet if no adapter is set (see CreateInteractionLib() and SetAdapter()); otherwise another @link IL::Result Result @endlink code.
        virtual Result UpdateInteraction() = 0;

        ///
        /// @}
    
    protected:
        ~InteractionLib() = default;
    };
}
