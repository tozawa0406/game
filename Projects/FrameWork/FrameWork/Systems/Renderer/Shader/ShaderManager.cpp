//-----------------------------------------------------------------------------
//
//	シェーダー管理[ShaderManager.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ShaderManager.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"

#include "Default.h"
#include "ZTexture.h"
#include "CascadeShadow.h"
#include "DepthShadow.h"
#include "BillboardShader.h"
#include "PointSprite.h"
#include "2DCircleGauge.h"
#include "CSSkinningRender.h"
#include "SkinningShadow.h"

ShaderManager::ShaderManager(Systems* systems) : Interface(systems)
{
	for (auto& shader : shader_)
	{
		shader = nullptr;
	}
}

ShaderManager::~ShaderManager(void)
{
	for (auto& shader : shader_)
	{
		DeletePtr(shader);
	}
}

HRESULT ShaderManager::Init(void)
{
	Windows* window = systems_->GetWindow();

	shader_[(int)Shader::ENUM::DEFAULT] = new DefaultShader(this);
	if (window->ErrorMessage("シェーダーの作成に失敗しました", "エラー", shader_[(int)Shader::ENUM::DEFAULT]->Init()))
	{ return E_FAIL; }

	shader_[(int)Shader::ENUM::ZTEXTURE] = new CascadeShadow(this);
	if (window->ErrorMessage("シェーダーの作成に失敗しました", "エラー", shader_[(int)Shader::ENUM::ZTEXTURE]->Init()))
	{ return E_FAIL; }

	shader_[(int)Shader::ENUM::DEPTH_SHADOW] = new DepthShadow(this);
	if (window->ErrorMessage("シェーダーの作成に失敗しました", "エラー", shader_[(int)Shader::ENUM::DEPTH_SHADOW]->Init()))
	{ return E_FAIL; }

	shader_[(int)Shader::ENUM::POINT_SPRITE] = new PointSpriteShader(this);
	if (window->ErrorMessage("シェーダーの作成に失敗しました", "エラー", shader_[(int)Shader::ENUM::POINT_SPRITE]->Init()))
	{ return E_FAIL; }

	shader_[(int)Shader::ENUM::BILLBOARD] = new BillboardShader(this);
	if (window->ErrorMessage("シェーダーの作成に失敗しました", "エラー", shader_[(int)Shader::ENUM::BILLBOARD]->Init()))
	{ return E_FAIL; }

	shader_[(int)Shader::ENUM::CIRCLE_GAUGE] = new CircleGaugeShader(this);
	if (window->ErrorMessage("シェーダーの作成に失敗しました", "エラー", shader_[(int)Shader::ENUM::CIRCLE_GAUGE]->Init()))
	{ return E_FAIL; }

	//shader_[(int)Shader::ENUM::CS_SKINNING] = new CSSkinningRenderShader(this);
	//if (window->ErrorMessage("シェーダーの作成に失敗しました", "エラー", shader_[(int)Shader::ENUM::CS_SKINNING]->Init()))
	//{ return E_FAIL; }
	//shader_[(int)Shader::ENUM::CS_SKINNING]->SetConstantBuffer(shader_[(int)Shader::ENUM::DEFAULT]->GetConstantBuffer(1));

	shader_[(int)Shader::ENUM::SKINNING_SHADOW] = new SkinningShadowShader(this);
	if (window->ErrorMessage("シェーダーの作成に失敗しました", "エラー", shader_[(int)Shader::ENUM::SKINNING_SHADOW]->Init()))
	{ return E_FAIL; }
	shader_[(int)Shader::ENUM::SKINNING_SHADOW]->SetConstantBuffer(shader_[(int)Shader::ENUM::DEFAULT]->GetConstantBuffer(1));


	return S_OK;
}
