#version 330

in vec2 fragTexCoord;
out vec4 fragColor;
uniform float waterLevel;
uniform float time;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

vec3 voronoi(vec2 st){
    vec3 color = vec3(.0);

    // Scale
    st *= 5.0;

    // Tile the space
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float m_dist = 1.;  // minimum distance

    // Step through and do voronoi 
    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(x),float(y));

            // Random position from current + neighbor place in the grid
            vec2 point = random2(i_st + neighbor);

			// Animate the point
            point = 0.5 + 0.5*sin(time*0.3 + 6.2831*point);

			// Vector between the pixel and the point
            vec2 diff = neighbor + point - f_st;

            // Distance to the point
            float dist = length(diff);

            // Keep the closer distance
            m_dist = min(m_dist, dist);
        }
    }
    color += m_dist;
    return color;
}

void main(void){
    if(fragTexCoord.x > 1-0.005 || fragTexCoord.x < 0.005 || fragTexCoord.y > 1-0.05 || fragTexCoord.y < 0.05){
        fragColor = vec4(0, 0, 0, 1);
        return;
    }

    float v = voronoi(fragTexCoord).x;

    float relativeWaterLevel = 1 - (waterLevel / 100.0); // Water level goes up to 100
    fragColor = vec4(0, v+0.5, 0, step(relativeWaterLevel, 1-fragTexCoord.x));
}