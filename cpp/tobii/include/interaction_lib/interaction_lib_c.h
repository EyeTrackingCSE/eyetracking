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

#ifndef interaction_lib_c_h_included
#define interaction_lib_c_h_included

#include <interaction_lib/api/typedefs.h>
#include <interaction_lib/api/Result.h>
#include <interaction_lib/api/Logger.h>
#include <interaction_lib/api/Adapter.h>
#include <interaction_lib/datatypes/WeightDistributionData.h>
#include <interaction_lib/datatypes/Validity.h>
#include <interaction_lib/datatypes/PresenceData.h>
#include <interaction_lib/datatypes/StreamInputType.h>
#include <interaction_lib/datatypes/TrackingStatus.h>
#include <interaction_lib/datatypes/InteractionEventTypes.h>
#include <interaction_lib/datatypes/GazePointData.h>
#include <interaction_lib/datatypes/GazeOriginData.h>
#include <interaction_lib/datatypes/HeadPoseData.h>
#include <interaction_lib/datatypes/InteractionEventTypes.h>
#include <interaction_lib/datatypes/ResponsiveGazePointData.h>
#include <interaction_lib/datatypes/StreamingDataEventTypes.h>
#include <interaction_lib/datatypes/Capability.h>
#include <interaction_lib/datatypes/StreamType.h>
#include <interaction_lib/datatypes/FieldOfUse.h>
#include <interaction_lib/math/Rectangle.h>


#ifdef WIN32
#ifndef IL_C_API
#define IL_C_API __declspec(dllexport)
#endif
#else 
#define IL_C_API
#endif 

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup intlib Interaction Library
///
/// C interface to the features exposed by the Interaction Library.
///
/// @{
///

/// A "handle" type that identifies an actual underlying Interaction Library implementation instance.
typedef struct IL_Context_
{
    /// Internal to the Interaction Library C binding implementation.
    void* int_lib;
} IL_Context;

/// Creates an Interaction Library instance with a default adapter, if available.
///
/// @see         IL_SetAdapter()
/// @param       fieldOfUse  This mandatory flag denotes if the Interaction Library is used for interactive or 
///                          analytical purposes. Please see the EULA and documentation for more information.
/// @param[out]  context     Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return                  IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_CreateInteractionLib(IL_Context** context, IL_FieldOfUse fieldOfUse);

/// Creates an Interaction Library instance with a default adapter, if available.
///
/// @see         IL_SetAdapter()
/// @param       fieldOfUse  This mandatory flag denotes if the Interaction Library is used for interactive or 
///                          analytical purposes. Please see the EULA and documentation for more information.
/// @param       logger      Optional pointer to a structure with information about a callback function
///                          that will be called by the Interaction Library for logging purposes.
///                          If @p logger is @c NULL, the Interaction Library does no logging related processing.
///                          The callback function is set in the @c callback field of the @IL_Logger structure.
///                          The @p level parameter denotes the highest log level at which the callback function will be called. 
///                          For example, if "info" is specified, then the callback will be called for "error", "warning" and "info", 
///                          but not for "debug" and "trace".
///                          The @p text parameter passed to the callback function points at a null terminated character array
///                          formatted in a format specified by Tobii. The parameter is only valid in the scope of the callback.
///                          The @p context parameter passed to the callback function is set in the @c context field of the @IL_Logger structure.
/// @param[out]  context     Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return                  IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_CreateInteractionLibWithLogger(IL_Context** context, IL_FieldOfUse fieldOfUse, IL_Logger* logger);

