/* 
* \file computeGreen.vert
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
uniform float time = 0;
uniform float frequency;
uniform float scale;
uniform float sourceRadius = 0.05; //display source radius
uniform	float c; //speed of sound
uniform vec3 virtSource;

layout(binding=0,rgba32f) uniform sampler2D colormap;
layout(rgba32f) uniform image3D destTex;   //is id of TextureUnit!=texID... layout needed by imageLoad else it is writeonly

uniform int haveCustomSources = 0;
uniform float SOURCES;
//$WS gets replaced by preprocessor
#define WORKGROUPSIZE $WS 

int sourceconfig = 0; //0 = circle 1 = line
layout (local_size_x = WORKGROUPSIZE , local_size_y = WORKGROUPSIZE ,local_size_z = WORKGROUPSIZE ) in;

float PI = 3.1415926535897932384626433832795;
float PI_2 = 6.283185307179586476925286766559;

//we can't use this, because AMD has a bug here :-/
/*layout(std140) struct source
    {
    	  float x;
        float y;
        float z;
        float nx;
        float ny;
        float nz;
        float a;
    };
*/

//our custom sources
layout(std430, binding = 2) buffer BufferObject
{
    float sources[];
};

//this is not part of glsl, but will be replaced by doPreprocessing() in shader.cpp
#include <shared.vert>

 // Source model for a point source: 3D Green's function.
 //
 //              1  e^(-i w/c |x-xs|)
 // G(x-xs,w) = --- -----------------
 //             4pi      |x-xs|
 //
 // see: Williams1999, p. 198
 //
vec2 greencmplx(float f,float r,float c,float phase)
{
  float w = 6.283185307179586*f;

  float wcr = r*w/c;
  vec2 G_cmplx= (0.07957747154)*vec2(cos(wcr) ,-sin(wcr))/r;
  vec2 shift = vec2(cos(phase), -sin(phase));
	vec2 shifted = multiply_cmplx(G_cmplx,shift); 
	return shifted;
}

// --- SFS Toolbox ------------------------------------------------
//omega = 2*pi*f;
//  x0          - position of the secondary sources / m [nx3]
//       nx0         - directions of the secondary sources / m [nx3]
//       xs          - position of virtual point source / m [nx3]
//       f           - frequency of the monochromatic source / Hz
// D using a point sink and large distance approximation
//
//              1  i w  (x0-xs) nx0
// D(x0,w) = - --- --- ------------- e^(-i w/c |x0-xs|)
//             2pi  c  |x0-xs|^(3/2)
//
// r = |x0-xs|
// r = vector_norm(x0-xs,2);
// driving signal
//D = -1/(2*pi) .* (1i*omega)/c .* ...
//   vector_product(x0-xs,nx0,2) ./ r.^(3/2) .* exp(-1i*omega/c.*r);
//
//result vec2 = vec2(Re(Z),Im(Z))
vec2 drivingSignalDefault3D(float f, vec3 secondarysource, vec3 virtualsource,vec3 nsecondarysource)
{
    float r = distance(secondarysource,virtualsource);
    float w = PI_2*f;
    float w_c = w/c;
    float wcr = r*w_c;
    float sigma = (-1.0/(2.0*PI))*w_c*dot(secondarysource-virtualsource,nsecondarysource)/pow(r,1.5);
    return sigma*vec2(sin(wcr),cos(wcr));
}

 
// using a 3D point source as source model
//
//             1  / i w      1    \  (x0-xs) nx0
//D(x0,w) = - --- | --- - ------- |  ----------- e^(-i w/c |x0-xs|)
//            2pi \  c    |x0-xs| /   |x0-xs|^2
//
// r = |x0-xs|
vec2 drivingSignalPS3D(float f, vec3 secondarysource, vec3 virtualsource,vec3 nsecondarysource)
{
    float r = distance(secondarysource,virtualsource);
    float w = PI_2*f;
    float w_c = w/c;
    float wcr = r*w_c;
    float sigma = (-w_c*dot(secondarysource-virtualsource,nsecondarysource))/(PI_2*pow(r,2));
    return sigma*vec2(w_c*sin(wcr)-(1/r)*cos(wcr),w_c*cos(wcr)+(1/r)*sin(wcr));
}

 // === Point source ===
 // secondary source selection (Spors 2008)
 //
 //      / 1, if (x0-xs) nx0 > 0
 // a = <
 //      \ 0, else
 //
bool isSourceActive(vec3 secondarysource, vec3 virtualsource,vec3 normalsecondarysource)
{
    return dot(secondarysource-virtualsource,normalsecondarysource)>0.0;
}

