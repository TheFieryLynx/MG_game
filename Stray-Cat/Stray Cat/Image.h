#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>

constexpr int tileSize = 32;

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

constexpr Pixel backgroundColor{0, 0, 0, 0};
Pixel blend(Pixel oldPixel, Pixel newPixel);

struct Image
{
    explicit Image(const std::string &a_path);
    Image(int a_width, int a_height, int a_channels);
    
    int Save(const std::string &a_path);
    
    int Width()    const { return width; }
    int Height()   const { return height; }
    int Channels() const { return channels; }
    size_t Size()  const { return size; }
    Pixel* Data()        { return  data; }
    Pixel* Data_save()   { return  data_save; }
    
    Pixel GetPixel(int x, int y) { return data[width * y + x];}
    void  PutPixel(int x, int y, const Pixel &pix) { data[width * y + x] = pix; }
    
    Pixel GetSavedPixel(int x, int y) { return data_save[width * y + x];}
    Pixel GetSavedCleanPixel(int x, int y) { return data_clean[width * y + x];}
    Pixel GetSavedWithoutItems(int x, int y) { return data_without_items[width * y + x];}
    
    void UpdateSavedTile(int x, int y, std::shared_ptr<Image> screen);
    
    void ScreenSave();
    void ScreenSaveClean();
    void ScreenSaveWithoutItems();
    
    ~Image();
    
private:
    int width = -1;
    int height = -1;
    int channels = 3;
    size_t size = 0;
    Pixel *data = nullptr;
    Pixel *data_save = nullptr;
    Pixel *data_clean = nullptr;
    Pixel *data_without_items = nullptr;
    bool self_allocated = false;
};



#endif //MAIN_IMAGE_H
