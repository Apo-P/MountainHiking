#### Initialize cmake
cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(mountainHiking VERSION 0.0.1)

# Configure c++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS off)

# Configure build type
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Create compile_commands.json for proper hints
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

### Source files

SOURCE_GROUP(sources REGULAR_EXPRESSION ".*/.*cpp$" )
SOURCE_GROUP(headers REGULAR_EXPRESSION ".*/.*hpp$" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*(vert|frag|comp|geom|tese|tesc)$" )
SOURCE_GROUP(obj REGULAR_EXPRESSION ".*/.*obj$" )

file(GLOB_RECURSE sources source/*.cpp)
file(GLOB_RECURSE headers source/*.hpp)
file(GLOB_RECURSE shaders shaders/*)
file(GLOB_RECURSE models resources/models/*)
file(GLOB_RECURSE textures resources/textures/*)

##### link files and libraries

add_subdirectory(external)
set(EXTERNAL_LIBRARIES glew_s glfw glm soil2 tinyobjloader)

include_directories(source)

##### Create executable

add_executable(${PROJECT_NAME} ${sources} ${headers})

##### Link libraries

target_link_libraries(${PROJECT_NAME} ${EXTERNAL_LIBRARIES})

#### Copy and link resources and headers

set (resources_src "${CMAKE_SOURCE_DIR}/resources")
set (resources_dst "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
 TARGET ${PROJECT_NAME} POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${resources_src} ${resources_dst}
 DEPENDS ${resources_dst}
 COMMENT "Symbolic link resources folder"
)

set (shaders_src "${CMAKE_SOURCE_DIR}/shaders")
set (shaders_dst "${CMAKE_CURRENT_BINARY_DIR}/shaders")

add_custom_command(
 TARGET ${PROJECT_NAME} POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${shaders_src} ${shaders_dst}
 DEPENDS ${shaders_dst}
 COMMENT "symbolic link shaders folder"
)