#version 330

// in vec2 fragTexCoord;

// out vec4 fragColor;

// uniform sampler2D texture0;
// uniform vec4 colDiffuse;

// void main()
// {
//     vec3 vigCol = vec3(0.0, 0.8, 0.4);
//     float strength = 0.35;
//     vec3 col = texture(texture0, fragTexCoord).rgb;
//     vec2 uv = fragTexCoord.xy;
   
//     uv *=  1.0 - uv.yx; // vec2(1.0)- uv.yx; -> 1.-u.yx; 
//     float vig = uv.x*uv.y * 10.0; // multiply with strength for intensity
//     vig = pow(vig, strength); // change pow for modifying the extend of the  vignette
    
//     col = mix(col * vigCol, col, vig);

//     fragColor = vec4(col, 1.0);
// }  

uniform sampler2D texture0; 
uniform float rt_w = 800;
uniform float rt_h = 600; 
uniform float pixel_w = 1; // 13.0
uniform float pixel_h = 1; // 10.0

in vec2 fragTexCoord;
out vec4 fragColor;

void main() 
{ 
  vec2 uv = fragTexCoord;
  
  vec3 tc = vec3(1.0, 0.0, 0.0);
    float dx = pixel_w*(1./rt_w);
    float dy = pixel_h*(1./rt_h);
    vec2 coord = vec2(dx*floor(uv.x/dx),
                      dy*floor(uv.y/dy));

    tc = texture2D(texture0, coord).rgb;

	fragColor = vec4(tc, 1.0);
}