//-----------------------------------------------------------------------------
//
//	DirectX11の処理[Dx11Wrapper.h]
//	Auther : 戸澤翔太
//
//-----------------------------------------------------------------------------
#ifndef _DIREXT_X11_WRAPPER_H_
#define _DIRECT_X11_WRAPPER_H_

#include "../Wrapper.h"
#include "DirectX11.h"
#include "Dx11Utility/Dx11Font.h"

#include "../../Systems/Renderer/Model/Model.h"
#include "../../Systems/Renderer/Model/LoadModel.h"
#include "../../Systems/Renderer/Shader/Shader.h"

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class Dx11Wrapper : public Wrapper
{
	friend DirectX11;
public:
	uint	CreateVertexBuffer(const void* v, uint size, uint vnum)	override;
	uint	CreateIndexBuffer(const WORD* v, uint vnum)				override;
	void	ReleaseBuffer(uint number, Wrapper::FVF fvf)			override;

	void    BeginDrawCanvasRenderer(void) override;	
	void    BeginDrawObjectRenderer(void) override;
	void    EndDrawRenderer(void)   override;

	void    SetTexture(int stage, int texNum = -1, int modelNum = -1) override;
	void    Draw(const CanvasRenderer::Image* obj, const Shader* shader)	override;
	void    Draw(const SpriteRenderer* obj, const Shader* shader)	override;
	void    Draw(MeshRenderer*   obj, const Shader* shader)			override;
	void	Draw(const Particle*	   obj, const Shader* shader)	override;
	void    Draw(const ColliderRenderer* obj) override;

	HRESULT LoadTexture(string fileName, int texNum, int modelNum = -1) override;
	void    ReleaseTexture(int texNum, int modelNum = -1) override;
	VECTOR2 GetTextureSize(int texNum)					  override { return texture_[0][texNum].size; }

	HRESULT LoadModel(string fileName, int modelNum)			override;
	HRESULT LoadModelAnimation(string fileName, int parent)		override;
	void    ReleaseModel(int modelNum)							override;

	uint	CreateVertexShader(string fileName, string method, string version, void* t, uint elemNum) override;
	HRESULT	SetVertexShader(uint number)	 override {	UNREFERENCED_PARAMETER(number); return S_OK; }
	void	ReleaseVertesShader(uint number) override { UNREFERENCED_PARAMETER(number); }

	uint	CreatePixelShader(string fileName, string method, string version) override;
	HRESULT	SetPixelShader(uint number)		override { UNREFERENCED_PARAMETER(number); return S_OK; }
	void	ReleasePixelShader(uint number) override { UNREFERENCED_PARAMETER(number); }

	uint    CreateGeometryShader(string fileName, string method, string version);
	uint	CreateComputeShader(string fileName, string method, string version, const void* v, uint size, uint num);
	uint	CreateConstantBuffer(uint size);

	ID3D11DeviceContext* GetContext(void) { return directX11_->GetDeviceContext(); }
	ID3D11Buffer*        GetConstantBuffer(uint num) { return constantBuffer_[num]; }

	MODEL&	GetModel(int i) { return model_[i]; }

	// ライトの設定
	void SetLight(LightInfo& light) override { UNREFERENCED_PARAMETER(light); }

	void DrawQuad(VECTOR2 position, VECTOR2 size, COLOR color = COLOR(1, 1, 1, 1)) override;

private:
	ID3DBlob*	CompiledShader(string fileName, string method, string version);
	long		ReadShader(string csoName, byte** b);

	// DirectX11のテクスチャ情報
	struct Dx11Texture : public TextureData
	{
		ID3D11ShaderResourceView* data;
	};

	struct VertexBuffer
	{
		ID3D11Buffer* buffer;
		uint stride;
		uint offset;
	};
	// DirectX11の頂点シェーダー
	struct VertexShader
	{
		ID3D11VertexShader* shader;
		ID3D11InputLayout*  layout;
	};
	// DirectX11のピクセルシェーダー
	struct PixelShader
	{
		ID3D11PixelShader*	shader;
		ID3D11SamplerState* sampler;			// テクスチャーサンプラー
	};
	struct ComputeShader
	{
		ID3D11ComputeShader*		shader;
		ID3D11Buffer*				buffer;
		ID3D11ShaderResourceView*	shaderResource;
		ID3D11UnorderedAccessView*	unorderedAcces;

	};

	struct ShaderData
	{
		std::vector<uint> vertexShader;
		std::vector<uint> pixelShader;
		std::vector<uint> geometryShader_;
		std::vector<uint> constantBuffer;
	};

	Dx11Wrapper(DirectX11* directX);
	~Dx11Wrapper(void) {};

	HRESULT Init(void)   override;
	void	Uninit(void) override;

	uint InsideBuffer(void);
	D3D11_PRIMITIVE_TOPOLOGY SelectPrimitiveType(PRIMITIVE::TYPE type);

	DirectX11* directX11_;

	std::vector<VertexBuffer>				vertexBuffer_;
	std::vector<ID3D11Buffer*>				indexBuffer_;
	std::vector<std::vector<Dx11Texture>>	texture_;
	std::vector<MODEL>						model_;

	ID3D11BlendState*          blendState_[(int)ALFA_BREND::MAX];
	ID3D11DepthStencilState*   depthState_;
	ID3D11RasterizerState*     rasterizerState_;

	std::vector<VertexShader>			vertexShader_;
	std::vector<PixelShader>			pixelShader_;
	std::vector<ID3D11GeometryShader*>	geometryShader_;
	std::vector<ComputeShader>			computeShader_;
	std::vector<ID3D11Buffer*>			constantBuffer_;		// アプリ←→シェーダー架け橋　ワールドから射影までの変換行列を渡すためのコンスタントバッファー

	ShaderData shader_[2];

public:
	ComputeShader& GetComputeShader(int i) { return computeShader_[i]; }

};

inline XMFLOAT4X4  XM4(const MATRIX& m)
{
	XMFLOAT4X4 temp;
	for (int i = 0; i < 4; ++i) { for (int j = 0; j < 4; ++j) { temp.m[i][j] = m.m[i][j]; } }
	return temp;
}

inline XMMATRIX    XM(const MATRIX&  m)
{
	XMMATRIX temp;
	for (int i = 0; i < 4; ++i) { for (int j = 0; j < 4; ++j) { temp.m[i][j] = m.m[i][j]; } }
	return temp;
}
inline XMVECTORF32 XM(const VECTOR3& v) { XMVECTORF32 temp = { v.x, v.y, v.z, 1 }; return temp; }

inline MATRIX  V(const XMMATRIX&    m)
{
	MATRIX temp;
	for (int i = 0; i < 4; ++i) { for (int j = 0; j < 4; ++j) { temp.m[i][j] = m.m[i][j]; } }
	return temp;
}
inline VECTOR3 V(const XMVECTORF32& v) { return VECTOR3(v.f[0], v.f[1], v.f[2]); }


#endif // _DIRECT_X11_WRAPPER_H