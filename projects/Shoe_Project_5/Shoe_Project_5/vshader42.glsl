//Scott Schumacher
//Project 4: vshader.glsl

#version 420 

in  vec4 vPosition;
in  vec4 vNormal;
out vec4 color;

uniform mat4 model_view;
uniform mat4 projection;

uniform vec4 lightPos;
uniform vec3 lightD, lightS, lightA;
uniform vec3 matlD, matlS, matlA;
uniform float shininess;



void main() 
{
    // transform vertex coordinates to eye coordinates
	vec3 pos = (model_view * vPosition).xyz; // swizzle
	vec3 L = normalize(lightPos.xyz - pos);
	vec3 E = normalize(-pos); // unit length vector points toward origin (eye)
	vec3 H = normalize(L + E); // halfway vector, used in Blinn-Phong
	vec3 N = normalize(model_view * vNormal).xyz; // transform normals to eye coordinates
	
	float Kd = max(dot(L,N), 0.0);
	vec3 diffuse = Kd * lightD * matlD;

	float Ks = pow(max(dot(N,H), 0.0), shininess);
	vec3 specular = Ks * lightS * matlS;

	vec3 ambient = lightA * matlA;

	/*if(dot(L,N) < 0.0)
	{
	specular = vec3(0.0, 0.0, 0.0);
	}*/
		
	gl_Position = projection * model_view * vPosition; 

	

	color = vec4(ambient + diffuse + specular, 1.0);
	
	
} 
