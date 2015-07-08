////////////////////////////////////////////////////////////////////////////////

#ifndef __StaticText_h_included__
#define __StaticText_h_included__

////////////////////////////////////////////////////////////////////////////////

class CStaticText : public CBaseControl
{
	public :

		CStaticText( CBaseControl * pParent = NULL );
		
		bool Create()
		{
			return false;
		}

		void Render();

		void			SetText			( const char * szText );

	private :

		dword	m_dwBackColor;	// ���� ����
		dword	m_dwTextColor;	// ���� ������

		CVertexBuffer * m_pTextVertices;
		static IVertexDecl * ms_pVertexDecl;
};

////////////////////////////////////////////////////////////////////////////////

#endif //#ifndef __StaticText_h_included__

////////////////////////////////////////////////////////////////////////////////