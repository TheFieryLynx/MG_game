
#ifndef castle_hpp
#define castle_hpp

#include "Point.h"
#include "Image.h"
#include <iostream>

#include <fstream>
#include <vector>
#include <map>
#include <sstream>

struct Rooms
{
    int left;
    int right;
    int top;
    int bottom;
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
    Point DrawRoom(std::shared_ptr<Image> screen, int num);
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
    
    void ReadMap();
    
    //void SaveScreen();
    Point DrawNewRoom();
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
    
    //Castle& operator= (const Castle &cast) = delete;
    
//    Castle& operator= (const Castle &cast)
//    {
//        return std::make_shared<Castle>(cast);
//
//    }
    //std::vector<Pixel *> screenStates;
};


#endif /* castle_hpp */
