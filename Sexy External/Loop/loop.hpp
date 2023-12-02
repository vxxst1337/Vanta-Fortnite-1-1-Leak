#include "../Includes/includes.hpp"
#include "../SDK/offsets.hpp"
#include <thread>
#include "../Driver/driver.h"
#include <vector>
#include "game.hpp"
#include "esp.hpp"
#include "../Includes/jrt.hpp"
#include "../SDK/defs.h"

//namespace StaticOffsets
//{
//	FortPTR PrimaryPickupItemEntry = 0x350;
//	FortPTR ItemDefinition = 0x18;
//	FortPTR DisplayName = 0x90;
//	FortPTR Tier = 0x73;
//}

void SetMouseAbsPosition(DWORD x, DWORD y)
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.dx = x;
	input.mi.dy = y;
	SendInput(1, &input, sizeof(input));
}

void aimbot(float x, float y)
{
	float ScreenCenterX = (width / 2);
	float ScreenCenterY = (height / 2);
	int AimSpeed = Features->Aimbot->smooth;
	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	SetMouseAbsPosition(TargetX, TargetY);
	//mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);

	return;
}

bool head;
bool neck;
bool chest;
bool pelvis;


bool rmb;
bool lmb;
bool caps;
bool shift;





bool radar = true;
bool rect_radar = false;
float radar_position_x{ 84.494f };
float radar_position_y{ 77.841f };
float RadarDistance = { 300.f };
void RadarRange(float* x, float* y, float range)
{
	if (fabs((*x)) > range || fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}
	}
}

#define FortPTR uintptr_t
#define FVector Vector3

void CalcRadarPoint(Vector3 vOrigin, int& screenx, int& screeny)
{
	CamewaDescwipsion vCamera = GetCamera();
	Vector3 vAngle = vCamera.Rotation;
	auto fYaw = vAngle.y * M_PI / 180.0f;
	float dx = vOrigin.x - vCamera.Location.x;
	float dy = vOrigin.y - vCamera.Location.y;

	float fsin_yaw = sinf(fYaw);
	float fminus_cos_yaw = -cosf(fYaw);

	float x = dy * fminus_cos_yaw + dx * fsin_yaw;
	x = -x;
	float y = dx * fminus_cos_yaw - dy * fsin_yaw;

	float range = (float)RadarDistance * 100.f;

	RadarRange(&x, &y, range);

	ImVec2 DrawPos = ImVec2(radar_position_x, radar_position_y);
	ImVec2 DrawSize = ImVec2(Features->Visuals->Radar->radarsize, Features->Visuals->Radar->radarsize);

	int rad_x = (int)DrawPos.x;
	int rad_y = (int)DrawPos.y;

	float r_siz_x = DrawSize.x;
	float r_siz_y = DrawSize.y;

	int x_max = (int)r_siz_x + rad_x - 5;
	int y_max = (int)r_siz_y + rad_y - 5;

	screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
	screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

	if (screenx > x_max)
		screenx = x_max;

	if (screenx < rad_x)
		screenx = rad_x;

	if (screeny > y_max)
		screeny = y_max;

	if (screeny < rad_y)
		screeny = rad_y;
}

