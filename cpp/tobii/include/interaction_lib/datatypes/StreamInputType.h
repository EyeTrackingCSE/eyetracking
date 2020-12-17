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

typedef enum IL_StreamInputType
{
    IL_StreamInputType_GazePointData,
    IL_StreamInputType_GazeOriginData,
    IL_StreamInputType_PresenceData,
    IL_StreamInputType_HeadPoseData,
    IL_StreamInputType_ResponsiveGazePointData,

    IL_StreamInputType_Count,
} IL_StreamInputType;

#ifdef __cplusplus

namespace IL
{
    enum class StreamInputType
    {
        GazePointData           = ::IL_StreamInputType_GazePointData,
        GazeOriginData          = ::IL_StreamInputType_GazeOriginData,
        PresenceData            = ::IL_StreamInputType_PresenceData,
        HeadPoseData            = ::IL_StreamInputType_HeadPoseData,
        ResponsiveGazePointData = ::IL_StreamInputType_ResponsiveGazePointData,
    };
}

IL_API extern bool operator==(const IL_StreamInputType& lhs, const IL::StreamInputType& rhs);
IL_API bool operator==(const IL::StreamInputType& lhs, const IL_StreamInputType& rhs);

#endif