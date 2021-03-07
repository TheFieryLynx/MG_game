#include "Player.h"

#include <iostream>
bool Player::Moved() const
{
    if(coords.x == old_coords.x && coords.y == old_coords.y) {
        return false;
    } else {
        return true;
    }
}

void Player::SetPhase(int phase)
{
    choose_skin.phase = phase; 
}

bool Player::CheckWall(Point coord)
{
    int x_int = coord.x / tileSize;
    int y_int = coord.y / tileSize;
    if (castle->GetBackGround()[(23 - y_int) * 24 + 24 * 24 * (castle->GetRoom() - 1) + x_int] == '#') {
        return true;
    }
    return false;
}

bool Player::CheckCorner(Point coord)
{
    int x_int = coord.x / tileSize;
    int y_int = coord.y / tileSize;
    
    if (castle->GetWallCorner()[(23 - y_int) * 24 + 24 * 24 * (castle->GetRoom() - 1) + x_int] != 0) {
        return true;
    }
    return false;
}

bool Player::CheckExit(Point coord)
{
    int x_int = coord.x / tileSize;
    int y_int = coord.y / tileSize;
    if (castle->GetBackGround()[(23 - y_int) * 24 + 24 * 24 * (castle->GetRoom() - 1) + x_int] == 'x') {
        return true;
    }
    return false;
}

bool Player::CheckKey(Point coord)
{
    int x_int = coord.x / tileSize;
    int y_int = coord.y / tileSize;
    
    for(auto i : items->GetKeyLocation()) {
        
        if (i.x / 32 == x_int && i.y / 32 == y_int) {
            return true;
        }
    }
    return false;
}

bool Player::CheckDoor(Point coord)
{
    int x_int = coord.x / tileSize;
    int y_int = coord.y / tileSize;
    std::vector<Point> doors = items->GetDoorLocation();
    for(auto i : doors) {
        
        if (i.x / tileSize == x_int && i.y / tileSize == y_int) {
            return true;
        }
    }
    return false;
}

bool Player::CheckWin(Point coord)
{
    int x_int = coord.x / tileSize;
    int y_int = coord.y / tileSize;
    if (castle->GetBackGround()[(23 - y_int) * 24 + 24 * 24 * (castle->GetRoom() - 1) + x_int] == 'Q') {
        return true;
    }
    return false;
}

bool Player::CheckMonster(Point coord)
{
    return monsters->matrix[coord.x / 32][coord.y / 32];
}

