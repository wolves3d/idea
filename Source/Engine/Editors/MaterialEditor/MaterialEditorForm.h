////////////////////////////////////////////////////////////////////////////////

#ifndef __MaterialEditorForm_h_included__
#define __MaterialEditorForm_h_included__

////////////////////////////////////////////////////////////////////////////////

class CMaterialEditorForm : public CWindow
{
	public :

	CMaterialEditorForm() :
	   CWindow( NULL )
	{
		SetSize( 200, 600 );
		SetPos( 600, 0 );

		m_pEntityList = NEW CListView( this );
		m_pEntityList->SetPos( 5, 300 );
		m_pEntityList->SetSize( 185, 200 );

		m_pEntityList->AddNode( "One" );
		m_pEntityList->AddNode( "Two" );
		m_pEntityList->AddNode( "Three" );
	  }

	private :

	CListView * m_pEntityList;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __MaterialEditorForm_h_included__

////////////////////////////////////////////////////////////////////////////////