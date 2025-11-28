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

    void stop();
    bool isRunning();
    ~Engine();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool _running;
    const int SCREEN_WIDTH=640;
    const int SCREEN_HEIGHT=640;
};
