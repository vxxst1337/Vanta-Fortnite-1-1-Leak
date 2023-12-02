#include <Windows.h>
#include <iostream>
#include "../Includes/ultis.hpp"





namespace ce {

	int TeamIndex;
	int ActorCount;
	float fov;
	DWORD64 bonearray;
	DWORD_PTR Uworld;
	DWORD_PTR LocalPawn;
	DWORD_PTR PlayerState;
	DWORD_PTR Localplayer;
	DWORD_PTR Rootcomp;
	DWORD_PTR PlayerController;
	DWORD_PTR Persistentlevel;
	Vector3 LocalActorPos;
	uint64_t TargetPawn;
	int localplayerID;
}

namespace fortnite {
	uintptr_t Uworld;
	uintptr_t GameInstance;
	uintptr_t LocalPlayers;
	uintptr_t LocalPlayer;
	uintptr_t LocalPawn;
	uintptr_t PlayerState;
	uintptr_t LocalPlayerState;
	uintptr_t Mesh;
	uintptr_t RootComponent;
	uintptr_t PlayerController;
	uintptr_t PersistentLevel;
	uintptr_t ULevel;
	uintptr_t CameraManager;
	uintptr_t MyObjectID;
	uintptr_t CurrentActor;
	uintptr_t PlayerCameraManager;
	uintptr_t CameraLocation;
	uintptr_t BoneArray;
	uintptr_t CurrentActorMesh;
	uintptr_t AimbotMesh;
	uintptr_t AActors;
}

namespace Offsets {

	uintptr_t UWorld = 0xF16DE38;
	uintptr_t ObjectID = 0x18;
	uintptr_t OwningGameInstance = 0x1B8;
	uintptr_t Levels = 0x170;
	uintptr_t GameState = 0x158;
	uintptr_t PersistentLevel = 0x30;
	uintptr_t AActors = 0xA0;
	uintptr_t ActorCount = 0xA8;
	uintptr_t LocalPlayers = 0x38;
	uintptr_t PlayerController = 0x30;
	uintptr_t AcknowledgedPawn = 0x338;
	uintptr_t PlayerCameraManager = 0x348;
	uintptr_t bHidden = 0x58;
	uintptr_t RootComponent = 0x198;
	uintptr_t RelativeLocation = 0x128;
	uintptr_t Mesh = 0x318;
	uintptr_t PlayerState = 0x2B0;
	uintptr_t TeamIndex = 0x10E0;
	uintptr_t ReviveFromDBNOTime = 0x4A68;
	uintptr_t LocalActorPos = 0x128;
	uintptr_t ComponetToWorld = 0x240;
	uintptr_t CurrentWeapon = 0x9F8;
	uintptr_t BoneArray = 0x610;
	uintptr_t Velocity = 0x170;

}

namespace Misc {

	int i = 0;

}


typedef struct players {
	uint64_t actor;
	int ia;
	char isDBNO;
	uint64_t mesh;
	uint64_t currentweapon;
	uint64_t aimbotmesh;
	uintptr_t bonearray;

	int teamid;
	int myteamid;
	uint64_t playerstate;
	uint64_t localplayerstate;
}players;
std::vector<players> entitylist;


