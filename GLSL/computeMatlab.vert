#version 430
uniform float time;
uniform float frequency;
uniform float boost;
uniform float radiusScale;
uniform float opacity;
uniform int WIDTH;
uniform int HEIGHT;
uniform int DEPTH;
uniform	float c;
uniform int jetColor;
uniform int db;
uniform int doCut;
uniform vec2 virtSource;
uniform float MINDB;
uniform float xCut;
uniform float yCut;
uniform float zCut;

layout(rgba32f, binding = 0) uniform image3D destTex;   //ist id der TextureUnit!=texID... layout für imageLoad benötigt ansonsten writeonly
layout(rgba32f, binding = 1) uniform image3D destTex2; 
#define WORKGROUPSIZE 10
int sourceconfig = 0; //0 = circle 1 = line
layout (local_size_x =WORKGROUPSIZE , local_size_y = WORKGROUPSIZE ,local_size_z = WORKGROUPSIZE ) in;
vec4 color;


bool calculateThis(ivec3 gridPos, vec3 size)
{
 int d = 1;
    return ((gridPos.z+zCut <= size.z / 2 + d && gridPos.z +zCut>= size.z / 2 - d) 
        || (gridPos.y +yCut<= size.y / 2 + d && gridPos.y +yCut>= size.y / 2 - d)
        || (gridPos.x+xCut <= size.x / 2 + d && gridPos.x +xCut>= size.x / 2 - d));
    
}

void main() 
{
//return;
    float scale = radiusScale;
    vec3 size = vec3(WIDTH,HEIGHT,DEPTH);
    ivec3 gridPos = ivec3(gl_GlobalInvocationID.xyz);
//imageStore(destTex, gridPos, vec4(0.0,0.0,0.0,0.0));
    bool include = calculateThis(gridPos,size);


    vec4 color = imageLoad(destTex,gridPos);
    if ( (doCut==1.0) && !include )
    {
        color.a = 0.0;
    }
    else
        color.a = length(color.xyz)*0.1*boost;

imageStore(destTex, gridPos, color);
        
    
 
    
  //imageStore(destTex, gridPos, color);
      
}