#include "Engine.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>

Engine::Engine() : window(nullptr), renderer(nullptr), _running(false)
{
}

bool Engine::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        // do stuff
        window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // the last flag  makes sure we see it instantly
        if (window == nullptr)
        {
            std::cout << "Failed to create window" << SDL_GetError() << std::endl;
            return false;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //-1 means "just initialize the first one that supports the requested flags."
        if (renderer == nullptr)
        {
            std::cout << "Failed to create renderer" << SDL_GetError() << std::endl;
            return false;
        }
        _running = true;
        return true;
    }
    else
    {
        std::cout << "Failed to initialise SDL" << std::endl;
        return false;
    }
}

void Engine::stop()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Engine::isRunning()
{
    return _running;
}

void Engine::handleInput(Player &player, Map &map)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            // quit
            _running = false;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_SPACE)
            {
                player.Interact(map);
            }
        }
    }
    const Uint8 *currentKeyState = SDL_GetKeyboardState(NULL);
    if (currentKeyState[SDL_SCANCODE_W])
        player.moveForward(map);
    if (currentKeyState[SDL_SCANCODE_S])
        player.moveBackward(map);
    if (currentKeyState[SDL_SCANCODE_A])
        player.moveLeft(map);
    if (currentKeyState[SDL_SCANCODE_D])
        player.moveRight(map);
    if (currentKeyState[SDL_SCANCODE_LEFT])
        player.turn(-player.turn_speed);
    if (currentKeyState[SDL_SCANCODE_RIGHT])
        player.turn(player.turn_speed);
}

void Engine::render(Map &map, const Player &player)
{

    for (int y = 0; y < SCREEN_HEIGHT / 2; y++)
    {
        float fogger = 1.0f - ((float)y / ((float)SCREEN_HEIGHT / 2.0f));

        Uint8 r = static_cast<Uint8>(30 * fogger);
        Uint8 g = static_cast<Uint8>(30 * fogger);
        Uint8 b = static_cast<Uint8>(30 * fogger);

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }
    for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++)
    {
        float fogger = ((float)(y - SCREEN_HEIGHT / 2) / ((float)SCREEN_HEIGHT / 2.0f));

        Uint8 r = static_cast<Uint8>(100 * fogger);
        Uint8 g = static_cast<Uint8>(100 * fogger);
        Uint8 b = static_cast<Uint8>(100 * fogger);

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        double cameraX = -1 + double(2 * x) / (double(SCREEN_WIDTH));
        double rayX = player.dirX + player.planeX * cameraX;
        double rayY = player.dirY + player.planeY * cameraX;

        int mapX = int(player.posX);
        int mapY = int(player.posY);

        double deltaDistX = std::abs(1 / rayX);
        double deltaDistY = std::abs(1 / rayY);

        int stepX = rayX > 0 ? 1 : -1;
        int stepY = rayY > 0 ? 1 : -1;

        double sideDistX = (rayX < 0 ? player.posX - mapX : -player.posX + mapX + 1.0) * deltaDistX;
        double sideDistY = (rayY < 0 ? player.posY - mapY : -player.posY + mapY + 1.0) * deltaDistY;
        double perpWallDist;
        int hit = 0;
        int side = 0;
        while (hit == 0)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }

            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

           

            int tile = map.getTile(mapX, mapY);
            if (tile == 99)
            {
                Door *door = map.getDoor(mapX, mapY);

                if (door)
                {
                    if (side == 0)
                    {
                        double Y_hit = player.posY + (sideDistX - deltaDistX) / rayX * rayY;
                        double Y_frac = Y_hit - int(Y_hit);
                        if (Y_frac < door->openAmount)
                        {
                            hit = 0;
                        }
                        else
                            hit = 99;
                    }
                    else if (side == 1)
                    {
                        double X_hit = player.posX + (sideDistY - deltaDistY) / rayY * rayX;
                        double X_frac = X_hit - int(X_hit);
                        if (X_frac < door->openAmount)
                        {
                            hit = 0;
                        }
                        else
                            hit = 99;
                    }
                }
                else
                {
                    hit = 99;
                }
            }
            else if (tile > 0)
            {
                hit = tile;
            }
        }
         perpWallDist = side == 0 ? sideDistX - deltaDistX : sideDistY - deltaDistY;
        int lineHeight = int(SCREEN_HEIGHT / perpWallDist);

        int drawStart = std::max(0, (SCREEN_HEIGHT - lineHeight) / 2);
        int drawEnd = std::min(SCREEN_HEIGHT - 1, (SCREEN_HEIGHT + lineHeight) / 2);

        Uint8 r, g, b;
        switch (hit)
        {
        case 1:
            r = 255;
            g = 0;
            b = 0;
            break;
        case 2:
            r = 0;
            g = 255;
            b = 0;
            break;
        case 3:
            r = 0;
            g = 100;
            b = 255;
            break;
        case 99:
            r = 150;
            b = 30;
            g = 70;
            break;
        default:
            r = 255;
            g = 255;
            b = 255;
            break;
        }

        if (side == 1)
        {
            r /= 2;
            g /= 2;
            b /= 2;
        }

        double maxDist = 12.0f;
        float intensity = 1.0f - (perpWallDist / maxDist);
        if (intensity < 0.0f)
            intensity = 0.0f;
        if (intensity > 1.0f)
            intensity = 1.0f;

        double hitX, hitY;
        if (side == 0)
        {
            hitX = mapX;
            if (stepX > 0)
                hitX += 0;
            else
                hitX += 1.0;
            hitY = player.posY + (perpWallDist * rayY);
        }
        else
        {
            hitY = mapY;
            if (stepY > 0)
                hitY += 0;
            else
                hitY += 1.0;
            hitX = player.posX + (perpWallDist * rayX);
        }

        double lightX = 22.0, lightY = 12.0; // light source
        double dx = lightX - hitX;
        double dy = lightY - hitY;
        double distSq = dx * dx + dy * dy;
        double dist = std::sqrt(distSq);
        double nx = (side == 0) ? -stepX : 0;
        double ny = (side == 1) ? -stepY : 0;

        double dot = (dx / dist) * nx + (dy / dist) * ny;
        if (dot < 0)
            dot = 0; // light behind wall

      
        bool inShadow = false;
        double shadowRayX = dx; 
        double shadowRayY = dy;
        double shadowDist = dist;

        // Step along the shadow ray from hit point to light
        
        int shadowSteps = static_cast<int>(shadowDist * 2.0);
        for (int i = 1; i < shadowSteps; i++)
        {
            double checkX = hitX + (shadowRayX * i / shadowSteps);
            double checkY = hitY + (shadowRayY * i / shadowSteps);

            if (map.getTile(static_cast<int>(checkX), static_cast<int>(checkY)) > 0)
            {
                // If it hits a wall (but not the light source itself)
                // Check if we hit a door
                int tile = map.getTile(static_cast<int>(checkX), static_cast<int>(checkY));
                if (tile != 99 || (map.getDoor(int(checkX), int(checkY))->state != DoorState::OPEN))
                {
                    inShadow = true;
                    break;
                }
            }
        }

       

        float lightPower = 15.0f; // max lamp power
        float lightIntensity = static_cast<float>((lightPower * dot) / (distSq));
         if (inShadow)
            lightIntensity = 0.0f;
        
        float finalIntensity = lightIntensity + (0.4f * intensity);
        if (finalIntensity > 1.0f)
            finalIntensity = 1.0f;

        r = static_cast<Uint8>(r * finalIntensity);
        g = static_cast<Uint8>(g * finalIntensity);
        b = static_cast<Uint8>(b * finalIntensity);

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }

    renderMinimap(map, player);

    SDL_RenderPresent(renderer);
}

