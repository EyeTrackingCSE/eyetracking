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

#include "../datatypes/LoggingLevel.h"

#ifdef __cplusplus
#include "../api/CallSignature.h"
#endif

/// @defgroup logger Logger
///
/// The logging abstraction layer between Interaction Library and its client.
///
/// @{
///

/// Prototype of the callback function that Interaction Library calls when a log entry is available.
///
/// @param level   The parameter denotes the highest log level at which the callback function will be called.
///                For example, if "info" is specified, then the callback will be called for "error", "warning" and "info",
///                but not for "debug" and "trace"
/// @param text    Pointer to a null terminated character array formatted in a format specified by Tobii.
///                The parameter is only valid in the scope of the callback.
/// @param context The context provided by the client in the IL_Logger::context field.
typedef void(*IL_Logger_Callback)(IL_LoggingLevel level, const char* text, void* context);



/// A structure with information about a callback function that will be called by the Interaction Library
/// for logging purposes.
typedef struct IL_Logger
{
    /// The callback function that is called when a log entry is available.
    IL_Logger_Callback callback;

    // The highest log level at which the callback function will be called.
    IL_LoggingLevel level;

    /// The context that is passed to the callback function.
    void* context;
} IL_Logger;

///
/// @}
