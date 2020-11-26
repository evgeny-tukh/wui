#pragma once

#include "GenericControlWrapper.h"

class CDateTimePickerWrapper : public CGenericControlWrapper
{
    public:
        CDateTimePickerWrapper (UINT uiControlID) : CGenericControlWrapper (uiControlID) {}
        CDateTimePickerWrapper (HWND hwndParent, UINT uiControlID);

        BOOL GetTime (SYSTEMTIME *pTime);
        void SetTime (SYSTEMTIME *pTime = 0);

        time_t GetTimestamp ();
        void SetTimestamp (time_t timestamp);
};
