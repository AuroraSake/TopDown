#include "mathW.h"

using namespace DirectX;

vec3 NormalizeVec3(vec3 &vec, float mag)
{
	vec3 result;

    XMStoreFloat3(&result, XMVector3Normalize(XMLoadFloat3(&vec)));
    result = FloatMult(result, mag, FLOAT3_SIZE);

    return result;
}


vec3 FloatAdd  (vec3& tar, vec3& ope, int size)
{
    float* ftar;
    float* fope;
    ftar = (float*)&tar;
    fope = (float*)&ope;
    XMFLOAT3 float3Result;
    float*   result = (float*) &float3Result;

    size = size * 0.25;//Float 4 byte, convert byte size to float size.
    if(size < 1)
        return tar;
    for(int i = 0; i < size; i++)
    {
        if(fabsf(*(fope + i) - 1.0f) < FLOAT_EQUALITY_PERCISION)
            continue;
        *(result + i) = *(ftar + i) + *(fope + i);
    }
    return float3Result;
}

vec3 FloatMult (vec3&tar, vec3& ope, int size)
{
    float* ftar;
    float* fope;
    ftar = (float*)&tar;
    fope = (float*)&ope;
    XMFLOAT3 float3Result;
    float*   result = (float*) &float3Result;
    
    size = size * 0.25;
    if(size < 1)
        return tar;
    for(int i = 0; i < size; i++)
    {
        if(fabsf(*(fope + i) - 1.0f) < FLOAT_EQUALITY_PERCISION)
            continue;
        *(result + i) = *(ftar + i) * *(fope + i);
    }
    
    return float3Result;
}

vec3 FloatMult (vec3& tar, float mul, int size)
{
    float* ftar;
    float* fope;
    ftar = (float*)&tar;
    fope = &mul;
    XMFLOAT3 float3Result;
    float*   result = (float*) &float3Result;
    
    size = size * 0.25;
    if(size < 1)
        return tar;
    for(int i = 0; i < size; i++)
    {
        if(fabsf(*(fope + i) - 1.0f) < FLOAT_EQUALITY_PERCISION)
            continue;
        *(result + i) = *(ftar + i) * *(fope);
    }
    
    return float3Result;
}
