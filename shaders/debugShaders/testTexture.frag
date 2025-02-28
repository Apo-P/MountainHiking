#version 330 core

// in from vertex
in vec2 uvCoords;

// texture samplers (starting from pos =0)
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	// gl_FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);

    // apply texture
    gl_FragColor = texture(texture0, uvCoords);
}