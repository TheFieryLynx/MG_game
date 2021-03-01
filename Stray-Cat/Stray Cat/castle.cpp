#include "castle.hpp"
#include <iostream>

void Castle::Draw(std::shared_ptr<Image> screen, std::shared_ptr<Image> pattern)
{
    for(int y = 0; y < tileSize / 2; ++y) {
        for(int x = 0; x < tileSize / 2; ++x) {
            screen->PutPixel(2 * x + coords.x, 2 * y + coords.y,
                            blend(screen->GetPixel(2 * x + coords.x, 2 * y + coords.y), pattern->GetPixel(x, tileSize / 2 - y - 1)));
            screen->PutPixel(2 * x + 1 + coords.x, 2 * y + coords.y,
                            blend(screen->GetPixel(2 * x + 1 + coords.x, 2 * y + coords.y), pattern->GetPixel(x, tileSize / 2 - y - 1)));
            screen->PutPixel(2 * x + coords.x, 2 * y + 1 + coords.y,
                            blend(screen->GetPixel(2 * x + coords.x, 2 * y + 1 + coords.y), pattern->GetPixel(x, tileSize / 2 - y - 1)));
            screen->PutPixel(2 * x + 1 + coords.x, 2 * y + 1 + coords.y,
                            blend(screen->GetPixel(2 * x + 1 + coords.x, 2 * y + 1 + coords.y), pattern->GetPixel(x, tileSize / 2 - y - 1)));
        }
    }
}

Point Castle::DrawRoom(std::shared_ptr<Image> screen, int num)
{
    Point return_point;
    int tilenum = 24 * 24;
    int j, corn;
    for(int i = (num - 1) * tilenum; i < num * tilenum; ++i) {
        coords.x = (i % 24) * tileSize;
        coords.y = (screen->Height() - tileSize) - tileSize * ((i % tilenum) / 24);
        j = temp[i];
        corn = corner[i];
        switch (back[i]) {
            case '.':
                if (j != 0) {
                    Draw(screen, floor[j - 1]);
                }
                if (corn != 0) {
                    Draw(screen, wall[corn - 1]);
                }
                break;
            case '#':
                if (j != 0) {
                    Draw(screen, wall[j - 1]);
                }
                if (corn != 0) {
                    Draw(screen, wall[corn - 1]);
                }
                break;
            case 'x':
                if (j != 0) {
                    Draw(screen, floor[j - 1]);
                }
                if (corn != 0) {
                    Draw(screen, wall[corn - 1]);
                }
                break;
            case '@':
                return_point = { .x = coords.x, .y = coords.y };
                if (j != 0) {
                    Draw(screen, floor[j - 1]);
                }
                if (corn != 0) {
                    Draw(screen, wall[corn - 1]);
                }
                break;
            default:
                break;
        }
    }
    return return_point;
}

void Castle::SetRoom(int room)
{
    current_room = room;
}

void Castle::DrawBackground()
{
    SetRoom(1);
    Point a = DrawRoom(screen, GetRoom());
    a.x += 0;
}
 
void Castle::InitResources() {
    std::string path;
    std::fstream floor_file("../../../Stray Cat/resources/BackGround/floor/floor_paths.txt");
    while (std::getline(floor_file, path)) {
        floor.push_back(std::make_shared<Image>(path));
    }
    floor_file.close();
    
    floor_file.open("../../../Stray Cat/resources/BackGround/wall/wall_paths.txt");
    while (std::getline(floor_file, path)) {
        wall.push_back(std::make_shared<Image>(path));
    }
    floor_file.close();
}

void Castle::InitTemplates()
{
    std::fstream file_temp;
    file_temp.open("../../../Stray Cat/resources/BackGround/Template.txt");
    int inp, i = 0;
    while (file_temp >> inp) {
        temp.push_back(inp);
        i++;
    }
    file_temp.close();
    
    file_temp.open("../../../Stray Cat/resources/BackGround/BackGround.txt");
    i = 0;
    char ch;
    while((ch = file_temp.get()) != EOF) {
        if (ch != '\n') {
            back.push_back(ch);
        }
    }
    file_temp.close();
    
    file_temp.open("../../../Stray Cat/resources/BackGround/WallCorner.txt");
    i = 0;
    while (file_temp >> inp) {
        corner.push_back(inp);
        i++;
    }
    file_temp.close();
}

void Castle::SaveScreen()
{
    screenStates.push_back(screen->Data_save());
}

Point Castle::DrawNewRoom()
{
    return DrawRoom(GetScreen(), GetRoom() + 1);
}

void Castle::SaveNewRoom()
{
    screen->ScreenSave();
}
