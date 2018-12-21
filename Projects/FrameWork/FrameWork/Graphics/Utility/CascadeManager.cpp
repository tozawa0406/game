#include "CascadeManager.h"
#include "../../Systems/GameSystems.h"
#include "../../Systems/Light.h"

#include "../../Scene/SceneManager.h"
#include "../../../../Sources/Scene/GameScene.h"
#include "../../Systems/Camera/CameraManager.h"

CascadeManager::CascadeManager(void) : 
	cameraPosition_(VECTOR3(0))
	, lightDirection_(VECTOR3(0))
	, isSoft_(true)
	, isCascadeColor_(false)
{
	for (auto& m : shadowMatrix_) { m.Identity(); }
	for (auto& s : splitShadowMap_) 
	{
		s.z = 0;
		s.xMax = 0;
		s.xMin = 0;
	}
}

CascadeManager::~CascadeManager(void)
{
}

HRESULT CascadeManager::Init(void)
{
	return S_OK;
}

void CascadeManager::Uninit(void)
{
}

void CascadeManager::ComputeShadowMatrixPSSM(void)
{
	Camera* camera = nullptr;
	LightInfo info;
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& sceneManager = systems->GetSceneManager())
		{
			if (sceneManager->GetSceneNum() == SceneList::GAME)
			{
				if (const auto& game = static_cast<GameScene*>(sceneManager->GetScene()))
				{
					if (const auto& light = game->GetLight())
					{
						info = light->GetLightInfo();
					}
					else { return; }
				}
				else { return; }
				if (const auto& cameraManager = sceneManager->GetCameraManager())
				{
					camera = cameraManager->GetCamera();
				}
			}
			else { return; }
		}
	}
	cameraPosition_ = camera->GetPos();

	// ライトの方向ベクトル
	lightDirection_ = VecNorm(info.direction);

	float diff = 265 - 60;
	float oneSize = diff / MAX_CASCADE;

	float nearFar[MAX_CASCADE + 1];
	for (int i = 0; i < MAX_CASCADE + 1; ++i)
	{
		nearFar[i] = i * oneSize;
	}
	float oneAria = 200;
	for (int j = 0; j < MAX_CASCADE; ++j)
	{
		float aria = oneAria * (1 + j);
		// 凸包
		VECTOR3 convexHull[8];
		convexHull[0] = VECTOR3(-aria, -aria, -aria);
		convexHull[1] = VECTOR3( aria, -aria, -aria);
		convexHull[2] = VECTOR3(-aria, -aria,  aria);
		convexHull[3] = VECTOR3( aria, -aria,  aria);
		convexHull[4] = VECTOR3(-aria,  aria, -aria);
		convexHull[5] = VECTOR3( aria,  aria, -aria);
		convexHull[6] = VECTOR3(-aria,  aria,  aria);
		convexHull[7] = VECTOR3( aria,  aria,  aria);

		// カメラの行列を求める
		VECTOR3 pos = camera->GetPos();
		VECTOR3 dir = camera->GetFront();
		Transform t;
		t.position = pos;
		t.rotation.x = atan2f(pos.z - dir.z, pos.y - dir.y);
		t.rotation.y = atan2f(pos.z - dir.z, pos.x - dir.x);
		t.rotation.z = atan2f(pos.x - dir.x, pos.y - dir.y);
		t.scale = VECTOR3(1);

		splitShadowMap_[j].xMin =  1000;
		splitShadowMap_[j].xMax = -1000;
		MATRIX world = MATRIX().Identity().Scaling(0.25f).Create(&t);
		for (int i = 0; i < 8; ++i)
		{
			convexHull[i] = VecTransform(convexHull[i], world);
			if (convexHull[i].x < splitShadowMap_[j].xMin) { splitShadowMap_[j].xMin = convexHull[i].x; }
			if (convexHull[i].x > splitShadowMap_[j].xMax) { splitShadowMap_[j].xMax = convexHull[i].x; }
		}

		// ライトのビュー行列を算出
		MATRIX lightView = CreateLookTo(info.position, lightDirection_, VECTOR3(0, 1, 0));

		// 求め直したライトのビュー行列を使ってAABBを求める
		VECTOR3 point = VecTransformCoord(convexHull[0], lightView);
		VECTOR3 mini = point;
		VECTOR3 maxi = point;
		for (int i = 1; i < 8; ++i)
		{
			point = VecTransformCoord(convexHull[i], lightView);
			mini = VecMin(mini, point);
			maxi = VecMax(maxi, point);
		}

		// サイズを求める
		float size = VecLength((maxi - mini));
		// ライトの射影行列
		MATRIX lightProj = CreateOrthographic(size, size, nearFar[j], nearFar[j + 1]);
		splitShadowMap_[j].z = nearFar[j + 1];

		// ライトのビュー射影行列を求める
		MATRIX lightViewProj = lightView * lightProj;

		point = VecTransformCoord(convexHull[0], lightViewProj);
		mini = point;
		maxi = point;
		for (int i = 1; i < 8; ++i)
		{
			point = VecTransformCoord(convexHull[i], lightViewProj);
			mini = VecMin(mini, point);
			maxi = VecMax(maxi, point);
		}
		BOX box;
		box.mini = mini;
		box.maxi = maxi;
		// クリップ行列を求める
		MATRIX clip = CreateUnitCubeClipMatrix(box);

		// シャドウマップめいっぱいに映るように
		lightProj = lightProj * clip;

		shadowMatrix_[j] = lightView * lightProj;
	}
}

