//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "ManagedObj.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MAX		(1)		// プレイヤーのMax人数


//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct PlayerData
{
    entityProp  prop;   //remember to put the prop as the first member so one point to the individual Data struct will be able to find the prop
	float 		timer;
    //int         state;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Player_Init(void);
void Player_Uninit(void);
void Player_Update(ManagedObj* PlayerObj, double dTime);