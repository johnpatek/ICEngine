/* 
 * File:   Component.h
 * Author: John Patek
 */

#ifndef COMPONENT_H
#define COMPONENT_H

#include "SystemManager.h"
#include "GameObject.h"

class Component
{
private:
    GameObject mGameObject;
    void setup(GameObject& gameObject);
protected:
    SystemManager mSystemManager;
public:
    void init();
    
    void start();
    
    void update();
    
    void fixedUpdate();
    
    void lateUpdate();
};

#endif /* COMPONENT_H */

