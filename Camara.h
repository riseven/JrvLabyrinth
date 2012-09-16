#ifndef __CAMARA_H__
#define __CAMARA_H__

#include <allegro.h>
#include <alleggl.h>

class Item ;

class Camara
{
private:
    Item *objetivo ;
public:
    Camara();
    ~Camara();
    void Linkar(Item *item);
    void Pintar();
    void Pintar(int ojo);
};

#endif
