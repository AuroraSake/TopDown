//=============================================================================
//
// �v���C���[���� [player.h]
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
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX		(1)		// �v���C���[��Max�l��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct PlayerData
{
    entityProp  prop;   //remember to put the prop as the first member so one point to the individual Data struct will be able to find the prop
	float 		timer;
    //int         state;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Player_Init(void);
void Player_Uninit(void);
void Player_Update(ManagedObj* PlayerObj, double dTime);