void fortnite_radar(float x, float y, float size, bool rect)
{
	SPOOF_FUNC;
	if (radar)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Once);
		static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
		ImGui::Begin(E("##radar"), nullptr, flags);

		float render_size = 5;
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		if (rect) {
			drawList->AddRectFilled(ImVec2(x, y), ImVec2(x + size, y + size), ImGui::GetColorU32({ 0.13f, 0.13f, 0.13f, 0.5f }), 0.0f, 0);

			drawList->AddRect(ImVec2(x, y), ImVec2(x + size, y + size), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 0.0f, 0, 1.0f);

			ImGui::GetForegroundDrawList()->AddLine(ImVec2(radar_position_x + (Features->Visuals->Radar->radarsize / 2), radar_position_y + (Features->Visuals->Radar->radarsize / 2)), ImVec2(radar_position_x, radar_position_y), ImGui::GetColorU32({ 2.55f, 1.f, 1.f, 1.f }), 1.f);
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(radar_position_x + (Features->Visuals->Radar->radarsize / 2), radar_position_y + (Features->Visuals->Radar->radarsize / 2)), ImVec2(radar_position_x + Features->Visuals->Radar->radarsize, radar_position_y), ImGui::GetColorU32({ 2.55f, 1.f, 1.f, 1.f }), 1.f);
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(radar_position_x + (Features->Visuals->Radar->radarsize / 2), radar_position_y + (Features->Visuals->Radar->radarsize / 2)), ImVec2(radar_position_x + (Features->Visuals->Radar->radarsize / 2), radar_position_y + Features->Visuals->Radar->radarsize), ImGui::GetColorU32({ 2.55f, 1.f, 1.f, 1.f }), 1.f);

			ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(radar_position_x + (Features->Visuals->Radar->radarsize / 2), radar_position_y + (Features->Visuals->Radar->radarsize / 2)), 3.f, ImColor(0, 255, 255), 12);
		}
		else {
			drawList->AddCircleFilled(ImVec2(x + size / 2, y + size / 2), size, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, 0.5f }), 1000);
			drawList->AddCircleFilled(ImVec2(x + size / 2, y + size / 2), render_size / 2 * sqrt(2), ImGui::GetColorU32({ 1.0f, 1.0f, 1.0f, 1.0f }), 1000);
			drawList->AddCircle(ImVec2(x + size / 2, y + size / 2), size, ImGui::GetColorU32({ 0.1f, 0.1f, 0.1f, 1.0f }), 1000, 5);
		}

		ImGui::End();
	}
}


void add_players_radar(Vector3 WorldLocation, ImU32 color) // from UC
{
	static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	ImGui::Begin(("##radar"), nullptr, flags);

	int ScreenX, ScreenY = 0;
	const auto& GetWindowDrawList = ImGui::GetWindowDrawList();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	CalcRadarPoint(WorldLocation, ScreenX, ScreenY);
	if (rect_radar)
	{
		drawList->AddCircleFilled(ImVec2(ScreenX, ScreenY), 4, color, 12);
		//drawList->AddRectFilled(ImVec2((float)ScreenX - 4.5, (float)ScreenY - 4.5), ImVec2((float)ScreenX + 4.5, (float)ScreenY + 4.5), ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }));
	}
	else drawList->AddCircleFilled(ImVec2(ScreenX, ScreenY), 5, color, 12);

	ImGui::End();
}


#pragma region FortniteGame Functions

class World
{
public:
	uintptr_t GetUWorld()
	{
		SPOOF_FUNC;
		return request->read<uintptr_t>(base_address + Offsets::UWorld);
	}

	uintptr_t GetGameInstance()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::Uworld + Offsets::OwningGameInstance);
	}

	uintptr_t GetPersistentLevel()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::Uworld + Offsets::PersistentLevel);
	}

	uintptr_t GetLocalPlayers()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::GameInstance + Offsets::LocalPlayers);
	}

	uintptr_t GetLocalPlayer()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::LocalPlayers);
	}

	uintptr_t GetPlayerController()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::LocalPlayer + Offsets::PlayerController);
	}

	uintptr_t GetLocalPawn()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::PlayerController + Offsets::AcknowledgedPawn);
	}

	uintptr_t GetPlayerState()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::CurrentActor + Offsets::PlayerState);
	}

	uintptr_t GetRootComponent()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::LocalPawn + Offsets::RootComponent);
	}

	Vector3 GetLocalActorPos()
	{
		SPOOF_FUNC;
		return request->read<Vector3>(fortnite::RootComponent + Offsets::LocalActorPos);
	}

	uintptr_t GetPlayerCameraManager()
	{
		SPOOF_FUNC;
		return request->read<uintptr_t>(fortnite::PlayerController + Offsets::PlayerCameraManager);
	}

	int GetActorCount()
	{
		SPOOF_FUNC;
		return request->read<int>(fortnite::PersistentLevel + 0xA8);
	}

	uintptr_t GetAActors()
	{
		SPOOF_FUNC;
		return request->read<uintptr_t>(fortnite::PersistentLevel + 0xA0);
	}

	uintptr_t GetCurrentActor()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::AActors + Misc::i * 0x8);
	}

	uintptr_t GetCurrentActorSkeletonMesh()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::CurrentActor + Offsets::Mesh);
	}

	int GetCurrentActorId()
	{
		SPOOF_FUNC;
		return request->read<int>(fortnite::CurrentActor + 0x18);
	}

	uintptr_t GetLocalPlayerState()
	{
		SPOOF_FUNC;
		return request->read<uint64_t>(fortnite::LocalPawn + Offsets::PlayerState);
	}

	int GetMyTeamId()
	{
		SPOOF_FUNC;
		return request->read<int>(fortnite::LocalPlayerState + Offsets::TeamIndex);
	}

	int GetTeamId()
	{
		SPOOF_FUNC;
		return request->read<int>(fortnite::PlayerState + Offsets::TeamIndex);
	}

	char GetDBNO()
	{
		SPOOF_FUNC;
		return (request->read<char>(fortnite::CurrentActor + 0x912) >> 4) & 1;
	}

	float GetReviveFromDBNOTime()
	{
		SPOOF_FUNC;
		return request->read<float>(fortnite::CurrentActor + Offsets::ReviveFromDBNOTime);
	}
}; World* FortniteGame_Functions;


