/* 
 * File:   Material.h
 * Author: John Patek
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Image.h"


class Material
{
public:
    Image& diffuse() const;
    //TODO: add implementation of Color
    //Color& diffuseColor() const;
    void init();
    
    //TODO: add implementation of Model,View,Projection
    //void render(Model& model,View& view,Projection& projection);
    
    void setupTextures();
};

#endif /* MATERIAL_H */

