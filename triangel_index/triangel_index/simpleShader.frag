#version 330

in vec3 ver_color;
in vec3 ver_normal;
in vec3 ver_pos;

out vec4 frag_color;

void main(void)
{
    //灯的位置
    vec3 lightPos = vec3(-2.0f,0.5f,10.0f);
    vec3 lightcolor = vec3(1,1,1);
    vec3 fvSpecular = vec3(1,0,0);
    vec3 eyePos = vec3(0,0,3.0f);
    
    float f = 0.6f;
    float af = 0.4f;//环境光系数
    float sPower = 10;//高光级别
    float val = max(0.0, dot(normalize(ver_normal), normalize(lightPos)));
 
    vec3  fvReflection     = normalize( ( ( 2.0 * normalize(ver_normal) ) * val ) - normalize(lightPos) );
    vec3  fvViewDirection  = normalize( eyePos );
    float fRDotV           = max( 0.0, dot( fvReflection, fvViewDirection ) );
    
    
    vec3 color1 = ver_color * af;//环境光
    vec3 color2 = lightcolor * val* f;//漫反射光
    vec3 color3 = fvSpecular * ( pow( fRDotV, sPower ) );//镜面光
    
    vec3 color = color1 + color2 + color3;
    
    frag_color = vec4(color, 1.0);
}

