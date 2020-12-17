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

#ifdef __cplusplus
#include <cstdbool>
#else
#include <stdbool.h>
#endif

#include "../datatypes/StreamInputType.h"

#include "../datatypes/GazePointData.h"
#include "../datatypes/GazeOriginData.h"
#include "../datatypes/PresenceData.h"
#include "../datatypes/HeadPoseData.h"
#include "../datatypes/ResponsiveGazePointData.h"

#include "../datatypes/TrackingStatus.h"
#include "../datatypes/StreamCoordinateTransformInfo.h"
#include "../datatypes/DeviceNotification.h"
#include "../datatypes/FieldOfUse.h"
#include "../datatypes/LoggingLevel.h"

/// @defgroup adapter Adapter
///
/// The abstraction layer between the Interaction Library and the eyetracking data provider.
///
/// There could be adapter implementations that use real Tobii eyetracking devices and data they provide, or
/// "fake" implementations that generate test data, or even combinations thereof.
///
/// @{
///

/// @name Functions Called by the Adapter
///
/// Typedefs for pointers to functions that the adapter will call.
/// They must be implemented in the adapter.
///
/// @{
///

typedef void(*ILAdapter_BeginTransactionCallback)(void* context);
typedef void(*ILAdapter_CommitTransactionCallback)(void* context);
typedef void(*ILAdapter_GetStreamTypesProvidedCallback)(const IL_StreamInputType*, unsigned count, void* context);
typedef void(*ILAdapter_GetStreamCoordinateTransformInfoCallback)(const IL_StreamCoordinateTransformInfo*, unsigned count, void* context);

typedef void(*ILAdapter_PostGazePointDataCallback) (const IL_GazePointData*, void* context);
typedef void(*ILAdapter_PostGazeOriginDataCallback)(const IL_GazeOriginData*, void* context);
typedef void(*ILAdapter_PostPresenceDataCallback)  (const IL_PresenceData*, void* context);
typedef void(*ILAdapter_PostHeadPoseDataCallback)  (const IL_HeadPoseData*, void* context);
typedef void(*ILAdapter_PostResponsiveGazePointDataCallback)   (const IL_ResponsiveGazePointData*, void* context);

typedef void(*ILAdapter_TrackingStatusCallback)(IL_TrackingStatus, void* context);
typedef void(*ILAdapter_DeviceNotificationCallback)(IL_DeviceNotification, void* context);

typedef void(*ILAdapter_LoggingCallback)(IL_LoggingLevel level, const char* message, const char* tag1, const char* tag2, const char* formattedMetadata, void* context);
typedef void(*ILAdapter_ForwardLoggingStatementCallback)(IL_LoggingLevel level, const char* forwardedLoggingStatement, void* context);

///
/// @}

/// @name Functions Called by InteractionLib
///
/// Typedefs for pointers to functions that the Interaction Library will call.
/// They must be implemented in the adapter.
///
/// @{
///

/// Pointer to an adapter-specific implementation detail context.
typedef void* ILAdapter_AdapterContext;

/// Lightweight creation/instantiation of the adapter should happen on this call. All heavy, I/O or IPC operations 
/// should be done on ILAdapter_StartAdapter.
/// 
/// @param createDestroyContext  When Interaction Library calls #ILAdapter_CreateAdapter this parameter will be set 
///                              to whatever is in the originally passed IL_AdapterFunctions.createDestroyContext.
/// @param context[out]          Before calling #ILAdapter_CreateAdapter, the caller must set the value pointed
///                              at by the parameter @p context to @c nullptr. The adapter implementation should
///                              set the out parameter @p context (dereferenced) and return true on success.
/// @return                      true on success, otherwise false.
typedef bool(*ILAdapter_CreateAdapter)(void* createDestroyContext, ILAdapter_AdapterContext* context);

/// Lightweight destroy of the adapter should happen on this call. All heavy, I/O or IPC operations 
/// needed to destroy the adapter should be done on ILAdapter_StopAdapter.
/// 
/// @param createDestroyContext  When Interaction Library calls #ILAdapter_DestroyAdapter this parameter will be set 
///                              to whatever is in the originally passed IL_AdapterFunctions.createDestroyContext.
/// @param context[in,out]       The adapter implementation should set the out parameter @p context (dereferenced)
///                              to @c nullptr after destroying the adapter.
typedef void(*ILAdapter_DestroyAdapter)(void* createDestroyContext, ILAdapter_AdapterContext* context);

/// The adapter implementation should generate IL_TrackingStatus_Ok on #ILAdapter_TrackingStatusCallback when invoked.
/// 
/// @param fieldOfUse  Will be the value that was used when the Interaction Library was created.
typedef void(*ILAdapter_StartAdapter)(ILAdapter_AdapterContext, IL_FieldOfUse fieldOfUse);

/// The adapter implementation should generate IL_TrackingStatus_NotAvailable on #ILAdapter_TrackingStatusCallback when invoked.
typedef void(*ILAdapter_StopAdapter)(ILAdapter_AdapterContext);

/// The adapter implementation should here generate calls for gaze point data, gaze origin data, etc., on registered callbacks. 
/// Expected to be consistent with what is reported by #ILAdapter_GetStreamTypesProvided and #ILAdapter_GetStreamTypesProvidedCallback.
typedef void(*ILAdapter_UpdateAdapter)(ILAdapter_AdapterContext);

/// The adapter implementation should block until a device connection is available.
typedef bool(*ILAdapter_WaitForDeviceConnection)(ILAdapter_AdapterContext);

