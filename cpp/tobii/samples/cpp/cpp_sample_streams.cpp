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

#include <iostream>
#include <interaction_lib/InteractionLib.h>
#include <interaction_lib/misc/InteractionLibPtr.h>

int main()
{
    // create the interaction library
    IL::UniqueInteractionLibPtr intlib(IL::CreateInteractionLib(IL::FieldOfUse::Interactive));

    // assume single screen with size 2560x1440 and use full screen (not window local) coordinates
    constexpr float width  = 2560.0f;
    constexpr float height = 1440.0f;
    constexpr float offset = 0.0f;

    intlib->CoordinateTransformAddOrUpdateDisplayArea(width, height);
    intlib->CoordinateTransformSetOriginOffset(offset, offset);

    // subscribe to gaze point data; print data to stdout when called
    intlib->SubscribeGazePointData([](IL::GazePointData evt, void* context)
    {
        std::cout
            << "x: " << evt.x
            << ", y: " << evt.y
            << ", validity: " << (evt.validity == IL::Validity::Valid ? "valid" : "invalid")
            << ", timestamp: " << evt.timestamp_us << " us"
            << "\n";
    }, nullptr);

    std::cout << "Starting interaction library update loop.\n";

    // setup and maintain device connection, wait for device data between events and 
    // update interaction library to trigger all callbacks, stop after 200 cycles
    constexpr size_t max_cycles = 200;
    size_t cycle = 0;
    while (cycle++ < max_cycles)
    {
        intlib->WaitAndUpdate();
    }
}
