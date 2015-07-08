////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "_Shared/Math/3dmath.h"
#include "Input.h"

#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dinput8.lib" )

////////////////////////////////////////////////////////////////////////////////

TIntfDesc CInput::ms_ClassDesc = 
{
	0,
	IT_INPUT,
	1,
	"Input by WW v0.1"
};


/**
*
*/
CInput::CInput() :
	m_pDI					( NULL	),
	m_pMouse				( NULL	),
	m_pKeyboard				( NULL	),
	m_nMouseBufferSize		( 0		),
	m_nMouseBufferPos		( 0		),
	m_nKeyboardBufferSize	( 0		),
	m_nKeyboardBufferPos	( 0		)
{
	for ( int i = 0; i < 256; ++i )
	{
		m_pMapASCII[ i ] = 0;
		m_pMapKeys[ i ] = 0;
	}

	m_pMapASCII[ 0 ]	= 0;	m_pMapKeys[ 0 ]		= 0;			// ???
	m_pMapASCII[ 1 ]	= 0;	m_pMapKeys[ 1 ]		= "esc";		// DIK_ESCAPE
	m_pMapASCII[ 2 ]	= '1';	m_pMapKeys[ 2 ]		= "1";			// DIK_1
	m_pMapASCII[ 3 ]	= '2';	m_pMapKeys[ 3 ]		= "2";			// DIK_2
	m_pMapASCII[ 4 ]	= '3';	m_pMapKeys[ 4 ]		= "3";			// DIK_3
	m_pMapASCII[ 5 ]	= '4';	m_pMapKeys[ 5 ]		= "4";			// DIK_4
	m_pMapASCII[ 6 ]	= '5';	m_pMapKeys[ 6 ]		= "5";			// DIK_5
	m_pMapASCII[ 7 ]	= '6';	m_pMapKeys[ 7 ]		= "6";			// DIK_6
	m_pMapASCII[ 8 ]	= '7';	m_pMapKeys[ 8 ]		= "7";			// DIK_7
	m_pMapASCII[ 9 ]	= '8';	m_pMapKeys[ 9 ]		= "8";			// DIK_8
	m_pMapASCII[ 10 ]	= '9';	m_pMapKeys[ 10 ]	= "9";			// DIK_8
	m_pMapASCII[ 11 ]	= '0';	m_pMapKeys[ 11 ]	= "0";			// DIK_8
	m_pMapASCII[ 12 ]	= '-';	m_pMapKeys[ 12 ]	= "minus";		// DIK_MINUS
	m_pMapASCII[ 13 ]	= '=';	m_pMapKeys[ 13 ]	= "equals";		// DIK_EQUALS
	m_pMapASCII[ 14 ]	= 0;	m_pMapKeys[ 14 ]	= "back";		// DIK_BACK

	m_pMapASCII[ 15 ]	= 9;	m_pMapKeys[ 15 ]	= "tab";		// DIK_TAB
	m_pMapASCII[ 16 ]	= 'q';	m_pMapKeys[ 16 ]	= "q";			// DIK_Q
	m_pMapASCII[ 17 ]	= 'w';	m_pMapKeys[ 17 ]	= "w";			// DIK_W
	m_pMapASCII[ 18 ]	= 'e';	m_pMapKeys[ 18 ]	= "e";			// DIK_E
	m_pMapASCII[ 19 ]	= 'r';	m_pMapKeys[ 19 ]	= "r";			// DIK_R
	m_pMapASCII[ 20 ]	= 't';	m_pMapKeys[ 20 ]	= "t";			// DIK_T
	m_pMapASCII[ 21 ]	= 'y';	m_pMapKeys[ 21 ]	= "y";			// DIK_Y
	m_pMapASCII[ 22 ]	= 'u';	m_pMapKeys[ 22 ]	= "u";			// DIK_U
	m_pMapASCII[ 23 ]	= 'i';	m_pMapKeys[ 23 ]	= "i";			// DIK_I
	m_pMapASCII[ 24 ]	= 'o';	m_pMapKeys[ 24 ]	= "o";			// DIK_O
	m_pMapASCII[ 25 ]	= 'p';	m_pMapKeys[ 25 ]	= "p";			// DIK_P
	m_pMapASCII[ 26 ]	= '[';	m_pMapKeys[ 26 ]	= "[";			// DIK_LBRACKET
	m_pMapASCII[ 27 ]	= ']';	m_pMapKeys[ 27 ]	= "]";			// DIK_RBRACKET
	m_pMapASCII[ 28 ]	= 13;	m_pMapKeys[ 28 ]	= "enter";		// DIK_RETURN

	m_pMapASCII[ 29 ]	= 0;	m_pMapKeys[ 29 ]	= "l_ctrl";		// DIK_LCONTROL
	m_pMapASCII[ 30 ]	= 'a';	m_pMapKeys[ 30 ]	= "a";			// DIK_A
	m_pMapASCII[ 31 ]	= 's';	m_pMapKeys[ 31 ]	= "s";			// DIK_S
	m_pMapASCII[ 32 ]	= 'd';	m_pMapKeys[ 32 ]	= "d";			// DIK_D
	m_pMapASCII[ 33 ]	= 'f';	m_pMapKeys[ 33 ]	= "f";			// DIK_F
	m_pMapASCII[ 34 ]	= 'g';	m_pMapKeys[ 34 ]	= "g";			// DIK_G
	m_pMapASCII[ 35 ]	= 'h';	m_pMapKeys[ 35 ]	= "h";			// DIK_H
	m_pMapASCII[ 36 ]	= 'j';	m_pMapKeys[ 36 ]	= "j";			// DIK_J
	m_pMapASCII[ 37 ]	= 'k';	m_pMapKeys[ 37 ]	= "k";			// DIK_K
	m_pMapASCII[ 38 ]	= 'l';	m_pMapKeys[ 38 ]	= "l";			// DIK_L
	m_pMapASCII[ 39 ]	= ';';	m_pMapKeys[ 39 ]	= ";";			// DIK_SEMICOLON
	m_pMapASCII[ 40 ]	= '\'';	m_pMapKeys[ 40 ]	= "apostrophe";	// DIK_APOSTROPHE
	m_pMapASCII[ 41 ]	= 0;	m_pMapKeys[ 41 ]	= "grave";		// DIK_GRAVE
	m_pMapASCII[ 42 ]	= 0;	m_pMapKeys[ 42 ]	= "l_shift";	// DIK_LSHIFT
	m_pMapASCII[ 43 ]	= '\\';	m_pMapKeys[ 43 ]	= "backslash";	// DIK_BACKSLASH

	m_pMapASCII[ 44 ]	= 'z';	m_pMapKeys[ 44 ]	= "z";			// DIK_Z
	m_pMapASCII[ 45 ]	= 'x';	m_pMapKeys[ 45 ]	= "x";			// DIK_X
	m_pMapASCII[ 46 ]	= 'c';	m_pMapKeys[ 46 ]	= "c";			// DIK_C
	m_pMapASCII[ 47 ]	= 'v';	m_pMapKeys[ 47 ]	= "v";			// DIK_V
	m_pMapASCII[ 48 ]	= 'b';	m_pMapKeys[ 48 ]	= "b";			// DIK_B
	m_pMapASCII[ 49 ]	= 'n';	m_pMapKeys[ 49 ]	= "n";			// DIK_N
	m_pMapASCII[ 50 ]	= 'm';	m_pMapKeys[ 50 ]	= "m";			// DIK_M
	m_pMapASCII[ 51 ]	= ',';	m_pMapKeys[ 51 ]	= "comma";		// DIK_COMMA
	m_pMapASCII[ 52 ]	= '.';	m_pMapKeys[ 52 ]	= "period";		// DIK_PERIOD
	m_pMapASCII[ 53 ]	= '/';	m_pMapKeys[ 53 ]	= "slash";		// DIK_SLASH
	m_pMapASCII[ 54 ]	= 0;	m_pMapKeys[ 54 ]	= "r_shift";	// DIK_RSHIFT
	m_pMapASCII[ 55 ]	= '*';	m_pMapKeys[ 55 ]	= "mult";		// DIK_MULTIPLY
	m_pMapASCII[ 56 ]	= 0;	m_pMapKeys[ 56 ]	= "l_menu";		// DIK_LMENU
	m_pMapASCII[ 57 ]	= ' ';	m_pMapKeys[ 57 ]	= "space";		// DIK_SPACE
	m_pMapASCII[ 58 ]	= 0;	m_pMapKeys[ 58 ]	= "capslock";	// DIK_CAPITAL
}


