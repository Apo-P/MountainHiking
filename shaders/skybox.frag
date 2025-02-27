#version 330 core

out vec4 color;

// is vec3 because of cubemap
in vec3 uvCoords;

// skybox
uniform samplerCube skybox;

// time for daylight calculation
// uniform float time;

// float triangle_func(in float x, in float A, in float T)
// {
//    return (A/T) * (T - abs(mod(x, (2*T)) - T) );
// }

// float get_time_of_day()
// {
//    float max_time = 5; 
//    float current_time = (sin(time / max_time) + 1) / 2.0;
//    return current_time;
// }

void main()
{
    vec4 texColor = texture(skybox, uvCoords);
    // color = mix(texture_color.bgra, texture_color.rgba, get_time_of_day());
    gl_FragColor = texColor;
}
