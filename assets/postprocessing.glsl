#version 330

in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

// NOTE: Add here your custom variables

void main()
{
    vec2 uv = fragCoord.xy / iResolution.xy;
   
    uv *=  1.0 - uv.yx;   //vec2(1.0)- uv.yx; -> 1.-u.yx; Thanks FabriceNeyret !
    
    float vig = uv.x*uv.y * 150.0; // multiply with sth for intensity
    
    vig = pow(vig, 0.5); // change pow for modifying the extend of the  vignette

    
    fragColor = vec4(vig); 
}  