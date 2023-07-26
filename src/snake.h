#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <memory>
#include "SDL.h"

// forward declaration
class Snake;
class SnakeBot;

class Snake
{
public:
  enum class Direction
  {
    kUp,
    kDown,
    kLeft,
    kRight
  };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  Snake(int grid_width, int grid_height, int x, int y)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(x),
        head_y(y) {}

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

private:
  void UpdateHead();

  bool growing{false};
  int grid_width;
  int grid_height;

protected:
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
};

class SnakeBot : public Snake
{
public:
  SnakeBot(int grid_width, int grid_height)
      : Snake(grid_width, grid_height, grid_width / 4, grid_height / 4)
  {
  }

  bool isAlive() { return alive; };
  void UpdateDirection();
  void SetFood(std::shared_ptr<SDL_Point> f) { food = f; }

private:
  bool growing{false};
  int grid_width;
  int grid_height;
  std::shared_ptr<SDL_Point> food;
};

#endif