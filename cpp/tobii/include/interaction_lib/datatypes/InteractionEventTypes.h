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

#include "../api/typedefs.h" // Fixed for C-Binding

typedef struct IL_GazeFocusEvent
{
    IL_Timestamp timestamp_us;
    IL_InteractorId id;
    bool hasFocus;
} IL_GazeFocusEvent;

typedef void(*IL_GazeFocusEventCallback)(IL_GazeFocusEvent, void*);

#ifdef __cplusplus
namespace IL
{
    using GazeFocusEvent = ::IL_GazeFocusEvent;
    
    template <typename T>
    using InteractionEventCallback = void(*)(T, void*);

    using GazeFocusEventCallback = InteractionEventCallback<GazeFocusEvent>;
}

bool operator==(const IL::GazeFocusEvent& lhs, const IL::GazeFocusEvent& rhs);
#endif
