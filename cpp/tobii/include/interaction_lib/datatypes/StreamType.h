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
#include "../api/CallSignature.h"
#endif

typedef enum IL_StreamType
{
    IL_StreamType_GazePointData,
    IL_StreamType_GazeOriginData,
    IL_StreamType_PresenceData,
    IL_StreamType_HeadPoseData,
    IL_StreamType_ResponsiveGazePointData,
} IL_StreamType;

#ifdef __cplusplus

namespace IL
{
    enum class StreamType
    {
        GazePointData           = ::IL_StreamType_GazePointData,
        GazeOriginData          = ::IL_StreamType_GazeOriginData,
        PresenceData            = ::IL_StreamType_PresenceData,
        HeadPoseData            = ::IL_StreamType_HeadPoseData,
        ResponsiveGazePointData = ::IL_StreamType_ResponsiveGazePointData,
    };
}

IL_API extern bool operator==(const IL_StreamType& lhs, const IL::StreamType& rhs);
IL_API bool operator==(const IL::StreamType& lhs, const IL_StreamType& rhs);

#endif