#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

static Image player_image("/Users/andrew/Documents/MG/Stray-Cat/Stray Cat/resources/Player.png");

struct Player
{
    explicit Player(Point pos = {.x = 10, .y = 10}, Image &pl_im = player_image) : coords(pos), old_coords(coords), player_img(pl_im)
  {};
  bool Moved() const;
  void ProcessInput(MovementDir dir);
  void Draw(Image &screen);
  
private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  int move_speed = 4;
  Image player_img;
};

#endif //MAIN_PLAYER_H
