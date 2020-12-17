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

#include "../api/typedefs.h"
#include "GazePointData.h"
#include "GazeOriginData.h"
#include "PresenceData.h"
#include "HeadPoseData.h"
#include "ResponsiveGazePointData.h"

typedef void(*IL_GazePointDataCallback)(IL_GazePointData, void*);
typedef void(*IL_GazeOriginDataCallback)(IL_GazeOriginData, void*);
typedef void(*IL_PresenceDataCallback)(IL_PresenceData, void*);
typedef void(*IL_HeadPoseDataCallback)(IL_HeadPoseData, void*);
typedef void(*IL_ResponsiveGazePointDataCallback)(IL_ResponsiveGazePointData, void*);
typedef void(*IL_DataStreamCapabilitiesChangedCallback)(void*);


#ifdef __cplusplus
namespace IL
{
    template <typename T>
    using StreamingDataCallback = void(*)(T, void*);

    using GazePointDataCallback           = StreamingDataCallback<GazePointData>;
    using GazeOriginDataCallback          = StreamingDataCallback<GazeOriginData>;
    using PresenceDataCallback            = StreamingDataCallback<PresenceData>;
    using HeadPoseDataCallback            = StreamingDataCallback<HeadPoseData>;
    using ResponsiveGazePointDataCallback = StreamingDataCallback<ResponsiveGazePointData>;

    using DataStreamCapabilitiesChangedCallback = void(*)(void*);
} // IL
#endif
