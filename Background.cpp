#include "pch.h"

ModelInfo* skybox = nullptr;
NJS_VECTOR Skybox_Scale = { 10.0f, 10.0f, 10.0f };

NJS_TEXNAME PAST01BG_TEXNAME[6]{};
NJS_TEXLIST PAST01BG_TEXLIST = { arrayptrandlength(PAST01BG_TEXNAME, Uint32) };

void __cdecl DrawSkyBox(ObjectMaster* obj) {
	fogEnabled = 0;
	njFogToggle();

	SaveControl3D();
	OnControl3D(NJD_CONTROL_3D_NO_CLIP_CHECK);
	OffControl3D(NJD_CONTROL_3D_DEPTH_QUEUE);

	njPushMatrix(0);
	njTranslateV(0, &pCameraLocations[CurrentScreen]->pos);
	njScale(0, 2.0, 2.0, 2.0);
	njSetTexture(&PAST01BG_TEXLIST);
	njScaleV_(&Skybox_Scale);

	DrawObject(skybox->getmodel());

	njScale(0, 1.0, 1.0, 1.0);
	njPopMatrix(1u);

	LoadControl3D();

	fogEnabled = 1;
	njFogToggle();
}


void InitSkybox() {
	LoadTextureList("skybox", &PAST01BG_TEXLIST);
	WriteJump((ObjectFuncPtr*)0x5DD213, DrawSkyBox);
	skybox = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath("resource\\gd_PC\\Models\\sky.sa2mdl"));
	LoadObject((LoadObj)0, "object_00170858", DrawSkyBox, 1)->DisplaySub = DrawSkyBox;
}

void FreeSkybox() {
	delete skybox;
	skybox = nullptr;
}