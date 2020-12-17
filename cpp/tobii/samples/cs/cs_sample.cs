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

using System;

namespace CSharpSample
{
    class Program
    {
        static void Main(string[] args)
        {
            // create the interaction library
            Tobii.InteractionLib.IInteractionLib intlib =
                Tobii.InteractionLib.InteractionLibFactory.CreateInteractionLib(
                    Tobii.InteractionLib.FieldOfUse.Interactive);

            // assume single screen with size 2560x1440 and use full screen (not window local) coordinates
            const float width  = 2560.0f;
            const float height = 1440.0f;
            const float offset = 0.0f;
            
            intlib.CoordinateTransformAddOrUpdateDisplayArea(width, height);
            intlib.CoordinateTransformSetOriginOffset(offset, offset);

            // setup ids and rectangles that define the interactors we want to use (one in each corner of the screen),
            // then add them to the interaction library
            const ulong idA = 0;
            const ulong idB = 1;
            const ulong idC = 2;
            const ulong idD = 3;

            const float size = 500.0f;
            var rectA = new Tobii.InteractionLib.Rectangle(           0,             0, size, size);
            var rectB = new Tobii.InteractionLib.Rectangle(width - size,             0, size, size);
            var rectC = new Tobii.InteractionLib.Rectangle(           0, height - size, size, size);
            var rectD = new Tobii.InteractionLib.Rectangle(width - size, height - size, size, size);

            const float z = 0.0f;


            intlib.BeginInteractorUpdates();

            intlib.AddOrUpdateInteractor(idA, rectA, z);
            intlib.AddOrUpdateInteractor(idB, rectB, z);
            intlib.AddOrUpdateInteractor(idC, rectC, z);
            intlib.AddOrUpdateInteractor(idD, rectD, z);

            intlib.CommitInteractorUpdates();

            // this is used to keep track of focus events (See below)
            ulong focusId    = ulong.MaxValue - 1;
            uint  focusCount = 0;

            // subscribe to gaze focus events
            // print event data to console when called and count the number of consecutive focus events
            intlib.GazeFocusEvent += e =>
            {
                Console.WriteLine("Interactor: {0}, focused: {1}, timestamp: {2} us",
                                  e.interactorId,
                                  e.hasFocus,
                                  e.timestamp_us);

                if (e.hasFocus)
                {
                    focusCount = focusId == e.interactorId ? focusCount + 1 : 1;
                    focusId = e.interactorId;
                }
            };

            // setup and maintain device connection, wait for device data between events and 
            // update interaction library to trigger all callbacks
            // stop after 3 consecutive focus events on the same interactor
            Console.WriteLine("Starting interaction library update loop.");

            const uint MaxFocusCount = 3;

            while (focusCount < MaxFocusCount)
            {
                intlib.WaitAndUpdate();
            }

            Console.WriteLine("Interactor {0} got focused {1} times", focusId, focusCount);

            // cleanup - here done explicitly with Dispose(). The alternative is to use a using statement,
            // which implicitly disposes an instance when the scope is left.
            intlib.Dispose();
        }
    }
}
