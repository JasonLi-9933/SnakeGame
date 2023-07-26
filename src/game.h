#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <mutex>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game
{
public:
  Game(std::size_t grid_width, std::size_t grid_height, bool add_bot);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

private:
  std::shared_ptr<SDL_Point> food = std::make_shared<SDL_Point>(SDL_Point({0, 0}));
  Snake snake;
  SnakeBot bot;
  bool addBot;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void GameUpdate();
  void GameRun(Controller const &controller, Renderer &renderer,
                std::size_t target_frame_duration);
};

#endif