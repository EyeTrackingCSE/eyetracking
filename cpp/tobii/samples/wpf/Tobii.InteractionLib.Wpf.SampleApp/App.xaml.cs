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
using System.Windows;

namespace Tobii.InteractionLib.Wpf.SampleApp
{
    /// Interaction logic for App.xaml
    public partial class App : Application
    {
        // Exposes the Interaction Library WPF host. Makes it possible for windows (like MainWindow in this sample)
        // to add themselves to the Interaction Library WPF binding of the host.
        public Tobii.InteractionLib.Wpf.InteractionLibWpfHost IntlibWpf { get; private set; }

        // This function is called when the application loads very first time. On startup, we instantiate the 
        // Interaction Library WPF host and assign it to the IntlibWpf property which can later be used by other windows.
        protected override void OnStartup(StartupEventArgs e) => IntlibWpf = new Tobii.InteractionLib.Wpf.InteractionLibWpfHost();

        // This function is called when the application exits.
        protected override void OnExit(ExitEventArgs e)
        {
            // Cleans up the Interaction Library WPF host instance before the application exits.
            IntlibWpf?.Dispose();

            base.OnExit(e);
        }
    }
}
