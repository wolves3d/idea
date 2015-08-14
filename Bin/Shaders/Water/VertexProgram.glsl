#version 330
uniform		mat4	mMVP;
uniform		mat4	modelView;
uniform		vec4	vLightPos;
uniform		vec4	eyePos;
uniform		vec4	waveParams;
/*
attribute	vec3 	vPos;
attribute	vec3 	vNormal;
//attribute	vec2 	vTex0;
attribute	vec3 	vTangent;
//attribute	vec3 	vBitangent;
*/
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vTangent;

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
	fragPosition = modelView * vec4(vPos, 1);
	vec2 vTex0 = vPos.xz * 0.09;
	texCoord		= vec2(vTex0.x + waveParams.x, vTex0.y);
	
	
//	fragLightPosition = (modelView * vLightPos).xyz;
	//fragEyePosition = (modelView * eyePos).xyz;
	
	fragEyePosition = (eyePos).xyz;
	
	mat3 normalMatrix = inverse(transpose(mat3(modelView)));
	//mat3 normalMatrix = mat3(modelView);
	
			//vec3 bitangent = normalize(cross(vNormal, vTangent));
			vec3 bitangent = normalize(cross(vTangent, vNormal));
	
			vec3 vertexNormal = normalMatrix * vNormal;
			vec3 vertexTangent = normalMatrix * vTangent;
			vec3 vertexBitangent = normalMatrix * bitangent;
			
			mat3 invTBN = transpose(mat3(
				normalize(vertexTangent),
				normalize(vertexBitangent),
				normalize(vertexNormal)
			));
	
			vec3 vertexPos_cameraspace = (modelView * vec4(vPos, 1)).xyz;
			fragEyePosition = vec3(0,0,0) - vertexPos_cameraspace;
			
			vec3 LightPosition_cameraspace = (modelView * vLightPos).xyz;
			vec3 LightDir_cameraspace = (LightPosition_cameraspace - vertexPos_cameraspace);
			fragLightPosition = invTBN * LightDir_cameraspace;	
			fragEyePosition = invTBN * fragEyePosition;
	
	//fragNormal = normalMatrix * vNormal;	
	//fragNormal = TBN * vNormal;
	//fragNormal = vec3(0,0,1);
	//fragNormal = vNormal;
	
	//fragNormal = (inverse(transpose(modelView)) * vec4(vNormal.xyz, 1)).xyz;
	
	
	gl_Position		= mMVP * vec4( vPos, 1);
	//gl_FrontColor	= vec4( 1, dot(vL, vNormal), 1, 1 );
}