void main() 
{
    vec3 size = vec3(WIDTH,HEIGHT,DEPTH);
    ivec3 gridPos = ivec3(gl_GlobalInvocationID.xyz);
    bool include = calculateThis(gridPos,size);
   
    //the grid is fixed to int(WIDTH), int(HEIGHT), int(DEPTH) e.g. 300x300x300
    //the gridposition is normed to values within 0 and 1 and then scaled to metrical space
    vec3 worldPos = vec3(gridPos.x/size.x,gridPos.y/size.y,gridPos.z/size.z)*scale;

    color = vec4(0,0,0,0);
    
		float freqHZ = 1.0;
    float magn = 0;
    float A = 1.0/SOURCES;

    vec3 R = vec3(size.xyz) / 3.5;//some relative radius within the volume
   
    vec3 middlePoint = vec3(0.5,0.5,0.5)*scale;// for the normals
    
    float sourcedist =size.x/float(SOURCES);
    int VSOURCES = 1; //theoretically more then one virtual source possible
    for (int v=0;v<VSOURCES;v++)
    {
        vec3 virtualSourceWorldPos =vec3(virtSource.x+0.4*v,virtSource.y,virtSource.z)*scale;
        //shows the virtual source as a green ball if it is within size
        if (distance(worldPos, virtualSourceWorldPos)<sourceRadius*2) 
        {
            imageStore(destTex, gridPos, vec4(0.0,1.0,0.0,1.0));
            return;
        }

		    int sourceIndex = 0; //every 7th float is a new Source (in case of custom Sources)
		    for (int i=0;i<SOURCES;i++)
		    {
            vec3 currentSource;
            vec3 sourceDirection;
            vec3 sourceWorldPos;
            float sourceAmp = 1.0;
     
            if (haveCustomSources == 1)
            {
        		    sourceIndex = i*7;
                currentSource = vec3(size.x*sources[sourceIndex],size.y*sources[sourceIndex+1],size.z*sources[sourceIndex+2])*scale;// 
                sourceDirection = normalize(vec3(size.x*sources[sourceIndex+3],size.y*sources[sourceIndex+4],size.z*sources[sourceIndex+5])*scale);
                sourceWorldPos = vec3(currentSource.x / size.x, currentSource.y / size.y, currentSource.z / size.z);
                sourceAmp = sources[sourceIndex+6];
            }
            else // default case construct some source geometries
            {
                currentSource = vec3(size.x*0.5,size.y*0.5,size.z*0.5)*scale;
      
                if (SOURCES > 1)
                {
                    if (sourceconfig == 0)
                        currentSource = (vec3(size.x/2.0, size.y/2.0, size.z/2.0) + vec3(R.x*cos(float(i*(6.283185307179586 / SOURCES))), R.y*sin(float(i*(6.283185307179586 / SOURCES))) , 0.0))*scale; //sources on circle   
                    else if (sourceconfig == 1)
                        currentSource = vec3(float(i)*sourcedist+size.x/(float(SOURCES)*2.0),size.y/2.0,size.z/2.0); //sources in line
                    else 
                        currentSource = (vec3(size.x/2.0, size.y/2.0, size.z/2.0) + vec3(R.x*cos(float(i*(6.283185307179586 / SOURCES))), R.y*sin(float(i*(6.283185307179586 / SOURCES))) , 0.0))*scale; //sources on circle   
            
                  sourceWorldPos = vec3(currentSource.x / size.x, currentSource.y / size.y, currentSource.z / size.z);
    
                 if (sourceconfig == 0)
                    sourceDirection = normalize(middlePoint-sourceWorldPos);
                 else
                    sourceDirection = normalize(sourceWorldPos+vec3(0,0,-1)); //Linesources
                }  
            }
      
            float	r = distance(worldPos, sourceWorldPos);
                    
            if (isSourceActive(sourceWorldPos,virtualSourceWorldPos,sourceDirection))
            {
                //Green's Function
				         vec2 G = greencmplx(frequency,r,c,-time*(1e-3)*PI_2);   
                //drivingsignals
                 vec2 D = drivingSignalDefault3D(frequency, sourceWorldPos,virtualSourceWorldPos,sourceDirection );			
	                
                 vec2 p = multiply_cmplx(D,G);
             
                 magn += sourceAmp*p.x;

                if (r<sourceRadius) //show sources
                {
                    imageStore(destTex, gridPos, vec4(1.0,0.0,1.0,1.0));
                    return;
                }
   
            }
				
		    }
    }

 //do some scaling
 magn*=100*scale*boost/(SOURCES*VSOURCES);

 color = colorclassify(magn,include);

 imageStore(destTex, gridPos, color);

}