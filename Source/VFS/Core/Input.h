////////////////////////////////////////////////////////////////////////////////

#include "_Shared/API/IInput.h"

#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>

/**
*
*/
class CInput : public IInput
{
	//==========================================================================
	// Tip: Public Methods & Fields
	//==========================================================================
	public:

	//-------------------------------------------------------------------------
	// Tip: IBaseIntf
	//-------------------------------------------------------------------------
	const TIntfDesc *	GetInterfaceDesc() const { return &ms_ClassDesc; }
	EResult				Init			( HINSTANCE hInst, HWND hWindow );
	void				Release			();
	void				Delete			() { delete this; }

	static				TIntfDesc		ms_ClassDesc;

	//-------------------------------------------------------------------------
	// Tip: IInput
	//-------------------------------------------------------------------------
	EResult				RegHandler		( IInputHandler * pHandler );
	void				DelHandler		( IInputHandler * pHandler );
	void				ProcHandlers	();

	bool				GetInputMsg		( TInputMsg & tData );
	const char *		GetKeyboardState();

	char				CodeToChar		( dword dwDik ) const;
	const char *		CodeToName		( dword dwDik ) const;
	dword				NameToCode		( const char * szName ) const;

	//-------------------------------------------------------------------------
	// Tip: CInput
	//-------------------------------------------------------------------------
			CInput				();
			~CInput				();
	size_t	FillMouseBuffer		();
	size_t	FillKeyboardBuffer	();

	//==========================================================================
	// Tip: Private Methods & Fields
	//==========================================================================
	private:

	CVector <IInputHandler *> m_InputHandlers;

	#define					MAX_BUFFER_SIZE  16
	LPDIRECTINPUT8			m_pDI;

	LPDIRECTINPUTDEVICE8	m_pMouse;
	HANDLE					m_hMouseEvent;
	size_t					m_nMouseBufferSize;
	size_t					m_nMouseBufferPos;
	DIDEVICEOBJECTDATA		m_pMouseBuffer[ MAX_BUFFER_SIZE ];

	LPDIRECTINPUTDEVICE8	m_pKeyboard;
	size_t					m_nKeyboardBufferSize;
	size_t					m_nKeyboardBufferPos;
	DIDEVICEOBJECTDATA		m_pKeyboardBuffer[ MAX_BUFFER_SIZE ];

	char					m_pMapASCII[ 256 ];
	const char *			m_pMapKeys[ 256 ];
};

////////////////////////////////////////////////////////////////////////////////