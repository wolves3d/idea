////////////////////////////////////////////////////////////////////////////////

#ifndef __XMLWrapper_h_included__
#define __XMLWrapper_h_included__

////////////////////////////////////////////////////////////////////////////////

#include "_Shared/API/IXMLWrapper.h"

////////////////////////////////////////////////////////////////////////////////

class CXMLWrapper : public IXMLWrapper
{
	public :

		//----------------------------------------------------------------------
		// Tip: IBaseInterface
		//----------------------------------------------------------------------
		const TIntfDesc *	GetInterfaceDesc() const;
		EResult				Init			();
		void				Release			();
		void				Delete			();

		//----------------------------------------------------------------------
		// Tip: IXMLWrapper
		//----------------------------------------------------------------------
		EResult	LoadXML( const char * szFileName, CXMLElement ** ppOutElement );
		EResult	LoadXMLFromMem( char * pBuffer, uint nSize, CXMLElement ** ppOutElement );
		EResult	SaveXML( const char * szFileName, const CXMLElement * pRootElement );

		static	TIntfDesc	ms_ClassDesc;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __XMLWrapper_h_included__

////////////////////////////////////////////////////////////////////////////////