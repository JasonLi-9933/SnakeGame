#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

class Renderer
{
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food);
  void Render(Snake const snake, SnakeBot const bot, SDL_Point const &food); // TODO
  void UpdateWindowTitle(int score, int fps);

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  void ClearScreen();
  void RenderFood(SDL_Rect &block, SDL_Point const &food);
  void RenderSnakeAndBot(SDL_Rect &block, Snake const &snake, SnakeBot const &bot);
  void RenderSnake(SDL_Rect &block, Snake const &snake);
  void RenderBot(SDL_Rect &block, SnakeBot const &bot);
};

#endif