////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

#include "Editor.h"
#include "../Core/DeskTop.h"

/**
*
*/
CEditor::CEditor() :
	m_bActive		( false ),
	m_bMidPressed	( false )
{
	m_pEditorForm = NEW CEditorForm;

	if ( m_pEditorForm )
	{
		m_pEditorForm->SetText( "EditorForm" );

		CBaseControl * pCtrl = m_pEditorForm;
		pCtrl->Init();
	}
}


/**
*
*/
CEditor::~CEditor()
{
	//RELEASE( m_pEditorForm );
}


/**
*
*/
bool CEditorForm::Init()
{
	SetSize( 200, 600 );
	SetPos( 600, 0 );

	m_pEntityList = NEW CListView( this );
	m_pEntityList->SetPos( 5, 300 );
	m_pEntityList->SetSize( 100, 250 );

	m_pEntityList->AddNode( "ListView item One" );
	m_pEntityList->AddNode( "ListView item Two" );
	m_pEntityList->AddNode( "ListView item Three" );

	return true;				
}


/**
*
*/
bool CEditor::ProcInputEvent( const TInputMsg & tData )
{
	vec3 vOffset( 0, 0, 0 );

	switch ( tData.eType )
	{
		case IMT_MOUSE_MKEY:
			m_bMidPressed = ( 0 != ( tData.dwData & 0x80 ) );
		break;

		case IMT_MOUSE_MOVE_X:

			if ( m_bMidPressed )
			{
				vOffset.x = ( -0.1f ) * (int)tData.dwData;
			}
			
		break;

		case IMT_MOUSE_MOVE_Y:

			if ( m_bMidPressed )
			{
				vOffset.z = ( -0.1f ) * (int)tData.dwData;
			}

		break;

		case IMT_MOUSE_LKEY:

			if ( tData.dwData & 0x80 )
			{
				/*
				// trace model to select here!

				// iterate world by view frustum
				g_pWorld->Reset();

				for (	IEntity * pEnt = g_pWorld->Next();
						NULL != pEnt;
						pEnt = g_pWorld->Next() )
				{
					IModel * pModel = pEnt->GetModel();

					// for every bbox in iteration
					bbox bbTest = pModel->GetBBox();
					//bbTest.Translate( pEnt->GetMatrix() );

					bbTest.vMin = pEnt->GetPos();
					bbTest.vMax.Set( 12, 12, 12 );

					//g_pEngine->PushBBox( pModel->GetBBox() );
					g_pRenderer->Line3D( bbTest.vMin, bbTest.vMax, 0xFFFFFFFF );
					g_pRenderer->Axis3D( bbTest.vMin );
					g_pRenderer->Axis3D( pEnt->GetMatrix() );
				}
				*/
			}
			
		break;
	}



	if ( m_bMidPressed )
	{
		quat * qCam = g_pGame->m_pCamera->GetQuat();
		mat4 m;
		qCam->ToMatrix( m );
		vOffset.Multiply( m );
		vOffset.y = 0;
	}

	g_pGame->m_pCamera->Move( vOffset );


	// trace model to select here!


	return false;
}


void CEditor::Update()
{
	// iterate world by view frustum
	g_pWorld->Reset();

	for (	IEntity * pEnt = g_pWorld->Next();
		NULL != pEnt;
		pEnt = g_pWorld->Next() )
	{
		IModel * pModel = pEnt->GetModel();

		pEnt->Update();

		// for every bbox in iteration
		bbox bbTest = pModel->GetBBox();
		bbTest.Translate( pEnt->GetMatrix() );

		g_pRenderer->BBox( bbTest );
		//g_pRenderer->Line3D( bbTest.vMin, bbTest.vMax, 0xFFFFFFFF );
		//g_pRenderer->Axis3D( bbTest.vMin );
		//g_pRenderer->Axis3D( pEnt->GetMatrix() );
		//pEnt->Render();
	}
}


/**
*
*/
void CEditor::Activate( bool bActivate )
{
	IUIManager * pMgr = g_pEngine->GetUIManager();

	m_bActive = bActivate;

	if ( m_bActive )
	{
		g_pInput->RegHandler( this );
		pMgr->SetDesktop( GetForm() );
	}
	else
	{
		g_pInput->DelHandler( this );
		pMgr->SetDesktop( g_pGame->m_pDeskTop );
	}
}