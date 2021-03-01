#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Point.h"
#include "castle.hpp"


enum class MovementDir
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class PlayerState
{
    ALIVE,
    CHANGING_ROOM
};

struct MovementPhase
{ 
    int phase;
    int dir;
};

struct Player
{
    explicit Player(Point pos = {.x = 10, .y = 10},
            std::string path = "../../../Stray Cat/resources/Player.png")
            : coords(pos), old_coords(coords), image(std::make_unique<Image>(path)) {};
    bool Moved() const;
    void ProcessInput(MovementDir dir);
    void Draw(std::shared_ptr<Image> screen);
    void SetPhase(int phase);
    void SetCastle(std::shared_ptr<Castle> cast);
    bool CheckWall(Point coord);
    bool CheckCorner(Point coord);
    bool CheckExit(Point coord);
    PlayerState GetState() { return  state; }
    void SetState(PlayerState st) { state = st; }
    void SetCoords(Point p) { coords.x = p.x; coords.y = p.y; }
    void TurnOnPlayer() { move_speed = move_speed_tmp; }
private:
    Point coords {.x = 10, .y = 10};
    Point old_coords {.x = 10, .y = 10};
    Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed_tmp;
    int move_speed = 4;
    PlayerState state = PlayerState::ALIVE;
    MovementPhase choose_skin { .phase = 0, .dir = 0 };
    std::unique_ptr<Image> image;
    std::shared_ptr<Castle> castle;
};

#endif //MAIN_PLAYER_H
