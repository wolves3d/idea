////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "MaterialEditor.h"
#include "MaterialEditorForm.h"


CMaterialEditor::CMaterialEditor() :
	m_bActive( false )
{
	m_pForm = NEW CMaterialEditorForm();
	m_pForm->SetText( "Material Editor Form" );
}


/**
*
*/
void CMaterialEditor::Activate( bool bActivate )
{
	IUIManager * pMgr = g_pEngine->GetUIManager();

	m_bActive = bActivate;

	if ( m_bActive )
	{
		pMgr->PushDesktop();
		pMgr->SetDesktop( m_pForm );
	}
	else
	{
		pMgr->PopDesktop();
	}
}