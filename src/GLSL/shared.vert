/* 
* \file shared.vert
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

uniform float xCut = 0;
uniform float yCut = 0;
uniform float zCut = 0;
uniform float opacity = 0.01;
uniform float opacityCutted = 0.0;
uniform float boost = 1;
uniform float multiplier = 1.0;
uniform int WIDTH;
uniform int HEIGHT;
uniform int DEPTH;
uniform float MINDB = -45;
uniform float colormode = 0; //0=rb 1=jet 2=bw 3..custommap
uniform float db = 0; //is calculation in db
uniform float doCut = 0; //cut planes
vec4 color;

//complex multiplication
vec2 multiply_cmplx(vec2 a, vec2 b)
{
  return vec2(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x); 
}   

//deprecated but may be usefull
vec4 getJetColor(float value,float alpha) {                                                                                                                      
     float fourValue = 4 * value;
     float red   = min(fourValue - 1.5, -fourValue + 4.5);
     float green = min(fourValue - 0.5, -fourValue + 3.5);
     float blue  = min(fourValue + 0.5, -fourValue + 2.5);
 
    return vec4(red, green, blue,alpha);
    
}

//val (-1...1)
float scaledDB(float val,float minDB)
{
  //val = clamp(val,-1.0,1.0);
 //val = sqrt(val*val); //rms
// val = (val + 1)*0.5; // scale to values between 0 and 1
  val = 20*log(abs(val)); //values of db FS are between -inf and 0db
  val = -(2/minDB)*val +1; // go to -1 and 1 again
  return val;
}

//this cuts the planes through the field
bool calculateThis(ivec3 gridPos, vec3 size)
{
    int d = 1;
    return ((gridPos.z+zCut <= size.z / 2 + d && gridPos.z +zCut>= size.z / 2 - d) 
        || (gridPos.y +yCut<= size.y / 2 + d && gridPos.y +yCut>= size.y / 2 - d)
        || (gridPos.x+xCut <= size.x / 2 + d && gridPos.x +xCut>= size.x / 2 - d));
    
}

//the magnitude gets clipped to a value within -1 and 1 and then transposed to a value in (0..1)
//which is mapped to a corresponding color
vec4 colorclassify(float magn,bool include)
{
        
   if (db==1.0)
      magn = scaledDB(magn, MINDB);

   magn = clamp(magn,-1.0,1.0);
        
   float intensity = magn/2.0 +0.5; 

   color = texture(colormap,vec2(intensity,colormode));
   color.a = opacity*abs(magn);
       
   if ( (doCut==1.0) && !include )
        color.a *= opacityCutted;
    
   return color*multiplier;

}
