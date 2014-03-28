//#version 410
//simple pass through vertex shader
void main()
	{	
		gl_Position =  gl_ModelViewProjectionMatrix*gl_Vertex;	
	  gl_TexCoord[0] = gl_MultiTexCoord0;
	}