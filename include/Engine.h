#include <SDL2/SDL.h>
#include "Map.h"
#include "Player.h"


class Engine{
public:
    Engine();
    void render( Map& map, const Player& player);
    void handleInput(Player& player, Map& map);
    void update(Map& map);
    bool init();
    void renderMinimap(const Map& map, const Player& player);
    void stop();
    bool isRunning();
    ~Engine();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool _running;
    const int SCREEN_WIDTH=1200;
    const int SCREEN_HEIGHT=800;
};
