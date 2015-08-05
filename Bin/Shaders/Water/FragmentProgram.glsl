uniform sampler2D DiffuseMap;

varying vec2 texCoord;

varying	vec4 fragPosition;
varying	vec3 fragNormal;
varying	vec3 fragLightPosition;
varying	vec3 fragEyePosition;

//varying vec4 colorMod;


void main(void)
{
	float maxD = 50.0;

	//vec4 baseColor = vec4(normalize(vec3(38, 37, 22)), 1.0);
	vec4 baseColor = vec4(normalize(vec3(28, 165, 204)), 1.0);
	
	//vec4 specColorMod = vec4(0.0, 190.0/255, 237.0/255, 1.0);
	vec4 specColorMod = vec4(1.0, 1.0, 1.0, 1.0);
	
	
	//vec3 lightDir = (fragLightPosition - fragPosition);
	vec3 lightDir = normalize(fragLightPosition);
	float a = (1.0 - min(length(fragLightPosition), maxD) / maxD);
	
	vec3 eyeDir = normalize(fragEyePosition);// - fragPosition);
	
	// vec3 normal = texture2D(DiffuseMap, texCoord).xyz;
	vec3 normal = normalize((2.0 * texture2D(DiffuseMap, texCoord).rgb) - 1.0);
	
	//normal = sqrt(1 - normal.x * normal.x + normal.y * normal.y);
	//normal = normalize(fragNormal + normal);
	//normal = normalize(fragNormal);
	
	vec3 directLight = normalize(vec3(0, 1, 0));
	//directLight = directLight - fragPosition;
	directLight = lightDir;
	vec3 reflection = normalize((2 * dot(directLight, normal) * normal) - directLight);
	
	float specular = 2.75 * max(0.0, pow(dot(reflection, eyeDir), 250));
	//float diffuse = 0.25 * dot(normal, lightDir);
	float diffuse = 1.0 * dot(vec3(0,0,1), lightDir);
	
	a = 1;
	float ambient = 0.5;
	//specular = 0.0;
	
	//gl_FragColor = (ambient + (a * (diffuse + specular))) * baseColor;
	gl_FragColor = (ambient + (a * (diffuse))) * baseColor + (specular * specColorMod);
	
	//gl_FragColor = vec4(normalize(fragNormal), 1);
	
	
	
	// color = ( tex2D( dif_1 ) * inf_1 ) + ( tex2D( dif_2 ) * inf_2 );
	
	
	//lightDir = normalize(lightDir);
	//float diffuse = dot(normalize(fragNormal), lightDir);
	//gl_FragColor = vec4(1, 1, 1, 1) * diffuse * a;	
	//gl_FragColor = vec4(normalize(fragNormal), 1);
}