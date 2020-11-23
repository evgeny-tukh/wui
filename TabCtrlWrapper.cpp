#include "TabCtrlWrapper.h"

CTabCtrlWrapper::CTabCtrlWrapper (HWND hwndParent, UINT uiControlID) : CGenericControlWrapper (hwndParent, uiControlID)
{
    strcpy (m_chClassName, "SysTabControl32");
}

int CTabCtrlWrapper::AddItem (char *pszText, LPARAM data)
{
    TCITEMA item;

    item.mask = TCIF_PARAM | TCIF_TEXT;
    item.lParam = data;
    item.pszText = pszText;

    return SendMessage (TCM_INSERTITEM, 0xFFFF, (LPARAM) & item);
}