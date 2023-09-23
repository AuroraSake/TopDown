#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "debugproc.h"
#include "input.h"




static ManagedObj sGameObject[MAX_MANAGED_OBJ];  //今回は一つのsceneしかないため、staticにする
static ManagedObj sPassiveObject[MAX_PASSIVE_OBJ];

static ID3D11Buffer* sVertexBuffer = NULL;

static double sDtime = 0;

static double gTimeScale = 1.0;




void Scene_DrawObject(ManagedObj& obj);



void Scene_Init(int mode)
{
	//Player_Uninit();
    
    // 頂点バッファ生成
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
    // 入力の更新処理
	UpdateInput();

	// カメラ更新
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
        sDtime = (double)dTime * 0.001 * gTimeScale;//miliSec to Sec ----時間単位を秒に変換
    else
        sDtime = (double)dTime * 0.001;


    //gGameMode = GameMode;
    
    Scene_Update();
    Scene_Draw();
}

//=============================================================================
// 描画処理
//=============================================================================
void Scene_Draw(void)
{
	// バックバッファクリア
	Clear();

	SetCamera();

	// 2Dの物を描画する処理
	SetViewPort(TYPE_FULL_SCREEN);

	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);

    
    // 頂点バッファ設定
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &sVertexBuffer, &stride, &offset);
    
    // マトリクス設定
    SetWorldViewProjection2D();
    
    // プリミティブトポロジ設定
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    
    // マテリアル設定
    MATERIAL material;
    ZeroMemory(&material, sizeof(material));
    material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);

    for(int i = 0; i < MAX_PASSIVE_OBJ; i ++)//layerの代わりに違う配列の中にobjを置いて制御する
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
	// デバッグ表示
	DrawDebugProc();
#endif

	// バックバッファ、フロントバッファ入れ替え
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
   // テクスチャ設定

   //プレイヤーの位置やテクスチャー座標を反映
   float px = obj.xform.pos.x;         // プレイヤーの表示位置X
   float py = obj.xform.pos.y;         // プレイヤーの表示位置Y
   float pw = obj.xform.bound.x * obj.xform.scale.x;       // プレイヤーの表示幅
   float ph = obj.xform.bound.y * obj.xform.scale.y;       // プレイヤーの表示高さ

   // １枚のポリゴンの頂点とテクスチャ座標を設定
   Sprite_2DSprite(sVertexBuffer, px, py, pw, ph, obj.spriteID,
    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), obj.xform.rotation.z);
        
}