#pragma endregion

#define FNAME_OFFSET 0xF2655BC
#define FNAMEPOOL_OFFSET 0xF33B480

static std::string GetNameFromIndex(int key)
{
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;
	uint64_t NamePoolChunk = request->read<uint64_t>(base_address + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset);
	uint16_t nameEntry = request->read<uint16_t>(NamePoolChunk);
	int nameLength = nameEntry >> 6;
	char buff[1024] = {};

	char* v2; // rdi
	int v4; // ebx
	unsigned int v5; // er9
	unsigned int v6; // er8
	__int64 result; // rax
	unsigned int v8; // er9
	int v9; // ecx

	v2 = buff;
	v4 = nameLength;
	v5 = request->read<unsigned int>(base_address + FNAME_OFFSET);
	v6 = v5 ^ 0x9C677CC5;
	result = 1041204193 * v5;
	v8 = v5 % 0x21;
	v9 = 0;
	if (v4)
	{
		mem::read(PVOID(NamePoolChunk + 2), (PVOID)buff, 2 * nameLength);
		do
		{
			result = v9 + v8;
			++v9;
			v6 += result;
			*v2++ ^= v6;
		} while (v9 < v4);
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	return std::string("");
}

static std::string GetNameFromFName(int key)
{
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = request->read<uint64_t>(base_address + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
	if (request->read<uint16_t>(NamePoolChunk) < 64)
	{
		auto a1 = request->read<DWORD>(NamePoolChunk + 2);
		return GetNameFromIndex(a1);
	}
	else
	{
		return GetNameFromIndex(key);
	}
}

namespace Engine
{
	FortPTR RootComponent(FortPTR actor)
	{
		return request->read<FortPTR>(actor + Offsets::RootComponent);
	}
	FVector GetLocation(FortPTR actor)
	{
		return request->read<FVector>(RootComponent(actor) + Offsets::RelativeLocation);
	}

	void K2_SetActorLocation(FortPTR actor, FVector DesiredLocation)
	{
		FortPTR RootComp = RootComponent(actor);

		request->write<char>(RootComp + 0x188, 0);
		request->write<FVector>(RootComp + Offsets::RelativeLocation, DesiredLocation);
	}
}

std::string GetPlayerName(FortPTR pState)
{
	SPOOF_FUNC;
	__int64 Name = request->read<__int64>(pState + 2792);
	if (!Name) return std::string("Bot");
	int Length = request->read<int>(Name + 16i64);
	__int64 v6 = Length;
	if (!v6) return std::string("Bot");
	uintptr_t Data = request->read<__int64>(Name + 8);
	if (!Data) return std::string("Bot");
	wchar_t* NameBuffer = new wchar_t[Length];
	mem::read((PVOID)Data, (PVOID)NameBuffer, (Length * 2));

	char v21; // al
	int v22; // r8d
	int i; // ecx
	int v25; // eax
	_WORD* v23;

	v21 = v6 - 1;
	if (!(_DWORD)v6)
		v21 = 0;
	v22 = 0;
	v23 = (_WORD*)NameBuffer;
	for (i = (v21) & 3; ; *v23++ += i & 7)
	{
		v25 = v6 - 1;
		if (!(_DWORD)v6)
			v25 = 0;
		if (v22 >= v25)
			break;
		i += 3;
		++v22;
	}

	std::wstring Temp{ NameBuffer };
	delete[] NameBuffer;
	return std::string(Temp.begin(), Temp.end());
}

FVector target_prediction(FVector TargetPosition, FVector ComponentVelocity, float player_distance, float ProjectileSpeed = 9.5)
{
	float gravity = abs(-336); // Gravity (Probably other ways to get this but I set it as a constant value)
	float time = player_distance / abs(ProjectileSpeed);
	float bulletDrop = (gravity / 250) * time * time;
	return FVector(TargetPosition.x += time * (ComponentVelocity.x), TargetPosition.y += time * (ComponentVelocity.y), TargetPosition.z += time * (ComponentVelocity.z));
}

void AimAt(DWORD_PTR entity, int hitbox, bool prediction)
{
	if (prediction)
	{
		auto closestMesh = request->read<FortPTR>(entity + Offsets::Mesh);
		FVector TargetPOS = UMesh->get_bone(closestMesh, 6);

		auto closestRoot = Engine::RootComponent(entity);
		auto componentVelocity = request->read<FVector>(closestRoot + Offsets::Velocity);

		auto distance = ce::LocalActorPos.Distance(TargetPOS) / 100.f;

		FVector TargetAim = target_prediction(TargetPOS, componentVelocity, distance);
		auto rootScreen = UFortCamera->W2S(TargetAim);

		if (rootScreen.y != 0 || rootScreen.y != 0)
		{
			aimbot(rootScreen.x, rootScreen.y);
		}
	}
	else
	{
		auto Mesh = request->read<FortPTR>(entity + Offsets::Mesh);
		auto rootHead = UMesh->get_bone(Mesh, hitbox);
		auto rootScreen = UFortCamera->W2S(rootHead);

		if (rootScreen.y != 0 || rootScreen.y != 0)
		{
			aimbot(rootScreen.x, rootScreen.y);
		}
	}
}


namespace cheat {

	void loop() {
		SPOOF_FUNC
			for (;;) {

				std::vector<players> tmpList;


				fortnite::Uworld = FortniteGame_Functions->GetUWorld();


				fortnite::GameInstance = FortniteGame_Functions->GetGameInstance();


				fortnite::PersistentLevel = FortniteGame_Functions->GetPersistentLevel();


				fortnite::LocalPlayers = FortniteGame_Functions->GetLocalPlayers();


				fortnite::LocalPlayer = FortniteGame_Functions->GetLocalPlayer();


				fortnite::PlayerController = FortniteGame_Functions->GetPlayerController();


				fortnite::LocalPawn = FortniteGame_Functions->GetLocalPawn();


				fortnite::PlayerState = FortniteGame_Functions->GetPlayerState();


				fortnite::RootComponent = FortniteGame_Functions->GetRootComponent();


				ce::LocalActorPos = FortniteGame_Functions->GetLocalActorPos();


				fortnite::PlayerCameraManager = FortniteGame_Functions->GetPlayerCameraManager();






				ce::ActorCount = FortniteGame_Functions->GetActorCount();
				fortnite::AActors = FortniteGame_Functions->GetAActors();

				for (Misc::i = 0; Misc::i < ce::ActorCount; ++Misc::i) {

					fortnite::CurrentActor = FortniteGame_Functions->GetCurrentActor();
					if (!fortnite::CurrentActor || fortnite::CurrentActor == NULL) { continue; }
					fortnite::CurrentActorMesh = FortniteGame_Functions->GetCurrentActorSkeletonMesh();
					if (!fortnite::CurrentActorMesh || fortnite::CurrentActorMesh == NULL)continue;
					int curactorid = FortniteGame_Functions->GetCurrentActorId();
					if (!curactorid || curactorid == NULL)continue;

					fortnite::PlayerState = FortniteGame_Functions->GetPlayerState();
					fortnite::LocalPlayerState = FortniteGame_Functions->GetLocalPlayerState();

					int MyTeamId = FortniteGame_Functions->GetMyTeamId();
					int TeamId = FortniteGame_Functions->GetTeamId();

					char isDBNO = FortniteGame_Functions->GetDBNO();


					if (FortniteGame_Functions->GetReviveFromDBNOTime() == 10 && fortnite::CurrentActorMesh != 0x0 && curactorid != 0x0 && fortnite::CurrentActor != 0x0)
					{

						if (fortnite::PlayerState)
						{

							players fnlEntity{ };
							fnlEntity.actor = fortnite::CurrentActor;
							fnlEntity.mesh = fortnite::CurrentActorMesh;
							fnlEntity.myteamid = MyTeamId;
							fnlEntity.teamid = TeamId;
							fnlEntity.isDBNO = isDBNO;

							fnlEntity.playerstate = fortnite::PlayerState;
							fnlEntity.localplayerstate = fortnite::LocalPlayerState;
							tmpList.push_back(fnlEntity);
						}
					}
				}


				entitylist.clear();
				entitylist = tmpList;
				std::this_thread::sleep_for(std::chrono::seconds(1));


			}
	}
}

//void BoundingBoxes(DWORD_PTR mesh)
//{
//	SPOOF_FUNC;
//
//	int boneIDs[] = {
//		109, // Hip
//		109,  // Neck
//		38,  // Upper Arm Left
//		9,   // Upper Arm Right
//		39,  // Left Hand
//		10,  // Right Hand
//		99,  // Left Hand 1
//		33,  // Right Hand 1
//		78,  // Right Thigh
//		71,  // Left Thigh
//		79,  // Right Calf
//		72,  // Left Calf
//		80,  // Left Foot
//		73,  // Right Foot
//		82,  // Left Feet
//		75,  // Right Feet
//		83,  // Left Feet Finger
//		76,  // Right Feet Finger
//	};
//	int numBones = sizeof(boneIDs) / sizeof(boneIDs[0]);
//
//	float mostleft = FLT_MAX;
//	float mostright = FLT_MIN;
//	float mosttop = FLT_MAX;
//	float mostbottom = FLT_MIN;
//
//	for (int i = 0; i < numBones; ++i) {
//		int boneID = boneIDs[i];
//		Vector3 bonePosition = UMesh->get_bone(mesh, boneID);
//
//		Vector3 w2sBonePosition = UFortCamera->W2S(bonePosition);
//
//		if (w2sBonePosition.x < mostleft) {
//			mostleft = w2sBonePosition.x;
//		}
//		if (w2sBonePosition.x > mostright) {
//			mostright = w2sBonePosition.x;
//		}
//		if (w2sBonePosition.y > mostbottom) {
//			mostbottom = w2sBonePosition.y;
//		}
//		if (w2sBonePosition.y < mosttop) {
//			mosttop = w2sBonePosition.y;
//		}
//	}
//
//	float ActorHeight = mosttop - mostbottom;
//	float ActorWidth = mostright - mostleft;
//
//	Normalbox(mostleft, mostbottom, ActorWidth, ActorHeight, ImColor(255, 255, 255), 1.5, 0);
//}
bool IsInScreen(Vector3 pos, int over = 30)
{
	SPOOF_FUNC;

	if (pos.x > -over && pos.x < width + over && pos.y > -over && pos.y < height + over)
		return true;
	return false;
}
void DrawSkeleton(DWORD_PTR mesh)
{
	SPOOF_FUNC;

	const int color = ImColor(48.f, 159.f, 164.f);
	Vector3 bonePositions[] = {
		UMesh->get_bone(mesh, 109), // head
		UMesh->get_bone(mesh, 67),  // neck
		UMesh->get_bone(mesh, 2),   // hip
		UMesh->get_bone(mesh, 64), // upper arm right
		UMesh->get_bone(mesh, 105), // elbo right
		UMesh->get_bone(mesh, 62), // right hand wrist
		UMesh->get_bone(mesh, 78),  // right hip
		UMesh->get_bone(mesh, 79),  // right knee
		UMesh->get_bone(mesh, 87),  // right ankle
		UMesh->get_bone(mesh, 83),  // right foot
		UMesh->get_bone(mesh, 35),  // upper arm left
		UMesh->get_bone(mesh, 10),  // elbo left
		UMesh->get_bone(mesh, 12), // left hand wrist //
		UMesh->get_bone(mesh, 71),  // left hip
		UMesh->get_bone(mesh, 72),  // left knee
		UMesh->get_bone(mesh, 86),  // left ankle
		UMesh->get_bone(mesh, 76),  // left foot
		UMesh->get_bone(mesh, 66),  // lower neck
		UMesh->get_bone(mesh, 29), // top of left finger
		UMesh->get_bone(mesh, 30),  // left finger
		UMesh->get_bone(mesh, 58),  // top of right finger
		UMesh->get_bone(mesh, 59)  // right finger
	};

	int numBones = sizeof(bonePositions) / sizeof(bonePositions[0]);

	Vector3 bonePositionsOut[sizeof(bonePositions)];
	for (int i = 0; i < numBones; ++i) {
		bonePositionsOut[i] = UFortCamera->W2S(bonePositions[i]);
	}

	if (IsInScreen(bonePositionsOut[1]))
	{
		// right arm

		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[0].x, bonePositionsOut[0].y), ImVec2(bonePositionsOut[17].x, bonePositionsOut[17].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[17].x, bonePositionsOut[17].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[17].x, bonePositionsOut[17].y), ImVec2(bonePositionsOut[3].x, bonePositionsOut[3].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[3].x, bonePositionsOut[3].y), ImVec2(bonePositionsOut[4].x, bonePositionsOut[4].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[4].x, bonePositionsOut[4].y), ImVec2(bonePositionsOut[5].x, bonePositionsOut[5].y), ImColor(0, 255, 255), 1.f);


		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[4].x, bonePositionsOut[4].y), ImVec2(bonePositionsOut[5].x, bonePositionsOut[5].y), ImColor(0, 255, 255), 1.f);


		// lower body
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), ImVec2(bonePositionsOut[6].x, bonePositionsOut[6].y), ImColor(0, 255, 255), 1.0);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[6].x, bonePositionsOut[6].y), ImVec2(bonePositionsOut[7].x, bonePositionsOut[7].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[7].x, bonePositionsOut[7].y), ImVec2(bonePositionsOut[8].x, bonePositionsOut[8].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[8].x, bonePositionsOut[8].y), ImVec2(bonePositionsOut[9].x, bonePositionsOut[9].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), ImVec2(bonePositionsOut[13].x, bonePositionsOut[13].y), ImColor(0, 255, 255), 1.f);

		// left arm
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[17].x, bonePositionsOut[17].y), ImVec2(bonePositionsOut[10].x, bonePositionsOut[10].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[10].x, bonePositionsOut[10].y), ImVec2(bonePositionsOut[11].x, bonePositionsOut[11].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[11].x, bonePositionsOut[11].y), ImVec2(bonePositionsOut[12].x, bonePositionsOut[12].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[13].x, bonePositionsOut[13].y), ImVec2(bonePositionsOut[14].x, bonePositionsOut[14].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[14].x, bonePositionsOut[14].y), ImVec2(bonePositionsOut[15].x, bonePositionsOut[15].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[15].x, bonePositionsOut[15].y), ImVec2(bonePositionsOut[16].x, bonePositionsOut[16].y), ImColor(0, 255, 255), 1.f);


		// right fingers
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[12].x, bonePositionsOut[12].y), ImVec2(bonePositionsOut[18].x, bonePositionsOut[18].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[18].x, bonePositionsOut[18].y), ImVec2(bonePositionsOut[19].x, bonePositionsOut[19].y), ImColor(0, 255, 255), 1.f);

		// left fingers
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[5].x, bonePositionsOut[5].y), ImVec2(bonePositionsOut[20].x, bonePositionsOut[20].y), ImColor(0, 255, 255), 1.f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(bonePositionsOut[20].x, bonePositionsOut[20].y), ImVec2(bonePositionsOut[21].x, bonePositionsOut[21].y), ImColor(0, 255, 255), 1.f);
	}

}
//void DebugBones(DWORD_PTR mesh)
//{
//	SPOOF_FUNC;
//
//	Vector3 bonePositionsOut[110];
//	for (int i = 0; i < 110; ++i) {
//		bonePositionsOut[i] = UFortCamera->W2S(UMesh->get_bone(mesh, i));
//
//		const Vector3& bonePosition = bonePositionsOut[i];
//		ImVec2 textPos = ImVec2(bonePosition.x, bonePosition.y);
//		ImGui::GetForegroundDrawList()->AddText(textPos, IM_COL32_WHITE, std::to_string(i).c_str());
//	}
//}

