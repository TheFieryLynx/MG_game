
#ifndef castle_hpp
#define castle_hpp

#include "Point.h"
#include "Image.h"

#include <fstream>
#include <vector>
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
    std::shared_ptr<Image> GetScreen() { return screen; }
    void InitTemplates();
    void InitResources();
    
    void SaveScreen();
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
    
    std::vector<Pixel *> screenStates;
};


#endif /* castle_hpp */
