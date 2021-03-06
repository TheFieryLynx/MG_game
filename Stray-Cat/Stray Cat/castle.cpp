#include "castle.hpp"
#include <iostream>
#include <math.h>

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


void Castle::DrawRoom(std::shared_ptr<Image> screen, int num)
{
    std::cout << "DRAWING ROOM IS " << num << std::endl;
    int tilenum = 24 * 24;
    int j, corn, x, y;
    //int itm;
    for(int i = (num - 1) * tilenum; i < num * tilenum; ++i) {
        coords.x = (i % 24) * tileSize;
        coords.y = (screen->Height() - tileSize) - tileSize * ((i % tilenum) / 24);
        j = temp[i];
        corn = corner[i];
        //itm = items[i];
        switch (back[i]) {
            case '.':
                if (j != 0) {
                    Draw(screen, floor[j - 1]);
                }
                if (corn != 0) {
                    Draw(screen, wall[corn - 1]);
                }
//                if (itm != 0) {
//                    std::cout << "IMAGE: " << itm << std::endl;
//                    Draw(screen, item_img[itm - 1]);
//                }
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
                if (j != 0) {
                    Draw(screen, floor[j - 1]);
                }
                if (corn != 0) {
                    Draw(screen, wall[corn - 1]);
                }
                y = coords.y / 32;
                x = coords.x / 32;
                std::cout << x << " " << y <<  std::endl;
                std::cout << coords.x / 32 << " " << coords.y / 32 << std::endl;
                if (y < 3 && x > 3 && x < 20) {
                    //std::cout << "A" << std::endl;
                    SetPoint({ .x = coords.x, .y = coords.y }, 1);
                } else if (y > 20 && x > 3 && x < 20) {
                    //std::cout << "B" << std::endl;
                    SetPoint({ .x = coords.x, .y = coords.y }, 3);
                } else if (x < 3 && y > 3 && y < 20) {
                    //std::cout << "C" << std::endl;
                    SetPoint({ .x = coords.x, .y = coords.y }, 0);
                } else if (x > 20 && y > 3 && y < 20) {
                    //std::cout << "D" << std::endl;
                    SetPoint({ .x = coords.x, .y = coords.y }, 2);
                }
                break;
            default:
                break;
        }
    }
}

Point Castle::GetPlayerPoint(std::string s)
{
    std::cout << std::endl << "0: " << possible_points[0].x / 32 << " " << possible_points[0].y / 32 << std::endl;
    std::cout << "1: " << possible_points[1].x / 32 << " " << possible_points[1].y / 32 << std::endl;
    std::cout << "2: " << possible_points[2].x / 32 << " " << possible_points[3].y / 32 << std::endl;
    std::cout << "3: " << possible_points[3].x / 32 << " " << possible_points[3].y / 32 << std::endl;
    
    if (s == "top") {
        return possible_points[1];
    } else if (s == "left") {
        return possible_points[2];
    } else if (s == "right") {
        return possible_points[0];
    } else if (s == "bottom") {
        return possible_points[3];
    }
    return { .x = 384, .y = 384 };
}

void Castle::SetPoint(Point a, int room)
{
    possible_points[room] = a;
}

void Castle::SetRoom(int room)
{
    //std::cout << "!!!" << std::endl;
    current_room = room;
}

void Castle::DrawBackground()
{
    DrawRoom(screen, GetRoom());
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
    
//    floor_file.open("../../../Stray Cat/resources/BackGround/items/item_paths.txt");
//    while (std::getline(floor_file, path)) {
//        //std::cout << path << std::endl;
//        item_img.push_back(std::make_shared<Image>(path));
//    }
//    floor_file.close();
    
}

void Castle::InitTemplates()
{
    std::fstream file_temp;
    file_temp.open("../../../Stray Cat/resources/BackGround/Template.txt");
    int inp;
    while (file_temp >> inp) {
        temp.push_back(inp);
    }
    file_temp.close();
    
    file_temp.open("../../../Stray Cat/resources/BackGround/BackGround.txt");
    char ch;
    while((ch = file_temp.get()) != EOF) {
        if (ch != '\n' && ch != ' ') {
            back.push_back(ch);
        }
    }
    file_temp.close();
    
    file_temp.open("../../../Stray Cat/resources/BackGround/WallCorner.txt");
    while (file_temp >> inp) {
        corner.push_back(inp);
    }
    file_temp.close();
    
//    file_temp.open("../../../Stray Cat/resources/BackGround/Items.txt");
//    while (file_temp >> inp) {
//        items.push_back(inp);
//    }
//    file_temp.close();
    
    ReadMap();
}

