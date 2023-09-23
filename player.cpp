//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "transform.h"
#include "input.h"
#include "collision.h"
#include <math.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(1072 * 0.125f)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(637  * 0.5f)		// 
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l



#define DURITION                    120
#define RANGE                       120

#define PLAYER_SPEED                60  //Temporary
#define ObjType_PlayerTemp          1
#define PLAYER_ANIMATION_FRAMETIME  1.35f


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


static BOOL		    sIsLoaded = FALSE;		                                    // ���������s�������̃t���O
static PlayerData	sPlayer[PLAYER_MAX];	                                    // �v���C���[�\����

static XMFLOAT2 cursor;
static XMFLOAT2 oldMouse;


//=============================================================================
// ����������
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
// �I������
//=============================================================================
void Player_Uninit(void)
{
	sIsLoaded = FALSE;
}

//=============================================================================
// �X�V����
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
// �`�揈��
//=============================================================================

//=============================================================================
// Player�\���̂̐擪�A�h���X���擾
//=============================================================================
