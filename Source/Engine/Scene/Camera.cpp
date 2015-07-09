#include "Common.h"
#include "gl/glu.h"

class CCamera : public ICamera
{
	public :

		inline CCamera()
		{
			m_vPos.Set( 0, 0, 10 );

			m_qOrient.vDir.x = 0;
			m_qOrient.vDir.y = 0;
			m_qOrient.vDir.z = 0;
			m_qOrient.fAngle = 1;

			m_vAngles.Set( 0, 0, 0 );

			SetParams(34.516f, 800, 600, 1, 200);
		}

		inline void Move( const vec3 & vOffset )
		{
			m_vPos.Add( vOffset );
		}

		void SetPos(const vec3 & newPos)
		{
			m_vPos = newPos;
		}

		void SetParams(float fov, float width, float height, float nearClip, float farClip)
		{
			m_FOV = fov;
			m_viewport.Set(width, height);
			m_near = nearClip;
			m_far = farClip;

			m_vViewport.x = 0;
			m_vViewport.y = 0;
			m_vViewport.z = m_viewport.x;
			m_vViewport.w = m_viewport.y;

			UpdateProjection();
		}

		void UpdateProjection()
		{
			const float aspectRatio = (m_viewport.x / m_viewport.y);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(m_FOV, aspectRatio, m_near, m_far);
			glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat *)m_mProj.pArray);

			m_mProj.SetProjection(m_FOV, aspectRatio, m_near, m_far);
		}

		inline void Update()
		{
			m_mView.Identity();

			mat4 m;
			//m_qOrient.ToMatrix( m );
			
			quat qInv = m_qOrient;
			qInv.vDir.Set( -qInv.vDir.x, -qInv.vDir.y, -qInv.vDir.z );
			qInv.ToMatrix( m );
			vec3 v( -m_vPos.x, -m_vPos.y, -m_vPos.z );

			m_mView.Translate( v );
			m_mView.Mult( m );

			m_mVP = m_mView;
			m_mVP.Mult( m_mProj );
		}

		void LookAt(const vec3 & targetPoint, const vec3 & up)
		{
			vec3 viewDirection = (m_vPos - targetPoint).Normalize();	// z-axis (back-view, openGL specific?)
			vec3 side = Cross(viewDirection, up).Normalize();			// x-axis
			vec3 newUp = Cross(viewDirection, side);					// y-axis

			g_pEngine->PushLine(m_vPos, targetPoint, 0xFFFF0000);
			g_pEngine->PushLine(m_vPos, m_vPos - viewDirection, 0xFF0000FF);
			g_pEngine->PushLine(m_vPos, m_vPos + side, 0xFFFF0000);
			g_pEngine->PushLine(m_vPos, m_vPos + newUp, 0xFF00FF00);
			
			mat4 m(mat4::mIdentity);
			m.vRow0.Set(side, 0);
			m.vRow1.Set(newUp, 0);
			m.vRow2.Set(viewDirection, 0);

			m_qOrient.FromRotationMatrix(m);
		}

		inline quat * GetQuat()
		{
			return &m_qOrient;
		}

		inline vec3 * GetAngles()
		{
			return &m_vAngles;
		}

		inline const vec3 & GetPos()
		{
			return m_vPos;
		}

		const mat4 & GetViewMatrix() const
		{
			return m_mView;
		}

		const mat4 & GetProjMatrix() const
		{
			return m_mProj;
		}

		const mat4 & GetViewProjMatrix() const
		{
			return m_mVP;
		}

		bool Unproject( vec3 * pOutRay, const ivec2 & vScreen )
		{
			if ( NULL == pOutRay )
				return false;

			mat4 mInvViewProj( GetViewProjMatrix() );

			// inverting ViewProj matrix
			if ( false == mInvViewProj.Inverse())
				return false;

			// Позиция курсора считается от левого верхнего угла
			// а нужно от левого нижнего
			const float fScreenY = float(m_vViewport.w - vScreen.y);

			vec4 vNearPoint(
				( 2 * (float)vScreen.x / m_vViewport.z ) - 1,	// Map to range -1 to 1
				( 2 * (float)fScreenY / m_vViewport.w ) - 1,	// Map to range -1 to 1
				-1, 1 );										// Near plane 

			vec4 vFarPoint(
				( 2 * (float)vScreen.x / m_vViewport.z ) - 1,	// Map to range -1 to 1
				( 2 * (float)fScreenY / m_vViewport.w ) - 1,	// Map to range -1 to 1
				1, 1 );											// Far plane 

			// transforming form screen_space to world_space
			vNearPoint.Multiply( mInvViewProj );

			if ( 0.f == vNearPoint.w )
				return false;

			// transforming form screen_space to world_space
			vFarPoint.Multiply( mInvViewProj );

			if ( 0.f == vFarPoint.w )
				return false;

			vNearPoint.w	= 1.f / vNearPoint.w;
			vFarPoint.w		= 1.f / vFarPoint.w;

			pOutRay[ 0 ].Set(	vNearPoint.x * vNearPoint.w,
								vNearPoint.y * vNearPoint.w,
								vNearPoint.z * vNearPoint.w );

			pOutRay[ 1 ].Set(	vFarPoint.x * vFarPoint.w,
								vFarPoint.y * vFarPoint.w,
								vFarPoint.z * vFarPoint.w );

			return true;
		}

		void RenderFrustum()
		{
			Update(); // hack

			//vec3 vOrigin = (m_vPos * m_mVP);
			//vec3 vDirection = vec3(0, 0, 5).Rotate(m_mVP);

			vec3 pRayBL[2];
			vec3 pRayBR[2];
			vec3 pRayTR[2];
			vec3 pRayTL[2];

			Unproject(pRayBL, ivec2(0, 0));
			Unproject(pRayBR, ivec2(m_viewport.x, 0));
			Unproject(pRayTR, ivec2(m_viewport.x, m_viewport.y));
			Unproject(pRayTL, ivec2(0, m_viewport.y));

			g_pEngine->PushLine(pRayBL[0], pRayBL[1]);
			g_pEngine->PushLine(pRayBR[0], pRayBR[1]);
			g_pEngine->PushLine(pRayTR[0], pRayTR[1]);
			g_pEngine->PushLine(pRayTL[0], pRayTL[1]);

			// near plane
			g_pEngine->PushLine(pRayBL[0], pRayBR[0]);
			g_pEngine->PushLine(pRayBR[0], pRayTR[0]);
			g_pEngine->PushLine(pRayTR[0], pRayTL[0]);
			g_pEngine->PushLine(pRayTL[0], pRayBL[0]);

			// far plane
			g_pEngine->PushLine(pRayBL[1], pRayBR[1]);
			g_pEngine->PushLine(pRayBR[1], pRayTR[1]);
			g_pEngine->PushLine(pRayTR[1], pRayTL[1]);
			g_pEngine->PushLine(pRayTL[1], pRayBL[1]);
		}

		const vec2 & GetViewport()
		{
			return m_viewport;
		}

	private :

		bool m_isMatrixDirty;

		float m_FOV, m_near, m_far;
		vec2 m_viewport;

		//mat4 m_mWorld;
		vec3 m_vPos;
		quat m_qOrient;
		vec3 m_vAngles;

		//bool	m_bInvViewProjValid;

		mat4	m_mView; //! < Видовая матрица
		mat4	m_mProj; //! < Матрица проекции
		mat4	m_mVP;
		ivec4	m_vViewport;
};


ICamera * CEngine::CreateCamera()
{
	return NEW CCamera;
}


void CEngine::SetCamera( ICamera * pCamera )
{
	m_pCamera = pCamera;
}


const ICamera * CEngine::GetCamera()
{
	return m_pCamera;
}