/// Creates an Interaction Library instance with a custom adapter.
///
/// @see         IL_SetAdapter()
/// @param       fieldOfUse        This mandatory flag denotes if the Interaction Library is used for interactive or 
///                                analytical purposes. Please see the EULA and documentation for more information.
/// @param       logger            Optional pointer to a structure with information about a callback function
///                                that will be called by the Interaction Library for logging purposes.
///                                If @p logger is @c NULL, the Interaction Library does no logging related processing.
///                                The callback function is set in the @c callback field of the @IL_Logger structure.
///                                The @p level parameter denotes the highest log level at which the callback function will be called. 
///                                For example, if "info" is specified, then the callback will be called for "error", "warning" and "info", 
///                                but not for "debug" and "trace".
///                                The @p text parameter passed to the callback function points at a null terminated character array
///                                formatted in a format specified by Tobii. The parameter is only valid in the scope of the callback.
///                                The @p context parameter passed to the callback function is set in the @c context field of the @IL_Logger structure.
/// @param       adapterFunctions  Pointer to an IL_AdapterFunctions structure defining the custom adapter.
///                                @c NULL is a valid parameter value which means the default adapter will not be set.
///                                Without a valid adapter the following functions will do nothing and return Result_Warning_NoAdapterSet:
///                                will return IL_Result_Error_NoAdapterSet: IL_Update(), IL_UpdateStreamingDataOutput(),
///                                IL_UpdateInteraction(), IL_SubscribeGazePointData(), IL_SubscribeGazeOriginData(),
///                                IL_SubscribePresenceData(), and IL_SubscribeHeadPoseData(). However, it is still possible to
///                                add interactors and subscribe to gaze focus events.
/// @param[out]  context           Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return                        IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_CreateInteractionLibWithAdapter(IL_Context** context, IL_FieldOfUse fieldOfUse, IL_Logger* logger, IL_AdapterFunctions* adapterFunctions);

/// Destroys an Interaction Library instance.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_DestroyInteractionLib(IL_Context* context);

/// Sets a custom adapter, or resets the current adapter.
///
/// @param  context           Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  adapterFunctions  Pointer to an IL_AdapterFunctions structure defining the custom adapter.
///                           @c NULL is a valid parameter value, with will cause the current adapter to be removed.
///                           Without a valid adapter the following functions will do nothing and return Result_Warning_NoAdapterSet:
///                           will return IL_Result_Error_NoAdapterSet: IL_Update(), IL_UpdateStreamingDataOutput(),
///                           IL_UpdateInteraction(), IL_SubscribeGazePointData(), IL_SubscribeGazeOriginData(),
///                           IL_SubscribePresenceData(), and IL_SubscribeHeadPoseData(). However, it is still possible to
///                           add interactors and subscribe to gaze focus events.
/// @return                   IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_SetAdapter(IL_Context* context, IL_AdapterFunctions* adapterFunctions);



/// Adds a display area (usually a screen) to the coordinate transform setup.
/// Typically call this method multiple times to add information on dpi scaling and location of all screens in the 
/// system (see sample/separate documention for details).
/// 
/// If you are in a single screen environment, or a defacto single screen environment (see above), or use other 
/// (external) mechanisms to handle multiple displays areas, you may call this only once and then omit (set to zero/null) all 
/// parameters except the first two.
/// 
/// If you are unable to determine this information in your environment, please use 
/// CoordinateTransformSetDisplayDensity() instead (see below).
/// 
/// @note If you are using a UI binding, it will usually take care of setting up all display areas.
/// 
/// @param  context               Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  widthUserUnits        The width  in your preferred units (eg. with dpi scaling in your environment, or other preferred units) 
/// @param  heightUserUnits       The height in your preferred units (eg. with dpi scaling in your environment, or other preferred units)
/// @param  widthVirtualUnits     The width  in system virtual screen units (or custom units that you use for system display setup)
/// @param  heightVirtualUnits    The height in system virtual screen units (or custom units that you use for system display setup)
/// @param  xOriginVirtualUnits   The display origin x coordinate in the system virtual screen coordinate system (or your custom space for system display setup)
/// @param  yOriginVirtualUnits   The display origin y coordinate in the system virtual screen coordinate system (or your custom space for system display setup)
/// @param  virtualDisplayAreaId  The virtual display id as a null terminated string (eg. on Windows this is a number)
/// @return                       IL_Result_Ok if the operation succeeded; otherwise an IL_Result warning or error code.
IL_C_API IL_Result IL_CoordinateTransformAddOrUpdateDisplayArea(IL_Context* context,
                                                                float widthUserUnits, float heightUserUnits, 
                                                                float widthVirtualUnits, float heightVirtualUnits, 
                                                                float xOriginVirtualUnits, float yOriginVirtualUnits, 
                                                                const char* virtualDisplayAreaId);


