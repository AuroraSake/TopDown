//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "transform.h"
#include "input.h"
#include "collision.h"
#include <math.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(1072 * 0.125f)	// キャラサイズ
#define TEXTURE_HEIGHT				(637  * 0.5f)		// 
#define TEXTURE_MAX					(1)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値



#define DURITION                    120
#define RANGE                       120

#define PLAYER_SPEED                60  //Temporary
#define ObjType_PlayerTemp          1
#define PLAYER_ANIMATION_FRAMETIME  1.35f


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************


static BOOL		    sIsLoaded = FALSE;		                                    // 初期化を行ったかのフラグ
static PlayerData	sPlayer[PLAYER_MAX];	                                    // プレイヤー構造体

static XMFLOAT2 cursor;
static XMFLOAT2 oldMouse;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Player_Init(void)
{
    TRANSFORM   initialXform = 
    {   {0.0f, 0.0f, 0.0f}, {TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f}, 
        {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
        nullptr
    };
    SpriteIndex initSprite  = {TEX_Player, 0}, Reverse_None;
                            
    entityProp  initProp    = {ObjType_PlayerTemp, ObjType_PlayerTemp/*temp HP*/, FALSE};


    for(int i = 0; i < PLAYER_MAX; i++)
    {
        sPlayer[i].prop  = initProp;
        sPlayer[i].timer = PLAYER_ANIMATION_FRAMETIME;
        ManagedObj  initializer = { TRUE, initialXform, initSprite, Player_Update, &sPlayer[i]};

        GameObject_Creat(initializer);
        

        cursor    = {SCREEN_CENTER_X, SCREEN_CENTER_Y};
        float x   =  GetMouseX();
        float y   =  GetMouseY();
        oldMouse  = {x, y};

    }
    sIsLoaded = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Player_Uninit(void)
{
	sIsLoaded = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void Player_Update(ManagedObj* PlayerObj, double dTime)
{
    float nowMouseX = GetMouseX();
    float nowMouseY = GetMouseY();

    PlayerData* prop = (PlayerData*) PlayerObj->prop;
    
    prop->timer = prop->timer - dTime;
    
    if( (prop->timer) < 0)
    {
        prop->timer += PLAYER_ANIMATION_FRAMETIME;
        PlayerObj->spriteID.Index++;
    }
    PlayerObj->xform.rotation.z = Transform_LookAt( &PlayerObj->xform, {nowMouseX ,nowMouseY , 0.0f}) + PI * 0.5f;

    PlayerObj->xform.velocity = {0.0f, 0.0f, 0.0f};
    


    if (GetKeyboardPress(DIK_DOWN))
    {
    	PlayerObj->xform.velocity.y =   TRUE;
    }
    else if (GetKeyboardPress(DIK_UP))
    {
    	PlayerObj->xform.velocity.y = - TRUE;
    }
    
       
    if (GetKeyboardPress(DIK_RIGHT))
    {
    	PlayerObj->xform.velocity.x =   TRUE;
    }
    else if (GetKeyboardPress(DIK_LEFT))
    {
    	PlayerObj->xform.velocity.x = - TRUE;
    }

    PlayerObj->xform.velocity = NormalizeVec3(PlayerObj->xform.velocity, PLAYER_SPEED);

    Transform_Update(&(PlayerObj->xform), dTime);
}

//=============================================================================
// 描画処理
//=============================================================================

//=============================================================================
// Player構造体の先頭アドレスを取得
//=============================================================================
