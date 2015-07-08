////////////////////////////////////////////////////////////////////////////////

#ifndef __Editor_h_included__
#define __Editor_h_included__

////////////////////////////////////////////////////////////////////////////////

class CEditorForm : public CWindow
{
	public :

		CEditorForm() :
		   CWindow( NULL )
		{
			m_pEntityList = NEW CListView( this );
		}

		~CEditorForm()
		{
			//DEL( m_pEntityList );
		}

		bool Init();

	private :

		CListView *	m_pEntityList;
};

////////////////////////////////////////////////////////////////////////////////

class CEditor : public IInputHandler
{
	public:

		// IInputHandler
		bool ProcInputEvent( const TInputMsg & tData );

		CEditor();
		~CEditor();

		void Activate( bool bActivate = true );

		inline bool IsActive() const
		{
			return m_bActive;
		}

		inline void	SetSelection( CSceneObject * pObject )
		{
			m_pSelection = pObject;
		}

		inline CSceneObject * GetSelection()
		{
			return m_pSelection;
		}

		inline CWindow * GetForm()
		{
			return m_pEditorForm;
		}

		void Update();

	private:

		bool			m_bActive;
		bool			m_bMidPressed;

		CSceneObject *	m_pSelection;
		CEditorForm * 	m_pEditorForm;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Editor_h_included__

////////////////////////////////////////////////////////////////////////////////