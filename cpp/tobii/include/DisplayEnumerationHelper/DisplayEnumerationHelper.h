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

// This is a single-header-only lib.
// Define DISPLAY_ENUMERATION_HELPER_IMPLEMENTATION once in a c/cpp file and include this file to generate code.
// Otherwise include without defining DISPLAY_ENUMERATION_HELPER_IMPLEMENTATION to get declarations (ie, use as a normal header).

#ifndef INCLUDE_DISPLAY_ENUMERATION_HELPER
#define INCLUDE_DISPLAY_ENUMERATION_HELPER

#define DISPLAY_ENUMERATOR_HELPER_VERSION "1.0.0"

#include <Windows.h>
#include <tchar.h>
#include <cfgmgr32.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DISPLAY_ENUM_HELPER_BUILDING_DYN_LIB
    #ifdef DISPLAY_ENUM_HELPER_EXPORTING
        #define DEH_API __declspec( dllexport )
    #else
        #define DEH_API __declspec( dllimport )
    #endif
#else
    #define DEH_API
#endif

// this is the output data struct
typedef struct MonitorDisplayAreaA
{
    float xVirtual;
    float yVirtual;
    float wVirtual;
    float hVirtual;

    char displayDeviceName[CCHDEVICENAME];
    char monitorId[MAX_DEVICE_ID_LEN];
} MonitorDisplayAreaA;

// this is the output data struct (wide chars)
typedef struct MonitorDisplayAreaW
{
    float xVirtual;
    float yVirtual;
    float wVirtual;
    float hVirtual;

    wchar_t displayDeviceName[CCHDEVICENAME];
    wchar_t monitorId[MAX_DEVICE_ID_LEN];
} MonitorDisplayAreaW;

#ifdef UNICODE
#define MonitorDisplayArea MonitorDisplayAreaW
#else
#define MonitorDisplayArea MonitorDisplayAreaA
#endif

// This enumerates all turned on, physical monitors in the system and outputs them into the output array of MonitorDisplayArea structs
// monitors is the output data array with monitor information, pre-allocated to maxCount length
// count will be the number of monitors found
// maxCount is the size of the pre-allocated MonitorDisplayArea array
// Will return false if more monitors are found than fit in the array, or if any error occurs, returns true on success
extern DEH_API BOOL EnumerateDisplayAreasA(MonitorDisplayAreaA* monitors, int* count, int maxCount);
// This enumerates all turned on, physical monitors in the system and outputs them into the output array of MonitorDisplayArea structs
// monitors is the output data array with monitor information, pre-allocated to maxCount length
// count will be the number of monitors found
// maxCount is the size of the pre-allocated MonitorDisplayArea array
// Will return false if more monitors are found than fit in the array, or if any error occurs, returns true on success
extern DEH_API BOOL EnumerateDisplayAreasW(MonitorDisplayAreaW* monitors, int* count, int maxCount);

#ifdef UNICODE
#define EnumerateDisplayAreas EnumerateDisplayAreasW
#else
#define EnumerateDisplayAreas EnumerateDisplayAreasA
#endif

#ifdef __cplusplus
}
#endif

#endif // INCLUDE_DISPLAY_ENUMERATION_HELPER  

// End of declarations

// -------------------------------------------------------------------------------------------------------------------------

// Start of definitions

#ifdef  DISPLAY_ENUMERATION_HELPER_IMPLEMENTATION
#ifndef DISPLAY_ENUMERATION_HELPER_IMPLEMENTATION_GUARD
#define DISPLAY_ENUMERATION_HELPER_IMPLEMENTATION_GUARD

static const char* const DISPLAY_STRA = "DISPLAY";
static const char* const BRACKET_STRA = "{";

static const wchar_t* const DISPLAY_STRW = L"DISPLAY";
static const wchar_t* const BRACKET_STRW = L"{";

#ifdef UNICODE
#define DISPLAY_STR DISPLAY_STRW
#define BRACKET_STR BRACKET_STRW
#else
#define DISPLAY_STR DISPLAY_STRA
#define BRACKET_STR BRACKET_STRA
#endif

// this is used internally when enumerating monitors to get the virtual desktop bounds of each monitor
typedef struct MonitorDisplayAreaEnumeratorA
{
    int count;
    MonitorDisplayAreaA* monitors;
} MonitorDisplayAreaEnumeratorA;

// this is used internally when enumerating monitors to get the virtual desktop bounds of each monitor (wide char version)
typedef struct MonitorDisplayAreaEnumeratorW
{
    int count;
    MonitorDisplayAreaW* monitors;
} MonitorDisplayAreaEnumeratorW;

#ifdef UNICODE
#define MonitorDisplayAreaEnumerator MonitorDisplayAreaEnumeratorW
#else
#define MonitorDisplayAreaEnumerator MonitorDisplayAreaEnumeratorA
#endif

