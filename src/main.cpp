#include "RPG_Game.h"

int main() {
    RPG_Game echoes;
    echoes.init(30 * 24, 16 * 24, "Skyward Scammer");
    echoes.startGameLoop();
    return 0;
}
