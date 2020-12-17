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

typedef enum IL_Validity
{
    IL_Validity_Invalid,
    IL_Validity_Valid
} IL_Validity;

#ifdef __cplusplus

namespace IL
{
    enum class Validity
    {
        Invalid = ::IL_Validity_Invalid,
        Valid   = ::IL_Validity_Valid
    };
}

IL_API extern bool operator==(const IL_Validity& lhs, const IL::Validity& rhs);
IL_API bool operator==(const IL::Validity& lhs, const IL_Validity& rhs);

#endif
