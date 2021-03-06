
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
    void DrawInventory();
private:
    Point coords {.x = 0, .y = 0};
    std::vector<std::shared_ptr<Image>> inv_img;
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
    void DrawDoor(std::shared_ptr<Image> screen, bool is_opened, double p);
    void Clear();
    void DrawSaved(std::shared_ptr<Image> screen, Point coords);
    bool GetDoorStatus(){ return door_is_opened; }
    void SetDoorStatus(){ door_is_opened = true; } 
    std::vector<Point> GetDoorLocation() { return door_location; }
private:
    std::vector<char> tmp;
    bool door_is_opened = false;
    std::vector<Point> bench_location;
    std::vector<Point> torch_location;
    std::vector<Point> door_location;
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
    
    //void SaveScreen();
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
    //
    std::map<int, Rooms> room_neighbors;
    std::array<Point, 4> possible_points;
    //Castle& operator= (const Castle &cast) = delete;
    
//    Castle& operator= (const Castle &cast)
//    {
//        return std::make_shared<Castle>(cast);
//
//    }
    //std::vector<Pixel *> screenStates;
};


#endif /* castle_hpp */
