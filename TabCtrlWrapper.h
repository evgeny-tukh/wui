#pragma once

#include "GenericControlWrapper.h"

class CTabCtrlWrapper : public CGenericControlWrapper
{
    public:
        CTabCtrlWrapper (UINT uiControlID) : CGenericControlWrapper (uiControlID) {}
        CTabCtrlWrapper (HWND hwndParent, UINT uiControlID);

        int AddItem (char *pszText, LPARAM data);
        int GetCurSel ();
        LPARAM GetItemData (int item);
};
