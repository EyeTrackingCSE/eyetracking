/*
COPYRIGHT 2020 - PROPERTY OF TOBII AB
-------------------------------------
2020 TOBII AB - KARLSROVAGEN 2D, DANDERYD 182 53, SWEDEN - All Rights Reserved.

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

typedef enum IL_LoggingLevel
{
    IL_LoggingLevel_Error,
    IL_LoggingLevel_Warn,
    IL_LoggingLevel_Info,
    IL_LoggingLevel_Debug,
    IL_LoggingLevel_Trace
} IL_LoggingLevel;

#ifdef __cplusplus

namespace IL
{
    enum class LoggingLevel
    {
        Error = ::IL_LoggingLevel_Error,
        Warn  = ::IL_LoggingLevel_Warn,
        Info  = ::IL_LoggingLevel_Info,
        Debug = ::IL_LoggingLevel_Debug,
        Trace = ::IL_LoggingLevel_Trace
    };
}

IL_API extern bool operator==(const IL_LoggingLevel& lhs, const IL::LoggingLevel& rhs);
IL_API bool operator==(const IL::LoggingLevel& lhs, const IL_LoggingLevel& rhs);

IL_API extern bool operator<(const IL_LoggingLevel& lhs, const IL::LoggingLevel& rhs);
IL_API bool operator<(const IL::LoggingLevel& lhs, const IL_LoggingLevel& rhs);

#endif
