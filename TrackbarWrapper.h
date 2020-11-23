#pragma once

#include "GenericControlWrapper.h"

class CTrackbarWrapper : public CGenericControlWrapper
{
    public:
        CTrackbarWrapper (UINT uiControlID) : CGenericControlWrapper (uiControlID) {}
        CTrackbarWrapper (HWND hwndParent, UINT uiControlID);
};