/// Sets your origin position, as it is in the virtual coordinate system.
/// Typically used to set the window position to use window local coordinates for streams and interactors, 
/// or one could set the display center to use a centered, left-handed coordinate system.
/// 
/// @note This command is optional. Not setting this will assume system virtual screen coordinates are being used for 
/// all on-display coordinates (or the custom coordinate system units that you used for system display setup).
///
/// @param  context                    Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  xOriginOffsetVirtualUnits  The x offset in the system virtual screen coordinate system (or your custom space)
/// @param  yOriginOffsetVirtualUnits  The y offset in the system virtual screen coordinate system (or your custom space)
/// @return                            IL_Result_Ok if the operation succeeded; otherwise an IL_Result warning or error code.
IL_C_API IL_Result IL_CoordinateTransformSetOriginOffset(IL_Context* context, float xOriginOffsetVirtualUnits, float yOriginOffsetVirtualUnits);


/// An alternative to IL_CoordinateTransformAddOrUpdateDisplayArea(), normally only used in special circumstances.
///
/// This method exists as an alternative or addition to IL_CoordinateTransformAddOrUpdateDisplayArea(). It is mainly 
/// intended to be used in edge cases where you are in a sandboxed environment and cannot access information about 
/// displays, or if a specialized adapter is used that you cannot access or modify for some reason and 
/// IL_CoordinateTransformAddOrUpdateDisplayArea() is insufficient to solve the dpi scaling problem. You can see if you 
/// need to use IL_CoordinateTransformSetDisplayDensity() by checking the return value from 
/// IL_CoordinateTransformAddOrUpdateDisplayArea(). In almost all situations, using only 
/// IL_CoordinateTransformAddOrUpdateDisplayArea() is sufficient.
/// 
/// @note Only to be used in special circumstances!
/// 
/// @param  context      Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  xUnitsPerMm  The number of horizontal units per mm.
/// @param  yUnitsPerMm  The number of vertical units per mm.
/// @return              IL_Result_Ok if the operation succeeded; otherwise an IL_Result warning or error code.
IL_C_API IL_Result IL_CoordinateTransformSetDisplayDensity(IL_Context* context, float xUnitsPerMm, float yUnitsPerMm);



/// Begins an interactor update transaction, in which many add/update/remove/clear calls are batched.
///
/// @note            Must be called before any add/update/remove/clear calls.
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_BeginInteractorUpdates(IL_Context* context);

/// Commits all batched interactor add/update/remove/clear calls done since IL_BeginInteractorUpdates() was called.
///
/// @note            Must be called for any interactor or interactor weights modifications to have effect.
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_CommitInteractorUpdates(IL_Context* context);

/// Adds an interactor or updates an existing one, specifying its @p bounds and @p z value (the value increases towards the user).
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  id
/// @param  bounds
/// @param  z
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_AddOrUpdateInteractor(IL_Context* context, IL_InteractorId id, IL_Rectangle bounds, float z);

/// Updates the @p bounds for an existing interactor.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  id
/// @param  bounds
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_UpdateInteractorBounds(IL_Context* context, IL_InteractorId id, IL_Rectangle bounds);

/// Updates the @p z value for an existing interactor (the value increases towards the user).
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  id
/// @param  z
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_UpdateInteractorZ(IL_Context* context, IL_InteractorId id, float z);

/// Removes an existing interactor and all its related data.
///
/// @note            Custom weight distribution bitmaps are not removed, since they may be shared between interactors.
///                  However, the *binding* between the interactor and the custom weight distribution bitmaps are removed.
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  id
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_RemoveInteractor(IL_Context* context, IL_InteractorId id);

/// Removes all existing interactors.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_ClearInteractors(IL_Context* context);

