#version 150
uniform		mat4	mMVP;
uniform		mat4	modelView;
uniform		vec4	vLightPos;
uniform		vec4	eyePos;
uniform		vec4	waveParams;

attribute	vec3 	vPos;
attribute	vec2 	vTex0;
attribute	vec3 	vTangent;
attribute	vec3 	vBitangent;
attribute	vec3 	vNormal;

varying		vec2	texCoord;
//varying		vec4	colorMod;

varying	vec4 fragPosition;
varying	vec3 fragLightPosition;
varying	vec3 fragEyePosition;

varying	vec3 fragTangent;
varying	vec3 fragBitangent;
varying	vec3 fragNormal;

void main()
{
	texCoord		= vec2(vTex0.x + waveParams.x, vTex0.y);
	
	fragPosition = modelView * vec4(vPos, 1);
	
	fragLightPosition = (modelView * vLightPos).xyz;
	
	//fragEyePosition = (modelView * eyePos).xyz;
	
	fragEyePosition = (eyePos).xyz;
	
	mat3 normalMatrix = mat3(modelView);
	
			//normalMatrix = inverse(transpose(normalMatrix));
			//normalMatrix = inverse(transpose(normalMatrix));
	
			vec3 vertexTangent = normalMatrix * vTangent;
			vec3 vertexBitangent = normalMatrix * vBitangent;
			vec3 vertexNormal = normalMatrix * vNormal;
			
			mat3 TBN = transpose(mat3(
				vertexTangent,
				vertexBitangent,
				vertexNormal
			));
			
			//fragNormal = vertexNormal;
	
			vec3 vertexPos_cameraspace = (modelView * vec4(vPos, 1)).xyz;
			fragEyePosition = vec3(0,0,0) - vertexPos_cameraspace;
			
			vec3 LightPosition_cameraspace = (modelView * vLightPos).xyz;
			vec3 LightDir_cameraspace = LightPosition_cameraspace - vertexPos_cameraspace;
			
			fragLightPosition = TBN * LightDir_cameraspace;	
			//fragLightPosition = TBN * LightDir_cameraspace;
	
	//fragNormal = normalMatrix * vNormal;	
	//fragNormal = TBN * vNormal;
	
	gl_Position		= mMVP * vec4( vPos, 1);
	//gl_FrontColor	= vec4( 1, dot(vL, vNormal), 1, 1 );
}