cmake_minimum_required(VERSION 3.24)
project(RPG)

set(CMAKE_CXX_STANDARD 17)
# Add the path to the FindSDL2*.cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${RPG_SOURCE_DIR}/cmake")
# Find SDL2, SDL2_ttf, SDL2_image, and SDL2_mixer
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Print the paths to the include directories and the names of the libraries
#message("SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
#message("SDL2_TTF_INCLUDE_DIR: ${SDL2_TTF_INCLUDE_DIR}")
#message("SDL2_IMAGE_INCLUDE_DIR: ${SDL2_IMAGE_INCLUDE_DIR}")
#message("SDL2_MIXER_INCLUDE_DIR: ${SDL2_MIXER_INCLUDE_DIR}")

#message("SDL2_LIBRARY: ${SDL2_LIBRARY}")
#message("SDL2_TTF_LIBRARY: ${SDL2_TTF_LIBRARIES}")
#message("SDL2_IMAGE_LIBRARY: ${SDL2_IMAGE_LIBRARIES}")
#message("SDL2_MIXER_LIBRARY: ${SDL2_MIXER_LIBRARIES}")

include_directories(include ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})

add_library(console-game-engine
        include/SimpleGameEngine.h
        src/SimpleGameEngine.cpp)

# Link the SDL2 libraries to your library
target_link_libraries(console-game-engine ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES})


add_executable(RPG src/main.cpp src/RPG_Maps.cpp include/RPG_Maps.h src/RPG_Assets.cpp include/RPG_Assets.h src/RPG_Dynamic.cpp include/RPG_Dynamic.h src/RPG_Commands.cpp include/RPG_Commands.h src/PathFinder.cpp include/PathFinder.h include/RPG_Game.h src/RPG_Game.cpp src/Pseudo3DPlane.cpp include/Pseudo3DPlane.h)
target_link_libraries(RPG console-game-engine)