// this is used internally to get the virtual desktop bounds of each monitor
static BOOL CALLBACK GetBoundsCallbackA(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    UNREFERENCED_PARAMETER(hdcMonitor);
    UNREFERENCED_PARAMETER(lprcMonitor);

	MonitorDisplayAreaEnumeratorA* enumerator = (MonitorDisplayAreaEnumeratorA*)dwData;

	MONITORINFOEXA monitorInfo;
	ZeroMemory(&monitorInfo, sizeof(monitorInfo));
	monitorInfo.cbSize = sizeof(monitorInfo);
	if (GetMonitorInfoA(hMonitor, (MONITORINFO*)&monitorInfo))
	{
        for(int i = 0;i < enumerator->count;i++)
        {
            MonitorDisplayAreaA* monitor = &enumerator->monitors[i];
            if (0 == strcmp(monitorInfo.szDevice, monitor->displayDeviceName))
            {
                monitor->xVirtual = (float)monitorInfo.rcMonitor.left;
                monitor->yVirtual = (float)monitorInfo.rcMonitor.top;
                monitor->wVirtual = (float)(monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left);
                monitor->hVirtual = (float)(monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top);
            }
        }
	}

	return TRUE;
}

// this is used internally to get the virtual desktop bounds of each monitor (wide char version)
static BOOL CALLBACK GetBoundsCallbackW(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    UNREFERENCED_PARAMETER(hdcMonitor);
    UNREFERENCED_PARAMETER(lprcMonitor);

	MonitorDisplayAreaEnumeratorW* enumerator = (MonitorDisplayAreaEnumeratorW*)dwData;

	MONITORINFOEXW monitorInfo;
	ZeroMemory(&monitorInfo, sizeof(monitorInfo));
	monitorInfo.cbSize = sizeof(monitorInfo);
	if (GetMonitorInfoW(hMonitor, (MONITORINFO*)&monitorInfo))
	{
        for(int i = 0;i < enumerator->count;i++)
        {
            MonitorDisplayAreaW* monitor = &enumerator->monitors[i];
            if (0 == wcscmp(monitorInfo.szDevice, monitor->displayDeviceName))
            {
                monitor->xVirtual = (float)monitorInfo.rcMonitor.left;
                monitor->yVirtual = (float)monitorInfo.rcMonitor.top;
                monitor->wVirtual = (float)(monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left);
                monitor->hVirtual = (float)(monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top);
            }
        }
	}

	return TRUE;
}

#ifdef UNICODE
#define GetBoundsCallback GetBoundsCallbackW
#else
#define GetBoundsCallback GetBoundsCallbackA
#endif

extern BOOL EnumerateDisplayAreasA(MonitorDisplayAreaA* monitors, int* count, int maxCount)
{
    if(maxCount < 1) return FALSE;

    *count = 0;

    // first, iterate over all display devices and find the physical monitor device for each display device (if there is one)
    // save the results in the output struct

    int numDevicesEnumerated = 0;
    DISPLAY_DEVICEA device;
    ZeroMemory(&device, sizeof(device));
    device.cb = sizeof(device);
    while(EnumDisplayDevicesA(NULL, numDevicesEnumerated, &device, 0))
    {
        numDevicesEnumerated++;

        // so, my understanding is, there will only be, at most, one physical, turned on monitor per display device
        int numMonitorsEnumerated = 0;
        DISPLAY_DEVICEA displayMonitor;
        ZeroMemory(&displayMonitor, sizeof(displayMonitor));
        displayMonitor.cb = sizeof(displayMonitor);
        while(EnumDisplayDevicesA(device.DeviceName, numMonitorsEnumerated, &displayMonitor, 0)) // enumerate monitors on the display device
        {
            numMonitorsEnumerated++;

            if(!((displayMonitor.StateFlags & DISPLAY_DEVICE_ACTIVE) && !(displayMonitor.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)))
            {
                // this monitor is not turned on, or it is not an actual physical monitor
                continue;
            }

            DISPLAY_DEVICEA displayMonitorInterface;
            ZeroMemory(&displayMonitorInterface, sizeof(displayMonitorInterface));
            displayMonitorInterface.cb = sizeof(displayMonitorInterface);
            if(!EnumDisplayDevicesA(device.DeviceName, numMonitorsEnumerated-1, &displayMonitorInterface, EDD_GET_DEVICE_INTERFACE_NAME))
            {
                return FALSE; // something broke
            }

            if(*count + 1 > maxCount) return FALSE; // we found a monitor but don't have sufficient memory left to store information about it

            // store the relevant monitor information we have in the output struct
            MonitorDisplayAreaA* monitor = &monitors[*count];
            BOOL success = TRUE;
            success = success && (0 == strcpy_s(monitor->displayDeviceName, CCHDEVICENAME, device.DeviceName));

            // we used to ask the SetupAPI for this but the id is right there in device interface name
            // since we don't need any other info from SetupAPI in this context we just pick the right part of the string and format it correctly
            char* start = strstr(displayMonitorInterface.DeviceID, DISPLAY_STRA);
            char* stop = strstr(displayMonitorInterface.DeviceID, BRACKET_STRA);
            success = success && (0 == strncpy_s(monitor->monitorId, sizeof(monitor->monitorId), start, stop-start-1));
            success = success && (0 == _strupr_s(monitor->monitorId, MAX_DEVICE_ID_LEN)); // uppercase it

            char* pointer = strchr(monitor->monitorId, '#');
            while (pointer)
            {
                *pointer = '\\';
                pointer = strchr(pointer, '#');
            }

            if(!success) return FALSE; // something broke

            (*count)++;
            break; // this is correct if there is really only one physical monitor per display device
        }
    }

    // get the virtual desktop bounds of each monitor
    MonitorDisplayAreaEnumeratorA enumerator;
    enumerator.count = *count;
    enumerator.monitors = monitors;
    EnumDisplayMonitors(NULL, NULL, &GetBoundsCallbackA, (LPARAM)&enumerator);

    // success!
    return TRUE;
}

