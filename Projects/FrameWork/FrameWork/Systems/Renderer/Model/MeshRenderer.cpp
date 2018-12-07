//-----------------------------------------------------------------------------
//
//	モデル描画[MeshRenderer.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "MeshRenderer.h"
#include "../../../Systems/GameSystems.h"
#include "../../../Graphics/Graphics.h"
#include "../../../Graphics/Wrapper.h"

#include "../../../Graphics/DirectX11/DirectX11Wrapper.h"
#include "../Shader/Default.h"
#include "../Shader/ZTexture.h"
#include "../../Camera/CameraManager.h"

MeshRenderer::MeshRenderer(void) : ObjectRenderer(ObjectRenderer::RendererType::MODEL)
								 , modelNum_((int)Model::Base::UNOWN), pattern_(0), patternOld_(0), animation_(0), animationOld_(-1)
								 , rate_(0), addRate_(0), isSkinning_(false)
{
}

MeshRenderer::~MeshRenderer(void)
{
}

void MeshRenderer::Init(Systems* systems, int modelNum, const Transform* transform)
{
	ObjectRenderer::Init(systems, transform);

	modelNum_ = modelNum;
	shader = Shader::ENUM::DEFAULT;

	const auto& model = ((DirectX11Wrapper*)systems->GetRenderer()->GetWrapper())->GetModel(modelNum);
	animationMax_.clear();
	if(model.bone.size() > 0)
	{
		for (const auto& anim : model.bone[0].animMtx)
		{
			animationMax_.emplace_back((int)anim.size());
		}
	}
}

void MeshRenderer::ChangeModel(int modelNum)
{
	modelNum_ = modelNum;
}

void MeshRenderer::ChangeAnimation(int animNum, int frame, bool end)
{
	if (animation_ != animNum)
	{
		end_ = end;
		patternOld_		= pattern_;
		animationOld_	= animation_;
		rate_			= 0;
		addRate_		= 1.0f / frame;
		animation_		= animNum;
		pattern_		= 0;
	}
}

void MeshRenderer::AnimEndPattern(void)
{
	pattern_ = static_cast<float>(animationMax_[animation_] - 1);
}

bool MeshRenderer::Animation(float add)
{
	isSkinning_ = false;
	if (animationMax_.size() <= (UINT)animation_) { return false; }

	if (animationOld_ >= 0)
	{
		rate_		+= addRate_;
		if (!end_)
		{
			patternOld_ += add;
		}
		if (rate_ >= 1)
		{
			animationOld_	= -1;
			rate_			= 0;
			addRate_		= 0;
			patternOld_		= 0;
		}
	}

	pattern_ += add;
	int m = (add < 0) ? 1 : 0;
	if(fabs(pattern_) >= animationMax_[animation_] - ((4 * m) + 1))
	{
		patternOld_ = pattern_ - add;
		pattern_ = 0;
		return true;
	}
	return false;
}

