#include "transform.h"

void Transform_Update(TRANSFORM* xform, double dTime)
{
    if(xform->parent == nullptr)
    {
        int size = FLOAT3_SIZE;
        float fDtime = (float)dTime;
        XMFLOAT3 dPos   = FloatMult( xform->velocity , fDtime, size);
        XMFLOAT3 dRot   = FloatMult( xform->rotVel   , fDtime, size);

        xform->pos      = FloatAdd ( xform->pos      , dPos , size);
        xform->rotation = FloatAdd ( xform->rotation , dRot , size);
    }
}

//void Transfrom_SetScale(TRANSFORM* xform, XMFLOAT3 scale)
//{
//    xform->scale = scale;
//}

void Transform_ConstantAcc(TRANSFORM* xform, XMFLOAT3 acc)
{
    int size = FLOAT3_SIZE;
    xform->velocity = FloatAdd(xform->velocity, acc, size);
}

float Transform_LookAt(TRANSFORM* xform, XMFLOAT3 target)
{
    //2D LookAt
    return atan2f(xform->pos.y - target.y, xform->pos.x - target.x);
}