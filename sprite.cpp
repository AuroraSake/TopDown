//=============================================================================
//
// スプライト処理 [sprite.cpp]
// Author : GP11B132 99 外岡高明
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


struct Atlas
{
    //defien how many sprit an atlas holds
    int x;//splites in X axis
    int y;//splotes in Y axis
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
Sprite getSpriteProp(SpriteIndex spriteID);




//*****************************************************************************
// グローバル変数
//*****************************************************************************

static ID3D11ShaderResourceView *sTexture[TEX_MAX] = { NULL };	        // テクスチャ情報

static char                     *sTexturName[TEX_MAX] = 
{
	"data/TEXTURE/tachyonship-Animated.png"
};

static Atlas TexureAtlas[TEX_MAX] = 
{
    {4, 1},
};

//static Sprite *SpriteAtlas[TEX_MAX];

static BOOL gIsLoaded = FALSE;

//=============================================================================
// 頂点データ設定
//=============================================================================

void Sprite_Init()
{
    ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEX_MAX; i++)
	{
		sTexture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			sTexturName[i],
			NULL,
			NULL,
			&sTexture[i],
			NULL);

	}

	gIsLoaded = TRUE;
}

void Sprite_Uninit()
{
    if (gIsLoaded == FALSE) return;

	for (int i = 0; i < TEX_MAX; i++)
	{
		if (sTexture[i])
		{
			sTexture[i]->Release();
			sTexture[i] = NULL;
		}
	}

	gIsLoaded = FALSE;
}

void Sprite_2DTile(ID3D11Buffer *buf,
    float X, float Y, float Width, float Height,
    SpriteIndex spriteID, XMFLOAT4 Color)
{
    
    GetDeviceContext()->PSSetShaderResources(0, 1, &sTexture[spriteID.TexID]);

	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

    Sprite spriteProp = getSpriteProp(spriteID);
   
	// 左上を原点として設定するプログラム
	vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	vertex[0].Diffuse  = Color;
	vertex[0].TexCoord = XMFLOAT2(spriteProp.UVStart.x, spriteProp.UVStart.y);

	vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	vertex[1].Diffuse  = Color;
	vertex[1].TexCoord = XMFLOAT2(spriteProp.UVEnd.x, spriteProp.UVStart.y);

	vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	vertex[2].Diffuse  = Color;
	vertex[2].TexCoord = XMFLOAT2(spriteProp.UVStart.x, spriteProp.UVEnd.y);

	vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	vertex[3].Diffuse  = Color;
	vertex[3].TexCoord = XMFLOAT2(spriteProp.UVEnd.x, spriteProp.UVEnd.y);



	GetDeviceContext()->Unmap(buf, 0);

}




void Sprite_2DSprite(ID3D11Buffer *buf, float X, float Y, float Width, float Height, SpriteIndex spriteID,
	XMFLOAT4 Color, float Rot)
{
    
    GetDeviceContext()->PSSetShaderResources(0, 1, &sTexture[spriteID.TexID]);

	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width  * 0.5f;		// コンピューターは割り算が苦手
	hh = Height * 0.5f;		// 掛け算の方が処理が速い

    int AtlasMax = TexureAtlas[spriteID.TexID].x * TexureAtlas[spriteID.TexID].y;
    
    Sprite spriteProp = getSpriteProp(spriteID);

    XMFLOAT4 vertices[4] = 
    {
        {- hw, - hh, spriteProp.UVStart.x , spriteProp.UVStart.y},
        {  hw, - hh, spriteProp.UVEnd.x   , spriteProp.UVStart.y},
        {- hw,   hh, spriteProp.UVStart.x , spriteProp.UVEnd.y},
        {  hw,   hh, spriteProp.UVEnd.x   , spriteProp.UVEnd.y},
    };
    
    float Sr = sinf(Rot);
    float Cr = cosf(Rot);
    for(int i = 0; i < 4; i++)
    {

	    float x = X + vertices[i].x * Cr - vertices[i].y * Sr;;
	    float y = Y + vertices[i].x * Sr + vertices[i].y * Cr;;
	    
        vertex[i].Position = XMFLOAT3(x, y, 0.0f);

        vertex[i].Diffuse = Color;
	    vertex[i].TexCoord = XMFLOAT2(vertices[i].z,vertices[i].w);
    }

	GetDeviceContext()->Unmap(buf, 0);
    GetDeviceContext()->Draw(4, 0);
}


void Sprite_2DStripe(ID3D11Buffer *buf, XMFLOAT3 pos , float Rot,
	XMFLOAT4 Color, XMFLOAT4 vertices[], int index)
{
    D3D11_MAPPED_SUBRESOURCE msr;
    GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;
    
    float Sr = sinf(Rot);
    float Cr = cosf(Rot);

    for(int i = 0; i < index; i++)
    {

        float x = pos.x + vertices[i].x * Cr - vertices[i].y * Sr;
        float y = pos.y + vertices[i].x * Sr + vertices[i].y * Cr;
	    vertex[i].Position = XMFLOAT3(x, y, 0.0f);
	    
        vertex[i].Diffuse = Color;
	    vertex[i].TexCoord = XMFLOAT2(vertices[i].z,vertices[i].w);

    }
        GetDeviceContext()->Unmap(buf, 0);
        GetDeviceContext()->Draw(index, 0);
}


Sprite getSpriteProp(SpriteIndex spriteID)
{
    Sprite spriteProp;
    {
        int index = spriteID.Index;
        int x = TexureAtlas[spriteID.TexID].x;
        int y = TexureAtlas[spriteID.TexID].y;

        float Ulenth = 1.0f / x;
        float Vlenth = 1.0f / y;

        spriteProp.UVStart = {Ulenth * (index % x) , Vlenth * (index / x)};
        spriteProp.UVEnd   = {spriteProp.UVStart.x + Ulenth, spriteProp.UVStart.y + Vlenth};
    }

    if(spriteID.reverse & Reverse_X)
    {
        float temp = spriteProp.UVStart.x;
        spriteProp.UVStart.x = spriteProp.UVEnd.x;
        spriteProp.UVEnd.x   = temp;
    }
    
    if(spriteID.reverse & Reverse_Y)
    {
        float temp = spriteProp.UVStart.y;
        spriteProp.UVStart.y = spriteProp.UVEnd.y;
        spriteProp.UVEnd.y   = temp;
    }

    return spriteProp;
}