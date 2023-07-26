#include "game.h"
#include <iostream>
#include <future>
#include <thread>
#include "SDL.h"

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
void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  std::future<void> ftr_snake = std::async([this, &controller, &renderer, target_frame_duration]()
                                          { GameRun(controller, renderer, target_frame_duration); });
  if (addBot)
  {
    std::future<void> ftr_bot = std::async([this]() {
      while(snake.isAlive() && bot.isAlive()) {
        bot.UpdateDirection();
        std::this_thread::sleep_for(std::chrono::microseconds(200));
      }
    }); 
    ftr_snake.wait();
    ftr_bot.wait();
  }
  else
  {
    ftr_snake.wait();
  }
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake or bot item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !bot.SnakeCell(x, y))
    {
      food->x = x;
      food->y = y;
      return;
    }
  }
}

void Game::GameUpdate()
{
  if (!snake.isAlive())
    return;

  bot.Update();
  snake.Update();
  int snake_x = static_cast<int>(snake.head_x);
  int snake_y = static_cast<int>(snake.head_y);
  int bot_x = static_cast<int>(bot.head_x);
  int bot_y = static_cast<int>(bot.head_y);

  // check if snake is dead: snake is dead if: 
  // 1. head clashed into bot snake 2. got bitten by the bot
  if (bot.SnakeCell(snake_x, snake_y)) {
    snake.setAlive(false);
    return;
  }

  if (food->x == bot_x && food->y == bot_y)
  {
    score++;
    PlaceFood();
    // Grow snake bot and increase speed.
    bot.GrowBody();
    bot.increaseSpeed(0.01);
  }
  // Check if there's food over here
  if (food->x == snake_x && food->y == snake_y)
  {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.increaseSpeed(0.02);
  }
}

void Game::GameRun(Controller const &controller, Renderer &renderer,
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
    controller.HandleInput(running, snake, addBot, bot);
    GameUpdate();
    if (addBot)
    {
      renderer.Render(snake, bot, *food);
    }
    else
    {
      renderer.Render(snake, *food);
    }

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

int Game::GetScore() const
{
  return score;
}
int Game::GetSize() const { return snake.size; }