////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>

#include "_Shared/Engine/UI.h"
#include "DeskTop.h"

#include "Engine/Scene/Scene.h"
#include "../Editor/Editor.h"


bool CDeskTop::ProcessInput( const TInputMsg & tData )
{
	/*if ( CWindow::ProcessInput( tData ) )
		return true;	
*/
	////////////////////////////////////////////////////////////////////////////

	quat * qCam = g_pGame->m_pCamera->GetQuat();

	const float fAngle = 0.1f * (int)tData.dwData;

	switch( tData.eType )
	{
		case IMT_MOUSE_MOVE_X:
		{
			float f = fAngle;
			quat qRot;
			qRot.vDir.Set( 0, 1, 0 );
			qRot.fAngle = cosf( 0.5f * f * TO_RADIAN );
			qRot.vDir.Scale( sinf( 0.5f * f * TO_RADIAN ) );
			qCam->Mult( qRot );
		}
		break;

		case IMT_MOUSE_MOVE_Y:
		{
			mat4 m;
			qCam->ToMatrix( m );

			float f = fAngle;

			quat qRot;
			qRot.vDir.Set( 1, 0, 0 );
			qRot.vDir.Multiply( m );
			qRot.fAngle = cosf( 0.5f * f * TO_RADIAN );
			qRot.vDir.Scale( sinf( 0.5f * f * TO_RADIAN ) );

			//CSceneObject * pObj = g_pEditor->GetSelection();
			//pObj->m_qRot.Mult( qRot );
			qCam->Mult( qRot );
		}
		break;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////