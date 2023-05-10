#include "pch.h"

HelperFunctions HelperFunctionsGlobal;

LandTableInfo* CurrentLandTableInfo = nullptr;

NJS_TEXNAME customlevel_texnames[1];
NJS_TEXLIST customlevel_texlist = { arrayptrandlength(customlevel_texnames) };

void LoadLevelMusic(char* name) {
	char character;
	int c = 0;

	do {
		character = name[c];
		CurrentSongName[c++] = character;
	} while (character);

	PlayMusic(name);
	ResetMusic();
}

void LoadLandTable(const char* path, LandTableFormat format, NJS_TEXLIST* texlist, const char* texname) {

	CurrentLandTableInfo = new LandTableInfo(HelperFunctionsGlobal.GetReplaceablePath(path));
	LandTable* landtable = CurrentLandTableInfo->getlandtable();

	if (landtable == nullptr) {
		PrintDebug("bad path");
		PrintDebug(path);
	}
	else {
		landtable->TextureList = texlist;
		landtable->TextureName = (char*)texname;


		LoadLandManager(landtable);

		LandTableSA2BModels = format == LandTableFormat_SA2B ? true : false;
	}
}

void UnloadLandTable() {
	if (CurrentLandTableInfo) {
		delete CurrentLandTableInfo;
		CurrentLandTableInfo = nullptr;
		CurrentLandTable = nullptr;
	}
}

void InitCurrentLevelAndScreenCount_r();
FunctionHook<void> InitCurrentLevelAndScreenCount_h(InitCurrentLevelAndScreenCount, InitCurrentLevelAndScreenCount_r);
void InitCurrentLevelAndScreenCount_r() {
	InitCurrentLevelAndScreenCount_h.Original();
	if (CurrentLevel == LevelIDs_CityEscape) {
		LoadStagePaths((LoopHead**)GetPaths());//all splines loaded here
	}
}

void RumbleRallyStageInit() {
	___njClipZ(-3.0f, -65535.0f);

	LoadTexPacks((TexPackInfo*)0x109E810, (NJS_TEXLIST***)0x109E748);
	LoadLandTable("resource\\gd_PC\\level.sa2blvl", LandTableFormat_SA2B, &customlevel_texlist, "textures");
	LoadSetObject(&CityEscape_ObjectList, LoadStageSETFile(NULL, 2048)); // 2048 is the buffer, make it higher if there is a huge lot of objects in your level
	LoadStageLight("stg13_light.bin");
	LoadLevelMusic((char*)"r_hwy.adx");
	LoadFogData_Fogtask("stg13_fog.bin", (FogData*)0x1A280C8);
	//LoadStageSounds("se_ac_gf.mlt", (void*)0x8A0F60);
	//LoadDeathZones(Deathzones);
	InitSkybox();

	DropRingsFunc_ptr = DropRings;
	DisplayItemBoxItemFunc_ptr = DisplayItemBoxItem;

	// Chaos Drives to use for this level, we use the default ones here
	*(void**)0x1DE4680 = (void*)0x6B6C20;
	*(void**)0x1DE4684 = (void*)0x6BBAE0;
	*(void**)0x1DE4688 = (void*)0x6BC450;
	*(void**)0x1DE468C = (void*)0x6BC4A0;
}

void RumbleRallyStageDelete() {
	FreeTexPacks((NJS_TEXLIST***)0x109E748, (TexPackInfo*)0x109E810);
	UnloadLandTable(); 
	FreeSkybox();

	DropRingsFunc_ptr = nullptr;
	DisplayItemBoxItemFunc_ptr = nullptr;

	*(void**)0x1DE4680 = nullptr;
	*(void**)0x1DE4684 = nullptr;
	*(void**)0x1DE4688 = nullptr;
	*(void**)0x1DE468C = nullptr;

	DeleteObject_(LandManagerPtr);
	DeleteObject_(SetObject_ptr);
}

void RumbleRallyStageOnFrame(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;

	// Runs every frame in your level, contain a few variables you can use in "data".
}

extern "C"
{
	// Optional.
	// This function runs code one time when the game starts up. Great for loading assets and setting things up.
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		HelperFunctionsGlobal = helperFunctions;

		CityEscapeHeader.Init = RumbleRallyStageInit; // Runs when the level is loading
		CityEscapeHeader.Free = RumbleRallyStageDelete; // Runs when the level is exited //Free = anonymous_2?
		CityEscapeHeader.subprgmanager = RumbleRallyStageOnFrame; // Object that is loaded when your level loads, and destroyed when stage exit is called

		StartPosition startPos = { LevelIDs_CityEscape, 0, 0, 0, { 0, 10, -0  }, { 0,0,0 }, { 0,0,0 } };
		StartPosition endPos = { LevelIDs_CityEscape, 0, 0, 0, { 0, 0, -0  }, { 0,0,0 }, { 0,0,0 } };

		for (int i = Characters_Sonic; i != Characters_Chaos + 1; i++) {
			HelperFunctionsGlobal.RegisterStartPosition(i, startPos);
			HelperFunctionsGlobal.RegisterEndPosition(i, endPos);
		}
	}

	// Optional.
	// This function runs code on every frame of the game, INCLUDING being in menus, cutscenes, etc.
	// It is recommended to test for game state so that you only run code at a specific time.
	NJS_VECTOR* pos = new NJS_VECTOR{ 0.0,0.0,0.0 };
	__declspec(dllexport) void __cdecl OnFrame()
	{
		// For every frame that you're in a level, add 100 points.
		if (GameState == GameStates_Ingame) {
			ScoreP1 += 100;
		}
	}

	// Optional.
	// This function runs code every time the player inputs. Good for adding custom inputs / overriding events.
	__declspec(dllexport) void __cdecl OnInput()
	{
	}

	// Optional.
	// This function runs while the game processes input.
	__declspec(dllexport) void __cdecl OnControl()
	{
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}
