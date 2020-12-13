#include <time.h>
#include "DateTimePickerWrapper.h"

CDateTimePickerWrapper::CDateTimePickerWrapper (HWND hwndParent, UINT uiControlID) : CGenericControlWrapper (hwndParent, uiControlID)
{
    strcpy (m_chClassName, "SysDateTimePick32");
}

BOOL CDateTimePickerWrapper::GetTime (SYSTEMTIME *pTime)
{
    return SendMessage (DTM_GETSYSTEMTIME, 0, (LPARAM) pTime) == GDT_VALID;
}

void CDateTimePickerWrapper::SetTime (SYSTEMTIME *pTime)
{
    SendMessage (DTM_SETSYSTEMTIME, pTime ? GDT_VALID : GDT_NONE, (LPARAM) pTime);
}

time_t CDateTimePickerWrapper::GetTimestamp ()
{
    SYSTEMTIME sysTime;
    time_t result = 0;

    if (GetTime (& sysTime))
    {
        tm time;

        time.tm_year = sysTime.wYear - 1900;
        time.tm_mon = sysTime.wMonth - 1;
        time.tm_mday = sysTime.wDay;
        time.tm_hour = sysTime.wHour;
        time.tm_min = sysTime.wMinute;
        time.tm_sec = sysTime.wSecond;

        result = mktime (& time);
    }

    return result;
}

void CDateTimePickerWrapper::SetTimestamp (time_t timestamp)
{
    if (timestamp)
    {
        SYSTEMTIME sysTime;
        tm *time = localtime (& timestamp);

        sysTime.wYear = time->tm_year + 1900;
        sysTime.wMonth = time->tm_mon + 1;
        sysTime.wDay = time->tm_mday;
        sysTime.wHour = time->tm_hour;
        sysTime.wMinute = time->tm_min;
        sysTime.wSecond = time->tm_sec;
        sysTime.wMilliseconds = 0;

        SetTime (& sysTime);
    }
    else
    {
        SetTime (0);
    }
}

void CDateTimePickerWrapper::SetFormat (char *format)
{
    SendMessageA (DTM_SETFORMAT, 0, (LPARAM) format);
}
