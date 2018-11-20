//-----------------------------------------------------------------------------
//
//	メッシュフィールド[MeshField.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "../../Define/Define.h"
#include "../DebugSetting/GUI.h"
#include "Sprite/SpriteRenderer.h"
#include "../GameSystems.h"

class MeshField : public GUI
{
	static constexpr char* FILE_NAME = "Data/fieldData.bfd";

	static constexpr int UV_X = 1;
	static constexpr int UV_Y = 1;

	static constexpr int VERTEX_NUM = 4;
	static constexpr int INDEX_NUM  = 6;

public:
	static constexpr float SIZE  = 10.0f;
	static constexpr float SPLIT = 1;

	MeshField(Systems* systems);
	~MeshField(void);

	float Hit(VECTOR3 position);

	HRESULT Init(VECTOR2 split = { SPLIT, SPLIT }, VECTOR2 size = { SIZE, SIZE });			//初期化処理

	void GuiUpdate(void) override;

private:
	HRESULT InputData(void);

	HRESULT CreateMeshField(void);
	void    CreateVertex(int num);
	void    CreateIndex(int num);

	bool CalcHit(VECTOR3* v, float& y);

	Systems* systems_;

	UINT vertexBuffer_;
	UINT indexBuffer_;

	std::vector<VERTEX> outputVertex_;
	std::vector<WORD>   outputIndex_;

	VECTOR2 size_;
	VECTOR2 split_;

	Transform	   transform_;
	SpriteRenderer renderer_;
};

#endif // _MESHFIELD_H_
