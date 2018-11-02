//-----------------------------------------------------------------------------
//
//	ビルボードシェーダー[Billboard.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "BillboardShader.h"
#include "ShaderManager.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"
#include "../../../Scene/SceneManager.h"
#include "../../Camera/CameraManager.h"

HRESULT BillboardShader::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(mtx);
	UNREFERENCED_PARAMETER(color);
	UNREFERENCED_PARAMETER(texcoord);

	return S_OK;
}
