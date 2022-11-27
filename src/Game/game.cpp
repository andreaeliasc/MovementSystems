#include <iostream>
#include "game.h"
#include "stexture.cpp"

#include "../entt/entt.hpp"
#include "../Scene/Scene.h"
#include <SDL2/SDL.h>

#include <SDL2/SDL.h>


#include "../Scene/Entities.hpp"
#include "../Scene/Components.hpp"
#include "../Scene/Systems.hpp"

int speed = 2;
int player_speed = 80;
int shadow = 30;
SDL_Rect coin;
SDL_Rect ant;

Game::Game ()
{
    std::cout << "Movimiento" << std::endl;
   
    FPS = 60;
    frameDuration = (1.0f / FPS) * 1000.0f;
    counter = 0;

}

Game::~Game ()
{
    std::cout << "~Game" << std::endl;
}
Uint32 fragment(Uint32 currentColor, float dT)
{
    if (currentColor == 0){
        return 0;
    }
    
    if (currentColor != 16777215 && currentColor != 13456847){
        Uint8 red = (currentColor >> 16) & 0xff;
        Uint8 green = (currentColor >> 8) & 0xff;
        Uint8 blue = currentColor  & 0xff;

        if (red > shadow){
            red -= shadow;
            
        }
        else{
            red = 0;
        }

        if (green > shadow){
            green -= shadow;
            
        }
        else{
            green = 0;
        }

        if (blue > shadow){
            blue -= shadow;
            
        }
        else{
            blue = 0;
        }


        Uint32 rgb = red;
        rgb = (rgb << 8) + green;
        rgb = (rgb << 8) + blue;

        return rgb;
    }
    else{
        return 16777215;
    }
}
Uint32 spriteBackground(Uint32 currentColor, float dT)
{


    if (currentColor == 13456847) {
        return 16777215;
    }
    else{
        return currentColor;
    }
}
Scene* scene;
void Game::setup(){
    
    coin.x = window_width/2;
    coin.y = 700;
    coin.w = 48;
    coin.h = 48;

    ant.x = 800;
    ant.y = 300;
    ant.w = 64;
    ant.h = 64;

    scene = new Scene("Dough's Adventure");

    scene->addSetupSystem(new CameraSetupSystem(
        4,
        window_width,
        window_height,
        24 * 16 * 4,
        19 * 16 * 4
    ));

    TileMapSystem* tileMapSystem = new TileMapSystem(renderer, window);
    scene->addSetupSystem(tileMapSystem);
    scene->addRenderSystem(tileMapSystem);

    scene->addSetupSystem(new CharacterSetupSystem(renderer));
    scene->addInputSystem(new PlayerInputSystem());

    scene->addRenderSystem(new SpriteRenderSystem());

    scene->addUpdateSystem(new MovementUpdateSystem());
    
    scene->setup();
    
}

void Game::frameStart(){
    frameStartTimeStamp = SDL_GetTicks();
    if (frameEndTimeStamp)
    {
        dT = (frameStartTimeStamp - frameEndTimeStamp) / 1000.0f;
    }
    else
    {
        dT = 0;
    }
}

void Game::frameEnd(){
    frameEndTimeStamp = SDL_GetTicks();

    float actualFrameDuration = frameEndTimeStamp - frameStartTimeStamp;

    if (actualFrameDuration < frameDuration){
        SDL_Delay(frameDuration - actualFrameDuration);
    }
    counter++;


}

void Game::init(const char* title, int widthi, int heighti){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 0, 0, widthi, heighti, 0);
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    window_height = heighti;
    window_width = widthi;
    isRunning = true;

}
void Game::handleEvents(){

    SDL_Event event;

    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        if (event.type == SDL_KEYDOWN)
    {
      switch (event.key.keysym.sym)
      {
      case SDLK_RIGHT:
        coin.w += 30;
       if (coin.w>30)
       {
        coin.w = coin.w/5;
       }else{
        coin.w = coin.w*5;
       }
        
        
        break;
      case SDLK_LEFT:
       if (coin.w>30)
       {
        coin.w = coin.w/5;
       }else{
        coin.w = coin.w*5;
       } 
        break;
      
      case SDLK_UP:
        if (coin.w>30)
       {
        coin.w = coin.w/5;
       }else{
        coin.w = coin.w*5;
       }
        break;
    
        case SDLK_DOWN:
        if (coin.w>30)
       {
        coin.w = coin.w/5;
       }else{
        coin.w = coin.w*5;
       }  
          
        break;
    
        
      }
    }
        scene->input(event);

    }
    

}


void Game::update(){

    scene->update(dT);  
    if (coin.x < 600)
    {
        coin.x +=2000;
    }
    
}
void Game::render(){

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
    SDL_RenderClear(renderer);
  
    

    SDL_Surface* coinS = IMG_Load("./assets/coin.png");
    SDL_Texture* coinT = SDL_CreateTextureFromSurface(renderer, coinS);
    SDL_FreeSurface(coinS);

    
    SDL_Surface* antS = IMG_Load("./assets/ant.png");
    SDL_Texture* antT = SDL_CreateTextureFromSurface(renderer, antS);
    SDL_FreeSurface(antS);


    scene->render(renderer);
    
   // SDL_RenderCopy(renderer, coinT, nullptr , &coin);
    SDL_DestroyTexture(coinT);

    SDL_RenderCopy(renderer, antT, nullptr , &ant);
    SDL_DestroyTexture(antT);


    SDL_RenderPresent(renderer);

}
void Game::clean(){

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();



}
bool Game::running(){
    return isRunning;
}