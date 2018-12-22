
#version 430

uniform vec3 color;

out vec4 vFragColor;

void main(void)
{ 
	if(color.r != -1.0) 
		vFragColor = vec4(color, 1.0);
	else 
		vFragColor = vec4(float(gl_PrimitiveID+1), float(gl_PrimitiveID+1), float(gl_PrimitiveID+1), 1.0);

}
	
    