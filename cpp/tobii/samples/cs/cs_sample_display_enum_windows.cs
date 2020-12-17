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
using System.Drawing;
using System.Windows.Forms;

namespace CSharpSample
{
    class Program
    {
        static void Main()
        {
            // create the interaction library
            Tobii.InteractionLib.IInteractionLib intlib =
                Tobii.InteractionLib.InteractionLibFactory.CreateInteractionLib(
                    Tobii.InteractionLib.FieldOfUse.Interactive);

            // EnumerateDisplayAreas will enumerate all displays on the windows machine, including their
            // virtual screen bounds and display IDs, and put the results into an IList<MonitorDisplayArea>.
            // The display IDs have the same formatting as are used on Tobii eyetrackers to 
            // identify which display they are attached to. 
            foreach (var area in DisplayEnumerationHelper.DisplayEnumerationHelper.EnumerateDisplayAreas())
            {
                // For each display we tell the Interaction Library
                // 1) the coordinate scaling we want to use by giving the size as we see it
                // 2) the size and origin in virtual screen coordinates, and 
                // 3) the ID of the display.
                // 
                // Interaction Library will use this information to map connected devices to display areas,
                // to transform all data streams when outputting them and to properly transform all added 
                // interactors to its internal, global coordinate space when performing gaze focus calculations.
                // 
                // Here we are using the native scaling (so first to parameters are the same as the virtual 
                // screen size) but we could have  used any scaling we like, eg (1.0, 1.0) for relative 
                // coordinates.
                intlib.CoordinateTransformAddOrUpdateDisplayArea(
                    area.wVirtual, area.hVirtual,
                    area.wVirtual, area.hVirtual,
                    area.xVirtual, area.yVirtual,
                    area.monitorId);
            }

            var form = new Form();
            form.Text = "Interactor Window";

            // By telling the Interaction Library to offset the coordinate system origin
            // so that it matches the location of the window client area, we can add and
            // update interactors using window-local coordinates.
            // The origin offset has to be in the virtual coordinate space we used when 
            // adding the display areas above.
            form.Move += (sender, e) =>
            {
                var offset = form.PointToScreen(new Point(0, 0));
                intlib.CoordinateTransformSetOriginOffset(offset.X, offset.Y);
            };

            // In this sample we want the entire window client area to be an interactor
            // that changes color when it gets and loses gaze focus.
            void AddOrUpdateInteractor(Size clientSize)
            {
                var rect = new Tobii.InteractionLib.Rectangle(0, 0, clientSize.Width, clientSize.Height);
                intlib.BeginInteractorUpdates();
                intlib.AddOrUpdateInteractor(0, rect, 0);
                intlib.CommitInteractorUpdates();
            };

            form.Resize += (sender, e) => AddOrUpdateInteractor(form.ClientSize);

            // Subscribe to gaze focus events. When the form client area (which is an
            // interactor) gets or loses gaze focus, set the form color accordingly.
            intlib.GazeFocusEvent += e => form.BackColor = e.hasFocus ?
                                                           SystemColors.Highlight :
                                                           SystemColors.Window;

            var closed = false;
            form.Closed += (sender, e) => closed = true;

            // Now that all events and subscriptions are set up, the physical form can be created.
            form.Show();
            AddOrUpdateInteractor(form.ClientSize);

            // Handle window events and update interaction library to trigger all callbacks
            while (!closed)
            {
                Application.DoEvents();
                intlib.Update();
            }

            intlib.Dispose();
        }
    }
}
