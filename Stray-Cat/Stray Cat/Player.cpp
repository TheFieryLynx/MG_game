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
    //std::cout << "ROOM: " << castle->GetRoom() << std::endl;
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


void Player::ProcessInput(MovementDir dir)
{
    int move_dist = move_speed * 1;
    Point tmp, tmp1;
    int offset = 0;
    switch(dir)
    {
        case MovementDir::UP:
            choose_skin.dir = 2;
            old_coords.y = coords.y;
            tmp.x = coords.x;
            tmp.y = coords.y + move_dist + 15;
            tmp1.x = coords.x + 25;
            tmp1.y = coords.y + move_dist + 15;
            if (coords.y + move_dist + 15 < 768) {
                if (!CheckWall(tmp) && !CheckWall(tmp1)) {
                    coords.y += move_dist;
                }
            }
            break;
        case MovementDir::DOWN:
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
        case MovementDir::LEFT:
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
        case MovementDir::RIGHT:
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
        default:
            break;
    }
    int next_room = 0;
    //std::cout << CheckExit(coords) << std::endl;
    if (CheckExit(coords) && move_speed != 0) {
        move_speed_tmp = move_speed;
        move_speed = 0;
        state = PlayerState::CHANGING_ROOM;
        
        
        if (coords.y >= 768 - 32 && coords.y < 768) {
            next_room = castle->GetRoomNeighbors()[castle->GetRoom()].top;
        } else if (coords.y > 0 && coords.y < 32) {
            next_room = castle->GetRoomNeighbors()[castle->GetRoom()].bottom;
        } else if (coords.x > 0 && coords.x < 32) {
            next_room = castle->GetRoomNeighbors()[castle->GetRoom()].left;
        } else if (coords.x >= 768 - 32 && coords.x < 768) {
            next_room = castle->GetRoomNeighbors()[castle->GetRoom()].right;
        }
        
        if (next_room == 0) {
            std::cout << "NO ROOM THERE" << std::endl;
        } else {
            std::cout << "Next room: " << next_room << std::endl;
            
            castle->SetRoom(next_room);
            std::cout << "Current room: " << castle->GetRoom() << std::endl;
        }
        //castle->ChangeRoom();
    }
}


void Player::SetCastle(Castle *cast)
{
    castle = cast;
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