/**
*
*/
CInput::~CInput()
{
	m_InputHandlers.Delete();

	RELEASE( m_pMouse );
	RELEASE( m_pDI );
}


/**
*
*/
void CInput::Release()
{
	PURE_DEL( this );
}


/**
*
*/
EResult CInput::Init( HINSTANCE hInst, HWND hWindow )
{
	//IEasyWin * pWin = g_pSystem->GetWindow();

	HRESULT hr = DirectInput8Create( hInst, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pDI, NULL ); 

	m_pDI->CreateDevice( GUID_SysMouse,	&m_pMouse, NULL );
	m_pMouse->SetDataFormat( &c_dfDIMouse );
	m_pMouse->SetCooperativeLevel( hWindow, /*DISCL_NONEXCLUSIVE*/DISCL_EXCLUSIVE | DISCL_FOREGROUND );

	m_pDI->CreateDevice( GUID_SysKeyboard,	&m_pKeyboard,	NULL );
	m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ); 
	m_pKeyboard->SetCooperativeLevel( hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ); 

	m_hMouseEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	m_pMouse->SetEventNotification( m_hMouseEvent );

	DIPROPDWORD dipdw;
	// the header
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = MAX_BUFFER_SIZE;

	m_pMouse->SetProperty	( DIPROP_BUFFERSIZE, &dipdw.diph );
	m_pKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );

	return R_GENERIC_ERROR;
}


