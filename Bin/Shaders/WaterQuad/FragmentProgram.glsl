uniform sampler2D DiffuseMap;

in vec2 fragTexCoord;
in vec4 fragWaveParams;
in mat4 fragIVP;


vec3 PlaneCollision(vec3 vStart, vec3 vEnd)
{
	vec3 vNormal = vec3(0, 1, 0);
	vec3 vOrigin = vec3(0, 0, 0);
		
	float r1 = dot(vNormal, vStart - vOrigin);
	float r2 = dot(vNormal, vEnd - vOrigin);

	return (vStart + ((vEnd - vStart) * (-r1 / (r2 - r1))));
}


vec4 Proj(vec4 vertex)
{
	vec4 pos = fragIVP * vec4(vertex.x, vertex.y, vertex.z, 1.0);
	pos.w = 1.0 / pos.w;
	return vec4(pos.x * pos.w, pos.y * pos.w, pos.z * pos.w, 1.0);
}


vec4 WaterPlane(vec2 UV)
{
	vec2 nCoord = vec2((2.0 * UV.x) - 1.0, ((2.0 * UV.y) - 1.0));
	
	vec4 s = Proj(vec4(nCoord.x, nCoord.y, -1.0, 1.0));
	vec4 e = Proj(vec4(nCoord.x, nCoord.y, 1.0, 1.0));
	
	vec3 res = PlaneCollision(s.xyz, e.xyz);
	return vec4(res.x, res.y, res.z, 1);
}

float HeightFunc(vec2 UV, float time)
{
	float WaveLen = 50.0;
	float Amp = 0.3;
	
	vec3 Dir = vec3(0.0, 1.0, 0.0);
	
	return (Amp * sin((dot(Dir, UV) * WaveLen) + time));
}

void main(void)
{
	//gl_FragColor = texture2D( DiffuseMap, fragTexCoord.st );
	
	vec4 fragCoord = gl_FragCoord;
	
	float invRatio = (1.0 / 256.0);
	vec2 UV = vec2(fragCoord.x * invRatio, fragCoord.y * invRatio);
	
	vec4 waterPos = WaterPlane(UV);
	//vec2 samplerUV = vec2(0.1 * waterPos.x + fragWaveParams.x * 4, 0.1 * waterPos.z);
	//waterPos.y += texture2D(DiffuseMap, samplerUV).r;
	waterPos.y += HeightFunc(0.1 * waterPos.xz, fragWaveParams.x * 25);
	
	gl_FragColor = waterPos;
}