////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
//#include "Shaders/BaseShader.h"

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::Line2D( const vec2 & vStart, const vec2 & vEnd, dword dwColor )
{
	glDisable( GL_TEXTURE_2D );
	GL_VALIDATE;

	glBegin( GL_LINES );
	GL_VALIDATE;

	glColor4ub(
		*( (GLubyte *)&dwColor + 2 ),
		*( (GLubyte *)&dwColor + 1 ),
		*( (GLubyte *)&dwColor ),
		*( (GLubyte *)&dwColor + 3 ) );
	GL_VALIDATE;

	glVertex2fv( vStart.pArray );
	GL_VALIDATE;

	glVertex2fv( vEnd.pArray );
	GL_VALIDATE;

	glEnd();
	GL_VALIDATE;

	glEnable( GL_TEXTURE_2D );
	GL_VALIDATE;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::Line3D( const vec3 & vStart, const vec3 & vEnd, dword dwColor )
{
	EnableDepthTest( false );

	glDisable( GL_TEXTURE_2D );
	GL_VALIDATE;

	glBegin( GL_LINES );

		glColor4ub(
			*( (GLubyte *)&dwColor + 2 ),
			*( (GLubyte *)&dwColor + 1 ),
			*( (GLubyte *)&dwColor ),
			*( (GLubyte *)&dwColor + 3 ) );

		glVertex3fv( vStart.pArray );
		glVertex3fv( vEnd.pArray );

	glEnd();
	GL_VALIDATE;

	glEnable( GL_TEXTURE_2D );
	GL_VALIDATE;

	EnableDepthTest();

	glColor4ub( 0xFF, 0xFF, 0xFF, 0xFF );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::Axis3D( const vec3 & vOrigin )
{
	Line3D( vOrigin, vOrigin + vec3( 1, 0, 0 ), 0xFFFF0000 );
	Line3D( vOrigin, vOrigin + vec3( 0, 1, 0 ), 0xFF00FF00 );
	Line3D( vOrigin, vOrigin + vec3( 0, 0, 1 ), 0xFF0000FF );
}


void CRenderer_GL::BBox( const bbox & box )
{
	Line3D( box.vMin, vec3( box.vMin.x, box.vMax.y, box.vMin.z ), 0xFFFF0000 );
	Line3D( vec3( box.vMin.x, box.vMax.y, box.vMin.z ), vec3( box.vMax.x, box.vMax.y, box.vMin.z ), 0xFFFF0000 );
	Line3D( vec3( box.vMax.x, box.vMax.y, box.vMin.z ), vec3( box.vMax.x, box.vMin.y, box.vMin.z ), 0xFFFF0000 );
	Line3D( vec3( box.vMax.x, box.vMin.y, box.vMin.z ), box.vMin, 0xFFFF0000 );

	Line3D( vec3( box.vMin.x, box.vMin.y, box.vMax.z ), vec3( box.vMin.x, box.vMax.y, box.vMax.z ), 0xFFFF0000 );
	Line3D( vec3( box.vMin.x, box.vMax.y, box.vMax.z ), vec3( box.vMax.x, box.vMax.y, box.vMax.z ), 0xFFFF0000 );
	Line3D( vec3( box.vMax.x, box.vMax.y, box.vMax.z ), vec3( box.vMax.x, box.vMin.y, box.vMax.z ), 0xFFFF0000 );
	Line3D( vec3( box.vMax.x, box.vMin.y, box.vMax.z ), vec3( box.vMin.x, box.vMin.y, box.vMax.z ), 0xFFFF0000 );

	Line3D( vec3( box.vMin.x, box.vMin.y, box.vMin.z ), vec3( box.vMin.x, box.vMin.y, box.vMax.z ), 0xFFFF0000 );
	Line3D( vec3( box.vMin.x, box.vMax.y, box.vMin.z ), vec3( box.vMin.x, box.vMax.y, box.vMax.z ), 0xFFFF0000 );
	Line3D( vec3( box.vMax.x, box.vMax.y, box.vMin.z ), vec3( box.vMax.x, box.vMax.y, box.vMax.z ), 0xFFFF0000 );
	Line3D( vec3( box.vMax.x, box.vMin.y, box.vMin.z ), vec3( box.vMax.x, box.vMin.y, box.vMax.z ), 0xFFFF0000 );
}



/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::Axis3D( const mat4 & mWorld )
{
	vec3 vAxis;
	vec3 vOrigin( 0, 0, 0 );
	vOrigin.Multiply( mWorld );

	vAxis.Set( 1, 0, 0 );
	vAxis.Multiply( mWorld );
	Line3D( vOrigin, vAxis, 0xFFFF0000 );

	vAxis.Set( 0, 1, 0 );
	vAxis.Multiply( mWorld );
	Line3D( vOrigin, vAxis, 0xFF00FF00 );

	vAxis.Set( 0, 0, 1 );
	vAxis.Multiply( mWorld );
	Line3D( vOrigin, vAxis, 0xFF0000FF );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::Circle3D( const mat4 & mWorld, float fRadius, dword dwColor )
{
	const int	nSegments	= 64;
	const float	fAngle		= ( 2 * M_PI ) / nSegments;

	for ( int n = 0; n < ( nSegments + 1 ); ++n )
	{
		vec3 vStart(	fRadius * sinf( fAngle * n ),
						fRadius * cosf( fAngle * n ),
						0 );

		vec3 vEnd(	fRadius * sinf( fAngle * ( n + 1 ) ),
					fRadius * cosf( fAngle * ( n + 1 ) ),
					0 );

		vStart.Multiply( mWorld );
		vEnd.Multiply( mWorld );
		
		Line3D( vStart, vEnd, dwColor );
	}
}

////////////////////////////////////////////////////////////////////////////////