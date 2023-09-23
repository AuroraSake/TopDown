#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "debugproc.h"
#include "input.h"




static ManagedObj sGameObject[MAX_MANAGED_OBJ];  //����͈��scene�����Ȃ����߁Astatic�ɂ���
static ManagedObj sPassiveObject[MAX_PASSIVE_OBJ];

static ID3D11Buffer* sVertexBuffer = NULL;

static double sDtime = 0;

static double gTimeScale = 1.0;




void Scene_DrawObject(ManagedObj& obj);



void Scene_Init(int mode)
{
	//Player_Uninit();
    
    // ���_�o�b�t�@����
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GetDevice()->CreateBuffer(&bd, NULL, &sVertexBuffer);

    Sprite_Init();
	Player_Init();
}

void Scene_Uninit(int mode)
{
    
    if (sVertexBuffer)
    {
        sVertexBuffer->Release();
        sVertexBuffer = NULL;
    }

    Sprite_Uninit();
	Player_Uninit();

	//Player_Init();
}

void Scene_Update()
{
    // ���͂̍X�V����
	UpdateInput();

	// �J�����X�V
	UpdateCamera();

    
    for(int i = 0; i < MAX_MANAGED_OBJ; i++)
    {
        if(!sGameObject[i].inUse)
            continue;

        sGameObject->Update(&sGameObject[i], sDtime);
    }
}


void Scene_Manager( DWORD dTime)
{
    if(fabs(gTimeScale - 1.0) > FLOAT_EQUALITY_PERCISION)
        sDtime = (double)dTime * 0.001 * gTimeScale;//miliSec to Sec ----���ԒP�ʂ�b�ɕϊ�
    else
        sDtime = (double)dTime * 0.001;


    //gGameMode = GameMode;
    
    Scene_Update();
    Scene_Draw();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Scene_Draw(void)
{
	// �o�b�N�o�b�t�@�N���A
	Clear();

	SetCamera();

	// 2D�̕���`�悷�鏈��
	SetViewPort(TYPE_FULL_SCREEN);

	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

    
    // ���_�o�b�t�@�ݒ�
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &sVertexBuffer, &stride, &offset);
    
    // �}�g���N�X�ݒ�
    SetWorldViewProjection2D();
    
    // �v���~�e�B�u�g�|���W�ݒ�
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    
    // �}�e���A���ݒ�
    MATERIAL material;
    ZeroMemory(&material, sizeof(material));
    material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);

    for(int i = 0; i < MAX_PASSIVE_OBJ; i ++)//layer�̑���ɈႤ�z��̒���obj��u���Đ��䂷��
    {
        if(sPassiveObject[i].inUse)
            Scene_DrawObject(sPassiveObject[i]);
    }
    for(int i = 0; i < MAX_MANAGED_OBJ; i ++)
    {
        if(sGameObject[i].inUse)
            Scene_DrawObject(sGameObject[i]);
    }

#ifdef _DEBUG
	// �f�o�b�O�\��
	DrawDebugProc();
#endif

	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();
}


void* Scene_getOpenSlot(void)
{
    ManagedObj* openSlot = nullptr;

    for(int i = 0; i < MAX_MANAGED_OBJ; i++)
    {
        if(!sGameObject[i].inUse)    
        {
            openSlot = &sGameObject[i];
            break;
        }
    }
    return openSlot;
}

void GameObject_Creat(ManagedObj initial)
{

    ManagedObj* slot = (ManagedObj*)Scene_getOpenSlot();
    if(slot != nullptr)
    {
        *slot = initial;
    }
};

void PassiveObject_Creat(ManagedObj initial)
{

    ManagedObj* slot = (ManagedObj*)Scene_getOpenSlot();
    if(slot != nullptr)
    {
        *slot = initial;
    }
};


void Scene_DrawObject(ManagedObj& obj)
{
   // �e�N�X�`���ݒ�

   //�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
   float px = obj.xform.pos.x;         // �v���C���[�̕\���ʒuX
   float py = obj.xform.pos.y;         // �v���C���[�̕\���ʒuY
   float pw = obj.xform.bound.x * obj.xform.scale.x;       // �v���C���[�̕\����
   float ph = obj.xform.bound.y * obj.xform.scale.y;       // �v���C���[�̕\������

   // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
   Sprite_2DSprite(sVertexBuffer, px, py, pw, ph, obj.spriteID,
    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), obj.xform.rotation.z);
        
}