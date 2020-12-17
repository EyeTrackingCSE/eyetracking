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

using System.Runtime.InteropServices;

namespace DisplayEnumerationHelper
{
    using System;
    using System.Linq;
    using System.Collections.Generic;

    // taken from 
    // http://www.pinvoke.net/default.aspx/user32/MONITORINFOEX.html
    // and heavily adjusted (eg to fit other signatures, naming etc)

    internal static class DisplayEnumerationConstants
    {
        // size of a device name string
        public const int CCHDEVICENAME = 32;
    }

    /// <summary>
    /// The MONITORINFOEX structure contains information about a display monitor.
    /// The GetMonitorInfo function stores information into a MONITORINFOEX structure or a MONITORINFO structure.
    /// The MONITORINFOEX structure is a superset of the MONITORINFO structure. The MONITORINFOEX structure adds a string member to contain a name
    /// for the display monitor.
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    internal struct MonitorInfoEx
    {
        /// <summary>
        /// The size, in bytes, of the structure. Set this member to sizeof(MONITORINFOEX) (72) before calling the GetMonitorInfo function.
        /// Doing so lets the function determine the type of structure you are passing to it.
        /// </summary>
        public int cbSize;

        /// <summary>
        /// A RECT structure that specifies the display monitor rectangle, expressed in virtual-screen coordinates.
        /// Note that if the monitor is not the primary display monitor, some of the rectangle's coordinates may be negative values.
        /// </summary>
        public Rect rcMonitor;

        /// <summary>
        /// A RECT structure that specifies the work area rectangle of the display monitor that can be used by applications,
        /// expressed in virtual-screen coordinates. Windows uses this rectangle to maximize an application on the monitor.
        /// The rest of the area in rcMonitor contains system windows such as the task bar and side bars.
        /// Note that if the monitor is not the primary display monitor, some of the rectangle's coordinates may be negative values.
        /// </summary>
        public Rect rcWork;

        /// <summary>
        /// The attributes of the display monitor.
        ///
        /// This member can be the following value:
        ///   1 : MONITORINFOF_PRIMARY
        /// </summary>
        public uint dwFlags;

        /// <summary>
        /// A string that specifies the device name of the monitor being used. Most applications have no use for a display monitor name,
        /// and so can save some bytes by using a MONITORINFO structure.
        /// </summary>
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = DisplayEnumerationConstants.CCHDEVICENAME )]
        public string szDevice;

        public void Init()
        {
            // this.Size = 40 + 2 * CCHDEVICENAME; // is probably to accomodate wide chars but that is implicit, using sizeof instead
            this.cbSize = Marshal.SizeOf(this);
            this.szDevice = string.Empty;
        }
    }

    /// <summary>
    /// The RECT structure defines the coordinates of the upper-left and lower-right corners of a rectangle.
    /// </summary>
    /// <see href="http://msdn.microsoft.com/en-us/library/dd162897%28VS.85%29.aspx"/>
    /// <remarks>
    /// By convention, the right and bottom edges of the rectangle are normally considered exclusive.
    /// In other words, the pixel whose coordinates are ( right, bottom ) lies immediately outside of the the rectangle.
    /// For example, when RECT is passed to the FillRect function, the rectangle is filled up to, but not including,
    /// the right column and bottom row of pixels. This structure is identical to the RECTL structure.
    /// </remarks>
    [StructLayout(LayoutKind.Sequential)]
    internal struct Rect
    {
        /// <summary>
        /// The x-coordinate of the upper-left corner of the rectangle.
        /// </summary>
        public int Left;

        /// <summary>
        /// The y-coordinate of the upper-left corner of the rectangle.
        /// </summary>
        public int Top;

        /// <summary>
        /// The x-coordinate of the lower-right corner of the rectangle.
        /// </summary>
        public int Right;

        /// <summary>
        /// The y-coordinate of the lower-right corner of the rectangle.
        /// </summary>
        public int Bottom;      
    }

    // taken from 
    // https://pinvoke.net/default.aspx/Enums/DisplayDeviceStateFlags.html
    // and 
    // https://pinvoke.net/default.aspx/Structures/DISPLAY_DEVICE.html
    // and adjusted

    // heavily adjusted, now in accordance with 
    // https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-display_devicew
    [Flags()]
    internal enum DisplayDeviceStateFlags : int
    {
        /// <summary>Specifies whether a monitor is presented as being "on" by the respective GDI view</summary>
        Active = 0x1,
        /// <summary>The primary desktop is on the device. For a system with a single display card, this is always set. For a system with multiple display cards, only one device can have this set.</summary>
        PrimaryDevice = 0x4,
        /// <summary>Represents a pseudo device used to mirror application drawing for remoting or other purposes.</summary>
        MirroringDriver = 0x8,
        /// <summary>The device is VGA compatible.</summary>
        VGACompatible = 0x10,
        /// <summary>The device is removable; it cannot be the primary display.</summary>
        Removable = 0x20,
        /// <summary>The device has more display modes than its output devices support.</summary>
        ModesPruned = 0x8000000,
    }

    [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Auto)]
    internal struct DisplayDevice
    {
        [MarshalAs(UnmanagedType.U4)]
        public int cb;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst=32)]
        public string DeviceName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst=128)]
        public string DeviceString;
        [MarshalAs(UnmanagedType.U4)]
        public DisplayDeviceStateFlags StateFlags;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst=128)]
        public string DeviceID;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst=128)]
        public string DeviceKey;

        public void Init()
        {
            this.cb = Marshal.SizeOf(this);
            this.DeviceName = string.Empty;
            this.DeviceString = string.Empty;
            this.DeviceID = string.Empty;
            this.DeviceKey = string.Empty;
        }
    }

    // invented here
    [Flags()]
    internal enum EnumDisplayDevicesFlags : int
    {
        GetDeviceInterfaceName = 0x1,
    }

    internal static class DisplayEnumerationAreaExterns
    {
        // taken from 
        // https://pinvoke.net/default.aspx/user32/GetMonitorInfo.html

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern bool GetMonitorInfo(IntPtr hMonitor, ref MonitorInfoEx lpmi);

        // [DllImport("user32.dll")]
        // static extern bool GetMonitorInfo(IntPtr hMonitor, ref MONITORINFO lpmi);

        // taken from 
        // https://pinvoke.net/default.aspx/user32/EnumDisplayMonitors.html
        // and adjusted

        public delegate bool EnumDisplayMonitorsSizesDelegate(IntPtr hMonitor, IntPtr hdcMonitor, ref Rect lprcMonitor, IntPtr dwData);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern bool EnumDisplayMonitors(IntPtr hdc, IntPtr lprcClip, EnumDisplayMonitorsSizesDelegate lpfnEnum, IntPtr dwData);

        // taken from
        // https://pinvoke.net/default.aspx/user32/EnumDisplayDevices.html
        // and adjusted

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern bool EnumDisplayDevices(string lpDevice, uint iDevNum, ref DisplayDevice lpDisplayDevice, EnumDisplayDevicesFlags dwFlags);
    }

    // this is the output data class
    public class MonitorDisplayArea
    {
        public float xVirtual;
        public float yVirtual;
        public float wVirtual;
        public float hVirtual;

        public string displayDeviceName;
        public string monitorId;
    };

    public static class DisplayEnumerationHelper
    {
        // This enumerates all turned on, physical monitors in the system and returns them
        // Will return an empty list if none found or on fail
        public static IList<MonitorDisplayArea> EnumerateDisplayAreas()
        {           
            // first, iterate over all display devices and find the physical monitor device for each display device (if there is one)
            var monitors = new List<MonitorDisplayArea>();
            uint numDevicesEnumerated = 0;
            var device = new DisplayDevice();
            device.Init();
            while(DisplayEnumerationAreaExterns.EnumDisplayDevices(null, numDevicesEnumerated, ref device, 0))
            {
                numDevicesEnumerated++;

                uint numMonitorsEnumerated = 0;
                var displayMonitor = new DisplayDevice();
                displayMonitor.Init();
                while(DisplayEnumerationAreaExterns.EnumDisplayDevices(device.DeviceName, numMonitorsEnumerated, ref displayMonitor, 0))
                {
                    numMonitorsEnumerated++;

                    if((displayMonitor.StateFlags & DisplayDeviceStateFlags.Active) == 0 || (displayMonitor.StateFlags & DisplayDeviceStateFlags.MirroringDriver) != 0)
                    {
                        // this monitor is not turned on, or it is not an actual physical monitor
                        continue;
                    }

                    var displayMonitorInterface = new DisplayDevice();
                    displayMonitorInterface.Init();
                    if(!DisplayEnumerationAreaExterns.EnumDisplayDevices(device.DeviceName, numMonitorsEnumerated-1, ref displayMonitorInterface, EnumDisplayDevicesFlags.GetDeviceInterfaceName))
                    {
                        return new List<MonitorDisplayArea>(); // something broke
                    }

                    // store the relevant monitor information we have in the output struct and add to list
                    
                    var displayArea = new MonitorDisplayArea();
                    displayArea.displayDeviceName = device.DeviceName;

                    int start = displayMonitorInterface.DeviceID.IndexOf("DISPLAY");
                    int stop = displayMonitorInterface.DeviceID.IndexOf('{');

                    if(start == -1 || stop == -1) return new List<MonitorDisplayArea>();

                    displayArea.monitorId =  displayMonitorInterface.DeviceID.Substring(start, stop - start - 1).Replace("#", "\\").ToUpperInvariant();

                    monitors.Add(displayArea);

                    break; // this is correct if there is really only one physical monitor per display device
                }
            }

            // get the virtual desktop bounds of each monitor

            DisplayEnumerationAreaExterns.EnumDisplayMonitors(IntPtr.Zero, IntPtr.Zero, 
            (IntPtr hMonitor, IntPtr hdcMonitor, ref Rect lprcMonitor,  IntPtr dwData) =>
            {
                var monitorInfo = new MonitorInfoEx();
                monitorInfo.Init();
                if(DisplayEnumerationAreaExterns.GetMonitorInfo(hMonitor, ref monitorInfo))
                {
                    var monitor = monitors.SingleOrDefault(mda => mda.displayDeviceName.Equals(monitorInfo.szDevice));
                    if(monitor != null)
                    {
                        monitor.xVirtual = monitorInfo.rcMonitor.Left;
                        monitor.yVirtual = monitorInfo.rcMonitor.Top;
                        monitor.wVirtual = monitorInfo.rcMonitor.Right - monitorInfo.rcMonitor.Left;
                        monitor.hVirtual = monitorInfo.rcMonitor.Bottom - monitorInfo.rcMonitor.Top;
                    }
                }

                return true;
            }
            , IntPtr.Zero);

            return monitors;
        }
    }

} // namespace