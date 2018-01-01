/* 
 * File:   Component.h
 * Author: John Patek
 */

#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
private:
    //TODO: add implementation of GameObject
    //GameObject mGameObject;
    //void setup(GameObject& gameObject);
protected:
    //TODO: add implementation of SystemsManager
    //SystemsManager mSystemsManager;
public:
    void init();
    
    void start();
    
    void update();
    
    void fixedUpdate();
    
    void lateUpdate();
};

#endif /* COMPONENT_H */

