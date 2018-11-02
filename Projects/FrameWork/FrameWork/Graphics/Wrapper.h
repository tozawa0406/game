//-----------------------------------------------------------------------------
//
//	Graphics毎に依存した処理[Wrapper.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include "../Define/Define.h"

// 前方定義
struct CanvasRenderer;
struct SpriteRenderer;
struct MeshRenderer;
class  Particle;
struct ColliderRenderer;
struct LightInfo;
class Shader;
//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class Wrapper
{
protected:
	MATRIX inverse_;		// viewの逆行列

public:	
	static constexpr UINT R_ERROR = UINT_MAX;		// エラー値

	// プリミティブの値
	class PRIMITIVE
	{
	public:
		// プリミティブの種類
		enum class TYPE
		{
			POINT = 0,
			LINE,
			LINELIST,
			TRIANGLE,
			TRIANGLE_STRIP,
			TRIANGLE_FAN,
		};
		// 頂点数
		class V
		{
		public:
			static constexpr int CIRCUMFERENCE		= 100;          // 円周
			static constexpr int POINT				= 1;			// 点
			static constexpr int LINE				= 2;			// 線
			static constexpr int RECT				= 5;			// 四角(くり抜き)
			static constexpr int FILL_TRIANG		= 3;			// 三角
			static constexpr int FILL_RECT			= 4;			// 四角
			static constexpr int FILL_CIRCLE		= 102;			// 円

			// 当たり判定描画に使用
			static constexpr int COLLIDER_RECTANGLE = 5;
			static constexpr int COLLIDER_CIRCLE	= CIRCUMFERENCE + 1;
			static constexpr int COLLIDER_SEGMENT	= 2;
			static constexpr int COLLIDER_SPHERE	= CIRCUMFERENCE + 1;
		};
	};

	// FVF
	enum class FVF
	{
		VERTEX_2D = 0,
		VERTEX_3D,
		VERTEX_POINT,
		INDEX,
	};
	// アルファブレンド
	enum class ALFA_BREND
	{
		DEF = 0,
		ADD,
		MAX
	};

	// テクスチャ情報
	struct TextureData
	{
		VECTOR2 size;
	};

	// コンストラクタ
	Wrapper(void) { inverse_.Identity(); }
	// デストラクタ
	virtual ~Wrapper(void) {}

	virtual void Init(void)   {}		// 初期化処理
	virtual void Uninit(void) {}		// 後処理

	// 頂点バッファ生成
	virtual UINT    CreateVertexBuffer(const void* v, UINT size, UINT vnum) = 0;
	// インデックスバッファ生成
	virtual UINT    CreateIndexBuffer(const WORD* v, UINT vnum)		 = 0;
	// バッファの解放(頂点バッファ、インデックスバッファの判定は第二引数のFVF)
	virtual void    ReleaseBuffer(UINT number, FVF fvf)    = 0;

	// 2D描画の開始
	virtual void    BeginDrawCanvasRenderer(void) = 0;
	// 2D描画の終了
	virtual void    EndDrawCanvasRenderer(void)   = 0;

	// 3D描画の開始
	virtual void    BeginDrawObjectRenderer(void) = 0;
	// 3D描画の終了
	virtual void    EndDrawObjectRenderer(void)   = 0;

	// 描画テクスチャのセット
	virtual void    SetTexture(int stage, int texNum = -1, int modelNum = -1) = 0;

	// 描画処理
	virtual void    Draw(const CanvasRenderer*   obj, const Shader* shader) = 0;
	virtual void    Draw(const SpriteRenderer*   obj, const Shader* shader) = 0;
	virtual void    Draw(MeshRenderer*     obj, const Shader* shader) = 0;
	virtual void	Draw(const Particle*	     obj, const Shader* shader) = 0;
	virtual void    Draw(const ColliderRenderer* obj) = 0;

	// テクスチャのロード
	virtual HRESULT LoadTexture(string fileName, int texNum, int modelNum = -1) = 0;
	// テクスチャの解放
	virtual void    ReleaseTexture(int texNum, int modelNum = -1) = 0;
	// テクスチャサイズの取得
	virtual VECTOR2 GetTextureSize(int texNum) = 0;

	// モデルの読み込み
	virtual HRESULT LoadModel(string fileName, int modelNum) = 0;
	// モデルアニメーションの読み込み
	virtual HRESULT LoadModelAnimation(string fileName, int parent) = 0;
	// モデルの解放
	virtual void    ReleaseModel(int modelNum) = 0;

	// view行列の生成
	virtual MATRIX  CreateViewMatrix(VECTOR3 position, VECTOR3 at, VECTOR3 up) = 0;
	// projection行列の生成
	virtual MATRIX  CreateProjectionMatrix(int fov, float aspect, float cnear, float cfar) = 0;

	// 頂点シェーダーの生成
	virtual UINT	CreateVertexShader(string fileName, string method, string version, void* t = nullptr, UINT elemNum = 0) = 0;
	// 頂点シェーダーのセット
	virtual HRESULT	SetVertexShader(UINT number)		= 0;
	// 頂点シェーダーの解放
	virtual void	ReleaseVertesShader(UINT number)	= 0;

	// ピクセルシェーダーの生成
	virtual UINT	CreatePixelShader(string fileName, string method, string version) = 0;
	// ピクセルシェーダーのセット
	virtual HRESULT	SetPixelShader(UINT number)		= 0;
	// ピクセルシェーダーの解放
	virtual void	ReleasePixelShader(UINT number) = 0;

	// ライトの設定
	virtual void SetLight(LightInfo& light) = 0;

	// シャドウマップの生成
	virtual HRESULT CreateShadowMap(void)  { return S_OK; }
	// シャドウマップに描画開始
	virtual void    BeginDrawShadow(void)  {}
	// シャドウマップに描画終了
	virtual void	EndDrawShadow(void)    {}
	// シャドウマップの解放
	virtual void	ReleaseShadowMap(void) {}

	// スクリーンショットの描画
	virtual void DrawScreenshot(CanvasRenderer& sprite) = 0;
	// スクリーンショットの生成
	virtual void CreateScreenshot(string& filename) = 0;
	// スクリーンショットテクスチャの解放
	virtual void ReleaseScreenshot(void) = 0;
};

#endif // _WRAPPER_H