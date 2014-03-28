#version 430

layout(binding=0,rgba32f) uniform sampler2D colormap;
layout(binding=0,rgba32f) uniform image3D destTex;   //is id of TextureUnit!=texID... layout needed by imageLoad else it is writeonly
layout(binding=1,rgba32f) uniform image3D sourceTex; 
//$WS gets replaced by preprocessor
#define WORKGROUPSIZE $WS

layout (local_size_x =WORKGROUPSIZE , local_size_y = WORKGROUPSIZE ,local_size_z = WORKGROUPSIZE ) in;

//this is not part of glsl, but will be replaced by doPreprocessing() in shader.cpp
#include <shared.vert>

void main() 
{
    vec3 size = vec3(WIDTH,HEIGHT,DEPTH);
    ivec3 gridPos = ivec3(gl_GlobalInvocationID.xyz);
    
    //check our cutplanes
    bool include = calculateThis(gridPos,size);
    vec4 valueSource = imageLoad(sourceTex,gridPos);

    //we only look at the red component, that stores our field amplitude
    float val = valueSource.x*boost;

    color = colorclassify(val,include);
           
    imageStore(destTex, gridPos, color);

}