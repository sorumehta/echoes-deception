cmake_minimum_required(VERSION 3.24)
project(RPG)

set(CMAKE_CXX_STANDARD 17)

include_directories(include /opt/homebrew/include/SDL2)
add_compile_options(-Wall)
add_library(console-game-engine
        include/SimpleGameEngine.h
        src/SimpleGameEngine.cpp)
target_link_libraries(console-game-engine -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer)
target_link_libraries(console-game-engine -L/opt/homebrew/lib/)
add_executable(RPG src/main.cpp src/RPG_Maps.cpp include/RPG_Maps.h src/RPG_Assets.cpp include/RPG_Assets.h src/RPG_Dynamic.cpp include/RPG_Dynamic.h src/RPG_Commands.cpp include/RPG_Commands.h src/PathFinder.cpp include/PathFinder.h include/RPG_Game.h src/RPG_Game.cpp src/Pseudo3DPlane.cpp include/Pseudo3DPlane.h)
target_link_libraries(RPG console-game-engine)
