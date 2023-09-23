#pragma once

#include "main.h"


struct TRANSFORM 
{
    XMFLOAT3 pos;
    XMFLOAT3 bound;
    XMFLOAT3 scale;
    XMFLOAT3 velocity;
    XMFLOAT3 rotation;
    XMFLOAT3 rotVel;

    TRANSFORM* parent = nullptr;
};


void  Transform_Update(TRANSFORM* xform, double dTime);
void  Transform_ConstantAcc(TRANSFORM* xform, XMFLOAT3 acc);
float Transform_LookAt(TRANSFORM* xform, XMFLOAT3 target);