////////////////////////////////////////////////////////////////////////////////

#ifndef __System_h_included__
#define __System_h_included__

////////////////////////////////////////////////////////////////////////////////
class CSystem : public ISystem
{
	public :

		CSystem ();

		///  init drief desc
		bool	Init	();
		///  release drief desc
		void	Release	();


		ICounter *		AddCounter		( const char * szName );
		ICounter *		GetCounter		( uint nCounter );
		uint			GetCounterCount	();
		void			RemoveCounter	( ICounter * pCounter );
		void			StartCount		( ICounter * pCounter );
		void			StopCount		( ICounter * pCounter );

		//------------------------------------------------------------------
		//	Tip: Memory management
		//------------------------------------------------------------------
		void *	Alloc	( size_t nSize, const char * szFileName, int nLine );
		void	Delete	( void * pData );
		void	DumpLeaks();

		uint GetSizeOf( ETypeID eType );
		ETypeID GetType( const char * szType );

		//------------------------------------------------------------------
		//	Tip: Library management
		//------------------------------------------------------------------
		bool		Init			( const char * szPath = NULL );
		void		InitModules		( const char * szPath = NULL );
		ILibrary *	LoadLibrary		( const char * szFileName );
		IBaseIntf *	CreateInterface	( int nIntfGUID );
		IBaseIntf *	GetInterface	( EInterfaceType eType );

		void AssertMsg( const char * szFile, int nLine, const char * szString );
		void WriteLog( const char * szLog );

		IClassDesc *	CreateClassDesc	( const char * szName, void * pFunc );
		void *			CreateClassInst	( const char * szName );

		ITestRes *		CreateTestRes	();

		bool			CreateVarStorage();
		IVarStorage *	GetVarStorage	();

		void * operator new( size_t bytes )
		{
			return malloc( bytes );
		}

	//----------------------------------------------------------------------
	//	Tip: Fields( vars )
	//----------------------------------------------------------------------

		struct TLibrary
		{
			typedef ILibrary *	(* PFNInitLibrary )		( ISystem * pSystem );
			typedef void		(* PFNUninitLibrary )	();

			HMODULE				hModule;
			PFNInitLibrary		pInitLibrary;
			PFNUninitLibrary	pUninitLibrary;
		};

		struct TIntfPath
		{
			int			nGUID;
			ILibrary *	pLibrary;
			int			nIntfID;
		};

		CContainer <TLibrary *> m_Librarys;
		CContainer <TIntfPath>	m_Interfaces;

		IBaseIntf * m_pModules[ IT_MAX_TYPES ];

	private :

		CStack <IClassDesc *> m_Classes;
		FILE * m_pLogFile;

		CStack <ICounter *> m_Counters;

		ICounter * m_pMemCounter;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __System_h_included__

////////////////////////////////////////////////////////////////////////////////