MATRIX CascadeManager::CreateUnitCubeClipMatrix(const BOX& box)
{
	// 単位キューブクリップ行列を求める
	MATRIX clip;
	clip._11 = 2.0f / (box.maxi.x - box.mini.x);
	clip._12 = 0.0f;
	clip._13 = 0.0f;
	clip._14 = 0.0f;

	clip._21 = 0.0f;
	clip._22 = 2.0f / (box.maxi.y - box.mini.y);
	clip._23 = 0.0f;
	clip._24 = 0.0f;

	clip._31 = 0.0f;
	clip._32 = 0.0f;
	clip._33 = 1.0f / (box.maxi.z - box.mini.z);
	clip._34 = 0.0f;

	clip._41 = -(box.maxi.x + box.mini.x) / (box.maxi.x - box.mini.x);
	clip._42 = -(box.maxi.y + box.mini.y) / (box.maxi.y - box.mini.y);
	clip._43 = -box.mini.z / (box.maxi.z - box.mini.z);
	clip._44 = 1.0f;

	return clip;
}

VECTOR3 CascadeManager::VecMin(const VECTOR3& v1, const VECTOR3& v2)
{
	return VECTOR3(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));
}

VECTOR3 CascadeManager::VecMax(const VECTOR3& v1, const VECTOR3& v2)
{
	return VECTOR3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
}

MATRIX CascadeManager::CreateLookTo(const VECTOR3& cameraPosition, const VECTOR3& cameraDirection, const VECTOR3& cameraUpVector)
{
	MATRIX result;
	VECTOR3 zaxis = -VecNorm(cameraDirection);
	VECTOR3 xaxis = VecNorm(VecCross(cameraUpVector, zaxis));
	VECTOR3 yaxis = VecNorm(VecCross(zaxis, xaxis));

	result._11 = xaxis.x;
	result._12 = yaxis.x;
	result._13 = zaxis.x;
	result._14 = 0.0f;

	result._21 = xaxis.y;
	result._22 = yaxis.y;
	result._23 = zaxis.y;
	result._24 = 0.0f;

	result._31 = xaxis.z;
	result._32 = yaxis.z;
	result._33 = zaxis.z;
	result._34 = 0.0f;

	result._41 = -VecDot(xaxis, cameraPosition);
	result._42 = -VecDot(yaxis, cameraPosition);
	result._43 = -VecDot(zaxis, cameraPosition);
	result._44 = 1.0f;

	return result;
}

MATRIX CascadeManager::CreateOrthographic(const float width, const float height, const float nearClip, const float farClip)
{
	assert(width != 0.0f);
	assert(height != 0.0f);
	register float diffNF = nearClip - farClip;
	assert(diffNF != 0.0f);

	MATRIX result;
	result._11 = 2.0f / width;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 2.0f / height;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = 1.0f / diffNF;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = nearClip / diffNF;
	result._44 = 1.0f;

	return result;
}

void CascadeManager::GuiUpdate(void)
{
	if (ImGui::Button("Color")) { isCascadeColor_ = !isCascadeColor_; }
	ImGui::SameLine();
	if (isCascadeColor_) { ImGui::Text("true"); }
	else { ImGui::Text("false"); }

	if (ImGui::Button("Soft")) { isSoft_ = !isSoft_; }
	ImGui::SameLine();
	if (isSoft_) { ImGui::Text("true"); }
	else { ImGui::Text("false"); }
}
