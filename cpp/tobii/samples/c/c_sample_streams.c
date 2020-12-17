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

#include <stdio.h>
#include <inttypes.h> // for PRId64 and timestamps
#include <interaction_lib/interaction_lib_c.h>

// this callback is used for the gaze point data subscription below
void gaze_point_data_callback(IL_GazePointData evt, void* context)
{
    printf("x: %f, y: %f, validity: %s, timestamp: %" PRId64 " us\n",
           evt.x,
           evt.y,
           evt.validity == IL_Validity_Valid ? "valid" : "invalid",
           evt.timestamp_us);
}

int main(int argc, char* argv[])
{
    // create the interaction library
    IL_Context* intlib = NULL;
    IL_CreateInteractionLib(&intlib, IL_FieldOfUse_Interactive);

    // assume single screen with size 2560x1440 and use full screen (not window local) coordinates
    const float width  = 2560.0f;
    const float height = 1440.0f;
    const float offset = 0.0f;

    IL_CoordinateTransformAddOrUpdateDisplayArea(intlib, width, height, 0, 0, 0, 0, NULL);
    IL_CoordinateTransformSetOriginOffset(intlib, offset, offset);

    // subscribe to gaze point data; print data to stdout using the callback defined above
    IL_SubscribeGazePointData(intlib, gaze_point_data_callback, NULL);

    printf("Starting interaction library update loop.\n");

    // setup and maintain device connection, wait for device data between events and 
    // update interaction library to trigger all callbacks, stop after 200 cycles
    const size_t max_cycles = 200;
    size_t cycle = 0;
    while (cycle++ < max_cycles)
    {
        IL_WaitAndUpdate(intlib, 1000);
    }

    // cleanup
    IL_DestroyInteractionLib(intlib);

    return 0;
}