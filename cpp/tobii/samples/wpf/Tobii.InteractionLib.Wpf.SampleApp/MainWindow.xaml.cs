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
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace Tobii.InteractionLib.Wpf.SampleApp
{
    /// Interaction logic for MainWindow.xaml
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            // Here we attach the current window to the already created instance of Interaction Library WpfBinding host.
            // Note, we are doing this before calling InitializeComponent so that WpfBinding has proper values
            // as soon as the window is loaded, without waiting for NotifyPropertyChanged method to be called.

            ((App)Application.Current).IntlibWpf?.WpfBinding?.AddWindow(this);

            InitializeComponent();
        }
    }
}
