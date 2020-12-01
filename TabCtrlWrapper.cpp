#include "TabCtrlWrapper.h"

CTabCtrlWrapper::CTabCtrlWrapper (HWND hwndParent, UINT uiControlID) : CGenericControlWrapper (hwndParent, uiControlID)
{
    strcpy (m_chClassName, "SysTabControl32");
}

int CTabCtrlWrapper::AddItem (char *pszText, LPARAM data)
{
    TCITEM item;

    item.mask = TCIF_PARAM | TCIF_TEXT;
    item.lParam = data;
    item.pszText = pszText;

    return SendMessage (TCM_INSERTITEM, 0xFFFF, (LPARAM) & item);
}

int CTabCtrlWrapper::GetCurSel ()
{
    return SendMessage (TCM_GETCURSEL, 0, 0);
}

void CTabCtrlWrapper::SetCurSel (int index)
{
    SendMessage (TCM_SETCURSEL, index, 0);
}

LPARAM CTabCtrlWrapper::GetItemData (int index)
{
    TCITEM item;

    item.mask = TCIF_PARAM;

    SendMessage (TCM_GETITEM, index, (LPARAM) & item);

    return item.lParam;
}
