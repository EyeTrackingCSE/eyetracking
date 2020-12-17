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

#include "../api/typedefs.h" // Fixed for C-Binding
#include "Validity.h" // Fixed for C-Binding

typedef struct IL_GazeOriginData
{
    IL_Timestamp timestamp_us;
    IL_Validity leftValidity;
    float left_xyz[3];
    IL_Validity rightValidity;
    float right_xyz[3];    
} IL_GazeOriginData;

#ifdef __cplusplus
namespace IL
{
    using GazeOriginData = ::IL_GazeOriginData;

    template <>
    const GazeOriginData& Empty<GazeOriginData>();
}

bool operator==(const IL::GazeOriginData& lhs, const IL::GazeOriginData& rhs);
#endif
