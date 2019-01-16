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

	VECTOR3 Hit(const VECTOR3& position, VECTOR3& velocity);

	HRESULT Init(VECTOR2 split = { SPLIT, SPLIT }, VECTOR2 size = { SIZE, SIZE }, int textureNum = 0);			//初期化処理

	void GuiUpdate(void) override;

private:
	HRESULT InputData(void);

	HRESULT CreateMeshField(void);
	void    CreateVertex(int num);
	void    CreateIndex(int num);

	bool CalcHit(VERTEX* v, float& y, VECTOR3& velocity);

	Systems* systems_;

	uint vertexBuffer_;
	uint indexBuffer_;

	std::vector<VERTEX> outputVertex_;
	std::vector<WORD>   outputIndex_;

	VECTOR2 size_;
	VECTOR2 split_;

	Transform	   transform_;
	SpriteRenderer renderer_;

	int				texNum_;

	VECTOR3 ttttt;
	VECTOR3 ttt;
	VECTOR3 tttt;
};

#endif // _MESHFIELD_H_
