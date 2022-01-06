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

in vec2 fragTexCoord;
out vec4 fragColor;

vec3 Gamma(vec3 value, float param)
{
    return vec3(
      pow(abs(value.r), param), 
      pow(abs(value.g), param), 
      pow(abs(value.b), param)
    );
}

void main() 
{ 
  vec2 uv = fragTexCoord;
  vec3 tc = texture2D(texture0, uv).rgb;
  tc = Gamma(tc, 1.25);

	fragColor = vec4(tc, 1.0);
}