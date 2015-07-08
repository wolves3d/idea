////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "EasyWin.h"

////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
CEasyWin::CEasyWin() :
	m_hWnd( 0 )
{
	m_ptSize.x = 400;
	m_ptSize.y = 300;
}


/*==============================================================================
//
//
==============================================================================*/
bool CEasyWin::Create( HINSTANCE hInst )
{
	m_hInstance = hInst;
	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof( WNDCLASSEX ); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= LoadIcon( NULL, (LPCTSTR)IDI_APPLICATION );
	wcex.hCursor		= LoadCursor( m_hInstance, IDC_ARROW );
	wcex.hbrBackground	= (HBRUSH)( COLOR_WINDOW );
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "MyWinClass";
	wcex.hIconSm		= NULL;

	if ( RegisterClassEx( &wcex ) )
	{
		if ( m_hWnd = CreateWindowEx(
			0,
			"MyWinClass", "MyWinName",
			/*WS_POPUP,*/ WS_OVERLAPPED | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, m_ptSize.x, m_ptSize.y,
			NULL, NULL, 0, NULL ) )
		{
			ShowWindow	( m_hWnd, SW_SHOW );
			UpdateWindow( m_hWnd );

			return true;
		}
	}

	return false;
}


/*==============================================================================
//
//
==============================================================================*/
void CEasyWin::SetSize( int nWidth, int nHeight )
{
	if ( m_hWnd )
	{
		RECT winRect;
		GetClientRect( m_hWnd, &winRect );

		nWidth	= nWidth + ( m_ptSize.x - ( winRect.right - winRect.left ) );
		nHeight	= nHeight + ( m_ptSize.y - ( winRect.bottom - winRect.top ) );

		MoveWindow( m_hWnd, 0, 0, nWidth, nHeight, TRUE );
	}

	m_ptSize.x = nWidth;
	m_ptSize.y = nHeight;
}

void CEasyWin::GetSize( int * pOutWidth, int * pOutHeight ) const
{
	if ( pOutWidth )
		(*pOutWidth) = m_ptSize.x;

	if ( pOutHeight )
		(*pOutHeight) = m_ptSize.y;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
bool CEasyWin::Idle( void )
{
	MSG msg;

	if ( PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE ) )
	{
		if ( !GetMessage( &msg, NULL, NULL, NULL ) )
			return false;

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return true;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CEasyWin::SetTitle( const char *szTitle )
{
	if ( !szTitle )
		szTitle = "";

	SetWindowText( m_hWnd, szTitle );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
bool CEasyWin::WaitForMessage( void )
{
	MSG msg;

	if ( GetMessage( &msg, NULL, 0, 0 ) ) 
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
