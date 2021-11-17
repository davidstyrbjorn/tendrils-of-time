#version 330

in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main()
{
    vec2 uv = fragTexCoord.xy;
   
    uv *=  1.0 - uv.yx; // vec2(1.0)- uv.yx; -> 1.-u.yx; 
    
    float vig = uv.x*uv.y * 10.0; // multiply with strength for intensity
    
    vig = pow(vig, 0.15); // change pow for modifying the extend of the  vignette
    
    fragColor = texture(texture0, fragTexCoord) * vec4(vig); 
}  