int textpos;
void draw()
{
	auto entityListCopy = entitylist;
	float closestDistance = FLT_MAX;
	DWORD_PTR closestPawn = NULL;

	int dynammedfov = (ce::fov - 80) + Features->Aimbot->fovsize;

	if (Features->Exploits->ProjectileTP) dynammedfov = 125000.f;
	if (Features->Aimbot->fov) {

		DrawCircle(width / 2.f, height / 2.f, dynammedfov, &Col.white, 100);
	}


	Vector3 w2shead;
	Vector3 Headpos;

	for (auto entity : entityListCopy)
	{

		uint64_t PlayerState = entity.playerstate;
		uint64_t LocalPlayerState = entity.localplayerstate;
		int TeamId = entity.teamid;
		int MyTeamId = entity.myteamid;

		bool IsTeammate = false;
		if (TeamId == MyTeamId)
			IsTeammate = true;
		if (IsTeammate)
			continue;

		if (!entity.playerstate || entity.playerstate == NULL)continue;
		if (fortnite::LocalPawn == entity.actor)continue;

		if (fortnite::LocalPawn) {

			if (!MyTeamId || MyTeamId == NULL)continue;
		}


		Headpos = UMesh->get_bone(entity.mesh, 109);
		ce::LocalActorPos = request->read<Vector3>(ce::Rootcomp + 0x128);
		float distance = ce::LocalActorPos.Distance(Headpos) / 100.f;

		Vector3 bone0 = UMesh->get_bone(entity.mesh, 0);
		Vector3 bottom = UFortCamera->W2S(bone0);
		Vector3 Headbox = UFortCamera->W2S(Vector3(Headpos.x, Headpos.y, Headpos.z + 15));
		w2shead = UFortCamera->W2S(Headpos);


		float BoxHeight = (float)(Headbox.y - bottom.y);
		float BoxWidth = BoxHeight * 0.380f;

		float LeftX = (float)Headbox.x - (BoxWidth / 1);
		float LeftY = (float)bottom.y;

		float CornerHeight = abs(Headbox.y - bottom.y);
		float CornerWidth = CornerHeight * 0.75;


		textpos = 10;

#pragma region Visuals

		if (Features->Visuals->box)
		{
			Normalbox(bottom.x - (BoxWidth / 2), bottom.y, BoxWidth, BoxHeight, ImColor(88, 173, 14), 1.5, 0);
		}

		if (Features->Visuals->radars)
		{
			if (IsVisible(fortnite::CurrentActorMesh))
			{
				add_players_radar(bone0, ImColor(255, 0, 0));
			}
			else {
				add_players_radar(bone0, ImColor(0, 0, 255));
			}
		}

		//if (Features->Visuals->boundingbox)
		//{
		//	BoundingBoxes(entity.mesh);
		//}

		//if (Features->Visuals->bone_dump)
		//{
		//	DebugBones(entity.mesh);
		//}

		if (Features->Visuals->Skeleton)
		{
			DrawSkeleton(entity.mesh);
		}

		if (Features->Visuals->snapline)
		{
			DrawLine(width / 2, height, bottom.x, bottom.y, &Col.white, 1);
		}

		if (Features->Visuals->names)
		{
			auto name = GetPlayerName(PlayerState);
			ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(name.c_str());
			ImGui::GetForegroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(255, 255, 255), name.c_str());
			textpos -= 15;
		}

		if (Features->Visuals->platform)
		{
			DWORD_PTR test_platform = request->read<DWORD_PTR>(PlayerState + 0x438); // 0x440 next update
			wchar_t platform[64];
			mem::read((PVOID)test_platform, reinterpret_cast<PVOID>(platform), sizeof(platform));
			std::wstring balls_sus(platform);
			std::string str_platform(balls_sus.begin(), balls_sus.end());
			ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(str_platform.c_str());

			if (str_platform.find(("XBL")) != std::string::npos || str_platform.find(("XSX")) != std::string::npos) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(128, 224, 0, 200), EX("XBox"));
			}
			else if (str_platform.find(("XSX")) != std::string::npos) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(128, 224, 0, 200), EX("Xbox Series S/X"));
			}
			else if (str_platform.find(("PSN")) != std::string::npos)
			{
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(0, 96, 192, 255), EX("PS4"));
			}
			else if (str_platform.find(("PS5")) != std::string::npos) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(0, 96, 192, 255), EX("PS5"));
			}
			else if (str_platform.find(("WIN")) != std::string::npos) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(255, 255, 0, 255), EX("Windows"));
			}
			else if (str_platform.find(("SWT")) != std::string::npos) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(255, 0, 0, 255), EX("NintendoSwitch"));
			}
			else if (str_platform.find(("AND")) != std::string::npos) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(255, 0, 0, 255), EX("Android"));
			}
			else if (str_platform.find(("MAC")) != std::string::npos) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(255, 0, 0, 255), EX("MacOS"));
			}
			else if (str_platform.find(("LNX")) != std::string::npos) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(255, 0, 0, 255), EX("Linux"));
			}
			else if (str_platform.find(("IOS")) != std::string::npos) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(255, 0, 0, 255), EX("IOS"));
			}
			else {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Headbox.x, Headbox.y - textpos), ImColor(255, 0, 0, 255), EX("BOT"));
			}
			textpos -= 15;
		}

		auto dx = w2shead.x - (width / 2);
		auto dy = w2shead.y - (height / 2);
		auto dist = sqrtf(dx * dx + dy * dy);

		if (dist < dynammedfov && dist < closestDistance) {
			closestDistance = dist;
			closestPawn = entity.actor;
		}

