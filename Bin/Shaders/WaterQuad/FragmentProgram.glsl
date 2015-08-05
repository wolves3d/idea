uniform sampler2D DiffuseMap;

in vec4 fragWaveSetup;
in vec4 fragWaveDirs[4];
in vec4 fragWaveParams[4];

in vec2 fragTexCoord;
//in vec4 fragWaveParams;
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


struct WaveVertex
{
	vec3 pos;
	vec3 normal;
	vec3 tangent;
};


WaveVertex GerstnerFunc(vec2 UV, int waveCount)
{
	float PI = 3.14159265358979323846264;	
	
	WaveVertex res;
	res.pos = vec3(UV.x, 0.0, UV.y);

	for (int i = 0; i < waveCount; ++i)
	{
		vec2 Dir = normalize(fragWaveDirs[i].xy);
		float WaveLen = fragWaveParams[i].x;
		float Amp = fragWaveParams[i].y;
		float Q = fragWaveParams[i].z;
		float time = fragWaveParams[i].w;

		float Freq = (2 * PI) / WaveLen;
		float sineArg = ((Freq * dot(Dir, UV)) + time);
		float C = cos(sineArg);
		float S = sin(sineArg);
	
		res.pos += vec3(
			(Q*Amp) * (Dir.x) * C,
			Amp * S,
			(Q*Amp) * (Dir.y) * C);
	}

	vec2 P = vec2(res.pos.xz);
	
	// normal
	
	res.normal = vec3(0, 1, 0);
	for (int i = 0; i < waveCount; ++i)
	{	
		vec2 Dir = normalize(fragWaveDirs[i].xy);
		float WaveLen = fragWaveParams[i].x;
		float Amp = fragWaveParams[i].y;
		float Q = fragWaveParams[i].z;
		float time = fragWaveParams[i].w;
		
		float Freq = (2 * PI) / WaveLen;
		float WA = (Freq * Amp);
		
		res.normal -= vec3(
			Dir.x * WA * cos(Freq * dot(Dir, P) + time),
			Q * WA * sin(Freq * dot(Dir, P) + time),
			Dir.y * WA * cos(Freq * dot(Dir, P) + time));
	}
	
	// tangent
	
	res.tangent = vec3(0, 0, 0);
	for (int i = 0; i < waveCount; ++i)
	{	
		vec2 Dir = normalize(fragWaveDirs[i].xy);
		float WaveLen = fragWaveParams[i].x;
		float Amp = fragWaveParams[i].y;
		float Q = fragWaveParams[i].z;
		float time = fragWaveParams[i].w;
		
		float Freq = (2 * PI) / WaveLen;
		float WA = (Freq * Amp);
		
		res.tangent += vec3(
			-Q * (Dir.x * Dir.y) * WA * sin(Freq * dot(Dir, P) + time),
			Dir.y * WA * cos(Freq * dot(Dir, P) + time),
			-Q * (Dir.y * Dir.y) * WA * sin(Freq * dot(Dir, P) + time));
	}
	
	res.tangent.z = (1 - res.tangent.z);
	
	return res;
}


void main(void)
{
	//gl_FragColor = texture2D( DiffuseMap, fragTexCoord.st );
	
	vec4 fragCoord = gl_FragCoord;
	
	float invRatio = (1.0 / 256.0);
	vec2 UV = vec2(fragCoord.x * invRatio, fragCoord.y * invRatio);
	
	vec4 waterPos = WaterPlane(UV);
	float time = fragWaveParams[0].w * 25;	
	
	
	int waveCount = (int)fragWaveSetup.x;	
	WaveVertex t = GerstnerFunc(waterPos.xz, waveCount);

//	float r = 0.05;
//	vec2 samplerUV = vec2(r * waterPos.x - (fragWaveParams[0].w * 0.3), r * waterPos.z);
//	t.pos.y += texture2D(DiffuseMap, samplerUV).r;
	
	
	//gl_FragData[0] = vec4((a.pos + b.pos + c.pos) / 3, 1);
	gl_FragData[0] = vec4(t.pos, 1);
	gl_FragData[1] = vec4(t.normal, 1);
	gl_FragData[2] = vec4(t.tangent, 1);
}