//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP11B132 99 �O������
//
//=============================================================================
#pragma once

enum reverseType 
{
    Reverse_None = 0x00,
    Reverse_X    = 0x01,
    Reverse_Y    = 0x02,
    Reverse_Both = Reverse_X | Reverse_Y, //0x03,

    Reverse_Max,
};

//*****************************************************************************
// �}�N����`
//*****************************************************************************

enum Texture_ID
{
    TEX_Player = 0,

    TEX_MAX,
};


struct Sprite
{
    XMFLOAT2 UVStart;
    XMFLOAT2 UVEnd;
};


//TexID; Index;
struct SpriteIndex
{
    UINT TexID;
    UINT Index;
    UINT reverse;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void Sprite_Init(void);
void Sprite_Uninit(void);

void Sprite_2DTile(ID3D11Buffer *buf,
    float X, float Y, float Width, float Height,
    SpriteIndex spriteID, XMFLOAT4 Color);

void Sprite_2DSprite(ID3D11Buffer *buf, float X, float Y, float Width, float Height, SpriteIndex spriteID,
	XMFLOAT4 Color, float Rot);

void Sprite_2DStripe(ID3D11Buffer *buf, XMFLOAT3 pos , float Rot,
	XMFLOAT4 Color, XMFLOAT4 vertices[], int index);