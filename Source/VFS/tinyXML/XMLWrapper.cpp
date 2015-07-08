////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "XMLWrapper.h"
#include "tinyxml.h"

////////////////////////////////////////////////////////////////////////////////

TIntfDesc CXMLWrapper::ms_ClassDesc = 
{
	2020,
	IT_UTIL,
	1,
	"XMLWrapper over tinyXML by WW"
};

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
const TIntfDesc * CXMLWrapper::GetInterfaceDesc() const
{
	return &ms_ClassDesc;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CXMLWrapper::Init()
{
	return R_OK;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CXMLWrapper::Release()
{
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CXMLWrapper::Delete()
{
	DEL( g_pVfs );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void Copy( const TiXmlElement * pTinyElem, CXMLElement * pElement )
{
	pElement->SetName( pTinyElem->Value() );

	for (	const TiXmlAttribute * pAttr = pTinyElem->FirstAttribute();
			NULL != pAttr;
			pAttr = pAttr->Next() )
	{
		pElement->AddAttr( pAttr->Name(), pAttr->Value() );
	}

	for (	const TiXmlElement * pChild = pTinyElem->FirstChildElement();
			NULL != pChild;
			pChild = pChild->NextSiblingElement() )
	{
		CXMLElement * pNewChild = NEW CXMLElement;
		Copy( pChild, pNewChild );
		pElement->AddChild( pNewChild );
	}
}

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CXMLWrapper::LoadXML( const char * szFileName, CXMLElement ** ppOutElement )
{
	if ( !szFileName )
		return R_INVALID_ARG;

	if ( !ppOutElement )
		return R_INVALID_ARG;

	TiXmlDocument tinyDoc;

	if ( !tinyDoc.LoadFile( szFileName ) )
		return R_GENERIC_ERROR;

	const TiXmlElement * pTinyRoot = tinyDoc.RootElement();

	(*ppOutElement) = NEW CXMLElement;
	Copy( pTinyRoot, (*ppOutElement) );

	return R_OK;
}




EResult CXMLWrapper::LoadXMLFromMem( char * pBuffer, uint nSize, CXMLElement ** ppOutElement )
{
	if ( !pBuffer )
		return R_INVALID_ARG;

	if ( !ppOutElement )
		return R_INVALID_ARG;

	TiXmlDocument tinyDoc;

	if ( !tinyDoc.LoadFile( NULL, TIXML_DEFAULT_ENCODING, pBuffer, nSize ) )
		return R_GENERIC_ERROR;
		
	const TiXmlElement * pTinyRoot = tinyDoc.RootElement();

	(*ppOutElement) = NEW CXMLElement;
	Copy( pTinyRoot, (*ppOutElement) );

	return R_OK;
}




/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CXMLWrapper::SaveXML( const char * szFileName, const CXMLElement * pElement )
{
	return R_GENERIC_ERROR;
}

////////////////////////////////////////////////////////////////////////////////