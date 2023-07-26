#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update()
{
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)}; // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)}; // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y)
  {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead()
{
  switch (direction)
  {
  case Direction::kUp:
    head_y -= speed;
    break;

  case Direction::kDown:
    head_y += speed;
    break;

  case Direction::kLeft:
    head_x -= speed;
    break;

  case Direction::kRight:
    head_x += speed;
    break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell)
{
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing)
  {
    // Remove the tail from the vector.
    body.erase(body.begin());
  }
  else
  {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body)
  {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y)
    {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y)
{
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y))
  {
    return true;
  }
  for (auto const &item : body)
  {
    if (x == item.x && y == item.y)
    {
      return true;
    }
  }
  return false;
}

void SnakeBot::UpdateHead()
{
  int food_x = food->x;
  int food_y = food->y;
  int cur_x = static_cast<int>(head_x);
  int cur_y = static_cast<int>(head_y);
  if (cur_x < food_x)
  {
    if (direction == Direction::kUp || direction == Direction::kDown)
    {
      direction = Direction::kRight;
    }
    else if (direction == Direction::kLeft)
    {
      direction = cur_y < food_y ? Direction::kUp : Direction::kDown;
    }
  }
  else if (cur_x > food_x)
  {
    if (direction == Direction::kUp || direction == Direction::kDown)
    {
      direction = Direction::kLeft;
    }
    else if (direction == Direction::kRight)
    {
      direction = cur_y < food_y ? Direction::kUp : Direction::kDown;
    }
  }
  else
  {
    if (direction == Direction::kUp && food_y < cur_y)
    {
      direction = Direction::kLeft;
    }
    else if (direction == Direction::kDown && food_y > cur_y)
    {
      direction = Direction::kRight;
    }
    else if (direction == Direction::kLeft || direction == Direction::kRight)
    {
      direction = cur_y > food_y ? Direction::kDown : Direction::kUp;
    }
  }
  switch (direction)
  {
  case Direction::kUp:
    head_y -= speed;
    break;

  case Direction::kDown:
    head_y += speed;
    break;

  case Direction::kLeft:
    head_x -= speed;
    break;

  case Direction::kRight:
    head_x += speed;
    break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}
