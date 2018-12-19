#pragma once

#include "../../../Define/Define.h"
#include "../../DirectX11/Dx11Include.h"

static const int MAX_CASCADE = 4;

class Camera;
struct SADOWSTATE
{
	ID3D11DepthStencilView*     pDSV[MAX_CASCADE];
	ID3D11ShaderResourceView*   pDepthSRV[MAX_CASCADE];
	ID3D11VertexShader*         pVS;
	ID3D11SamplerState*         pSmp;
	D3D11_VIEWPORT              viewport;
};

struct BOX
{
	VECTOR3 mini;
	VECTOR3 maxi;
};

struct CONSTANT_DRAW
{
	VECTOR4		texlSize;
	VECTOR4		cameraPos;
	VECTOR4		lightDirection_;
	float		splitPos[MAX_CASCADE];
	float		splitPosXMax[MAX_CASCADE];
	float		splitPosXMin[MAX_CASCADE];
	MATRIX		shadow[MAX_CASCADE];
};

struct SHADOWMAP_SPLIT
{
	float z;
	float xMax;
	float xMin;
};

class DirectX11;
class CascadeManager
{
public:
	/* @brief	コンストラクタ		*/
    CascadeManager(void);

	/* @brief	デストラクタ		*/
    ~CascadeManager(void);



	/* @brief	初期化処理
	 * @param	なし
	 * @return	成功失敗			*/
	HRESULT Init(DirectX11* directX);

	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
    void	Uninit(void);

	/* @brief	影描画準備
	 * @param	描画するシャドウマップの番号
	 * @return	なし				*/
	void Begin(int i);

	/* @brief	影描画終了
	 * @param	なし
	 * @return	なし				*/
	void End(void);
	
	/* @brief	オブジェクト描画ConstantBufferの設定
	 * @param	なし
	 * @return	なし				*/
	void Set(void);



	/* @brief	シャドウマップの描画
	 * @param	なし
	 * @return	なし				*/
	void DrawShadowMap(void);

	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし				*/
	void GuiUpdate(void);

	/* @brief	シャドウ行列の受け渡し
	 * @param	(i)		どのシャドウ行列か(4まで)
	 * @return	シャドウ行列		*/
	inline const MATRIX& GetShadowMatrix(int i) { return shadowMatrix_[i]; }

private:
	/* @brief	シャドウマップの初期化処理
	 * @param	なし
	 * @return	成功失敗			*/
	HRESULT InitShadowState(DirectX11* directX);

	/* @brief	シャドウマップの後処理
	 * @param	なし
	 * @return	なし				*/
	void	UninitShadowState(void);

	/* @brief	シャドウ描画行列の分割処理
	 * @sa		Begin()
	 * @param	なし
	 * @return	なし				*/
	void ComputeShadowMatrixPSSM(void);

	/* @brief	単位キューブクリップ行列を求める
	 * @param	(box)	箱
	 * @return	単位キューブクリップ行列		*/
	MATRIX CreateUnitCubeClipMatrix(const BOX& box);

	/* @brief	VECTOR3内の小さい要素を返す
	 * @param	(v1)	1つ目
	 * @param	(v2)	2つ目
	 * @return	一番小さいベクトル	*/
	VECTOR3 VecMin(const VECTOR3& v1, const VECTOR3& v2);

	/* @brief	VECTOR3内の大きい要素を返す
	 * @param	(v1)	1つ目
	 * @param	(v2)	2つ目
	 * @return	一番大きいベクトル	*/
	VECTOR3 VecMax(const VECTOR3& v1, const VECTOR3& v2);

	/* @brief	ビュー行列の生成
	 * @param	(cameraPosition)	カメラ位置
	 * @param	(cameraDirection)	カメラ方向
	 * @param	(cameraUpVector)	カメラ上ベクトル
	 * @return	ビュー行列			*/
	MATRIX CreateLookTo(const VECTOR3& cameraPosition, const VECTOR3& cameraDirection, const VECTOR3& cameraUpVector); 

	/* @brief	プロジェクション行列の生成
	 * @param	(width)		幅
	 * @param	(height)	高さ
	 * @param	(nearClip)	ニア
	 * @param	(farClip)	ファー
	 * @return	プロジェクション行列		*/
	MATRIX CreateOrthographic(const float width, const float height, const float nearClip, const float farClip);

	//! 描画するシャドウマップ
	int drawNum_ = 0;
	//! シャドウマップ
	SADOWSTATE			shadowState_;

	//! 描画で渡すコンストタントバッファ
    ID3D11Buffer*		ConstantDraw_;

	//! カメラポジション
	VECTOR3				cameraPosition_;
	//! ライト方向
    VECTOR3				lightDirection_;
	//! シャドウマトリクス
	MATRIX				shadowMatrix_[MAX_CASCADE];
	//! シャドウマップの分割位置
	SHADOWMAP_SPLIT		splitShadowMap_[MAX_CASCADE];

	bool	isSoft_;			//! ソフトシャドウか
	bool	isCascadeColor_;	//! カスケードに色は付けるか
};