/// Sets the built-in standard weight distribution of an interactor, e.g., center weighted or flat.
///
/// @param  context                  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  id
/// @param  weightDistributionTypes
/// @return                          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_UpdateInteractorStandardWeightDistributionTypes(IL_Context* context, IL_InteractorId id, IL_WeightDistributionType weightDistributionTypes);

/// Adds a new custom weight distribution bitmap or updates an existing one.
///
/// @param  context               Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  weightDistributionId
/// @param  isWeightDistribution
/// @param  isStencilMap
/// @param  bitmapColumns
/// @param  bitmapRows
/// @param  bitmap
/// @return                       IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_AddOrUpdateCustomWeightDistributionBitmap(IL_Context* context, IL_WeightDistributionId weightDistributionId, bool isWeightDistribution, bool isStencilMap, int bitmapColumns, int bitmapRows, const unsigned char * bitmap);

/// Removes an existing custom weight distribution bitmap.
///
/// @param  context               Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  weightDistributionId
/// @return                       IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_RemoveCustomWeightDistributionBitmap(IL_Context* context, IL_WeightDistributionId weightDistributionId);

/// Adds a new custom weight distribution bitmap and binds it to an existing interactor, or changes an existing binding.
///
/// @param  context               Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  id
/// @param  weightDistributionId
/// @return                       IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_AddOrUpdateBindingToCustomWeightDistribution(IL_Context* context, IL_InteractorId id, IL_WeightDistributionId weightDistributionId);

/// Removes an existing binding to a custom weight distribution bitmap for the specified interactor.
///
/// @param  context               Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  id
/// @param  weightDistributionId
/// @return                       IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_RemoveBindingToCustomWeightDistribution(IL_Context* context, IL_InteractorId id, IL_WeightDistributionId weightDistributionId);



/// Subscribes to interactor gaze focus events.
///
/// @param  context          Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  callback
/// @param  callbackContext  Will be passed to the callback when the callback is invoked.
/// @return                  IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_SubscribeGazeFocusEvents(IL_Context* context, IL_GazeFocusEventCallback callback, void* callbackContext);

/// Unsubscribes from interactor gaze focus events.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_UnsubscribeGazeFocusEvents(IL_Context* context);



/// Subscribes to gaze point data.
///
/// @param  context          Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  callback
/// @param  callbackContext  Will be passed to the callback when the callback is invoked.
/// @return                  IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_SubscribeGazePointData(IL_Context* context, IL_GazePointDataCallback callback, void* callbackContext);

/// Subscribes to gaze origin data.
///
/// @param  context          Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  callback
/// @param  callbackContext  Will be passed to the callback when the callback is invoked.
/// @return                  IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_SubscribeGazeOriginData(IL_Context* context, IL_GazeOriginDataCallback callback, void* callbackContext);

/// Subscribes to presence data.
///
/// @param  context          Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  callback
/// @param  callbackContext  Will be passed to the callback when the callback is invoked.
/// @return                  IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_SubscribePresenceData(IL_Context* context, IL_PresenceDataCallback callback, void* callbackContext);

/// Subscribes to head pose data.
///
/// @param  context          Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  callback
/// @param  callbackContext  Will be passed to the callback when the callback is invoked.
/// @return                  IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_SubscribeHeadPoseData(IL_Context* context, IL_HeadPoseDataCallback callback, void* callbackContext);

/// Unsubscribes from gaze point data.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_UnsubscribeGazePointData(IL_Context* context);

/// Unsubscribes from gaze origin data.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_UnsubscribeGazeOriginData(IL_Context* context);

/// Unsubscribes from presence data.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_UnsubscribePresenceData(IL_Context* context);

/// Unsubscribes from head pose data.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_UnsubscribeHeadPoseData(IL_Context* context);



/// Get information on the current system capabilities for a data stream.
/// 
/// Data stream availability is dependent on if a device is present, what type of device it is and current system state 
/// of the device being used. Data streams are typically not enabled unless the they are subscribed to, either internally 
/// by the Interaction Library or from this api. 
/// 
/// @param  context         Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  streamType      The stream type to get capability information on.
/// @param[out] capability  Will be set to the system capability for the stream type.
/// @return                 IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_GetDataStreamCapability(IL_Context* context, IL_StreamType streamType, IL_Capability* capability);

