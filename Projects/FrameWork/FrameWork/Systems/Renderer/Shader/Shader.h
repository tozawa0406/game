//-----------------------------------------------------------------------------
//
//	シェーダー[Shader.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _SHADER_H_
#define _SHADER_H_

#include "../../../Define/Define.h"
#include "../../../Graphics/Graphics.h"
#include "../../../Graphics/Wrapper.h"

static const string shaderDirectoryName = Define::ResourceDirectoryName + "Data/";

class Shader
{
	friend class ShaderManager;
public:
	enum class ENUM
	{
		UNKOUWN = -1,
		DEFAULT = 0,
		ZTEXTURE,
		DEPTH_SHADOW,
		BILLBOARD,
		POINT_SPRITE,
		CIRCLE_GAUGE,
		CS_SKINNING,
		SKINNING_SHADOW,
		MAX
	};

	ShaderManager*	GetManager(void)		const { return manager_;		}
	uint			GetVertexShader(void)	const { return vertexShader_;   }
	uint			GetPixelShader(void)	const { return pixelShader_;	}
	uint			GetConstantBuffer(int i)	const { return (i < (int)constantBuffer_.size()) ? constantBuffer_[i] : constantBuffer_[0]; }
	string			GetVMethod(void)		const { return vMethod_;	    }

	void SetVMethod(string methodName)	{ vMethod_  = methodName;	}
	void SetVVersion(string version)	{ vVersion_ = version;		}
	void SetPMethod(string methodName)	{ pMethod_  = methodName;	}
	void SetPVersion(string version)	{ pVersion_ = version;		}
	void SetTech(string tech)			{ techName_ = tech;			}
	void SetConstantBuffer(int i)       { constantBuffer_.emplace_back(i); }

	virtual HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const = 0;

	virtual ~Shader(void) { if (dev_) { dev_->ReleaseVertesShader(vertexShader_); dev_->ReleasePixelShader(pixelShader_); } }

protected:
	Shader(ShaderManager* manager, string fileName) : manager_(manager), fileName_(fileName), vMethod_("VS_Main"), vVersion_("vs_2_0")
													, pMethod_("PS_Main"), pVersion_("ps_2_0"), techName_("Tech"), dev_(nullptr)
													, layout_(nullptr), layoutSize_(0), constantBuffer_(0) {}

	uint				vertexShader_;
	uint				pixelShader_;
	std::vector<uint>	constantBuffer_;

	string			fileName_;
	string			vMethod_;
	string			vVersion_;
	string			pMethod_;
	string			pVersion_;

	string			techName_;
	ShaderManager*	manager_;
	Wrapper*		dev_;

	void*			layout_;
	uint			layoutSize_;

	virtual HRESULT Init(void);
};

#endif // _SHADER_H_