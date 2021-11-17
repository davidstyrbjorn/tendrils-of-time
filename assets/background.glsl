#version 330

in vec2 fragTexCoord;

out vec4 fragColor;

void main()
{
    vec3 color1 = vec3(7, 200, 249) / 255.0;
    vec3 color2 = vec3(13, 65, 225) / 255.0;

    float t = smoothstep(0, 1, fragTexCoord.x);
    vec3 result = mix(color1, color2, t);
    
    fragColor = vec4(result.rgb, 1);
}  