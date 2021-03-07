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
            case 'Q':
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
                    SetPoint({ .x = coords.x, .y = coords.y }, 1);
                } else if (y > 20 && x > 3 && x < 20) {
                    SetPoint({ .x = coords.x, .y = coords.y }, 3);
                } else if (x < 3 && y > 3 && y < 20) {
                    SetPoint({ .x = coords.x, .y = coords.y }, 0);
                } else if (x > 20 && y > 3 && y < 20) {
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

int Items::GetDoorStatus(int room)
{
    auto search = door_is_opened.find(room);
    if (search != door_is_opened.end()) {
        return door_is_opened[room];
    } else {
        SetDoorStatus(room, 0);
        return false;
    }
}

void Items::SetDoorStatus(int room, bool status)
{
    door_is_opened[room] = status;
}

bool Items::GetKeyStatus(int room)
{
    auto search = key_in_room.find(room);
    if (search != key_in_room.end()) {
        return key_in_room[room];
    } else {
        SetKeyStatus(room, true);
        return false;
    }
}

void Items::SetKeyStatus(int room, bool status)
{
    key_in_room[room] = status;
}

//=================================================================================================

void Items::InitResources()
{
    std::fstream file_temp;
    file_temp.open("../../../Stray Cat/resources/BackGround/Items.txt");
    char ch;
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
    
    std::fstream key_file("../../../Stray Cat/resources/BackGround/items/keys/key_paths.txt");
    while (std::getline(key_file, path)) {
        std::cout << path << std::endl;
        key.push_back(std::make_shared<Image>(path));
    }
    key_file.close();
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
            case 'd': //door
                door_location.push_back({ .x = x, .y = y });
                break;
            case 'k': //key
                key_location.push_back({ .x = x, .y = y });
                key_location_copy.push_back({ .x = x, .y = y });
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

void Items::DrawSavedWithoutItems(std::shared_ptr<Image> screen, Point coords)
{
    for(int y = coords.y; y < coords.y + tileSize; ++y)
    {
        for(int x = coords.x; x < coords.x + tileSize; ++x)
        {
            Pixel pix = screen->GetSavedWithoutItems(x, y);
            screen->PutPixel(x, y, pix);
        }
    }
}

void Items::DrawKey(std::shared_ptr<Image> screen, float time, bool status)
{
    static float cnt = 0;
    cnt += time * 4;
    int j = int(cnt) % 4;
    if (status) {
        for(auto i : key_location_copy) {
            DrawSaved(screen, i);
            Draw(screen, key[j], i, 1);
            screen->UpdateSavedTile(i.x, i.y, screen);
        }
    } else {
        for(auto i : key_location_copy) {
            DrawSavedWithoutItems(screen, i);
            Draw(screen, key[j], i, 0);
            screen->UpdateSavedTile(i.x, i.y, screen);
        }
    }
}

void Items::DrawAnimatedImages(std::shared_ptr<Image> screen, float time)
{
    //Draw torch
    static int delta = 1;
    static float cnt = 0;
    cnt += time * 4;
    int j = int(cnt) % 4;
    
    for(auto i : torch_location) {
        delta++;
        DrawSaved(screen, i);
        Draw(screen, torch[(j + delta) % 4], i, 1);
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

void Items::DrawDoor(std::shared_ptr<Image> screen, int opening_status, double p)
{
    int door_i = 0;
    for(auto i : door_location) {
        if (opening_status == 1) {
            std::cout << i.x << " " << i.y << std::endl;
            DrawSaved(screen, i);
            Draw(screen, door[(door_i++) % 4], i, p);
        } else if (opening_status == 2) {
            Draw(screen, door[(door_i++) % 4], i, 0);
        } else {
            screen->UpdateSavedTile(i.x, i.y, screen);
            Draw(screen, door[(door_i++) % 4], i, 1);
        }
    }
}

void Items::Clear()
{
    torch_location.clear();
    bench_location.clear();
    door_location.clear();
    key_location.clear();
    key_location_copy.clear();
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

void Inventory::DrawFont(std::shared_ptr<Image> pattern)
{
    for(int y = 0; y < tileSize / 4; ++y) {
        for(int x = 0; x < tileSize / 4; ++x) {
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
        inv_img.push_back(std::make_shared<Image>(path));
    }
    floor_file.close();
    
    std::fstream font_file("../../../Stray Cat/resources/Inventory/font/font_paths.txt");
    while (std::getline(font_file, path)) {
        fonts.push_back(std::make_shared<Image>(path));
    }
    font_file.close();
}

void Inventory::ReadTempate()
{
    std::fstream file_temp;
    file_temp.open("../../../Stray Cat/resources/Inventory/Background.txt");
    char ch;
    while((ch = file_temp.get()) != EOF) {
        if (ch != '\n' && ch != ' ') {
            back.push_back(ch);
        }
    }
    file_temp.close();
}

void Inventory::DrawInventory(int num_keys)
{
    int tilenum = 11 * 24;
    for (int i = 0; i < tilenum; ++i) {
        coords.x = (i % 11) * tileSize;
        coords.y = (768 - tileSize) - tileSize * ((i % tilenum) / 11);
        
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
            case 'k':
                Draw(inv_img[0]);
                Draw(inv_img[9]);
                break;
            case 'o':
                Draw(inv_img[0]);
                break;
            case '-':
                Draw(inv_img[0]);
                Draw(fonts[0]);
                break;
            case 'n':
                Draw(inv_img[0]);
                Draw(fonts[num_keys + 1]);
                break;
        }
    }
}

//========================================================
void Monster::ReadTemplate()
{
    std::fstream file_temp;
    file_temp.open("../../../Stray Cat/resources/Monsters/monsters.txt");
    char ch;
    //std::cout << "!!!!!!!!!!!!!" << std::endl;
    while((ch = file_temp.get()) != EOF) {
        if (ch != '\n' && ch != ' ') {
            tmp.push_back(ch);
        }
    }
    file_temp.close();
}

void Monster::InitResources()
{
    std::string path;
    std::fstream monster1_file("../../../Stray Cat/resources/Monsters/monster1.txt");
    while (std::getline(monster1_file, path)) {
        monster1.push_back(std::make_shared<Image>(path));
    }
    monster1_file.close();
    
    std::fstream monster2_file("../../../Stray Cat/resources/Monsters/monster2.txt");
    while (std::getline(monster2_file, path)) {
        monster2.push_back(std::make_shared<Image>(path));
    }
    monster2_file.close();
    
    std::fstream monster3_file("../../../Stray Cat/resources/Monsters/monster3.txt");
    while (std::getline(monster3_file, path)) {
        monster3.push_back(std::make_shared<Image>(path));
    }
    monster3_file.close();
    
    std::fstream monster4_file("../../../Stray Cat/resources/Monsters/monster4.txt");
    while (std::getline(monster4_file, path)) {
        monster4.push_back(std::make_shared<Image>(path));
    }
    monster4_file.close();
    
    std::fstream monster5_file("../../../Stray Cat/resources/Monsters/monster5.txt");
    while (std::getline(monster5_file, path)) {
        monster5.push_back(std::make_shared<Image>(path));
    }
    monster5_file.close();
    
    std::fstream monster6_file("../../../Stray Cat/resources/Monsters/monster6.txt");
    while (std::getline(monster6_file, path)) {
        monster6.push_back(std::make_shared<Image>(path));
    }
    monster6_file.close();
}

void Monster::ReadTemplate(int room)
{
    int x, y, tilenum = 576;
    for(int i = (room - 1) * tilenum; i < room * tilenum; ++i) {
        x = (i % 24) * tileSize;
        y = (768 - tileSize) - tileSize * ((i % tilenum) / 24);
        switch (tmp[i]) {
            case '1': //torch
                monster1_location.push_back({ .x = x, .y = y });
                break;
            case '2':
                monster2_location.push_back({ .x = x, .y = y });
                break;
            case '3':
                monster3_location.push_back({ .x = x, .y = y });
                break;
            case '4':
                monster4_location.push_back({ .x = x, .y = y });
                break;
            case '5':
                monster5_location.push_back({ .x = x, .y = y });
                break;
            case '6':
                monster6_location.push_back({ .x = x, .y = y });
                break;
        }
    }
}

void Monster::CleanMatrix()
{
    for (auto i : matrix) {
        for (auto j : i) {
            j = 0;
        }
    }
}

void Monster::InitMatrix()
{
    std::vector<std::vector<int>> mx(25, std::vector<int>(25, 0));
    for(auto i : monster1_location) {
        mx[i.x / 32][i.y / 32] = 1;
    }
    for(auto i : monster2_location) {
        mx[i.x / 32][i.y / 32] = 1;
    }
    for(auto i : monster3_location) {
        mx[i.x / 32][i.y / 32] = 1;
    }
    for(auto i : monster4_location) {
        mx[i.x / 32][i.y / 32] = 1;
    }
    for(auto i : monster5_location) {
        mx[i.x / 32][i.y / 32] = 1;
    }
    for(auto i : monster6_location) {
        mx[i.x / 32][i.y / 32] = 1;
    }
    matrix = mx;
}

void Monster::DrawSaved(std::shared_ptr<Image> screen, Point coords)
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

void Monster::Draw(std::shared_ptr<Image> screen, std::shared_ptr<Image> pattern, Point coords, double p)
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

void Monster::DrawMonsters(std::shared_ptr<Image> screen, float time)
{
    //Draw torch
    static float cnt = 0;
    cnt += time * 4;
    int j = int(cnt) % 4;
    //std::cout << "kekw " << cnt  << " " <<  j << " " << torch_location.size() << std::endl;
    for(auto i : monster1_location) {
        //std::cout << "coords" << i.x << " " << i.y << std::endl;
        DrawSaved(screen, i);
        Draw(screen, monster1[j], i, 1);
        screen->UpdateSavedTile(i.x, i.y, screen);
    }
    
    for(auto i : monster2_location) {
        //std::cout << "coords" << i.x << " " << i.y << std::endl;
        DrawSaved(screen, i);
        Draw(screen, monster2[j], i, 1);
        screen->UpdateSavedTile(i.x, i.y, screen);
    }
    
    for(auto i : monster3_location) {
        //std::cout << "coords" << i.x << " " << i.y << std::endl;
        DrawSaved(screen, i);
        Draw(screen, monster3[j], i, 1);
        screen->UpdateSavedTile(i.x, i.y, screen);
    }
    
    for(auto i : monster4_location) {
        //std::cout << "coords" << i.x << " " << i.y << std::endl;
        DrawSaved(screen, i);
        Draw(screen, monster4[j], i, 1);
        screen->UpdateSavedTile(i.x, i.y, screen);
    }
    
    for(auto i : monster5_location) {
        //std::cout << "coords" << i.x << " " << i.y << std::endl;
        DrawSaved(screen, i);
        Draw(screen, monster5[j], i, 1);
        screen->UpdateSavedTile(i.x, i.y, screen);
    }
    
    for(auto i : monster6_location) {
        //std::cout << "coords" << i.x << " " << i.y << std::endl;
        DrawSaved(screen, i);
        Draw(screen, monster6[j], i, 1);
        screen->UpdateSavedTile(i.x, i.y, screen);
    }
    //std::cout << "kekw " << cnt  << " " <<  j << " " << torch_location.size() << std::endl;
}

void Monster::Clear()
{
    monster1_location.clear();
    monster2_location.clear();
    monster3_location.clear();
    monster4_location.clear();
    monster5_location.clear();
    monster6_location.clear();
}
