#pragma once
#include "SA2ModLoader.h"

extern HelperFunctions HelperFunctionsGlobal;

void InitSkybox();
void __cdecl DrawSkyBox(ObjectMaster* obj);
void FreeSkybox();
LoopHead** GetPaths();