#include "TrackbarWrapper.h"

CTrackbarWrapper::CTrackbarWrapper (HWND hwndParent, UINT uiControlID) : CGenericControlWrapper (hwndParent, uiControlID)
{
    strcpy (m_chClassName, "msctls_trackbar32");
}
