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

typedef enum IL_Capability
{
    IL_Capability_Unknown,
    IL_Capability_Unavailable,
    IL_Capability_Available,
    IL_Capability_Enabled,

    IL_Capability_Count,
} IL_Capability;

#ifdef __cplusplus

namespace IL
{
    enum class Capability
    {
        Unknown         = ::IL_Capability_Unknown,
        Unavailable     = ::IL_Capability_Unavailable,
        Available       = ::IL_Capability_Available,
        Enabled         = ::IL_Capability_Enabled     
    };

    // ordering from least to most capabable is intentional and should be preserved
}

IL_API extern bool operator==(const IL_Capability& lhs, const IL::Capability& rhs);
IL_API bool operator==(const IL::Capability& lhs, const IL_Capability& rhs);

#endif