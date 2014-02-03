#version 430

uniform float multiplier;
uniform float camX;
uniform float camY;
uniform float camZ;
uniform float thresh; //for intersection
uniform float stepwidth;
uniform float stddev;
uniform int width;
uniform int height;
uniform float blur; //DOF blur
uniform int quality;
uniform sampler3D field;
int DEPTH = 10;
#define USECOLOR
mat4 rotX;
mat4 rotY;

//random values from 0 to 1
//layout(std430, binding = 1) buffer BufferObject
//{
//    float randomdata[];
//};


#ifdef USECOLOR
vec4 intersectColor(in vec3 ro, in vec3 rd)
{
    vec4 result = vec4(0.0);
    float dZ = stepwidth;//kleine werte ergeben eine feinere tiefenauflösung
 vec4 col_acc = vec4(0,0,0,0); // The dest color
    float alpha_acc = 0.0;                // The  dest alpha for blending
    float length_acc = 0.0;
    vec4 color_sample; // The src color 
    float alpha_sample; // The src alpha

//fraglich ist, wie tief der strahl ins feld dringen muss, werte nur zwischen 0 und 1?? wg, Texture

    for (float z=0;z<DEPTH;z+=dZ)

    {
        //float randomStep = randomdata[i];
        vec3 p = ro + rd * z;//step in direction of ray
        length_acc +=length(p);
//skip out of field area
        if (-p.z<0.0) continue; //our texture starts at 0
        if (-p.z>1.0 || -p.x>1.0 ||-p.y >1.0)break; //warum nicht > 1 ??-> weil box bei 0 bis +1 und ray von 1.5 bis -unendlich
    
        //rotate and translate the ray: works but maybe there is a better way...
        vec3 trans = vec3(0.5,0.5,0.5);
        p +=trans;
        p = (vec4(p.xyz,1)*rotX).xyz;
        p = (vec4(p.xyz,1)*rotY).xyz;
        p -=trans;
        
        vec4 color_sample = texture3D(field,-p);


      float alpha_sample = color_sample.a * dZ;
      col_acc   += (1.0 - alpha_acc) * color_sample * alpha_sample *stddev*10;
 //col_acc   += (1.0 - alpha_sample) * color_sample * alpha_acc *stddev*1000;
      alpha_acc += alpha_sample;
     
      if(alpha_acc >thresh) break; // terminate if opacity > 1 or the ray is outside the volume
     // dZ*=1.1;

    }
return col_acc;
}

#else
float intersect(in vec3 ro, in vec3 rd)
{

   float result = 0.0;
    float dZ =0.01;//kleine werte ergeben eine feinere tiefenauflösung
//fraglich ist, wie tief der strahl ins feld dringen muss, werte nur zwischen 0 und 1?? wg, Texture
    for (float z=0;z<DEPTH;z+=dZ)
    {
        //float randomStep = randomdata[i];
        vec3 p = ro + rd * z;//step in direction of ray
        if (-p.z<0.0) continue; //our texture starts at 0
        if (-p.z>1.0 || -p.x>1.0 ||-p.y >1.0)
            break; //warum nicht > 1 ??-> weil box bei 0 bis +1 und ray von 1.5 bis -unendlich
      
 //rotate and translate the ray: works but maybe there is a better way...
        vec3 trans = vec3(0.5,0.5,0.5);
        p +=trans;
        p = (vec4(p.xyz,1)*rotX).xyz;
        p = (vec4(p.xyz,1)*rotY).xyz;
        p -=trans;
        
            vec4 current = texture3D(field,-p);
      // result = current.a;
       // result += vec4(current.xyz,1.0)*current.a; //do alphablending
        result += length(vec4(current.xyz,0.0))*current.a;
        //result= current;
        if (length(result)>thresh)
        //if (result.a>0.00001)
            break;

        dZ*=stddev*randomdata[int(z+length(ro))]+1.0;//idee:randomraysampling

 
    }

   
return result;
}
#endif
mat4 rotationMatrix(vec3 axis, float angle)
{
axis = normalize(axis);
float s = sin(angle);
float c = cos(angle);
float oc = 1.0 - c;
return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
0.0, 0.0, 0.0, 1.0);
}
void main(void)
{
 //calculate the rotationmatricees for rotating the rays
    rotX = rotationMatrix(vec3(1,0,0),camY*10);
    rotY = rotationMatrix(vec3(0,1,0),-camX*10);
  

    //pixelcoordinates from 0 to 1
    vec2 uv = (gl_FragCoord.xy/vec2(width,height).xy);
    vec4 col = vec4(0.0);
    //DOF durch überlagerung von 
    for (int i =0;i<quality;i++)
    {
        // ray from eyePos in direction lookDirection
        //vec3 eyePos = vec3(-0.5,-0.5,camZ+0.5);// no DOF
        vec3 eyePos = vec3(-0.5+(i-quality/2)*blur,-0.5+(i-quality/2)*blur,camZ+0.5);
// vec3 eyePos = vec3(-0.5,-0.5,camZ+0.5);
        vec3 lookDirection = normalize( vec3(1.0-2.0*uv,-1.0));
   
   
    #ifdef USECOLOR
        col += intersectColor(eyePos,lookDirection)*multiplier/quality;
    #else
        float id = intersect(eyePos,lookDirection)*multiplier/quality;
    
        if (id > 0.0)
            col += vec4(id);

    #endif

    }
gl_FragColor = col;
}


