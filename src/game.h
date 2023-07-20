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
  Snake snake;
  SnakeBot bot;
  std::shared_ptr<SDL_Point> food = std::make_shared<SDL_Point>();
  std::mutex mtx;
  bool addBot;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
  void SnakeRun(Controller const &controller, Renderer &renderer,
                std::size_t target_frame_duration);
  void BotRun(Renderer &renderer,
              std::size_t target_frame_duration);
};

#endif