/// This callback will be triggered on IL_UpdateStreamingDataOutput()/IL_Update()/IL_WaitAndUpdate() if capabilities have changed.
/// Querying capabilites with GetDataStreamCapability(...) on this callback is allowed.
/// 
/// @param  context          Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @param  callback
/// @param  callbackContext  Will be passed to the callback when the callback is invoked.
/// @return                  IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_SubscribeDataStreamCapabilitiesChanged(IL_Context* context, IL_DataStreamCapabilitiesChangedCallback callback, void* callbackContext);

/// Unsubscribes from data stream capabilities changed notifications.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Ok If successful; otherwise another IL_Result warning or error code.
IL_C_API IL_Result IL_UnsubscribeDataStreamCapabilitiesChanged(IL_Context* context);



/// Checks/waits for device connection, waits for data, and updates. 
/// 
/// This is a *convenience method* that aggregates all necessary control methods into one, and sleeps the calling thread for 
/// the appropriate amount of time between connection attempts when no device is connected. The recommended connection attempt 
/// interval is one second, passing 0 means no sleep.
/// 
/// If you need granular control over updates and/or non-blocking calls use a suitable combination of the various other control methods.
///
/// @return IL_Result_Ok if all goes well. See the other control methods for information about other possible return codes.
IL_C_API IL_Result IL_WaitAndUpdate(IL_Context* context, unsigned long connectionIntervalMs);

/// Waits for a device to be available and a connection to be established.
///
/// @return IL_Result_Warning_NoDeviceAvailable  if a device can't be connected to (wait time is short but varies with adapter).
/// @return IL_Result_Ok                         if a device is connected to. When a device connection has already been made 
///                                              previously and is still available, it returns IL_Result_Ok immediately.
IL_C_API IL_Result IL_WaitForDeviceConnection(IL_Context* context);

/// Updates the system and receives event callbacks that are called on the calling thread.
///
/// @param  context  Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return          IL_Result_Warning_NoAdapterSet if no adapter is set (see IL_CreateInteractionLib() and IL_SetAdapter()); otherwise another IL_Result code.
IL_C_API IL_Result IL_Update(IL_Context* context);

/// Waits for data from the current device.
///
/// May timeout or hang indefinitely depending on adapter implementation. Waiting is optional but if you do not wait,
/// remember to call Update() or UpdateStreamingDataOutput() at least 10 times per second.
///
/// @param  context                         Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return IL_Result_Warning_NoAdapterSet  if no adapter is set (see IL_CreateInteractionLib() and IL_SetAdapter()); otherwise another IL_Result code.
IL_C_API IL_Result IL_WaitForDeviceDataAvailable(IL_Context* context);

/// Updates stream buffers and process filters only, and publishes results on the calling thread via the registered streaming data callbacks.
///
/// Use *instead of* IL_Update() if you need more granular control or use multiple threads.
///
/// @param  context                       Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return IL_Result_Error_NoAdapterSet  if no adapter is set (see IL_CreateInteractionLib() and IL_SetAdapter()); otherwise another IL_Result code.
IL_C_API IL_Result IL_UpdateStreamingDataOutput(IL_Context* context);

/// Updates interaction buffers, performs interactor selection calculations, and publishes results on calling thread via the registered interaction event callbacks.
///
/// Use *instead of* IL_Update() if you need more granular control or use multiple threads.
///
/// @note                                 IL_UpdateStreamingDataOutput() needs to be called before this call.
/// @param  context                       Pointer to an IL_Context structure used internally to identify an actual underlying Interaction Library implementation.
/// @return IL_Result_Error_NoAdapterSet  if no adapter is set (see IL_CreateInteractionLib() and IL_SetAdapter()); otherwise another IL_Result code.
IL_C_API IL_Result IL_UpdateInteraction(IL_Context* context);

///
/// @}

#ifdef __cplusplus
}
#endif

#endif /* interaction_lib_c_h_included */