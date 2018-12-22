
#version 430

uniform vec3 color;

out vec4 vFragColor;

void main(void)
{ 
	vFragColor = vec4(color,1.0);
}
	
    