/**
* Регистрация обработчика событий ввода
* Замечание: новый обработчик ставиться в конец очереди
*/
EResult CInput::RegHandler( IInputHandler * pHandler )
{
	if ( pHandler )
	{
		DelHandler( pHandler );

		m_InputHandlers.AddToTail( pHandler );
		return R_OK;
	}

	return R_INVALID_ARG;
}


/**
* Удаление зарегистрированного обработчика
*/
void CInput::DelHandler( IInputHandler * pHandler )
{
	for ( index_t i = 0; i < m_InputHandlers.GetCount(); ++i )
	{
		if ( m_InputHandlers[ i ] == pHandler )
		{
			m_InputHandlers.Remove( i );
			break;
		}
	}
}


/**
* Обработка зарегистрированных обработчиков
*/
void CInput::ProcHandlers()
{
	TInputMsg tMsg;

	while ( GetInputMsg( tMsg ) )
	{
		for ( index_t i = 0; i < m_InputHandlers.GetCount(); ++i )
		{
			if ( m_InputHandlers[ i ]->ProcInputEvent( tMsg ) )
				break;
		}
	}
}


/**
* Заполняет буффер мышки
*/
size_t CInput::FillMouseBuffer()
{
	DIMOUSESTATE2 state;

	ZeroMemory(&state, sizeof(state)); 
	HRESULT h = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &state);	

	DWORD dwItems = MAX_BUFFER_SIZE;

	if ( FAILED( m_pMouse->Acquire() ) )
		return 0;

	if ( FAILED( m_pMouse->Poll() ) )
		return 0;

	if ( FAILED( m_pMouse->GetDeviceData(
		sizeof( DIDEVICEOBJECTDATA ),
		m_pMouseBuffer,
		&dwItems, 0 ) ) )
	{
		return 0;
	}

	return dwItems;
}