void Castle::ReadMap()
{
    std::fstream file_temp;
    file_temp.open("../../../Stray Cat/resources/BackGround/Map.txt");
    int n, inp;
    file_temp >> n;
    std::vector<int> vec;
    while (file_temp >> inp) {
        vec.push_back(inp);
    }
    int size = int (vec.size()), line = size / n, y = 0;
    Rooms neighbors;
    for(int i = 0; i < size; i++) {
        if (vec[i] != 0) {
            if (i - line >= 0) {
                neighbors.top = vec[i - line];
            } else {
                neighbors.top = 0;
            }
            y = i / line;
            if (i + 1 < (y + 1) * line) {
                neighbors.right = vec[i + 1];
            } else {
                neighbors.right = 0;
            }
            
            if (i + line < size) {
                neighbors.bottom = vec[i + line];
            } else {
                neighbors.bottom = 0;
            }
            if (i - 1 >= y * line) {
                neighbors.left = vec[i - 1];
            } else {
                neighbors.left = 0;
            }
            room_neighbors[vec[i]] = neighbors;
        }
        
    }
    for (auto i : room_neighbors) {
        std::cout << i.first << ":" << std::endl;
        std::cout << "LEFT: " << i.second.left << std::endl;
        std::cout << "TOP: " << i.second.top << std::endl;
        std::cout << "RIGHT: " << i.second.right << std::endl;
        std::cout << "BOTTOM: " << i.second.bottom << std::endl << std::endl;
    }
}

void Castle::DrawNewRoom()
{
    DrawRoom(GetScreen(), GetRoom());
}

void Castle::SaveNewRoom()
{
    screen->ScreenSaveClean();
    screen->ScreenSave();
    
}

//=================================================================================================

void Items::InitResources()
{
    std::fstream file_temp;
    file_temp.open("../../../Stray Cat/resources/BackGround/Items.txt");
    char ch;
    //std::cout << "!!!!!!!!!!!!!" << std::endl;
    while((ch = file_temp.get()) != EOF) {
        if (ch != '\n' && ch != ' ') {
            tmp.push_back(ch);
        }
    }
    file_temp.close();
}

void Items::InitStaticImages()
{
    std::string path;
    std::fstream floor_file("../../../Stray Cat/resources/BackGround/items/bench.txt");
    while (std::getline(floor_file, path)) {
        bench.push_back(std::make_shared<Image>(path));
    }
    floor_file.close();
    
    std::fstream door_file("../../../Stray Cat/resources/BackGround/items/Door/door_paths.txt");
    while (std::getline(door_file, path)) {
        std::cout << path << std::endl;
        door.push_back(std::make_shared<Image>(path));
    }
    door_file.close();
}

void Items::InitAnimatedImages()
{
    std::string path;
    std::fstream floor_file("../../../Stray Cat/resources/BackGround/items/torch/torch.txt");
    while (std::getline(floor_file, path)) {
        torch.push_back(std::make_shared<Image>(path));
    }
    floor_file.close();
}

void Items::ReadTemplate(int room)
{
    int x, y, tilenum = 576;
    for(int i = (room - 1) * tilenum; i < room * tilenum; ++i) {
        x = (i % 24) * tileSize;
        y = (768 - tileSize) - tileSize * ((i % tilenum) / 24);
        switch (tmp[i]) {
            case 't': //torch
                torch_location.push_back({ .x = x, .y = y });
                break;
            case 'b': //bench
                bench_location.push_back({ .x = x, .y = y });
                break;
            case 'd':
                door_location.push_back({ .x = x, .y = y });
                break;
        }
    }
}

void Items::Draw(std::shared_ptr<Image> screen, std::shared_ptr<Image> pattern, Point coords, double p)
{
    Pixel pix;
    for(int y = 0; y < tileSize / 2; ++y) {
        for(int x = 0; x < tileSize / 2; ++x) {
            pix = pattern->GetPixel(x, tileSize / 2 - y - 1);
            pix.r *= p;
            pix.g *= p;
            pix.b *= p;
            pix.a *= p;
            screen->PutPixel(2 * x + coords.x, 2 * y + coords.y,
                            blend(screen->GetPixel(2 * x + coords.x, 2 * y + coords.y), pix));
            pix = pattern->GetPixel(x, tileSize / 2 - y - 1);
            pix.r *= p;
            pix.g *= p;
            pix.b *= p;
            pix.a *= p;
            screen->PutPixel(2 * x + 1 + coords.x, 2 * y + coords.y,
                            blend(screen->GetPixel(2 * x + 1 + coords.x, 2 * y + coords.y), pix));
            pix = pattern->GetPixel(x, tileSize / 2 - y - 1);
            pix.r *= p;
            pix.g *= p;
            pix.b *= p;
            pix.a *= p;
            screen->PutPixel(2 * x + coords.x, 2 * y + 1 + coords.y,
                            blend(screen->GetPixel(2 * x + coords.x, 2 * y + 1 + coords.y), pix));
            pix = pattern->GetPixel(x, tileSize / 2 - y - 1);
            pix.r *= p;
            pix.g *= p;
            pix.b *= p;
            pix.a *= p;
            screen->PutPixel(2 * x + 1 + coords.x, 2 * y + 1 + coords.y,
                            blend(screen->GetPixel(2 * x + 1 + coords.x, 2 * y + 1 + coords.y), pix));
        }
    }
}

