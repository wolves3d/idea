uniform sampler2D DiffuseMap;

varying vec2 texCoord;

varying	vec4 fragPosition;
varying	vec3 fragNormal;
varying	vec3 fragLightPosition;
varying	vec3 fragEyePosition;

//varying vec4 colorMod;


void main(void)
{
	float maxD = 16.0f;
	
	vec4 baseColor = vec4(normalize(vec3(38, 37, 22)), 1.0);
	//vec4 specColorMod = vec4(0.0, 190.0/255, 237.0/255, 1.0);
	vec4 specColorMod = vec4(1.0, 1.0, 1.0, 1.0);
	
	
	vec3 lightDir = (fragLightPosition);// - fragPosition);
	float a = (1.0f - min(length(lightDir), maxD) / maxD);
	lightDir = normalize(lightDir);
	
	vec3 eyeDir = normalize(fragEyePosition);// - fragPosition);
	
	// vec3 normal = texture2D(DiffuseMap, texCoord).xyz;
	vec3 normal = normalize(texture2D(DiffuseMap, texCoord).rgb * 2.0 - 1.0);
	
	//normal = sqrt(1 - normal.x * normal.x + normal.y * normal.y);
	//normal = normalize(fragNormal + normal);
	//normal = normalize(fragNormal);
	
	vec3 directLight = normalize(vec3(0, 1, 0));
	//directLight = directLight - fragPosition;
	directLight = lightDir;
	vec3 reflection = normalize((2 * dot(directLight, normal) * normal) - directLight);
	
	float specular = 2.75 * max(0.0, pow(dot(reflection, eyeDir), 25));
	float diffuse = 0.25 * dot(normal, lightDir);
	
	a = 1;
	float ambient = 0.0;
	//specular = 0;
	
	
	
	//gl_FragColor = (ambient + (a * (diffuse + specular))) * baseColor;
	gl_FragColor = (ambient + (a * (diffuse))) * baseColor + (specular * specColorMod);
	
	
	
	// color = ( tex2D( dif_1 ) * inf_1 ) + ( tex2D( dif_2 ) * inf_2 );
	
	gl_FragColor = vec4(1, 1, 1, 1);
}