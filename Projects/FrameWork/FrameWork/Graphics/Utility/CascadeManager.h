#ifndef _CASCADE_MANAGER_H_
#define _CASCADE_MANAGER_H_

#include "../../Define/Define.h"
#include "../../Systems/DebugSetting/GUI.h"

class Camera;
struct BOX
{
	VECTOR3 mini;
	VECTOR3 maxi;
};

struct SHADOWMAP_SPLIT
{
	float z;
	float xMax;
	float xMin;
};

class CascadeManager
{
public:
	static const int MAX_CASCADE	= 4;
	static const int MAP_SIZE		= 1024 * 2;

	/* @brief	コンストラクタ		*/
    CascadeManager(void);

	/* @brief	デストラクタ		*/
    ~CascadeManager(void);



	/* @brief	初期化処理
	 * @param	なし
	 * @return	成功失敗			*/
	HRESULT Init(void);

	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
    void	Uninit(void);
	
	/* @brief	シャドウ描画行列の分割処理
	 * @param	なし
	 * @return	なし				*/
	void ComputeShadowMatrixPSSM(void);

	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし				*/
	void GuiUpdate(void);

	/* @brief	シャドウ行列の受け渡し
	 * @param	(i)		どのシャドウ行列か(4まで)
	 * @return	シャドウ行列		*/
	inline const MATRIX& GetShadowMatrix(int i) { return shadowMatrix_[i]; }

	/* @brief	カメラの位置の取得
	 * @param	なし
	 * @return	カメラの位置		*/
	inline const VECTOR3& GetCameraPosition(void) { return cameraPosition_; }
	
	/* @brief	ライト方向の取得
	 * @param	なし
	 * @return	ライト方向			*/
	inline const VECTOR3& GetLightDirection(void) { return lightDirection_; }
	
	/* @brief	シャドウの分割位置取得
	 * @param	(i)		どのシャドウか(4まで)
	 * @return	シャドウの分割位置	*/
	inline const SHADOWMAP_SPLIT& GetShadowSplit(int i) { return splitShadowMap_[i]; }

	/* @brief	ソフトシャドウか			*/
	inline bool IsSoft(void) { return isSoft_; }

	/* @brief	カスケードに色は付けるか	*/
	inline bool IsCascadeColor(void) { return isCascadeColor_; }

private:
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

#endif // _CASCADE_MANAGER_H_