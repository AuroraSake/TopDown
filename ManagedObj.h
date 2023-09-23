#pragma once
#include "main.h"
#include "transform.h"
#include "scene.h"
#include "sprite.h"
#include "mathW.h"

//BOOL                      inUse;
//TRANSFORM                 xform
//ID3D11ShaderResourceView  ppTexture
//function:                 void update (ManagedObj self)
struct ManagedObj
{
    BOOL        inUse;
    TRANSFORM   xform;
    SpriteIndex spriteID;

    void(*Update)( ManagedObj* obj, double dTime);

    void*       prop = nullptr;
};


//type; hp; BOOL flying
struct entityProp
{
    int type;
    int hp;
};

void GameObject_Creat(ManagedObj initial);
void PassiveObject_Creat(ManagedObj initial);