Engine::~Engine()
{
    Engine::stop();
}

void Engine::update(Map &map)
{
    static int32_t lastTime = 0;
    uint32_t currTime = SDL_GetTicks();
    float deltaTime = (currTime - lastTime) / 1000.0f;
    lastTime = currTime;

    if (deltaTime >= 0.1f)
        deltaTime = 0.1f; // clamp to solve instant opening
    for (auto &door : map.getDoors())
    {
        switch (door.state)
        {
        case DoorState::OPENING:
            door.openAmount += 1.0 * deltaTime;
            if (door.openAmount >= 1)
            {
                door.state = DoorState::OPEN;
                door.openAmount = 1.0;
                door.timer = 3.0;
                break;
            }
            break;
        case DoorState::OPEN:
            if (door.timer > 0)
            {
                door.timer -= 1.0 * deltaTime;
            }

            else
            {
                door.state = DoorState::CLOSING;
                door.timer = 0;
            }
            break;
        case DoorState::CLOSING:
            door.openAmount -= 1.0 * deltaTime;
            if (door.openAmount <= 0)
            {
                door.state = DoorState::CLOSED;
                door.openAmount = 0;
            }
            break;
        case DoorState::CLOSED:
            door.openAmount = 0.0;
            break;
        }
    }
}

void Engine::renderMinimap(const Map &map, const Player &player)
{

    int tileSize = 2;
    int startX = SCREEN_WIDTH - Map::WIDTH * tileSize - 10;
    int startY = 10;

    SDL_Rect bgRect = {startX, startY, Map::WIDTH * tileSize, Map::HEIGHT * tileSize};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    for (int x = 0; x < Map::WIDTH; x++)
    {
        for (int y = 0; y < Map::HEIGHT; y++)
        {
            int tile = map.getTile(x, y);
            if (tile > 0 && tile != 99)
            {
                SDL_Rect tileRect = {startX + (x * tileSize), startY + (y * tileSize), tileSize, tileSize};

                switch (tile)
                {
                case 1:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    break;
                case 2:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    break;
                case 3:
                    SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    break;
                }
                SDL_RenderFillRect(renderer, &tileRect);
            }
        }
    }

    for (int x = 0; x < Map::WIDTH; x++)
    {
        for (int y = 0; y < Map::HEIGHT; y++)
        {
            if (map.getTile(x, y) == 99)
            {
                SDL_Rect doorRect = {startX + (x * tileSize), startY + (y * tileSize), tileSize, tileSize};
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &doorRect);
            }
        }
    }
    SDL_Rect playerRect = {
        startX + int(player.posX * tileSize) - 1,
        startY + int(player.posY * tileSize) - 1,
        4, 4};
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &playerRect);
}
