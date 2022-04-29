cmake_minimum_required(VERSION 3.0.0)
project(DarwSim VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
include(CTest)
enable_testing()

add_subdirectory(src)

if(WIN32)
    message(FATAL_ERROR "Framework not yet supported on windows. Only Unix is supported")
endif(WIN32)

if(UNIX)
    find_package(GLEW REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(glm REQUIRED)
    include_directories(./inc "/usr/include")
    add_executable(DarwSim main.cpp)
    set_property(TARGET DarwSim PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_link_libraries(DarwSim ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLM_LIBRARIES} ${CMAKE_DL_LIBS} glfw)
endif(UNIX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
