#version 330 core

// in from vertex
in vec2 uvCoords;
in vec3 WorldPos;

// texture samplers (starting from pos =0)
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

// height for blending
uniform float optimalHeight0 = 10; // rocky texture
uniform float optimalHeight1 = 150; // grass
uniform float optimalHeight2 = 200; // snow

// basic fog
uniform vec3 cameraPosition; // camera position in world space
// uniform vec3 fogColor;
uniform float fogStart = 200.0;  // Start altitude
uniform float fogEnd = 270.0;   // Full fog altitude



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
    else if (Height < optimalHeight1-100) { //if height is lower than optimalHeight1 (and is in blend zone which is 100 for ths optimal height)
        // get color from textures
        vec4 Color0 = texture(texture0, uvCoords);
        vec4 Color1 = texture(texture1, uvCoords);
        //set range to normalize
        float Delta = (optimalHeight1-100) - optimalHeight0;
        // normalize
        float Factor = (Height - optimalHeight0) / Delta;
        //mix the final color
        TexColor = mix(Color0, Color1, Factor);
        // test
        // TexColor = vec4(0,1,0,1);
    }  
    else if (Height < optimalHeight1) { //if height is lower than optimalHeight1 (and above previous blend zone) (doing this because I want less blending zone of grass and dirt)
        // get color from grass texture only
        TexColor = texture(texture1, uvCoords);
    } 
    else if (Height < optimalHeight2) { //if height is lower than optimalHeight2
        // get color from textures
        vec4 Color0 = texture(texture1, uvCoords);
        vec4 Color1 = texture(texture2, uvCoords);
        //set range to normalize
        float Delta = optimalHeight2 - optimalHeight1;
        // normalize
        float Factor = (Height - optimalHeight1) / Delta;
        //mix the final color
        TexColor = mix(Color0, Color1, Factor);
        // test
        // TexColor = vec4(0,1,0,1);
    }
    else { //else
        // get color from highest texture only
        TexColor = texture(texture2, uvCoords);
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

        vec4 textureColor = CalcTexColor();
        // basic fog

        //temp
        vec3 fogColor = vec3(0.5f, 0.5f, 0.5f);//vec3(0.5f, 0.5f, 0.5f);

        float fogFactor = smoothstep(fogStart, fogEnd, cameraPosition.y);
        fogFactor=0.5 * fogFactor; // only allow for 50% mix at max height

        vec3 color = mix(vec3(textureColor),fogColor, fogFactor);

        gl_FragColor = vec4(color, 1.0);

    }
    else {
        gl_FragColor = texture(texture1, uvCoords);
    }
}