/**
* Заполняет буффер клавиатуры
*/
size_t CInput::FillKeyboardBuffer()
{
	DWORD              dwElements = MAX_BUFFER_SIZE;
	HRESULT            hResult;

	hResult = m_pKeyboard->GetDeviceData(
		sizeof( DIDEVICEOBJECTDATA ),
		m_pKeyboardBuffer,
		&dwElements, 0 );

	if ( DI_OK != hResult )
	{
		while( DIERR_INPUTLOST == m_pKeyboard->Acquire() )
		{
			// do nothing
		}

		return 0;
	}

	return dwElements;
}


/**
* Если есть сообщение, то возвращает истину.
* Замечание: сначала обрабатываются события мышки, затем клавиатуры.
*/
bool CInput::GetInputMsg( TInputMsg & tData )
{
	//==========================================================================
	// Tip: Process mouse events
	//==========================================================================
	if ( !m_nMouseBufferSize ) 
	{
		m_nMouseBufferSize	= FillMouseBuffer();
		m_nMouseBufferPos	= 0;
	}

	if ( m_nMouseBufferSize ) // если есть данные в буффере мыши
	{
		switch ( m_pMouseBuffer[ m_nMouseBufferPos ].dwOfs )
		{
			case DIMOFS_X: // Mouse horizontal motion
				tData.eType = IMT_MOUSE_MOVE_X;
				tData.dwData = m_pMouseBuffer[ m_nMouseBufferPos ].dwData;
			break;

			case DIMOFS_Y: // Mouse vertical motion
				tData.eType = IMT_MOUSE_MOVE_Y;
				tData.dwData = m_pMouseBuffer[ m_nMouseBufferPos ].dwData;
			break;

			case DIMOFS_BUTTON0:
				tData.eType = IMT_MOUSE_LKEY;
				tData.dwData= m_pMouseBuffer[ m_nMouseBufferPos ].dwData;
			break;

			case DIMOFS_BUTTON2:
				tData.eType = IMT_MOUSE_MKEY;
				tData.dwData= m_pMouseBuffer[ m_nMouseBufferPos ].dwData;
			break;
		}

		++m_nMouseBufferPos;

		if ( m_nMouseBufferPos >= m_nMouseBufferSize )
			m_nMouseBufferSize = 0;

		return true;
	}

	//==========================================================================
	// Tip: Process keyboard events
	//==========================================================================
	if ( !m_nKeyboardBufferSize ) 
	{
		m_nKeyboardBufferSize = FillKeyboardBuffer();
		m_nKeyboardBufferPos = 0;
	}

	if ( m_nKeyboardBufferSize ) // если есть данные в буффере клавиатуры
	{
		const DIDEVICEOBJECTDATA & kbdData = m_pKeyboardBuffer[ m_nKeyboardBufferPos ];

		tData.eType		= ( kbdData.dwData & 0x80 ) ? IMT_KEY_DOWN : IMT_KEY_UP;
		tData.dwData	= kbdData.dwOfs;

		++m_nKeyboardBufferPos;

		if ( m_nKeyboardBufferPos >= m_nKeyboardBufferSize )
			m_nKeyboardBufferSize = 0;

		return true;
	}

	//--------------------------------------------------------------------------
	// Tip: No events occured
	//--------------------------------------------------------------------------
	return false;
}


/**
*
*/
const char * CInput::GetKeyboardState()
{
	static char pBuffer[ 256 ];

	if ( SUCCEEDED( m_pKeyboard->GetDeviceState( 256, (LPVOID)&pBuffer ) ) )
	{
		return pBuffer;
	}

	return NULL;
}


/**
*
*/
char CInput::CodeToChar( dword dwDik ) const
{
	if ( dwDik < 256 )
		return m_pMapASCII[ dwDik ];
	
	return 0;
}


/**
*
*/
const char * CInput::CodeToName( dword dwDik ) const
{
	if ( dwDik < 256 )
		return m_pMapKeys[ dwDik ];

	return 0;
}


/**
*
*/
dword CInput::NameToCode( const char * szName ) const
{
	for ( dword i = 0; i < 256; ++i )
	{
		if ( CStr::Compare( szName, m_pMapKeys[ i ] ) )
			return i;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////