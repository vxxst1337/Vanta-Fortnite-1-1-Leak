#include "../Includes/ultis.hpp"
#include "../Driver/driver.h"
#include <d3d9types.h>
#include "../Matrix/d3dx9math.h"




D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}


D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

struct FQuat
{
	double x;
	double y;
	double z;
	double w;

};

struct FTransform
{
	FQuat Rotation; 
	Vector3 Translation; 
	uint32_t pad0001; 
	Vector3 Scale3D; 
	uint32_t pad0002;

	FTransform() : Rotation{ (0.f, 0.f, 0.f, 0.f) }, Translation(0.f, 0.f, 0.f), Scale3D(0.f, 0.f, 0.f), pad0001(0), pad0002(0) { }

	FTransform(const FQuat& rot, const Vector3& translation, const Vector3& scale)
	{
		Rotation = rot;
		Translation = translation;
		Scale3D = scale;

		pad0001 = 0;
		pad0002 = 0;
	}

	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;

		m._41 = Translation.x;
		m._42 = Translation.y;
		m._43 = Translation.z;

		float x2 = Rotation.x + Rotation.x;
		float y2 = Rotation.y + Rotation.y;
		float z2 = Rotation.z + Rotation.z;

		float xx2 = Rotation.x * x2;
		float yy2 = Rotation.y * y2;
		float zz2 = Rotation.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * Scale3D.x;
		m._22 = (1.0f - (xx2 + zz2)) * Scale3D.y;
		m._33 = (1.0f - (xx2 + yy2)) * Scale3D.z;


		float yz2 = Rotation.y * z2;
		float wx2 = Rotation.w * x2;
		m._32 = (yz2 - wx2) * Scale3D.z;
		m._23 = (yz2 + wx2) * Scale3D.y;


		float xy2 = Rotation.x * y2;
		float wz2 = Rotation.w * z2;
		m._21 = (xy2 - wz2) * Scale3D.y;
		m._12 = (xy2 + wz2) * Scale3D.x;


		float xz2 = Rotation.x * z2;
		float wy2 = Rotation.w * y2;
		m._31 = (xz2 + wy2) * Scale3D.z;
		m._13 = (xz2 - wy2) * Scale3D.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

struct CamewaDescwipsion
{
	Vector3 Location;
	Vector3 Rotation;
	float FieldOfView;
};

CamewaDescwipsion GetCamera()
{
	CamewaDescwipsion camera;

	auto location_pointer = request->read<uintptr_t>(fortnite::Uworld + 0x110);
	auto rotation_pointer = request->read<uintptr_t>(fortnite::Uworld + 0x120);

	struct FNRot
	{
		double a; //0x0000
		char pad_0008[24]; //0x0008
		double b; //0x0020
		char pad_0028[424]; //0x0028
		double c; //0x01D0
	}fnRot;

	fnRot.a = request->read<double>(rotation_pointer);
	fnRot.b = request->read<double>(rotation_pointer + 0x20);
	fnRot.c = request->read<double>(rotation_pointer + 0x1d0);

	camera.Location = request->read<Vector3>(location_pointer);
	camera.Rotation.x = asin(fnRot.c) * (180.0 / M_PI);
	camera.Rotation.y = ((atan2(fnRot.a * -1, fnRot.b) * (180.0 / M_PI)) * -1) * -1;
	camera.FieldOfView = request->read<float>((uintptr_t)fortnite::PlayerController + 0x394) * 90.f;

	return camera;
}



class cem
{
public:
	Vector3 get_bone(const uintptr_t mesh, const int id)
	{
		fortnite::BoneArray = request->read<uintptr_t>(mesh + Offsets::BoneArray);
		FTransform bone = request->read<FTransform>(fortnite::BoneArray + (id * 0x60));
		FTransform component_to_world = request->read<FTransform>(mesh + Offsets::ComponetToWorld);
		D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
		return Vector3(matrix._41, matrix._42, matrix._43);
	}
}; cem* UMesh;

class comera
{
public:
	Vector3 W2S(Vector3 WorldLocation)
	{

		CamewaDescwipsion ViewPoint = GetCamera();
		ce::fov = ViewPoint.FieldOfView;
		D3DMATRIX tempMatrix = Matrix(ViewPoint.Rotation);
		Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
		Vector3 vDelta = WorldLocation - ViewPoint.Location;
		Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));
		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;
		return Vector3((width / 2.0f) + vTransformed.x * (((width / 2.0f) / tanf(ViewPoint.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (height / 2.0f) - vTransformed.y * (((width / 2.0f) / tanf(ViewPoint.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, 0);

	}
}; comera* UFortCamera;


bool IsVisible(uintptr_t mesh)
{
	float fLastSubmitTime = request->read<float>(mesh + 0x368);// + 20
	float fLastRenderTimeOnScreen = request->read<float>(mesh + 0x370);// + 24
	const float fVisionTick = 0.06f;
	bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
	return bVisible;
}


