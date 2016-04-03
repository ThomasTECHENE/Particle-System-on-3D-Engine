#version 420

/* Pour utiliser le buffer des matrices de la caméra
#extension GL_ARB_shading_language_include : enable
#include "/Materials/Common/Common"
*/
#line 8

layout(std140) uniform CPU
{
	mat4 CPU_modelViewProjMatrix;
	mat4 CPU_modelViewMatrix;
	mat4 viewMatrix;
	float temps;
	vec3 translationVector;
	vec3 CamPos;
};

layout(location = 0) in vec3 Position;
//layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Texture_Coordinates;
//layout(location = 4) in vec3 Tangent;
//layout(location = 5) nothing here
layout(location = 6) in vec3 majPosition;
layout(location = 7) in float majLifetime;
layout(location = 8) in float majAngle;

out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};


out Vertex{
	vec3 Position;
	//vec3 Normal;
	vec3 Texture_Coordinates;
	//vec3 Tangent;
	float lifetime;
}v_out;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main(){

	/*vec3 offset = Texture_Coordinates;
	mat4 inverseMV = inverse(CPU_modelViewMatrix);
	// gl_position obligatoire pour rasterization
	vec3 axis1 = vec3(	inverseMV[0][0],   
						inverseMV[1][0],  
						inverseMV[2][0]); 

	vec3 axis2 = vec3(	inverseMV[0][1],   
						inverseMV[1][1],  
						inverseMV[2][1]); 

	vec3 corner = (offset.x*axis1 + offset.y*axis2) + Position;*/

	// look from point to camera
	vec3 tmplook = CamPos - Position;
	tmplook = normalize (tmplook);
	// camera Up vector
	vec3 cameraUp = vec3(0.0,1.0,0.0);
	// billboard Right vector
	vec3 rightBillboard = cross(cameraUp, tmplook);
	// billboard Up vector
	vec3 UpBillboard = cross(tmplook, rightBillboard);
	// billboard look vector
	vec3 lookBillboard = cross(rightBillboard, UpBillboard);
	// matrice de transformation du repere local au billboard au repere monde
	mat4 localToGlobalBillboard = mat4(rightBillboard.x, UpBillboard.x, lookBillboard.x, Position.x,
									   rightBillboard.y, UpBillboard.y, lookBillboard.y, Position.y,
									   rightBillboard.z, UpBillboard.z, lookBillboard.z, Position.z,
									   0.0, 0.0, 0.0, 1.0);
	// passage au repere de la camera
	vec4 PositionView = CPU_modelViewMatrix * vec4(Position, 1.0);
	// remise de la position dans l'espace monde
	vec4 PositionBillboard = localToGlobalBillboard * PositionView;

	gl_Position = CPU_modelViewProjMatrix * vec4(Position,1.0);
	//gl_Position = CPU_modelViewProjMatrix * PositionBillboard;
	
	//gl_Position = viewMatrix * vec4(Position, 1.0);
	gl_Position += vec4(majPosition, 1.0);

	
	v_out.Position = gl_Position.xyz;
	//v_out.Normal = Normal;
	v_out.Texture_Coordinates = Texture_Coordinates;
	//v_out.Tangent = Tangent;
	v_out.lifetime = majLifetime;
	
	
	
}
