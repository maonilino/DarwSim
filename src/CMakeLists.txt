cmake_minimum_required(VERSION 3.0.0)
project(DarwSim VERSION 0.1.0)

include(CTest)
enable_testing()

if(WIN32)
    message(FATAL_ERROR "Framework not yet supported on windows. Only Unix is supported")
endif(WIN32)

if(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(glm REQUIRED)
    include_directories(../inc "/usr/include")
    add_library(DarwSimWin SHARED glad.c Window.cpp)
    target_link_libraries(DarwSimWin ${OPENGL_LIBRARIES} glfw)
    add_library(DarwSimSprite SHARED glad.c Graphics.cpp SpriteRenderer.cpp)
    target_link_libraries(DarwSimSprite ${CMAKE_DL_LIBS})
    add_library(DarwSimMap SHARED Map.cpp GridMap.cpp)
    # set_property(TARGET DarwSimMap PROPERTY POSITION_INDEPENDENT_CODE ON) # equiv tp -fPic
    target_link_libraries(DarwSimMap ${GLM_LIBRARIES})
    add_library(DarwSimSim SHARED Simulation.cpp Environment.cpp)
    target_link_libraries(DarwSimSim ${GLM_LIBRARIES} ${CMAKE_DL_LIBS} DarwSimMap)
endif(UNIX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
