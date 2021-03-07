#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Point.h"
#include "castle.hpp"


enum class PlayerAction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    INTERACTION
};

enum class PlayerState
{
    ALIVE,
    CHANGING_ROOM,
    OPENING_DOOR,
    DYING,
    WINNING
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
            : coords(pos), old_coords(coords), image(std::make_unique<Image>(path)) {
                castle = NULL;
            };
    bool Moved() const;
    void ProcessInput(PlayerAction dir);
    void Draw(std::shared_ptr<Image> screen);
    void SetPhase(int phase);
    
    void SetCastle(const Castle &cast);
    bool CheckKey(Point coord);
    bool CheckWall(Point coord);
    bool CheckCorner(Point coord);
    bool CheckExit(Point coord);
    bool CheckDoor(Point coord);
    bool CheckMonster(Point coord);
    bool CheckWin(Point coord);
    PlayerState GetState() { return  state; }
    void SetState(PlayerState st) { state = st; }
    void SetCoords(Point p) { coords.x = p.x; coords.y = p.y; }
    void TurnOnPlayer() { move_speed = move_speed_tmp; }
    void SetCastle(Castle *cast);
    void SetItems(Items *itm);
    void SetMonsters(Monster *monster);
    Point GetNewCoords();
    std::string GetRoomDirection() { return next_room_direction; }
    int num_of_keys = 0;
private:
    Monster* monsters;
    Castle* castle;
    Items* items;
    Point coords {.x = 10, .y = 10};
    std::string next_room_direction;
    Point old_coords {.x = 10, .y = 10};
    Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed_tmp;
    int move_speed = 4;
    PlayerState state = PlayerState::ALIVE;
    MovementPhase choose_skin { .phase = 0, .dir = 0 };
    std::unique_ptr<Image> image;
};

#endif //MAIN_PLAYER_H