void Items::DrawSaved(std::shared_ptr<Image> screen, Point coords)
{
    for(int y = coords.y; y < coords.y + tileSize; ++y)
    {
        for(int x = coords.x; x < coords.x + tileSize; ++x)
        {
            Pixel pix = screen->GetSavedCleanPixel(x, y);
            screen->PutPixel(x, y, pix);
        }
    }
}



void Items::DrawAnimatedImages(std::shared_ptr<Image> screen, float time)
{
    //Draw torch
    static float cnt = 0;
    cnt += time * 4;
    int j = int(cnt) % 4;
    //std::cout << "kekw " << cnt  << " " <<  j << " " << torch_location.size() << std::endl;
    for(auto i : torch_location) {
        //std::cout << "coords" << i.x << " " << i.y << std::endl;
        DrawSaved(screen, i);
        Draw(screen, torch[j], i, 1);
        screen->UpdateSavedTile(i.x, i.y, screen);
    }
}

void Items::DrawStaticImages(std::shared_ptr<Image> screen)
{
    int bench_i = 0;
    for(auto i : bench_location) {
        Draw(screen, bench[(bench_i++) % 2], i, 1);
    }
}

void Items::DrawDoor(std::shared_ptr<Image> screen, bool is_opened, double p)
{
    int door_i = 0;
    if (is_opened) {
        std::cout << "!!!!!" << std::endl;
        door_i = 4;
    }
    for(auto i : door_location) {
        if (is_opened) {
            std::cout << i.x << " " << i.y << std::endl;
            DrawSaved(screen, i);
            Draw(screen, door[(door_i++) % 4], i, p);
        } else {
            Draw(screen, door[(door_i++) % 4], i, 1);
        }
    }
}

void Items::Clear()
{
    torch_location.clear();
    bench_location.clear();
    door_location.clear();
}


//=================================================================

void Inventory::Draw(std::shared_ptr<Image> pattern)
{
    for(int y = 0; y < tileSize / 2; ++y) {
        for(int x = 0; x < tileSize / 2; ++x) {
            inv_screen->PutPixel(2 * x + coords.x, 2 * y + coords.y,
                            blend(inv_screen->GetPixel(2 * x + coords.x, 2 * y + coords.y), pattern->GetPixel(x, tileSize / 2 - y - 1)));
            inv_screen->PutPixel(2 * x + 1 + coords.x, 2 * y + coords.y,
                            blend(inv_screen->GetPixel(2 * x + 1 + coords.x, 2 * y + coords.y), pattern->GetPixel(x, tileSize / 2 - y - 1)));
            inv_screen->PutPixel(2 * x + coords.x, 2 * y + 1 + coords.y,
                            blend(inv_screen->GetPixel(2 * x + coords.x, 2 * y + 1 + coords.y), pattern->GetPixel(x, tileSize / 2 - y - 1)));
            inv_screen->PutPixel(2 * x + 1 + coords.x, 2 * y + 1 + coords.y,
                            blend(inv_screen->GetPixel(2 * x + 1 + coords.x, 2 * y + 1 + coords.y), pattern->GetPixel(x, tileSize / 2 - y - 1)));
        }
    }
}

void Inventory::InitResources()
{
    std::string path;
    std::fstream floor_file("../../../Stray Cat/resources/Inventory/Inventory_paths.txt");
    while (std::getline(floor_file, path)) {
        //std::cout << path << std::endl;
        inv_img.push_back(std::make_shared<Image>(path));
    }
    floor_file.close();
}

void Inventory::ReadTempate()
{
    std::fstream file_temp;
    file_temp.open("../../../Stray Cat/resources/Inventory/Background.txt");
    char ch;
    while((ch = file_temp.get()) != EOF) {
        if (ch != '\n' && ch != ' ') {
            //std::cout << ch;
            back.push_back(ch);
        }
    }
    file_temp.close();
}

void Inventory::DrawInventory()
{
    int tilenum = 11 * 24;
    for (int i = 0; i < tilenum; ++i) {
        coords.x = (i % 11) * tileSize;
        coords.y = (768 - tileSize) - tileSize * ((i % tilenum) / 11);
        //std::cout << "============ " << back[i] << coords.x << " " << coords.y << std::endl;
        switch (back[i]) {
            case 't':
                Draw(inv_img[2]);
                break;
            case 'q':
                Draw(inv_img[1]);
                break;
            case 'e':
                Draw(inv_img[3]);
                break;
            case 'l':
                Draw(inv_img[4]);
                break;
            case 'r':
                Draw(inv_img[5]);
                break;
            case 'b':
                Draw(inv_img[7]); 
                break;
            case 'z':
                Draw(inv_img[6]);
                break;
            case 'v':
                Draw(inv_img[8]);
                break;
            case 'o':
                Draw(inv_img[0]);
                break;
        }
    }
}