extern BOOL EnumerateDisplayAreasW(MonitorDisplayAreaW* monitors, int* count, int maxCount)
{
    if(maxCount < 1) return FALSE;

    *count = 0;

    // first, iterate over all display devices and find the physical monitor device for each display device (if there is one)
    // save the results in the output struct

    int numDevicesEnumerated = 0;
    DISPLAY_DEVICEW device;
    ZeroMemory(&device, sizeof(device));
    device.cb = sizeof(device);
    while(EnumDisplayDevicesW(NULL, numDevicesEnumerated, &device, 0))
    {
        numDevicesEnumerated++;

        // so, my understanding is, there will only be, at most, one physical, turned on monitor per display device
        int numMonitorsEnumerated = 0;
        DISPLAY_DEVICEW displayMonitor;
        ZeroMemory(&displayMonitor, sizeof(displayMonitor));
        displayMonitor.cb = sizeof(displayMonitor);
        while(EnumDisplayDevicesW(device.DeviceName, numMonitorsEnumerated, &displayMonitor, 0)) // enumerate monitors on the display device
        {
            numMonitorsEnumerated++;

            if(!((displayMonitor.StateFlags & DISPLAY_DEVICE_ACTIVE) && !(displayMonitor.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)))
            {
                // this monitor is not turned on, or it is not an actual physical monitor
                continue;
            }

            DISPLAY_DEVICEW displayMonitorInterface;
            ZeroMemory(&displayMonitorInterface, sizeof(displayMonitorInterface));
            displayMonitorInterface.cb = sizeof(displayMonitorInterface);
            if(!EnumDisplayDevicesW(device.DeviceName, numMonitorsEnumerated-1, &displayMonitorInterface, EDD_GET_DEVICE_INTERFACE_NAME))
            {
                return FALSE; // something broke
            }

            if(*count + 1 > maxCount) return FALSE; // we found a monitor but don't have sufficient memory left to store information about it

            // store the relevant monitor information we have in the output struct
            MonitorDisplayAreaW* monitor = &monitors[*count];
            BOOL success = TRUE;
            success = success && (0 == wcscpy_s(monitor->displayDeviceName, CCHDEVICENAME, device.DeviceName));

            // we used to ask the SetupAPI for this but the id is right there in device interface name
            // since we don't need any other info from SetupAPI in this context we just pick the right part of the string and format it correctly
            wchar_t* start = wcsstr(displayMonitorInterface.DeviceID, DISPLAY_STRW);
            wchar_t* stop = wcsstr(displayMonitorInterface.DeviceID, BRACKET_STRW);
            success = success && (0 == wcsncpy_s(monitor->monitorId, MAX_DEVICE_ID_LEN, start, stop-start-1));
            success = success && (0 == _wcsupr_s(monitor->monitorId, MAX_DEVICE_ID_LEN)); // uppercase it

            wchar_t* pointer = wcschr(monitor->monitorId, L'#');
            while (pointer)
            {
                *pointer = L'\\';
                pointer = wcschr(pointer, L'#');
            }

            if(!success) return FALSE; // something broke

            (*count)++;
            break; // this is correct if there is really only one physical monitor per display device
        }
    }

    // get the virtual desktop bounds of each monitor
    MonitorDisplayAreaEnumeratorW enumerator;
    enumerator.count = *count;
    enumerator.monitors = monitors;
    EnumDisplayMonitors(NULL, NULL, &GetBoundsCallbackW, (LPARAM)&enumerator);

    // success!
    return FALSE;
}

#endif // DISPLAY_ENUMERATION_HELPER_IMPLEMENTATION_GUARD
#endif // DISPLAY_ENUMERATION_HELPER_IMPLEMENTATION