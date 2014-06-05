/* 
* \file computeMatlab.vert
*
* \author Marcus Zepp
*
* \version $Revision: 1.0 $
*
* \date $Date: 2014/03/26 14:16:20 $
*
* Contact: zeppfisj@mailbox.tu-berlin.de
*
* This file is part of sfs-visualizer.
*
* sfs-visualizer is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* sfs-visualizer is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with sfs-visualizer. If not, see <http://www.gnu.org/licenses/>.
*/

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