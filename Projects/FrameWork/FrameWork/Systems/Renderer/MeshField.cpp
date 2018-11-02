//-----------------------------------------------------------------------------
//
//	メッシュフィールド[MeshField.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "MeshField.h"
#include "../../Windows/Windows.h"
#include "../../Graphics/Graphics.h"
#include "../../Graphics/Wrapper.h"
#include "../GameSystems.h"
#include "Sprite/Texture.h"
#include "../Camera/CameraManager.h"
#include "../DebugSetting/GuiManager.h"

HRESULT MeshField::Init(VECTOR2 split, VECTOR2 size)
{
	split_ = split;
	size_  = size;

	InputData();
	return CreateMeshField();
}

MeshField::MeshField(Systems* systems) : GUI(systems, nullptr, "MeshField")
										, systems_(systems), vertexBuffer_(0), indexBuffer_(0)
										, size_(0, 0), split_(0, 0)
{
};

MeshField::~MeshField(void)
{
};

HRESULT MeshField::CreateMeshField(void)
{
	int vertexNum = 0;
	if (outputVertex_.size() > 0) 
	{
		vertexNum = (int)outputVertex_.size();
	}
	else
	{
		vertexNum = ((int)split_.x + 1) * ((int)split_.y + 1);
	}

	int indexNum = ((((int)split_.x + 1) * (int)split_.y + 1) * 2 + ((int)split_.y) * 2);

	CreateVertex(vertexNum);

	CreateIndex(indexNum);

	renderer_.Init(systems_, (int)Texture::Game::FIELD, &transform_);
	renderer_.SetVertex(vertexBuffer_, vertexNum);
	renderer_.SetIndex(indexBuffer_, indexNum);
	renderer_.shader = Shader::ENUM::DEPTH_SHADOW;
	renderer_.sort   = false;
	renderer_.fastDraw = true;

	return S_OK;
}

void MeshField::CreateVertex(int num)
{
	bool data = false;
	if (outputVertex_.size() > 0) { data = true; }

	VECTOR2 oneSize = { size_.x / split_.x, size_.y / split_.y };

	if (!data)
	{
		for (int i = 0; i < num; ++i)
		{
			float y = 0;

			VECTOR3 nomals = VECTOR3(0.0f, 1.0f, 0.0f);
			VECTOR3 vertexTemp = { (oneSize.x * (i % ((int)split_.x + 1))) - Half(size_.x), y, -(oneSize.y * ((int)(i / (split_.x + 1)))) + Half(size_.y) };
			int v = (int)split_.x + 1;
			if (((i > v)&&(i < num - v)) && ((i % v != 0) && (i % (int)split_.x != 0)))
			{
				VECTOR3 cross1 = VecNorm(outputVertex_[i - ((int)split_.x + 1)].position - vertexTemp);
				VECTOR3 cross2 = VecNorm(outputVertex_[i - 1].position - vertexTemp);

				nomals = VecNorm(VecCross(cross1, cross2));
				nomals.y *= -1;
			}

			VERTEX3D t = { vertexTemp, nomals, COLOR(1, 1, 1, 1), VECTOR2((float)(i % (int)(split_.x + 1)), (float)(int)(i / (split_.x + 1))) };

			// 分割数が奇数の時
			if ((int)split_.x % 2 == 1)
			{
				t.position.x -= Half(oneSize.x);
				t.position.z += Half(oneSize.y);
			}

			outputVertex_.emplace_back(t);
		}
	}

	vertexBuffer_ = systems_->GetRenderer()->GetWrapper()->CreateVertexBuffer(&outputVertex_[0], sizeof(outputVertex_[0]), num);
}

void MeshField::CreateIndex(int num)
{
	bool data = false;
	if (outputIndex_.size() > 0) { data = true; }

	WORD index;
	if(!data)
	{
		int k = 0;
		for (int i = 0; i < num; i++)
		{
			if (i % ((int)split_.x * 2 + 4) == split_.x * 2 + 2)
			{
				index = outputIndex_[i - 1];
				outputIndex_.emplace_back(index);
			}
			else if (i % ((int)split_.x * 2 + 4) == split_.x * 2 + 3)
			{
				index = (WORD)(split_.x + 1 + k);
				outputIndex_.emplace_back(index);
			}
			else if (i % 2 == 0)
			{
				index = (WORD)(split_.x + 1 + k);
				outputIndex_.emplace_back(index);
			}
			else if (i % 2 == 1)
			{
				index = (WORD)k;
				outputIndex_.emplace_back(index);
				k++;
			}
		}
	}
	indexBuffer_ = systems_->GetRenderer()->GetWrapper()->CreateIndexBuffer(&outputIndex_[0], num);
}

