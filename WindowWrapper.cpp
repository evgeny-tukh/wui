#include <StdLib.h>
#include <Windows.h>
#include <WinUser.h>
#include "WindowWrapper.h"

std::vector<CWindowWrapper *> CWindowWrapper::m_arrInstances;

CWindowWrapper::CWindowWrapper (HINSTANCE hInstance, HWND hwndParent, const char *pszClassName, HMENU hmnuMenu, const char *pszIcon, const char *pszCursor, const UINT uiBrush)
{    
    if (pszClassName != NULL)
        strncpy (m_chClassName, pszClassName, sizeof (m_chClassName));

    m_bIsDialogBox = FALSE;
    m_dwLastError  = 0;    
    m_hwndParent   = hwndParent;
    m_hInstance    = hInstance;
    m_hwndHandle   = NULL;
    m_hmnuMenu     = hmnuMenu;
    m_pszIcon      = (char *) pszIcon;
    m_pszCursor    = (char *) pszCursor;
    m_uiBrush      = uiBrush;
    m_aClass       = pszClassName ? RegisterClass () : 0;
}

CWindowWrapper::CWindowWrapper (HWND hwndParent, UINT_PTR uiControlID)
{
    memset (m_chClassName, 0, sizeof (m_chClassName));

    m_bIsDialogBox = FALSE;
    m_dwLastError  = 0;    
    m_hwndParent   = hwndParent;
    m_hwndHandle   = GetDlgItem (hwndParent, uiControlID); // May be yet NULL
    m_pszIcon      = NULL;
    m_pszCursor    = NULL;
    m_uiBrush      = NULL;
    m_hmnuMenu     = (HMENU) uiControlID;
    
    if (m_hwndHandle == NULL)
    {
        m_hInstance = (HINSTANCE) GetWindowLongPtr (hwndParent, GWLP_HINSTANCE);
        
        memset (m_chClassName, 0, sizeof (m_chClassName));
    }
    else
    {
        m_hInstance = (HINSTANCE) GetWindowLongPtr (m_hwndHandle, GWLP_HINSTANCE);
    
        GetClassName (m_hwndHandle, m_chClassName, sizeof (m_chClassName));
    }
    
    if (m_hwndHandle)
        AddInstance (this);
}

CWindowWrapper::~CWindowWrapper ()
{
    if (m_hwndHandle != NULL)
    {    
        RemoveInstance (m_hwndHandle);
        
        Destroy ();
    }
}

BOOL CWindowWrapper::RegisterClass ()
{
    WNDCLASS wcClass;
    
    memset (& wcClass, 0, sizeof (wcClass));
    
    wcClass.style         = CS_VREDRAW | CS_HREDRAW;
    wcClass.lpfnWndProc   = CWindowWrapper::WindowProc;
    wcClass.hInstance     = m_hInstance;
    wcClass.hIcon         = LoadIcon (NULL, m_pszIcon);
    wcClass.hCursor       = LoadCursor (NULL, m_pszCursor);
    wcClass.hbrBackground = (HBRUSH) GetStockObject (m_uiBrush);
    wcClass.lpszClassName = m_chClassName;
    
    m_aClass = ::RegisterClass (& wcClass);
    
    return m_aClass != 0;
}
                    
LRESULT CALLBACK CWindowWrapper::WindowProc (HWND hwndHandle, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
    #if 1
    CWindowWrapper *pInstance;

    if (uiMessage == WM_CREATE)
    {
        CREATESTRUCT *pData = (CREATESTRUCT *) lParam;
        
        pInstance = (CWindowWrapper *) pData->lpCreateParams;

        pInstance->m_hwndHandle = hwndHandle;

        AddInstance (pInstance);
        pInstance->OnCreate ();
    }
    else if (uiMessage == WM_INITDIALOG)
    {
        pInstance = (CWindowWrapper *) lParam;

        pInstance->m_hwndHandle = hwndHandle;

        AddInstance (pInstance);
    }
    else
    {
        pInstance = FindInstance (hwndHandle);
    }

    //return DefWindowProc (hwndHandle, uiMessage, wParam, lParam);
    return (pInstance == NULL) ? DefWindowProc (hwndHandle, uiMessage, wParam, lParam) : pInstance->OnMessage (uiMessage, wParam, lParam);
    #else
    CWindowWrapper *pInstance;

    if (uiMessage == WM_CREATE)
    {
        CREATESTRUCT   *pData = (CREATESTRUCT *) lParam;
        CWindowWrapper *pThis = (CWindowWrapper *) pData->lpCreateParams;
        
        pThis->m_hwndHandle = hwndHandle;
    }
    else if (uiMessage == WM_INITDIALOG)
    {
        CWindowWrapper *pThis = (CWindowWrapper *) lParam;

        pThis->m_hwndHandle = hwndHandle;
    }

    pInstance = FindInstance (hwndHandle);

    if (pInstance == NULL)
        pInstance = (CWindowWrapper *) GetWindowLongPtr (hwndHandle, GWLP_USERDATA);

    if (pInstance == NULL)
    {
        if (GetWindowLongPtr (hwndHandle, DWLP_DLGPROC) != NULL)
            return uiMessage == WM_INITDIALOG;
        else
            return DefWindowProc (hwndHandle, uiMessage, wParam, lParam);
    }

    if (pInstance == NULL && m_arrInstances.size () > 0)
    {
        pInstance = m_arrInstances.back ();
        
        pInstance->m_hwndHandle = hwndHandle;
    }
    
    return (pInstance == NULL) ? 0 : pInstance->OnMessage (uiMessage, wParam, lParam);
    #endif
}

