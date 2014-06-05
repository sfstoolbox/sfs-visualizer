/* 
* \file raytrace.vert
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

uniform float thresh; //for intersection
uniform float stepwidth;
uniform float alphaboost;
uniform float zoom;
uniform int width;//of screen
uniform int height;//of screen
uniform int WIDTH;//of field
uniform int HEIGHT;//of field
uniform int DEPTH;//of field
uniform float blur; //DOF blur
uniform float quality;
uniform sampler3D field;
uniform mat4 rotation;//the ModelViewMatrix
int MaxRayDepth = 100;

//random values from 0 to 1
layout(std430, binding = 1) buffer BufferObject
{
    float randomdata[];
};

vec4 intersectColor(in vec3 eyePos, in vec3 lookDirection)
{
    vec4 result = vec4(0.0);
    float dZ = stepwidth;//kleine werte ergeben eine feinere tiefenauflösung
    vec4 color_dst = vec4(0,0,0,0); // The accumulated dest color
    float alpha_dst = 0.0;     // The accumulated dest alpha for blending
    float length_dst = 0.0; // The accumulated length
    vec4 color_src; // The src color 
    float alpha_src; // The src alpha
    
    //fraglich ist, wie tief der strahl ins feld dringen muss, werte nur zwischen 0 und 1?? wg, Texture
    for (float z=0;z<MaxRayDepth;z+=dZ)
    {
        //float randomStep = randomdata[i];
        vec3 p = eyePos + lookDirection * z;//step in direction of ray
        length_dst +=length(p);
        
        //skip out of field area
        if (-p.z<0.0) continue; //our texture starts at 0
        if (-p.z>1.0 || -p.x>1.0 ||-p.y >1.0) break; //warum nicht > 1 ??-> weil box bei 0 bis +1 und ray von 1.5 bis -unendlich
    
        //rotate and translate the ray: works but maybe there is a better way...
        vec3 trans = vec3(0.5,0.5,0.5);
        p +=trans;
        p = (vec4(p.xyz,1)*rotation).xyz;
        p -=trans;
        
        //read value from our texture
        color_src = texture3D(field,-p);
        alpha_src = color_src.a;

        //front to back compositing     s.16 Real Time volume rendering
       color_dst += (1.0 - alpha_dst) * color_src * alpha_src*alphaboost;
       alpha_dst += (1-alpha_dst)*alpha_src;
     
      if(alpha_dst >thresh) break; // terminate if opacity > 1 or the ray is outside the volume
     
    }
    return color_dst;
}


void main(void)
{
    //pixelcoordinates from 0 to 1
    vec2 uv = (gl_FragCoord.xy/vec2(width,height).xy);
    vec4 col = vec4(0.0);
    
    for (int i =0;i<quality;i++)
    {
        // ray from eyePos in direction lookDirection
       vec3 eyePos = vec3(-0.5,-0.5,zoom+0.5);// no DOF
    
        eyePos+=vec3((i-quality/2)*blur,(i-quality/2)*blur,(i-quality/2)*blur);

        vec3 lookDirection = normalize( vec3(1.0-2.0*uv,-1.0));
   
        col += intersectColor(eyePos,lookDirection)/quality;
    }
    gl_FragColor = col;
}