void Player::ProcessInput(PlayerAction dir)
{
    int move_dist = move_speed * 1;
    Point tmp, tmp1;
    int offset = 0;
    switch(dir)
    {
        case PlayerAction::UP:
            choose_skin.dir = 2;
            old_coords.y = coords.y;
            tmp.x = coords.x;
            tmp.y = coords.y + move_dist + 28;
            tmp1.x = coords.x + 25;
            tmp1.y = coords.y + move_dist + 28;
            if (coords.y + move_dist + 28 < 768) {
                if (!CheckWall(tmp) && !CheckWall(tmp1)) { //!items->GetDoorStatus() &&
                    if ((!CheckDoor(tmp) && !CheckDoor(tmp1)) || items->GetDoorStatus(castle->GetRoom())) {
                        coords.y += move_dist;
                    }
                }
            }
            break;
        case PlayerAction::DOWN:
            if (CheckCorner(coords)) {
                offset = 5;
            }
            choose_skin.dir = 1;
            old_coords.y = coords.y;
            tmp.x = coords.x;
            tmp.y = coords.y - move_dist - offset;
            tmp1.x = coords.x + 25;
            tmp1.y = coords.y - move_dist - offset;
            if (coords.y - move_dist - offset > -1) {
                if (!CheckWall(tmp) && !CheckWall(tmp1)) {
                    coords.y -= move_dist;
                }
            }
            break;
        case PlayerAction::LEFT:
            choose_skin.dir = 0;
            old_coords.x = coords.x;
            tmp.x = coords.x - move_dist;
            tmp.y = coords.y;
            if (coords.x - move_dist > -1) {
                if (!CheckWall(tmp)) {
                    coords.x -= move_dist;
                }
            }
            break;
        case PlayerAction::RIGHT:
            choose_skin.dir = 3;
            old_coords.x = coords.x;
            tmp.x = coords.x + move_dist + 25;
            tmp.y = coords.y;
            if (coords.x + move_dist + 25 < 768) {
                if (!CheckWall(tmp)) {
                    coords.x += move_dist;
                }
            }
            break;
        case PlayerAction::INTERACTION:
            std::cout << "TAKE THIS ITEM" << std::endl;
            
            if (CheckDoor({ .x = coords.x, .y = coords.y + 32 }) && num_of_keys) {
                num_of_keys--;
                move_speed_tmp = move_speed;
                move_speed = 0;
                state = PlayerState::OPENING_DOOR;
                std::cout << "KEKE" << std::endl;
                items->SetDoorStatus(castle->GetRoom(), 1);
            }
            
           
        default:
            break;
    }
    int next_room = 0;
    if (CheckKey(coords)) {
        items->SetKeyStatus(castle->GetRoom(), false);
        num_of_keys++;
        items->ClearKeyLocation();
        std::cout << num_of_keys << std::endl;
    }
    
    if (CheckMonster(coords)) {
        move_speed_tmp = move_speed;
        move_speed = 0;
        state = PlayerState::DYING;
    }
    
    if (CheckWin(coords)) {
        move_speed_tmp = move_speed;
        move_speed = 0;
        state = PlayerState::WINNING;
    }
    
    if (CheckExit(coords) && move_speed != 0) {
        move_speed_tmp = move_speed;
        move_speed = 0;
        state = PlayerState::CHANGING_ROOM;
        
        if (coords.y >= 768 - 32 && coords.y < 768) {
            next_room = castle->GetRoomNeighbors()[castle->GetRoom()].top;
            next_room_direction = "top";
        } else if (coords.y > 0 && coords.y < 32) {
            next_room = castle->GetRoomNeighbors()[castle->GetRoom()].bottom;
            next_room_direction = "bottom";
        } else if (coords.x > 0 && coords.x < 32) {
            next_room = castle->GetRoomNeighbors()[castle->GetRoom()].left;
            next_room_direction = "left";
        } else if (coords.x >= 768 - 32 && coords.x < 768) {
            next_room = castle->GetRoomNeighbors()[castle->GetRoom()].right;
            next_room_direction = "right";
        }
        
        if (next_room == 0) {
            std::cout << "NO ROOM THERE" << std::endl;
        } else {
            
            castle->SetRoom(next_room);
            
        }
        
    }
}


void Player::SetCastle(Castle *cast)
{
    castle = cast;
}

void Player::SetItems(Items *itm)
{
    items = itm;
}

void Player::SetMonsters(Monster *mnst)
{
    monsters = mnst;
}


void Player::Draw(std::shared_ptr<Image> screen)
{
    if(Moved())
    {
        for(int y = old_coords.y; y < old_coords.y + tileSize; ++y)
        {
            for(int x = old_coords.x; x < old_coords.x + tileSize; ++x)
            {
                Pixel pix = screen->GetSavedPixel(x, y);
                screen->PutPixel(x, y, pix);
            }
        }
        old_coords = coords;
    } else {
        choose_skin.phase = 0;
    }
    
    for(int y = coords.y; y < coords.y + tileSize; ++y)
    {
        for(int x = coords.x; x < coords.x + tileSize; ++x)
        {
            Pixel oldPix = screen->GetSavedPixel(x, y);
            Pixel newPix = image->GetPixel(choose_skin.dir * tileSize + x - coords.x, (choose_skin.phase + 1) * tileSize - y + coords.y - 1);
            screen->PutPixel(x, y, blend(oldPix, newPix));
        }
    }
}
