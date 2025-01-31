# Notes

## General
- shared pointers and smart pointers for better memory management and class hierarchy of callers. https://stackoverflow.com/questions/7657718/when-to-use-shared-ptr-and-when-to-use-raw-pointers https://www.geeksforgeeks.org/shared_ptr-in-cpp/
- [X] start using shared pointers for shared objects

- how to map one enum to another? For example my enum to GLenum for easier coding

- Why we need static referances in order to compile code? https://stackoverflow.com/questions/26963510/error-passing-const-as-this-argument-of-discards-qualifiers

- We need constant referances to the highest obj to compile. Maybe make a Super Intendant class like an engine class?

- constructor quick set variables. https://en.cppreference.com/w/cpp/language/constructor

- static pointer? https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

- Use a map of assets to be able to use copies of came obj instead of loading it again.

- Put everything in a namespace?

- Using sticky Notes for organizing

- Public Inheritance https://www.learncpp.com/cpp-tutorial/inheritance-and-access-specifiers/

- Using UBO for things that remain constant between shaders :https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/8.advanced_glsl_ubo/advanced_glsl_ubo.cpp

- static or dynamic draw for UBO? https://www.reddit.com/r/opengl/comments/mkxn2w/using_a_uniform_buffer_object_with/

## Suggestions for future versions

### Features
- [x] use normal mapping for lighting

### Optimizations
- [X] Use EBO instead of VBO
- [] Use Asset map instead of loading everything
- [] Use UBO for View and Projection matrix that stay the same between frames 