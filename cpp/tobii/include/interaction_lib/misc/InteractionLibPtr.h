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

#include <memory>
#include "../InteractionLib.h"

namespace IL
{

struct InteractionLibDeleter final
{
    void operator()(InteractionLib* instance)
    {
        DestroyInteractionLib(instance);
    }
};

using UniqueInteractionLibPtr = std::unique_ptr<InteractionLib, InteractionLibDeleter>;

class ScopedInteractionLibPtr final
{
public:
    ScopedInteractionLibPtr(InteractionLib* instance) : m_instance(instance) {}

    ScopedInteractionLibPtr() = delete;
    ScopedInteractionLibPtr(const ScopedInteractionLibPtr&) = delete;
    ScopedInteractionLibPtr(ScopedInteractionLibPtr&&) = delete;
    ScopedInteractionLibPtr& operator=(const ScopedInteractionLibPtr&) = delete;
    ScopedInteractionLibPtr& operator=(ScopedInteractionLibPtr&&) = delete;

    ~ScopedInteractionLibPtr() { DestroyInteractionLib(m_instance); }

    InteractionLib* operator->() { return m_instance; }
    InteractionLib& operator*() { return *m_instance; }

private:
    InteractionLib* m_instance;
};

}