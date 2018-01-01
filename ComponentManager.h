/* 
 * File:   ComponentManager.h
 * Author: John Patek
 */

#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <vector>

#include "Component.h"
#include "RenderManager.h"
#include "SceneManager.h"


class ComponentManager
{
private:
    std::vector<Component> mComponents;

protected:
    void reset();
    
public:
    void init();
    void callFunction(Component* component);
    void registerComponent();
};

#endif /* COMPONENTMANAGER_H */