void MeshRenderer::Skinning(void)
{
//	if (isSkinning_) { return; }
	isSkinning_ = true;

	DirectX11Wrapper* dx11 = ((DirectX11Wrapper*)systems_->GetRenderer()->GetWrapper());
	const auto& context = dx11->GetContext();
	auto& model = dx11->GetModel(modelNum_);

	MATRIX rootMtx;
	bool rootFrameTransformMatrix = true;
	if (model.transMtx == 0.0f) { rootFrameTransformMatrix = false; }
	else { rootMtx = model.transMtx; }

	DefaultShader::CONSTANT cbuf;
	MATRIX initMtx = MATRIX().Identity();
	memcpy_s(&cbuf.view , sizeof(MATRIX), &systems_->GetSceneManager()->GetCameraManager()->GetView(), sizeof(MATRIX));
	memcpy_s(&cbuf.proj , sizeof(MATRIX), &systems_->GetSceneManager()->GetCameraManager()->GetProj(), sizeof(MATRIX));

	cbuf.texcoord = VECTOR4(0, 0, 1, 1);
	memcpy_s(&cbuf.diffuse, sizeof(float) * 4, &material.diffuse, sizeof(float) * 4);

	const ZTexture* depth = (ZTexture*)systems_->GetShader()->GetShader(Shader::ENUM::ZTEXTURE);
	cbuf.lightView = depth->GetLightView();
	cbuf.lightProj = depth->GetLightProj();

	DefaultShader* defShader = ((DefaultShader*)systems_->GetShader()->GetShader(Shader::ENUM::DEFAULT));
	const auto& constant = dx11->GetConstantBuffer(defShader->GetConstantBuffer(0));

	const auto& constant1 = dx11->GetConstantBuffer(defShader->GetConstantBuffer(1));

	DefaultShader::CONSTANT_BONE cbuf1;
	ZeroMemory(&cbuf1, sizeof(DefaultShader::CONSTANT_BONE));

	int s = (int)model.bone.size();
	for (int j = 0; j < s; ++j)
	{
		memcpy_s(&cbuf1.boneInv[j], sizeof(MATRIX), &model.bone[j].inverseMtx, sizeof(MATRIX));

		int anim = (int)pattern_;
		if (model.bone[j].animMtx.size() <= (UINT)animation_ || model.bone[j].animMtx[animation_].size() == 0)
		{
			memcpy_s(&cbuf1.boneAnim[j], sizeof(MATRIX), &model.bone[j].offsetMtx, sizeof(MATRIX));
			memcpy_s(&model.bone[j].nowBone, sizeof(MATRIX), &model.bone[j].offsetMtx, sizeof(MATRIX));
		}
		else
		{
			float rate = rate_;
			anim = (anim % model.bone[j].animMtx[animation_].size());
			if (animationOld_ < 0)
			{
				float patternRate = pattern_ - (int)pattern_;
				MATRIX mtx1, mtx2;
				memcpy_s(&mtx1, sizeof(MATRIX), &model.bone[j].animMtx[animation_][anim], sizeof(MATRIX));

				int next = anim + 1;
				next = (next >= animationMax_[animation_]) ? 0 : next;
				memcpy_s(&mtx2, sizeof(MATRIX), &model.bone[j].animMtx[animation_][next], sizeof(MATRIX));

				MATRIX m = (mtx2 * patternRate) + (mtx1 * (1 - patternRate));
				memcpy_s(&cbuf1.boneAnim[j], sizeof(MATRIX), &m, sizeof(MATRIX));
				memcpy_s(&model.bone[j].nowBone, sizeof(MATRIX), &m, sizeof(MATRIX));
			}
			else
			{
				MATRIX mtx1, mtx2;
				if (model.bone[j].animMtx.size() >(UINT)animationOld_ || model.bone[j].animMtx[animationOld_].size() > 0)
				{
					memcpy_s(&mtx1, sizeof(MATRIX), &model.bone[j].animMtx[animation_][anim], sizeof(MATRIX));
					anim = (int)patternOld_;
					anim = (anim % model.bone[j].animMtx[animationOld_].size());
					memcpy_s(&mtx2, sizeof(MATRIX), &model.bone[j].animMtx[animationOld_][anim], sizeof(MATRIX));

					MATRIX m = (mtx1 * rate) + (mtx2 * (1 - rate));
					memcpy_s(&cbuf1.boneAnim[j], sizeof(MATRIX), &m, sizeof(MATRIX));
					memcpy_s(&model.bone[j].nowBone, sizeof(MATRIX), &m, sizeof(MATRIX));
				}
			}
		}
	}
	context->UpdateSubresource(constant1, 0, NULL, &cbuf1, 0, 0);

	context->VSSetConstantBuffers(1, 1, &constant1);


	for (auto& mesh : model.mesh)
	{
		if (mesh.computeShader != Wrapper::R_ERROR)
		{
//			const auto& cs = dx11->GetComputeShader(mesh.computeShader);

			MATRIX mtx = mtx.Identity();
			// モデル行列
			if (rootFrameTransformMatrix) { mtx *= rootMtx; }
			if (mesh.transMtx != 0) { mtx *= mesh.transMtx; }
			// ワールド行列
			mtx.Create(transform_);

			memcpy_s(&cbuf.world, sizeof(MATRIX), &mtx, sizeof(MATRIX));

			context->UpdateSubresource(constant, 0, NULL, &cbuf, 0, 0);

			//context->CSSetShader(cs.shader, NULL, 1);
			//context->CSSetConstantBuffers(0, 1, &constant);
			//context->CSSetConstantBuffers(1, 1, &constant1);
			//context->CSSetShaderResources(0, 1, &cs.shaderResource);
			//context->CSSetUnorderedAccessViews(0, 1, &cs.unorderedAcces, nullptr);

			//context->Dispatch(mesh.vertex.size(), 1, 1);

			//ID3D11ShaderResourceView*  view   = nullptr;
			//ID3D11UnorderedAccessView* access = nullptr;
			//ID3D11Buffer* cons = nullptr;
			//context->CSSetShader(NULL, NULL, 1);
			//context->CSSetShaderResources(0, 1, &view);
			//context->CSSetUnorderedAccessViews(0, 1, &access, nullptr);
			//context->CSSetConstantBuffers(0, 1, &cons);
			//context->CSSetConstantBuffers(1, 1, &cons);
		}
	}
}
