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


vec4 GrestnerFunc(vec2 UV, vec2 Dir, vec4 params)
{
	float PI = 3.14159265358979323846264;
	
	float WaveLen = params.x;
	float Amp = params.y;
	float Q = params.z;
	float time = params.w;
	
	float Freq = (2 * PI) / WaveLen;
	//vec2 Dir = vec2(1.0, 0.0);
	
	float sineArg = ((Freq * dot(Dir, UV)) + time);
	float C = cos(sineArg);
	float S = sin(sineArg);
	
	return vec4(
		UV.x + (Q*Amp) * (Dir.x) * C,
		Amp * S,
		UV.y + (Q*Amp) * (Dir.y) * C,
		1);
}


void main(void)
{
	//gl_FragColor = texture2D( DiffuseMap, fragTexCoord.st );
	
	vec4 fragCoord = gl_FragCoord;
	
	float invRatio = (1.0 / 256.0);
	vec2 UV = vec2(fragCoord.x * invRatio, fragCoord.y * invRatio);
	
	vec4 waterPos = WaterPlane(UV);
	float time = fragWaveParams.x * 25;	
	vec4 waveA = GrestnerFunc(waterPos.xz, vec2(0.0, 1.0), vec4(10.0, 0.6, 0.3, time));
	vec4 waveB = GrestnerFunc(waterPos.xz, normalize(vec2(1.0, 0.0)), vec4(7.0 + 1 * sin(time * 0.3), 0.7, 1.7, time));
	vec4 waveC = GrestnerFunc(waterPos.xz, normalize(vec2(1.0, 1.0)), vec4(3.0, 0.5, 1.5, time * 0.5));
	
	waterPos = (waveA + waveB + waveC) / 3;
	
	vec2 samplerUV = vec2(0.1 * waterPos.x - fragWaveParams.x * 4, 0.1 * waterPos.z);
	waterPos.y += 3 * texture2D(DiffuseMap, samplerUV).r;
	
	gl_FragData[0] = waterPos;
	gl_FragData[1] = normalize(waterPos) * (-1);
}