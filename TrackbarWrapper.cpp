#include "TrackbarWrapper.h"

CTrackbarWrapper::CTrackbarWrapper (HWND hwndParent, UINT uiControlID) : CGenericControlWrapper (hwndParent, uiControlID)
{
    strcpy (m_chClassName, "msctls_trackbar32");
}

void CTrackbarWrapper::SetRange (LPARAM lpMin, LPARAM lpMax)
{
    SendMessage (TBM_SETRANGEMIN, TRUE, lpMin);
    SendMessage (TBM_SETRANGEMAX, TRUE, lpMax);
}

LPARAM CTrackbarWrapper::GetPos ()
{
    return SendMessage (TBM_GETPOS);
}