void MeshField::GuiUpdate(void)
{
	VECTOR2 splitOld = split_;
	VECTOR2 sizeOld  = size_;

	int sp = (int)split_.x;
	ImGui::SliderInt("split W", &sp, 1, 100);
	split_.x = (float)sp;
	sp = (int)split_.y;
	ImGui::SliderInt("split H", &sp, 1, 100);
	split_.y = (float)sp;

	ImGui::Text("              size W");
	if (ImGui::Button("w_dw")) { size_.x = (float)((int)(size_.x - 0.0001f)); }
	ImGui::SameLine();
	if (ImGui::Button("w_up")) { size_.x = (float)((int)size_.x + 1); }
	ImGui::SliderFloat("w", &size_.x, 1, 500);

	ImGui::Text("              size H");
	if (ImGui::Button("h_dw")) { size_.y = (float)((int)(size_.y - 0.0001f)); }
	ImGui::SameLine();
	if (ImGui::Button("h_up")) { size_.y = (float)((int)size_.y + 1); }
	ImGui::SliderFloat("h", &size_.y, 1, 500);

	if (splitOld != split_ || sizeOld != size_)
	{
		outputVertex_.clear();
		outputIndex_.clear();
		CreateMeshField();
	}
};

HRESULT MeshField::InputData(void)
{
	FILE* pFile;

	string temp = Define::ResourceDirectoryName + MeshField::FILE_NAME;
	fopen_s(&pFile, temp.c_str(), "rb");
	if (!pFile) { return E_FAIL; }

	fread(&split_, sizeof(VECTOR2), 1, pFile);
	fread(&size_, sizeof(VECTOR2), 1, pFile);

	UINT size = 0;
	fread(&size, sizeof(size), 1, pFile);
	for (int i = 0; i < (int)size; ++i)
	{
		VERTEX3D v;
		fread(&v.position, sizeof(VECTOR3), 1, pFile);
		fread(&v.normal, sizeof(VECTOR3), 1, pFile);
		fread(&v.color, sizeof(VECTOR4), 1, pFile);
		fread(&v.texcoord, sizeof(VECTOR2), 1, pFile);
		outputVertex_.emplace_back(v);
	}

	fread(&size, sizeof(size), 1, pFile);
	for (int i = 0; i < (int)size; ++i)
	{
		UINT v;
		fread(&v, sizeof(UINT), 1, pFile);
		outputIndex_.emplace_back(v);
	}

	fclose(pFile);
	return S_OK;
}

float MeshField::Hit(VECTOR3 position)
{
	int max = (int)outputVertex_.size();
	VECTOR2 oneSize = { size_.x / split_.x, size_.y / split_.y };

	int row = -1, line = -1;
	float dif = Half(size_.x);
	for (int i = 0; i < split_.x; ++i)
	{
		if (oneSize.x * i - dif <= position.x)
		{
			if (oneSize.x * (i + 1) - dif >= position.x)
			{
				row = i + 1;
				break;
			}
		}
	}
	dif = Half(size_.y);
	for (int i = 0; i < split_.y; ++i)
	{
		if (dif - oneSize.y * i <= position.z)
		{
			if (dif - oneSize.y * (i - 1) >= position.z)
			{
				line = i;
				break;
			}
		}
	}

	if (row < 0 || line < 0) { return 0; }

	int playerLineRow[2] = { 0, 0 };

	playerLineRow[0] = (int)((split_.x + 1) * (line - 1) + (row - 1));
	playerLineRow[1] = (int)((playerLineRow[0] + (split_.x + 1)));

	for (int i = 0; i < 2; i++)
	{
		if (playerLineRow[i] < 0) { continue; }
		VECTOR3 temp[4];
		ZeroMemory(temp, sizeof(VECTOR3) * 4);
		temp[3] = position;

		float y = 0;
		if (playerLineRow[i] >= max) { continue; }
		if (outputVertex_[playerLineRow[i]].position.x == Half(size_.x)) { continue; }
		if (playerLineRow[i] + split_.x + 1 < max)
		{
			temp[0] = outputVertex_[playerLineRow[i]].position;
			temp[1] = outputVertex_[playerLineRow[i] + 1].position;
			temp[2] = outputVertex_[playerLineRow[i] + (int)split_.x + 1].position;

			if (CalcHit(temp, y)) { return y - position.y; }
		}
		if (playerLineRow[i] - (split_.x + 2) > 0)
		{
			if (playerLineRow[i] + 1 >= max) { continue; }
			temp[0] = outputVertex_[playerLineRow[i] + 1].position;
			temp[1] = outputVertex_[playerLineRow[i]].position;
			temp[2] = outputVertex_[playerLineRow[i] - ((int)split_.x + 0)].position;

			if (CalcHit(temp, y)) { return y - position.y; }
		}
	}
	return 0;
}

bool MeshField::CalcHit(VECTOR3* v, float& y)
{
	for (int i = 0; i < 3; i++)
	{
		int t = (i + 2) % 3;

		VECTOR3 vertex = v[i] - v[t];
		VECTOR3 vp     = v[3] - v[t];

		float c = VecCross(VECTOR2(vertex.x, vertex.z), VECTOR2(vp.x, vp.z));
		if (c > 0) { return false; }
	}

	VECTOR3 p0 = v[2];
	VECTOR3 n = VecCross(v[0] - v[2], v[1] - v[0]);
	if (n.y == 0) { return false; }
	y = p0.y - (n.x * (v[3].x - p0.x) + n.z * (v[3].z - p0.z)) / n.y;
	return true;
}
