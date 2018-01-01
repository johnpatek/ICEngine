/* 
 * File:   Image.h
 * Author: John Patek
 */

#ifndef IMAGE_H
#define IMAGE_H

class Image
{
private:
    std::string mFilePath;
    int mWidth;
    int mHeight;
    int mChannels;
    char* mPixels;
public:
    //TODO: ADD CONSTRUCTORS (JORDAN)
    
    ~Image();//Use this instead of freePixels()
    
    void setFilePath(const std::string& filePath);
    
    char* getPixels() const;
    
    int getWidth() const;
    
    int getHeight() const;
    
    int getChannels() const;
};

#endif /* IMAGE_H */

