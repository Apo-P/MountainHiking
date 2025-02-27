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
#include <unordered_map>
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

#include <glm/gtc/type_ptr.hpp>

#define DEFAULT_POSITION glm::vec3(0)
#define DEFAULT_ORIENTATION glm::quat(glm::vec3(0,0,0))  //x:pitch, y:yaw, z:roll
#define DEFAULT_SCALE glm::vec3(1)

// convert float value to bits
inline uint32_t floatToBits(float f) {
    uint32_t result;
    std::memcpy(&result, &f, sizeof(result));  // Same as using a union for type-punning
    return result;
}

// Custom hash function for FloatPair
struct FloatPairHash {
    // operator when called
    size_t operator()(const std::pair<float,float>& pair) const {
        // convert floats to bits
        uint32_t xBits = floatToBits(pair.first);
        uint32_t yBits = floatToBits(pair.second);

        // Combine the bits using a prime multiplier to minimize collisions
        size_t hash = xBits;
        hash = hash * 31 + yBits;  // 31 is a small prime number

        return hash;
    }
};

// Custom hash function for IntPair
struct IntPairHash {
    // operator when called
    size_t operator()(const std::pair<int,int>& pair) const {
        // convert ints to bits
        uint32_t xBits = floatToBits(pair.first);
        uint32_t yBits = floatToBits(pair.second);

        // Combine the bits using a prime multiplier to minimize collisions
        size_t hash = xBits;
        hash = hash * 31 + yBits;  // 31 is a small prime number

        return hash;
    }
};




#endif