#version 330 core

layout (triangles) in;
// be careful with max Vertices it must be specified before compilation
// max vertices are 
// 2 for each vertex normal (6 total)
// 2 for the face normal
// 2 for each face gradient
layout (line_strip, max_vertices = 10) out;

in VS_OUT {
    vec3 normal;
} gs_in[]; // input of normals for each vertex

out vec4 DebugColor;

const float MAGNITUDE = 0.4;
  
// VPmatrix UBO
layout(std140) uniform VPmatrices {
    mat4 V;
    mat4 P;
};

void GenerateLine(int index)
{
    gl_Position = P * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = P * (gl_in[index].gl_Position + 
                                vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

// returns the normal average of a triangle face
vec3 calcAverageNormal() {
    //Calculate the average normal for the triangle
    vec3 averageNormal = normalize((gs_in[0].normal + gs_in[1].normal + gs_in[2].normal) / 3.0);

    return averageNormal;
}

// returns the center of triangle face
vec4 calcTriangleCenter() {
    //find the center of the triangle
    vec4 A = gl_in[0].gl_Position;
    vec4 B = gl_in[1].gl_Position;
    vec4 C = gl_in[2].gl_Position;
    vec4 triangleCenter = (A + B + C) / 3.0;

    return triangleCenter;
}

//TODO:clean up variable names
// display's a Line of Magnitude from a point (Usually display's a normal)
void showLine(vec3 averageNormal, vec4 triangleCenter, float Magnitude) {
   
    

    // Output the normal lines starting from the center
    vec4 normalStart = triangleCenter;
    vec4 normalEnd = triangleCenter + vec4(averageNormal,0) * Magnitude;

    // Could be used as a trick later?
    //this had an interesting effect of making the line always point towards the positive Y of the screen
    // because the direction vector wasn't calculate my model and view matrix
    // vec4 normalEnd = triangleCenter + vec4(0,1,0,0) * 2 * MAGNITUDE;


    // Output the line representing the normal
    gl_Position = P * normalStart;
    EmitVertex();

    gl_Position = P * normalEnd;
    EmitVertex();

    EndPrimitive();
}


// calculates and shows face normal
void showFaceNormal() {

    // find average normal
    vec3 averageNormal = calcAverageNormal();
    // find face center
    vec4 triangleCenter = calcTriangleCenter();
    // display normal
    // set debug color
    DebugColor = vec4(0.0, 0.0, 1.0, 1.0);
    // make line of magnitude
    showLine(averageNormal, triangleCenter, 2 * MAGNITUDE);
}

// computes a gradient form a normal (calculates for height in Y axis)
vec2 computeGradient(vec3 normal) {
    // Ensure the normal's Y component is not zero to avoid division errors
    if (abs(normal.y) < 1e-6) {
        return vec2(0.0); // Return zero gradient for near-vertical normals
    }

    // Compute the gradient (∂h/∂x, ∂h/∂z)
    return vec2(-normal.x / normal.y, -normal.z / normal.y);
}

// will calculate and show the slope of each face in regard to y plane
void showFaceSlope() {

    // find average normal
    vec3 averageNormal = calcAverageNormal();
    // find face center
    vec4 triangleCenter = calcTriangleCenter();

    // find Gradient
    // Find view matrix
    mat3 viewMatrix = mat3(V);
    // Multiply by inverse of View matrix to calculate gradient in worldspace
    //? maybe we could change this to not multiply the View matrix in vertex shader?
    // get - to show it like the normal (however now gradient is inverse)
    vec2 gradient = - computeGradient(inverse(viewMatrix) * averageNormal);

    // make a vector pointing to x axis
    vec3 axisX = normalize(vec3(vec4(viewMatrix * vec3(1,0,0), 0.0)));
    // make a vector pointing to z axis
    vec3 axisZ = normalize(vec3(vec4(viewMatrix * vec3(0,0,1), 0.0)));

    // total slope - combine grad

    float slope = atan(length(gradient));
    float slopeDeg = degrees(slope);

    // Map slope to color scale
    vec3 totalColor;
    // if the slope is above 40 degrees mark it as to steep
    if (slopeDeg > 40.0){
        totalColor = vec3(1,0,1); //set purple
    }
    else {
        float intensity = clamp(slopeDeg / 40.0, 0.0, 1.0); // Normalize 0°-40°
        
        totalColor = vec3(intensity,1,intensity); // gradient of green to white
    }
    
    // show combined gradient
    DebugColor = vec4(totalColor, 1.0);
    vec3 totalGrad =(axisX * gradient.x)+(axisZ * gradient.y);
    showLine(normalize(totalGrad), triangleCenter, 1 * length(gradient));
}

// will calculate and display face normal and gradient
void DebugFace() {

    // find average normal
    vec3 averageNormal = calcAverageNormal();
    // find face center
    vec4 triangleCenter = calcTriangleCenter();

    // find Gradient
    // Find view matrix
    mat3 viewMatrix = mat3(V);
    // Multiply by inverse of View matrix to calculate gradient in worldspace
    //? maybe we could change this to not multiply the View matrix in vertex shader?
    // get - to show it like the normal (however now gradient is inverse)
    vec2 gradient = - computeGradient(inverse(viewMatrix) * averageNormal);

    // Visualize gradient as color (mapping slope to RGB)
    // vec3 gradientColor = vec3(abs(gradient), 0.0); // Red = dH/dX, Green = dH/dZ
    // DebugColor = vec4(gradientColor, 1.0); // use color in 


    // display normal -------------------
    // set debug color
    DebugColor = vec4(0.0, 0.0, 1.0, 1.0);
    // make line of magnitude
    showLine(averageNormal, triangleCenter, 1 * MAGNITUDE);


    //display gradient-------------------

    //display X axis
    // set color to Red (color could change if gradient is above a threshold to make it more obvious)
    DebugColor = vec4(1.0, 0.0, 0.0, 1.0);
    // make a vector pointing to x axis
    vec3 axisX = normalize(vec3(vec4(viewMatrix * vec3(1,0,0), 0.0)));
    // display gradient in X axis with magnitude as gradient
    showLine(axisX, triangleCenter, 1 * gradient.x);

    //display Z axis
    // set color to Red (color could change if gradient is above a threshold to make it more obvious)
    DebugColor = vec4(1.0, 1.0, 0.0, 1.0);
    // make a vector pointing to z axis
    vec3 axisZ = normalize(vec3(vec4(viewMatrix * vec3(0,0,1), 0.0)));
    // display gradient in Z axis (don't mind the y because of vec2)
    showLine(axisZ, triangleCenter, 1 * gradient.y);

    
    // total slope - combine grad

    float slope = atan(length(gradient));
    float slopeDeg = degrees(slope);

    // Map slope to color scale
    vec3 totalColor;
    // if the slope is above 40 degrees mark it as to steep
    if (slopeDeg > 40.0){
        totalColor = vec3(1,0,1); //set purple
    }
    else {
        float intensity = clamp(slopeDeg / 40.0, 0.0, 1.0); // Normalize 0°-40°
        
        totalColor = vec3(intensity,1,intensity); // gradient of green to white
    }
    
    // show combined gradient
    DebugColor = vec4(totalColor, 1.0);
    vec3 totalGrad =(axisX * gradient.x)+(axisZ * gradient.y);
    showLine(normalize(totalGrad), triangleCenter, 1 * length(gradient));
}



void main()
{
    // we process in primitives in this case triangles

    // // generate the normal for each vertex

    // // this will make the first vertex of each triangle yellow
    // DebugColor = vec4(1.0, 1.0, 0.0, 1.0);
    // GenerateLine(0); // first vertex normal
    // // this will make the second vertex of each triangle red
    // DebugColor = vec4(1.0, 0.0, 0.0, 1.0);
    // GenerateLine(1); // second vertex normal
    // // this will make the third vertex of each triangle green
    // DebugColor = vec4(0.0, 1.0, 0.0, 1.0);
    // GenerateLine(2); // third vertex normal


    // generate the normal in the middle of the face

    // showFaceNormal();


    //Debug whole face
    // DebugFace();

    //debug slope
    showFaceSlope();

} 