#pragma endregion

#pragma region Exploits
		static Vector3 PlayerFlyAddValue; //outside the loop
		static Vector3 PlayerFlySetLocation; //outside the loop
		if (Features->Exploits->pfly)
		{
			CamewaDescwipsion vCamera = GetCamera();
			Vector3 current_rotation = vCamera.Rotation;
			bool pushing;

			if (GetAsyncKeyState_Spoofed('W'))
			{
				pushing = true;
			}
			else if (GetAsyncKeyState_Spoofed('S'))
			{
				current_rotation = { -current_rotation.x, current_rotation.y + 180, 0 };
				pushing = true;
			}
			else if (GetAsyncKeyState_Spoofed('A'))
			{
				current_rotation = { 0, current_rotation.y + 270, 0 };
				pushing = true;
			}
			else if (GetAsyncKeyState_Spoofed('D'))
			{
				current_rotation = { 0, current_rotation.y + 90, 0 };
				pushing = true;
			}
			else
			{
				pushing = false;
			}

			float angle;

			angle = current_rotation.y * (M_PI / 180.0);
			float sy = sinf(angle);
			float cy = cosf(angle);

			angle = -current_rotation.x * (M_PI / 180.0);
			float sp = sinf(angle);
			float cp = cosf(angle);

			int PlayerFlySpeedValue = 10.f;

			if (pushing)
			{
				PlayerFlyAddValue = Vector3(PlayerFlyAddValue + Vector3{ cp * cy , cp * sy, -sp } *PlayerFlySpeedValue);
			}

			Engine::K2_SetActorLocation(fortnite::LocalPawn, PlayerFlySetLocation + PlayerFlyAddValue);
		}
