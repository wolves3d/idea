///////////////////////////////////////////////////////////////////////////////

#ifndef __EasyWin_h_included__
#define __EasyWin_h_included__

///////////////////////////////////////////////////////////////////////////////

class CEasyWin : public IEasyWin
{
	public :

		CEasyWin	();

		bool		Create		( HINSTANCE hInst );
		void		Destroy		(){};
		void		SetPosition	( int x, int y );
		void		SetSize	( int nWidth, int nHeight );
		void		GetSize	( int * pOutWidth, int * pOutHeight ) const;
		void		SetTitle	( const char *szTitle );

		bool		Idle			();
		bool		WaitForMessage	();

		//----------------------------------------------------------------------
		//	Tip: Inline methods
		//----------------------------------------------------------------------
		inline HWND	GetHandle() const
		{
			return m_hWnd;
		}

		inline HINSTANCE GetInstance() const
		{
			return m_hInstance;
		}

	private :

		HWND  m_hWnd;
		POINT m_ptSize;
		HINSTANCE m_hInstance;
};

///////////////////////////////////////////////////////////////////////////////

#endif // __EasyWin_h_included__

///////////////////////////////////////////////////////////////////////////////