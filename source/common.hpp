#ifndef COMMONINCLUDES_HPP
#define COMMONINCLUDES_HPP
// list of all common includes

#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>
#include <string>
#include <cstring> //for memcmp
#include <map>
#include <random>
#include <utility> // for std::pair

// for file streams
#include <fstream>
#include <sstream>

//include glew before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/string_cast.hpp>

#include <glm/gtc/quaternion.hpp>


#define DEFAULT_POSITION glm::vec3(0)
#define DEFAULT_ORIENTATION glm::quat(glm::vec3(0,0,0))  //x:pitch, y:yaw, z:roll
#define DEFAULT_SCALE glm::vec3(1)



#endif