#version 430

uniform sampler2D sampler;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
layout(std140) uniform CPU
{
	vec3 color;
	float lifetimeMax;
	int height;
	int width;
};
in Vertex{
	vec3 Position;
	//vec3 Normal;
	vec3 Texture_Coordinates;
	//vec3 Tangent;
	float lifetime;
}v_in;



layout (location = 0) out vec4 Color;

// For multiple render target
//layout (location = 1) out vec4 Color1;
//layout (location = 2) out vec4 Color2;
//layout (location = 3) out vec4 Color3;
// */

void main()
{
	//Color = vec4(1.0,0.0,0.0,1.0);


	vec3 blackColor = vec3(0.0,0.0,0.0);
	//calcul de l'intensité de la couleur selon la durée de vie
	//vec3 partColor = color*(lt/200);
	float divisor = lifetimeMax / height;
	

	vec4 col = vec4(blackColor, 1.0);

// Rendu avec une texture uniforme selon la durée de vie
	//col = texture(sampler, v_in.Texture_Coordinates.xy);

// Rendu avec une variation de transparence selon la durée de vie
	col = texture(sampler, v_in.Texture_Coordinates.xy);
	col = col*(v_in.lifetime/lifetimeMax);
//FIRE
	
/*if (v_in.lifetime > 100){
	col = texture(sampler, v_in.Texture_Coordinates.xy);
	col = col*(v_in.lifetime/lifetimeMax);
}else if(v_in.lifetime > 75 && v_in.lifetime < 101){
	col = mix(texture(sampler, v_in.Texture_Coordinates.xy), texture(sampler2, v_in.Texture_Coordinates.xy), 0.7);
	col = col*(v_in.lifetime/lifetimeMax);
}else if(v_in.lifetime > 50 && v_in.lifetime < 76){
	col = mix(texture(sampler2, v_in.Texture_Coordinates.xy), texture(sampler3, v_in.Texture_Coordinates.xy), 0.5);
	col = col*(v_in.lifetime/lifetimeMax);
}else{
	col = texture(sampler2, v_in.Texture_Coordinates.xy);
	col = col*(v_in.lifetime/lifetimeMax);
}*/
	
	if (col.a < 0.3){
		discard;
	}
	
	Color = col;
	// */
}