#pragma endregion
	}

	FortPTR persistentlevel = request->read<uintptr_t>(fortnite::Uworld + Offsets::PersistentLevel);
	FortPTR actors = request->read<uintptr_t>(persistentlevel + Offsets::AActors);
	int actorcount = request->read<int>(persistentlevel + Offsets::ActorCount);

	for (int i = 0; i < actorcount; i++)
	{
		if (!fortnite::LocalPawn) continue;
		if (!fortnite::CurrentActor) continue;
		if (!fortnite::CurrentActorMesh) continue;

		uintptr_t currentactor = request->read<uintptr_t>(actors + i * 0x8);
		int currentactorid = request->read<int>(currentactor + 0x18);

		FVector Location = Engine::GetLocation(currentactor);
		FVector Screen = UFortCamera->W2S(Location);

		float distance = ce::LocalActorPos.Distance(Location) / 100.f;

		std::string ObjectName = GetNameFromFName(currentactorid);
		//std::cout << ObjectName;
		if (Features->Exploits->ObjectNames)
		{
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), ObjectName.c_str());
		}

		if (Features->Exploits->ProjectileTP)
		{
			if (strstr(ObjectName.c_str(), "B_Prj_") || ObjectName.find(("B_Prj_")))
			{
				FVector EnemyLocation = Engine::GetLocation(closestPawn);
				Engine::K2_SetActorLocation(currentactor, EnemyLocation);
			}
		}
	}

	if (Features->Aimbot->aimbot)
	{
		if (closestPawn)
		{
			if (GetAsyncKeyState_Spoofed(VK_RBUTTON))
			{
				int hitbox = 6;

				AimAt(closestPawn, 6, Features->Aimbot->prediction);
			}
		}
	}
	if (Features->Visuals->radars)
	{
		if (Features->Visuals->Radar->radartype == 0)
		{
			fortnite_radar(radar_position_x, radar_position_y, Features->Visuals->Radar->radarsize, false);
		}
		else if (Features->Visuals->Radar->radartype == 1)
		{
			fortnite_radar(radar_position_x, radar_position_y, Features->Visuals->Radar->radarsize, true);
		}
	}
}