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

HRESULT MeshField::Init(VECTOR2 split, VECTOR2 size, int textureNum)
{
	split_ = split;
	size_  = size;

	texNum_ = textureNum;
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

	renderer_.Init(systems_, texNum_, &transform_);
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

			VERTEX t;
			t.position	= vertexTemp;
			t.normal	= nomals;
			t.tangent	= VECTOR3(0, 0, 0);
			t.color		= COLOR(1, 1, 1, 1);
			t.boneIndex = VECTOR4(0, 0, 0, 0);
			t.weight	= VECTOR4(0, 0, 0, 0);
			t.texcoord  = VECTOR2((i % (int)(split_.x + 1)), (int)(i / (split_.x + 1)));

			// 分割数が奇数の時
			if ((int)split_.x % 2 == 1)
			{
				t.position.x -= Half(oneSize.x);
				t.position.z += Half(oneSize.y);
			}

			outputVertex_.emplace_back(t);
		}
	}

	vertexBuffer_ = systems_->GetGraphics()->GetWrapper()->CreateVertexBuffer(&outputVertex_[0], sizeof(outputVertex_[0]), num);
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
	indexBuffer_ = systems_->GetGraphics()->GetWrapper()->CreateIndexBuffer(&outputIndex_[0], num);
}

void MeshField::GuiUpdate(void)
{
	ImGui::Text("vel %.2f, %.2f, %.2f", ttt.x, ttt.y, ttt.z);
	ImGui::Text("dot    %.2f", tttt.x);
	ImGui::Text("%.2f, %.2f, %.2f", ttttt.x, ttttt.y, ttttt.z);

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

	uint size = 0;
	fread(&size, sizeof(size), 1, pFile);
	for (int i = 0; i < (int)size; ++i)
	{
		VERTEX v;
		fread(&v.position, sizeof(VECTOR3), 1, pFile);
		fread(&v.normal  , sizeof(VECTOR3), 1, pFile);
		fread(&v.color   , sizeof(VECTOR4), 1, pFile);
		fread(&v.texcoord, sizeof(VECTOR2), 1, pFile);
		v.tangent	= VECTOR3(0, 0, 0);
		v.boneIndex = VECTOR4(0, 0, 0, 0);
		v.weight	= VECTOR4(0, 0, 0, 0);
		outputVertex_.emplace_back(v);
	}

	fread(&size, sizeof(size), 1, pFile);
	for (int i = 0; i < (int)size; ++i)
	{
		uint v;
		fread(&v, sizeof(uint), 1, pFile);
		outputIndex_.emplace_back(v);
	}

	fclose(pFile);
	return S_OK;
}

VECTOR3 MeshField::Hit(const VECTOR3& position, VECTOR3& velocity)
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

	if (row < 0 || line < 0) { return position; }

	int playerLineRow[2] = { 0, 0 };

	playerLineRow[0] = (int)((split_.x + 1) * (line - 1) + (row - 1));
	playerLineRow[1] = (int)((playerLineRow[0] + (split_.x + 1)));

	for (int i = 0; i < 2; ++i)
	{
		if (playerLineRow[i] < 0) { continue; }
		VERTEX temp[4];
		ZeroMemory(temp, sizeof(VERTEX) * 4);
		temp[3].position = position;

		float y = 0;
		if (playerLineRow[i] >= max) { continue; }
		if (outputVertex_[playerLineRow[i]].position.x == Half(size_.x)) { continue; }
		if (playerLineRow[i] + split_.x + 1 < max)
		{
			temp[0] = outputVertex_[playerLineRow[i]];
			temp[1] = outputVertex_[playerLineRow[i] + 1];
			temp[2] = outputVertex_[playerLineRow[i] + (int)split_.x + 1];

			if (CalcHit(temp, y, velocity)) { return VECTOR3(position.x, position.y + y, position.z); }
		}
		if (playerLineRow[i] - (split_.x + 2) > 0)
		{
			if (playerLineRow[i] + 1 >= max) { continue; }
			temp[0] = outputVertex_[playerLineRow[i] + 1];
			temp[1] = outputVertex_[playerLineRow[i]];
			temp[2] = outputVertex_[playerLineRow[i] - ((int)split_.x + 0)];

			if (CalcHit(temp, y, velocity)) { return VECTOR3(position.x, position.y + y, position.z); }
		}
	}
	return position;
}

bool MeshField::CalcHit(VERTEX* v, float& y, VECTOR3& velocity)
{
	for (int i = 0; i < 3; ++i)
	{
		int t = (i + 2) % 3;

		VECTOR3 vertex = v[i].position - v[t].position;
		VECTOR3 vp     = v[3].position - v[t].position;

		float c = VecCross(VECTOR2(vertex.x, vertex.z), VECTOR2(vp.x, vp.z));
		if (c > 0) { return false; }
	}

	VECTOR3 p0 = v[2].position;
	VECTOR3 n = VecCross(v[0].position - v[2].position, v[1].position - v[0].position);

	VECTOR3 nn = VecNorm(n);
	VECTOR3 velocityNorm = VecNorm(velocity);
	float dot = VecDot(velocityNorm, nn);

	VECTOR3 temp = VecNorm(velocityNorm - (nn * dot)) * VecLength(velocity);

	if (temp.y != 0)
	{
		if (Abs(velocityNorm.x) < Abs(velocityNorm.z)) { temp.x += temp.y; }
		else { temp.z += temp.y; }
		temp.y = 0;
	}

	ttt = temp;
	tttt.x = dot;
	ttttt = VecNorm(velocity) + nn;

	VECTOR3 vp = v[3].position - v[2].position;
	if (vp.y < 0)
	{
		if (n.y == 0) { return false; }
		y = p0.y - (n.x * (v[3].position.x - p0.x) + n.z * (v[3].position.z - p0.z)) / n.y;
	}

	return true;
}
