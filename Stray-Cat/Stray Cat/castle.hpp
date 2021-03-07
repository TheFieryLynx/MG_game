
#ifndef castle_hpp
#define castle_hpp

#include "Point.h"
#include "Image.h"
#include <iostream>

#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <array>

struct Rooms
{
    int left;
    int right;
    int top;
    int bottom;
};

struct Monster
{
    Monster() {
        ReadTemplate();
        InitResources();
    }
    void ReadTemplate();
    void InitMatrix();
    void InitResources();
    void CleanMatrix();
    void ReadTemplate(int room);
    void DrawMonsters(std::shared_ptr<Image> screen, float time);
    void DrawSaved(std::shared_ptr<Image> screen, Point coords);
    void Clear();
    void Draw(std::shared_ptr<Image> screen, std::shared_ptr<Image> pattern, Point coords, double p);
    std::vector<std::vector<int>> matrix;
private:
    std::vector<char> tmp;
    std::vector<Point> monster1_location;
    std::vector<std::shared_ptr<Image>> monster1;
    
    std::vector<Point> monster2_location;
    std::vector<std::shared_ptr<Image>> monster2;
    
    std::vector<Point> monster3_location;
    std::vector<std::shared_ptr<Image>> monster3;
    
    std::vector<Point> monster4_location;
    std::vector<std::shared_ptr<Image>> monster4;
    
    std::vector<Point> monster5_location;
    std::vector<std::shared_ptr<Image>> monster5;
    
    std::vector<Point> monster6_location;
    std::vector<std::shared_ptr<Image>> monster6;
    
    
    
};

struct Inventory
{
    Inventory() {
        InitResources();
        ReadTempate();
    }
    void ReadTempate();
    void InitResources();
    void SetScreen(Image &s) { inv_screen = std::make_shared<Image>(s) ;};
    void Draw(std::shared_ptr<Image> pattern);
    void DrawFont(std::shared_ptr<Image> pattern);
    void DrawInventory(int num_keys);
private:
    Point coords {.x = 0, .y = 0};
    std::vector<std::shared_ptr<Image>> inv_img;
    std::vector<std::shared_ptr<Image>> fonts;
    std::shared_ptr<Image> inv_screen;
    std::vector<char> back;
};

struct Items
{
    Items() {
        InitAnimatedImages();
        InitStaticImages();
        InitResources();
    }
    void Draw(std::shared_ptr<Image> screen, std::shared_ptr<Image> pattern, Point coords, double p);
    void InitResources();
    void ReadTemplate(int room);
    void InitAnimatedImages();
    void InitStaticImages();
    void DrawAnimatedImages(std::shared_ptr<Image> screen, float timer);
    void DrawStaticImages(std::shared_ptr<Image> screen);
    void DrawDoor(std::shared_ptr<Image> screen, int opening_status, double p);
    void DrawSavedWithoutItems(std::shared_ptr<Image> screen, Point coords);
    void Clear();
    void DrawSaved(std::shared_ptr<Image> screen, Point coords);
    int GetDoorStatus(int room);
    void SetDoorStatus(int room, bool status);
    bool GetKeyStatus(int room);
    void SetKeyStatus(int room, bool status);
    
    std::vector<Point> GetDoorLocation() { return door_location; }
    std::vector<Point> GetKeyLocation() { return key_location ; }
    void DrawKey(std::shared_ptr<Image> screen, float time, bool status);
    void ClearKeyLocation() { key_location.clear(); }
    int current_room;
private:
    std::vector<char> tmp;
    std::map<int, bool> key_in_room;
    std::map<int, int> door_is_opened;
    std::vector<Point> bench_location;
    std::vector<Point> torch_location;
    std::vector<Point> door_location;
    std::vector<Point> key_location;
    std::vector<Point> key_location_copy;
    std::vector<std::shared_ptr<Image>> key;
    std::vector<std::shared_ptr<Image>> torch;
    std::vector<std::shared_ptr<Image>> bench;
    std::vector<std::shared_ptr<Image>> door;
};

struct Castle
{
    explicit Castle(Point pos = {.x = 0, .y = 0}, int room = 1) : coords(pos), current_room(room) {
        std::cout << "I'm Constuctor" << std::endl;
        InitTemplates();
        InitResources();
    };
    void Draw(std::shared_ptr<Image> screen, std::shared_ptr<Image> pattern);
    void DrawBackground();
    void DrawRoom(std::shared_ptr<Image> screen, int num);
    std::vector<char> GetBackGround() { return back; };
    std::vector<int> GetWallCorner() { return corner; };
    std::vector<std::shared_ptr<Image>> GetWall(){ return wall; };
    void SetRoom(int room);
    void SetScreen(Image &s) { screen = std::make_shared<Image>(s) ;};
    int GetRoom() { return current_room; };
    std::map<int, Rooms> GetRoomNeighbors() { return room_neighbors;  }
    std::shared_ptr<Image> GetScreen() { return screen; }
    void InitTemplates();
    void InitResources();
    void SetPoint(Point a, int room); // 0 - left, 1 - top, 2 - right, 3 - bottom;
    Point GetPlayerPoint(std::string s);
    void ReadMap();
    
    void DrawNewRoom();
    void SaveNewRoom();
private:
    Point coords {.x = 0, .y = 0};
    int current_room;
    std::vector<int> temp;
    std::vector<int> corner;
    
    std::vector<char> back;
    std::shared_ptr<Image> screen;
    std::vector<std::shared_ptr<Image>> floor;
    std::vector<std::shared_ptr<Image>> wall;

    std::map<int, Rooms> room_neighbors;
    std::array<Point, 4> possible_points;
};


#endif /* castle_hpp */