/// The adapter implementation should block until any streaming data is available.
typedef bool(*ILAdapter_WaitForStreamingDataAvailable)(ILAdapter_AdapterContext);

/// The adapter implementation should invoke #ILAdapter_GetStreamTypesProvidedCallback.
typedef void(*ILAdapter_GetStreamTypesProvided)(ILAdapter_AdapterContext, ILAdapter_GetStreamTypesProvidedCallback, void* context);

/// The adapter implementation should invoke #ILAdapter_GetStreamCoordinateTransformInfoCallback.
/// The IL_StreamCoordinateTransformInfo struct and its contents will be copied at the receiving end when the 
/// ILAdapter_GetStreamCoordinateTransformInfoCallback callback is invoked and does not need to be kept alive afterwards.
typedef void(*ILAdapter_GetStreamCoordinateTransformInfo)(ILAdapter_AdapterContext, ILAdapter_GetStreamCoordinateTransformInfoCallback, void* context);

///
/// @}

/// @name Callback Registration Functions Called by InteractionLib
///
/// Typedefs for pointers to callback registration functions that the Interaction Library will call.
/// The adapter implementation should call the registered callbacks in its #ILAdapter_UpdateAdapter function.
///
/// All invocations of stream callbacks are expected to be surrounded by calls to #ILAdapter_BeginTransactionCallback and 
/// #ILAdapter_CommitTransactionCallback in #ILAdapter_UpdateAdapter.
///
/// @{
///

typedef void(*ILAdapter_RegisterBeginTransactionCallback)           (ILAdapter_AdapterContext, ILAdapter_BeginTransactionCallback, void* context);
typedef void(*ILAdapter_RegisterCommitTransactionCallback)          (ILAdapter_AdapterContext, ILAdapter_BeginTransactionCallback, void* context);
typedef bool(*ILAdapter_RegisterPostGazePointDataCallback)          (ILAdapter_AdapterContext, ILAdapter_PostGazePointDataCallback, void* context);
typedef bool(*ILAdapter_RegisterPostGazeOriginDataCallback)         (ILAdapter_AdapterContext, ILAdapter_PostGazeOriginDataCallback, void* context);
typedef bool(*ILAdapter_RegisterPostPresenceDataCallback)           (ILAdapter_AdapterContext, ILAdapter_PostPresenceDataCallback, void* context);
typedef bool(*ILAdapter_RegisterPostHeadPoseDataCallback)           (ILAdapter_AdapterContext, ILAdapter_PostHeadPoseDataCallback, void* context);
typedef bool(*ILAdapter_RegisterPostResponsiveGazePointDataCallback)(ILAdapter_AdapterContext, ILAdapter_PostResponsiveGazePointDataCallback, void* context);

/// #ILAdapter_TrackingStatusCallback is expected to be invoked by (at least) #ILAdapter_StartAdapter and #ILAdapter_StopAdapter.
typedef bool(*ILAdapter_RegisterTrackingStatusCallback)             (ILAdapter_AdapterContext, ILAdapter_TrackingStatusCallback, void* context);

typedef bool(*ILAdapter_RegisterDeviceNotificationCallback)         (ILAdapter_AdapterContext, ILAdapter_DeviceNotificationCallback, void* context);

typedef bool(*ILAdapter_RegisterLoggingCallback)                    (ILAdapter_AdapterContext, ILAdapter_LoggingCallback, void* context);
typedef bool(*ILAdapter_RegisterForwardLoggingStatementCallback)    (ILAdapter_AdapterContext, ILAdapter_ForwardLoggingStatementCallback, void* context);

///
/// @}

typedef struct IL_AdapterFunctions
{
    void* createDestroyContext;

    ILAdapter_CreateAdapter createAdapter;
    ILAdapter_DestroyAdapter destroyAdapter;

    ILAdapter_StartAdapter startAdapter;
    ILAdapter_StopAdapter stopAdapter;
    ILAdapter_UpdateAdapter updateAdapter;

    ILAdapter_WaitForDeviceConnection waitForDeviceConnection;
    ILAdapter_WaitForStreamingDataAvailable waitForStreamingDataAvailable;
    ILAdapter_GetStreamTypesProvided getStreamTypesProvided;
    ILAdapter_GetStreamCoordinateTransformInfo getStreamCoordinateTransformInfo;

    ILAdapter_RegisterBeginTransactionCallback registerBeginTransactionCallback;
    ILAdapter_RegisterCommitTransactionCallback registerCommitTransactionCallback;

    ILAdapter_RegisterPostGazePointDataCallback registerPostGazePointDataCallback;
    ILAdapter_RegisterPostGazeOriginDataCallback registerPostGazeOriginDataCallback;
    ILAdapter_RegisterPostPresenceDataCallback registerPostPresenceDataCallback;
    ILAdapter_RegisterPostHeadPoseDataCallback registerPostHeadPoseDataCallback;
    ILAdapter_RegisterPostResponsiveGazePointDataCallback registerPostResponsiveGazePointDataCallback;
                                                          
    ILAdapter_RegisterTrackingStatusCallback registerTrackingStatusCallback;
    ILAdapter_RegisterDeviceNotificationCallback registerDeviceNotificationCallback;

    ILAdapter_RegisterLoggingCallback registerLoggingCallback;
    ILAdapter_RegisterForwardLoggingStatementCallback registerForwardLoggingStatementCallback;

} IL_AdapterFunctions;

///
/// @}

