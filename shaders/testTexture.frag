#version 330 core

// in from vertex
in vec2 uvCoords;
in vec3 WorldPos;

// texture samplers (starting from pos =0)
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

uniform float optimalHeight0 = 40;
uniform float optimalHeight1 = 150;

uniform bool Terrain = false;

vec4 CalcTexColor()
{
    // set output color
    vec4 TexColor;

    //get height of fragment
    float Height = WorldPos.y;

    if (Height < optimalHeight0) { //if height is lower than optimalHeight0
        // get color from lowest texture only
        TexColor = texture(texture0, uvCoords);
        //test
        // TexColor = vec4(1,0,0,1);
    } 
    else if (Height < optimalHeight1) { //if height is lower than optimalHeight1
        // get color from textures
        vec4 Color0 = texture(texture0, uvCoords);
        vec4 Color1 = texture(texture1, uvCoords);
        //set range to normalize
        float Delta = optimalHeight1 - optimalHeight0;
        // normalize
        float Factor = (Height - optimalHeight0) / Delta;
        //mix the final color
        TexColor = mix(Color0, Color1, Factor);
        // test
        // TexColor = vec4(0,1,0,1);
    }  
    else { //else
        // get color from highest texture only
        TexColor = texture(texture1, uvCoords);
        //test
        // TexColor = vec4(0,0,1,1);
    }

    // return final color
    return TexColor;
}


void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	// gl_FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);

    // apply texture
    // gl_FragColor = texture(texture0, uvCoords);

    // gl_FragColor = CalcTexColor();

    if (Terrain){
        gl_FragColor = CalcTexColor();
    }
    else {
        gl_FragColor = texture(texture1, uvCoords);
    }
}