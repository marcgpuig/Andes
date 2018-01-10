# Project definition
cmake_minimum_required(VERSION 3.1)
project(Andes)

# Don't genmerate ZERO_CHECK
# set(CMAKE_SUPPRESS_REGENERATION ON)

# Source directorioes and files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
file(GLOB_RECURSE SOURCES 
	"${SRC_DIR}/*.cpp"
	"${SRC_DIR}/*.h"
)

# Executable definition and properties
add_executable(${PROJECT_NAME} "${SOURCES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

# GLFW
# Set GLFW directory
set(GLFW_DIR "${LIB_DIR}/glfw")

# Set GLFW options
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

# With the options disabled, it is now safe for us to tell
# CMake to parse GLFW's CMakeLists.txt file
add_subdirectory("${GLFW_DIR}")

target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# GLAD
# Set GLAD directory
set(GLAD_DIR "${LIB_DIR}/glad")
# We need to tell CMake that we want to compile the glad
# source file into a library with the name "glad"
add_library("glad" "${GLAD_DIR}/src/glad.c")
# We need to let CMake know where it can find the header
# files it will look for while compiling glad
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
# We will also want access to those same header files from our own project
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
# We'll ask CMake to link the compiled glad library to our project
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")


# GLM
# Set GLM directory
set(GLM_DIR "${LIB_DIR}/glm")
add_subdirectory("${GLM_DIR}")
target_link_libraries(${PROJECT_NAME} "glm")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}/glm")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

if (MSVC) # If is compiling for Visual Studio
	# Set ${PROJECT_NAME} as the starting project
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif(MSVC)