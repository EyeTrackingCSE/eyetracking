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

// this struct is used to maintain a focus count (see below)
typedef struct focus_context
{
    IL_InteractorId id;
    size_t          count;
} focus_context;

// this callback is used for the gaze focus event subscription below
// print event data to std out when called and count the number of consecutive focus events
void gaze_focus_callback(IL_GazeFocusEvent evt, void* context)
{
    printf("Interactor: %" PRIu64 ", focused: %s, timestamp: %" PRId64 " us\n",
           evt.id,
           evt.hasFocus ? "true" : "false",
           evt.timestamp_us);

    if (evt.hasFocus)
    {        
        focus_context* focus = context;
        focus->count = focus->id == evt.id ? focus->count + 1 : 1;
        focus->id = evt.id;
    }
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

    // setup ids and rectangles that define the interactors we want to use (one in each corner of the screen),
    // then add them to the interaction library
    const IL_InteractorId idA = 0;
    const IL_InteractorId idB = 1;
    const IL_InteractorId idC = 2;
    const IL_InteractorId idD = 3;

    const float size = 500.0f;
    const IL_Rectangle rectA = {            0,             0, size, size };
    const IL_Rectangle rectB = { width - size,             0, size, size };
    const IL_Rectangle rectC = {            0, height - size, size, size };
    const IL_Rectangle rectD = { width - size, height - size, size, size };

    const float z = 0.0f;


    IL_BeginInteractorUpdates(intlib);

    IL_AddOrUpdateInteractor(intlib, idA, rectA, z);
    IL_AddOrUpdateInteractor(intlib, idB, rectB, z);
    IL_AddOrUpdateInteractor(intlib, idC, rectC, z);
    IL_AddOrUpdateInteractor(intlib, idD, rectD, z);

    IL_CommitInteractorUpdates(intlib);

    // subscribe to gaze focus events using the callback defined above
    // it prints event data to std out when called and counts the number of consecutive focus events
    focus_context focus = { IL_EMPTYINTERACTORID, 0 };

    IL_SubscribeGazeFocusEvents(intlib, gaze_focus_callback, &focus);

    // setup and maintain device connection, wait for device data between events and 
    // update interaction library to trigger all callbacks
    // stop after 3 consecutive focus events on the same interactor
    printf("Starting interaction library update loop.\n");

    const size_t max_focus_count = 3;

    while (focus.count < max_focus_count)
    {
        IL_WaitAndUpdate(intlib, 1000);
    }

    printf("Interactor %" PRIu64 " got focused %zu times\n", focus.id, focus.count);

    // cleanup
    IL_DestroyInteractionLib(intlib);

    return 0;
}
