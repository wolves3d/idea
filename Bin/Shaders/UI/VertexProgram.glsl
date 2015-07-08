uniform mat4x4	mMVP;
uniform vec4	vOffset;
uniform vec4	vColorMod;

attribute vec2 vPos;
attribute vec2 vTex0;

void main()
{
	gl_Position = mMVP * vec4( vPos.x, vPos.y, 0, 1 );
	
	gl_TexCoord[ 0 ] = /*gl_TextureMatrix[ 0 ] */ vec4( vTex0, 1, 1 );

	gl_FrontColor = vColorMod;
}