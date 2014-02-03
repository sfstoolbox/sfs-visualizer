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
layout(rgba32f) uniform image3D destTex;   //layout für imageLoad benötigt ansonsten writeonly
uniform int SOURCES;
#define WORKGROUPSIZE 10
int sourceconfig = 0; //0 = circle 1 = line
layout (local_size_x =WORKGROUPSIZE , local_size_y = WORKGROUPSIZE ,local_size_z = WORKGROUPSIZE ) in;
vec4 color;
float PI = 3.14159265358979323;

//komplexe multiplikation
vec2 multiply_cmplx(vec2 a, vec2 b)
{
  return vec2(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x); 
}   

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
//ergebnis vec2 = vec2(Re(Z),Im(Z))
vec2 drivingSignals(float f, vec3 secondarysource, vec3 virtualsource,vec3 nsecondarysource)
{
float r = distance(secondarysource,virtualsource);
float w = 6.283185307179586*f;
float w_c = w/c;
float wcr = r*w_c;
float sigma = (-1.0/(2.0*PI))*w_c*dot(secondarysource-virtualsource,nsecondarysource)/pow(r,1.5);
return sigma*vec2(sin(wcr),cos(wcr));
}

 
// D using a point source as source model
//
//             1  / i w      1    \  (x0-xs) nx0
//D(x0,w) = - --- | --- - ------- |  ----------- e^(-i w/c |x0-xs|)
//            2pi \  c    |x0-xs| /   |x0-xs|^2
//
// r = |x0-xs|
vec2 drivingSignalPS(float f, vec3 secondarysource, vec3 virtualsource,vec3 nsecondarysource)
{
float r = distance(secondarysource,virtualsource);
float w = 6.283185307179586*f;
float w_c = w/c;
float wcr = r*w_c;
float sigma = (-1.0/(2.0*PI))*w_c*dot(secondarysource-virtualsource,nsecondarysource)/pow(r,2);
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

//value muss zwischen 0 und 1 liegen
vec4 getJetColor(float value,float alpha) {                                                                                                                      
     float fourValue = 4 * value;
     float red   = min(fourValue - 1.5, -fourValue + 4.5);
     float green = min(fourValue - 0.5, -fourValue + 3.5);
     float blue  = min(fourValue + 0.5, -fourValue + 2.5);
 
     return clamp(1* vec4(red, green, blue,alpha), 0.0, 1.0 );
}

vec4 GetColour(float v,float vmin,float vmax)
{
   vec4 c = vec4(1.0,1.0,1.0,abs(v)); // white
   float dv;

   if (v < vmin)
      v = vmin;
   if (v > vmax)
      v = vmax;
   dv = vmax - vmin;

   if (v < (vmin + 0.25 * dv)) {
      c.x = 0;
      c.y = 4 * (v - vmin) / dv;
   } else if (v < (vmin + 0.5 * dv)) {
      c.x = 0;
      c.z = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
   } else if (v < (vmin + 0.75 * dv)) {
      c.x = 4 * (v - vmin - 0.5 * dv) / dv;
      c.z = 0;
   } else {
      c.y = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
      c.z = 0;
   }

   return(c);
}

//val (-1...1)
float scaledDB(float val,float minDB)
{
  //val = clamp(val,-1.0,1.0);
 //val = sqrt(val*val); //rms
// val = (val + 1)*0.5; // auf werte zwischen 0 und 1 bringen
  val = 20*log(abs(val)); //die werte in db FS liegen zwischen -unendlich und 0db
  val = -(2/minDB)*val +1; // wieder auf werte zwischen -1 und 1 bringen
  return val;
}

vec4 doAlphaSlice(ivec3 gridPos, vec3 size,bool slice)
{
   

    
    if (slice)
    {
     /* float offset = 0.1;
        color.x +=offset;
        color.y +=offset;
        color.z += offset;
        color.w = 1.0; */
    }
    else
    {
        color.w = 0.0;
    }
    return color;
}

bool calculateThis(ivec3 gridPos, vec3 size)
{
 int d = 1;
    return ((gridPos.z <= size.z / 2 + d && gridPos.z >= size.z / 2 - d) 
        || (gridPos.y <= size.y / 2 + d && gridPos.y >= size.y / 2 - d)
        || (gridPos.x <= size.x / 2 + d && gridPos.x >= size.x / 2 - d));
    
}


void main() 
{
    float scale = radiusScale;
    vec3 size = vec3(WIDTH,HEIGHT,DEPTH);
    ivec3 gridPos = ivec3(gl_GlobalInvocationID.xyz);
    bool include = calculateThis(gridPos,size);
    if ((doCut==1.0) && !include)
    {
        imageStore(destTex, gridPos, vec4(0.0,0.0,0.0,0.0));
        return;
    }
        
    //das grid ist gerastert auf int(WIDTH), int(HEIGHT), int(DEPTH) z.b. 300x300x300
    //um die Berechnungen im metrischen Raum durchzuführen, wird die gridposition zunächst auf werte zwischen 0 und 1 normiert
    //und dann skaliert
    //2DO: berechnungen alle nur zwischen 0 und 1 durchführen und nachträglich skalieren
    vec3 worldPos = vec3(gridPos.x/size.x,gridPos.y/size.y,gridPos.z/size.z)*scale;
    color = vec4(0,0,0,0);// imageLoad(destTex,gridPos);
    
		float freqHZ = 1.0;
    float magn = 0;
    float A = 1.0/SOURCES;

    vec3 R = vec3(size.xyz) / 3.5;//Radius unserer Quellenanordnung 
   
    vec3 middlePoint = vec3(0.5,0.5,0.5)*scale;//HACK die normale ist nicht immer auf den mittelpunkt gerichtet (nur bei kreisanordnung)
    

float sourcedist =size.x/float(SOURCES);
int VSOURCES = 1;
for (int v=0;v<VSOURCES;v++)
{
    vec3 virtualSourceWorldPos =vec3(virtSource.x+0.4*v,virtSource.y,0.5)*scale;
    if (distance(worldPos, virtualSourceWorldPos)<0.05) //anzeige der virtuellen quelle (funzt nur wenn vq innerhalb von size)
    {
        imageStore(destTex, gridPos, vec4(0.0,1.0,0.0,1.0));
        return;
    }

		for (int i=0;i<SOURCES;i++)
		{
        vec3 currentSource = vec3(size.x*0.5,size.y*0.5,size.z*0.5)*scale;
 
        if (SOURCES > 1)
        {
            if (sourceconfig == 0)
                currentSource = (vec3(size.x/2.0, size.y/2.0, size.z/2.0) + vec3(R.x*cos(float(i*(6.283185307179586 / SOURCES))), R.y*sin(float(i*(6.283185307179586 / SOURCES))) , 0.0))*scale; //sources on circle   
            else
                currentSource = vec3(float(i)*sourcedist+size.x/(float(SOURCES)*2.0),size.y/2.0,size.z/2.0); //sources in line

        }  
        vec3 sourceWorldPos = vec3(currentSource.x / size.x, currentSource.y / size.y, currentSource.z / size.z);
        float	r = distance(worldPos, sourceWorldPos);
     
        vec3 sourceDirection;
        if (sourceconfig == 0)
            sourceDirection = normalize(middlePoint-sourceWorldPos);
        else
            sourceDirection = normalize(sourceWorldPos+vec3(0,0,-1)); //Linesources

        if (isSourceActive(sourceWorldPos,virtualSourceWorldPos,sourceDirection))
        {

            //greenfunktion
				     vec2 g = greencmplx(frequency,r,c,-time*6.283185307179586 );   
            //drivingsignals
             vec2 D = drivingSignalPS(frequency, sourceWorldPos,virtualSourceWorldPos,sourceDirection );			
	            //
              g = multiply_cmplx(D,g);
            //pseudo tapering 
			       //float distVirtualSourceSource = 1.0/pow(distance(sourceWorldPos,virtualSourceWorldPos),8);		
            if (r<0.01) //quellen hervorheben
            {
                imageStore(destTex, gridPos, vec4(1.0,1.0,1.0,1.0));
                return;
            }
            else
                magn += g.x/(frequency*0.001);//frequenzdämfung - wegen alphablending




        }
				
		}
}

magn*=100*scale*boost/(SOURCES*VSOURCES);


        if (db==1.0)
          magn = scaledDB(magn, MINDB);

         if (jetColor==1.0)
          {
             color = getJetColor(magn / 2 + 0.5, abs(magn)*0.1);
          }
          else
          {
            if (db==1.0)
              color = GetColour(magn,MINDB,0.0); 
            else
                //color = vec4(-0.5*magn,0.5,0.7*magn,abs(magn));//black/white
              color = vec4(magn/2,0.0,-magn/2,abs(magn)*0.01);//red|blue
           }
   
   /* if (doCut==1.0)
          color = doAlphaSlice(gridPos, size,include);*/

	color.a*=opacity;
	
  imageStore(destTex, gridPos, color);
      
}