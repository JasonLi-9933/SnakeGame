#include "game.h"
#include <iostream>
#include <future>
#include "SDL.h"

// TODO: lock game operations

Game::Game(std::size_t grid_width, std::size_t grid_height, bool add_bot)
    : snake(grid_width, grid_height),
      bot(grid_width, grid_height, food),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      addBot(add_bot)
{
  PlaceFood();
}
// TODO: run the method with std::async
// TODO: create a new method to run the bot with std::async
void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  // std::future<void> ftr = std::async(&Game::SnakeRun, this, controller, renderer, target_frame_duration);
  std::future<void> ftr_player = std::async([this, &controller, &renderer, target_frame_duration]()
                                            { SnakeRun(controller, renderer, target_frame_duration); });
  ftr_player.wait();
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // TODO:
    // Check that the location is not occupied by a snake or bot item before placing
    // food.
    if (!snake.SnakeCell(x, y))
    {
      food->x = x;
      food->y = y;
      return;
    }
  }
}

// TODO: add code for bot snake
void Game::Update()
{
  if (!snake.alive)
    return;

  snake.Update();
  // TODO: check if snake is dead: snake is dead if: 1. head clashed into bot snake 2. got bitten by the bot
  // SnakeKilledByBot
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food->x == new_x && food->y == new_y)
  {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

void Game::SnakeRun(Controller const &controller, Renderer &renderer,
                    std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, *food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::BotRun(Renderer &renderer, std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  while (snake.alive)
  {
  }
}

int Game::GetScore() const
{
  return score;
}
int Game::GetSize() const { return snake.size; }