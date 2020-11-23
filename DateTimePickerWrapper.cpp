#include "DateTimePickerWrapper.h"

CDateTimePickerWrapper::CDateTimePickerWrapper (HWND hwndParent, UINT uiControlID) : CGenericControlWrapper (hwndParent, uiControlID)
{
    strcpy (m_chClassName, "SysDateTimePick32");
}
