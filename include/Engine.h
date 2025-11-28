#include <SDL2/SDL.h>
#include "Map.h"
#include "Player.h"


class Engine{
public:
    Engine();
    void render(const Map& map, const Player& player);
    void handleInput(Player& player, const Map& map);
    void update();
    bool init();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

};
