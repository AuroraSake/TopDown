#pragma once



                            //��������֐��ŕ�����float�����Z�A��Z�̎��Ɏg��size�f�[�^�B����XMFLOAT3�����v�Z���Ȃ�����}�N���ɂ���
#define FLOAT3_SIZE     12  //used for FloatAdd and FloatMult--Currently only used to operate XMFLOAT3

#define FLOAT_EQUALITY_PERCISION    (0.00001f)
#define PI                          (3.1415926535897f)



//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************



#include <DirectXMath.h>
#include <math.h>
#include "main.h"

typedef DirectX::XMFLOAT2 vec2;
typedef DirectX::XMFLOAT3 vec3;


vec3 NormalizeVec3(vec3 &vec, float mag);

vec3 FloatAdd  (vec3& tar, vec3& ope,  int size);
vec3 FloatMult (vec3& tar, vec3& ope,  int size);
vec3 FloatMult (vec3& tar, float mul, int size);