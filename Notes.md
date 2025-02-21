# Notes

## General
- shared pointers and smart pointers for better memory management and class hierarchy of callers. https://stackoverflow.com/questions/7657718/when-to-use-shared-ptr-and-when-to-use-raw-pointers https://www.geeksforgeeks.org/shared_ptr-in-cpp/
- [X] start using shared pointers for shared objects

- how to map one enum to another? For example my enum to GLenum for easier coding

- enum class to int https://stackoverflow.com/questions/8357240/how-to-automatically-convert-strongly-typed-enum-into-int

- Why we need static references in order to compile code? https://stackoverflow.com/questions/26963510/error-passing-const-as-this-argument-of-discards-qualifiers

- We need constant references to the highest obj to compile. Maybe make a Super Intendant class like an engine class?

- constructor quick set variables. https://en.cppreference.com/w/cpp/language/constructor

- static pointer? https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

- Use a map of assets to be able to use copies of came obj instead of loading it again.

- Put everything in a namespace?

- Using sticky Notes for organizing

- Public Inheritance https://www.learncpp.com/cpp-tutorial/inheritance-and-access-specifiers/

- Using UBO for things that remain constant between shaders :https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/8.advanced_glsl_ubo/advanced_glsl_ubo.cpp

- static or dynamic draw for UBO? https://www.reddit.com/r/opengl/comments/mkxn2w/using_a_uniform_buffer_object_with/

- Make_shared calls constructor 

- Use callbacks or polling for input handling?? I think polling will create less problem for now

- Do all the .get() of the shared pointers create a noticeable performance issue?

- Handling fix for wsl could be similar to the one I did in the Lab

- Should i export parameters for some objects to json files for changing without restarting?

- dont forget to use by referance even in auto& keyword

- maybe pass normalazations in order to optimise, by reducing computes?

- maybe store data of generation in a jpeg and the upscale? https://www.reddit.com/r/proceduralgeneration/comments/1c65s7b/best_approach_to_upscale_height_maps/

- Maybe use terrain generation in a shader to simulate low res terrain in a distance

- Pass the game-engine into controls to have a way to interact with other systems?

## Suggestions for future versions

- find workaround for mouse input problem on wsl (I mentioned this on :https://github.com/microsoft/wslg/issues/376)

- check if we are running on wsl automatically and change input mode (https://superuser.com/questions/1749781/how-can-i-check-if-the-environment-is-wsl-from-a-shell-script)

- Add gamepad controls 

- Add inertia to objects

- Resolve double key press behavior

- Use glBufferSubData to update only needed vertex

- improve multiple input handling

- the terrain could store multiple materials and textures and change/blend them based on gradient/ height

- Maybe make a simple sloped mesh for collisions on top of the complex one as they do in video games?

- Maybe store gradient of a face somewhere?

- Could be cool to generate a jpeg image as a map. That map could also be used and modified instead of storing an unordered map.

- Store a list of object to be spawned and find their position via a random placement algorithm. Make rules for the placement of each block based on, slope, frequency, height, cold etc.

### Features
- [] use normal mapping for lighting

### Optimizations
- [X] Use EBO instead of VBO
- [] Use Asset map instead of loading everything
- [X] Use UBO for View and Projection matrix that stay the same between frames 