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

namespace CSharpSampleStreams
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

            // subscribe to gaze point data; print data to console when called
            intlib.GazePointDataEvent += evt =>
            {
                Console.WriteLine(
                    "x: " + evt.x
                    + ", y: " + evt.y
                    + ", validity: " + (evt.validity == Tobii.InteractionLib.Validity.Valid ? "valid" : "invalid")
                    + ", timestamp: " + evt.timestamp_us + " us"
                );
            };

            Console.WriteLine("Starting interaction library update loop.");

            // setup and maintain device connection, wait for device data between events and 
            // update interaction library to trigger all callbacks, stop after 200 cycles
            const int max_cycles = 200;
            var cycle = 0;
            while (cycle++ < max_cycles)
            {
                intlib.WaitAndUpdate();
            }

            // cleanup - here done explicitly with Dispose(). The alternative is to use a using statement,
            // which implicitly disposes an instance when the scope is left.
            intlib.Dispose();
        }
    }
}
