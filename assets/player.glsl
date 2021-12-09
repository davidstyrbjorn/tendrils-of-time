#version 330

out vec4 outColor;

in vec2 fragTexCoord;

uniform float time;
uniform float water_level = 0.0;

void main(){
    vec2 st = fragTexCoord; 

    // outline border
    float r = step(0.95, st.x);
    float l = step(st.x, 0.05);
    float t = step(0.975, st.y);
    float b = step(st.y, 0.025);

    if(r + l + t + b == 0){
        float a = step(1 - fragTexCoord.y, water_level);
        outColor = vec4(0.72, 0.95, 0.9, a);
    }
    else {
        outColor = vec4(0.6, 0.8, 0, 1);
    }
}