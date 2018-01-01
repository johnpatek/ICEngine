/* 
 * File:   Camera.h
 * Author: John Patek
 */

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
private:
    float mFieldOfView;
    float mNearClippingPane;
    float mFarClippingPane;
    bool mOffscreen;
    //TODO: add implementation of Rectangle,ManagerProtocal,mat4
    //Rectangle mRectangle
    //ManagerProtocol mManagerProtocol;
public:
    void init();
    
    float getFieldOfView() const;
    float getFieldOfView() const;
    float getFieldOfView() const;
    bool isOffscreen() const;
    
    float getFieldOfView(const float fieldOfView);
    float getFieldOfView() const;
    float getFieldOfView() const;
    bool isOffscreen() const;
    
    
    
    //mat4 getViewMatrix();
    //mat4 getProjectionMatrix();
};

#endif /* CAMERA_H */

