#pragma once
#include "ManagedObj.h"

#define MAX_MANAGED_OBJ         (64)
#define MAX_PASSIVE_OBJ         (1024)

void Scene_Draw  (void);
void Scene_Update(void);

void Scene_Init(int mode);
void Scene_Manager(DWORD dTime);

void* Scene_getOpenSlot(void);
