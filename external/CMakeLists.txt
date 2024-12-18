###Add some definitions

if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
    add_definitions( "/MP" )
endif()


add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
)

### GLFW ###
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)

add_subdirectory(glfw)

### GLEW ###
option(BUILD_UTILS OFF)
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory(glew-2.2.0/build/cmake)
target_include_directories(glew_s PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glew-2.2.0/include>")

### GLM ###
add_subdirectory(glm)

### SOIL ###
add_subdirectory(soil2)

### TinyOBJLoader ###
add_subdirectory(tinyobjloader)