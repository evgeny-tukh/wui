#pragma once

#include "GenericControlWrapper.h"

class CDateTimePickerWrapper : public CGenericControlWrapper
{
    public:
        CDateTimePickerWrapper (UINT uiControlID) : CGenericControlWrapper (uiControlID) {}
        CDateTimePickerWrapper (HWND hwndParent, UINT uiControlID);
};