BOOL CWindowWrapper::Create (const char *pszText, const int nX, const int nY, const int nWidth, const int nHeight, const UINT uiStyle)
{
    m_hwndHandle = CreateWindow (m_chClassName, pszText, uiStyle, nX, nY, nWidth, nHeight, m_hwndParent, m_hmnuMenu, m_hInstance, this);
    
    if (m_hwndHandle == NULL)
        m_dwLastError = GetLastError ();
        
    return m_hwndHandle != NULL;
}

BOOL CWindowWrapper::Destroy ()
{
    return (m_hwndHandle != NULL) ? DestroyWindow (m_hwndHandle) : NULL;
}

LRESULT CWindowWrapper::OnMessage (UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult;
    
    switch (uiMessage)
    {
        case WM_NOTIFY:
            lResult = OnNotify ((NMHDR *) lParam); break;
            
        case WM_SIZE:
            lResult = OnSize (wParam, LOWORD (lParam), HIWORD (lParam)); break;
        
        case WM_COMMAND:
            lResult = OnCommand (wParam, lParam); break;
            
        case WM_SYSCOMMAND:
            lResult = OnSysCommand (wParam, lParam); break;
            
        case WM_TIMER:
            lResult = OnTimer (wParam); break;

        case WM_PAINT:
            lResult = OnPaint (); break;

        case WM_DESTROY:
            OnDestroy ();

            RemoveInstance (m_hwndHandle);

        default:
            lResult = m_bIsDialogBox ? 0 : DefWindowProc (m_hwndHandle, uiMessage, wParam, lParam);
    }
    
    return lResult;
}

LRESULT CWindowWrapper::OnNotify (NMHDR *pHeader)
{
    return FALSE;
}

LRESULT CWindowWrapper::OnSize (const DWORD dwRequestType, const WORD wX, const WORD wY)
{
    return TRUE;
}

LRESULT CWindowWrapper::OnPaint ()
{
    return DefWindowProc (m_hwndHandle, WM_PAINT, 0, 0);
}

LRESULT CWindowWrapper::OnDestroy ()
{
    return FALSE;
}

LRESULT CWindowWrapper::OnCommand (WPARAM wParam, LPARAM lParam)
{
    return TRUE;
}

void CWindowWrapper::OnCreate ()
{
}

LRESULT CWindowWrapper::OnTimer (UINT uiTimerID)
{
    return DefWindowProc(m_hwndHandle, WM_TIMER, uiTimerID, 0);
}

LRESULT CWindowWrapper::OnSysCommand (WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc (m_hwndHandle, WM_SYSCOMMAND, wParam, lParam);;
}

void CWindowWrapper::AddInstance (CWindowWrapper *pInstance)
{
    m_arrInstances.push_back (pInstance);
}

void CWindowWrapper::RemoveInstance (HWND hwndHandle)
{
    for (auto instance = m_arrInstances.begin (); instance != m_arrInstances.end (); ++ instance)
    {
        if ((*instance)->m_hwndHandle == hwndHandle)
        {
            m_arrInstances.erase (instance); break;
        }
    }
}

CWindowWrapper *CWindowWrapper::FindInstance (HWND hwndHandle, int *pnIndex)
{
    for (auto instance = m_arrInstances.begin (); instance != m_arrInstances.end (); ++ instance)
    {
        if ((*instance)->m_hwndHandle == hwndHandle)
        {
            if (pnIndex) *pnIndex = instance - m_arrInstances.begin ();

            return *instance;
        }
    }
    
    return 0;
}

int CWindowWrapper::MessageBox (const char *pszText, const char *pszCaption, UINT uiType)
{
    return ::MessageBox (m_hwndHandle, pszText, pszCaption, uiType);
}

void CWindowWrapper::MessageLoop ()
{
    MSG msgMessage;
    
    while (GetMessage (& msgMessage, NULL, NULL, NULL))
    {
        if (!IsDialogMessage (& msgMessage))
        {
            TranslateMessage (& msgMessage);
            DispatchMessage (& msgMessage);
        }
    }
}

LRESULT CWindowWrapper::SendMessage (UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
    return ::SendMessage (m_hwndHandle, uiMessage, wParam, lParam);
}

void CWindowWrapper::Enable (const BOOL bEnabled)
{
    ::EnableWindow (m_hwndHandle, bEnabled);
}

int CWindowWrapper::GetTextLength()
{
    return (int) SendMessage (WM_GETTEXTLENGTH);
}

int CWindowWrapper::GetText (char *pchBuffer, const int nSize)
{
    return GetWindowText (m_hwndHandle, pchBuffer, nSize);
}

void CWindowWrapper::SetText (const char *pszString)
{
    SetWindowText (m_hwndHandle, pszString);
}

BOOL CWindowWrapper::IsDialogMessage (MSG *pMessage)
{
    BOOL            bResult;
    CWindowWrapper *pInstance;
    int             i;

    for (auto instance = m_arrInstances.begin (); instance != m_arrInstances.end (); ++ instance)
    {
        if ((*instance)->IsDialogBox () && ::IsDialogMessage ((*instance)->GetHandle (), pMessage))
            return TRUE;
    }

    return FALSE;
}
