//-----------------------------------------------------------------------------
//
//	assimp[LoadAssimp.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _LOAD_MODEL_H_
#define _LOAD_MODEL_H_

#include "../../../Define/Define.h"
#include "Model.h"

class LoadM
{
public:
	MODEL   Load(string fileName);
	HRESULT LoadAnimation(string fileName, MODEL& model);

private:
	void GetMesh(FILE* fp, MODEL& model);
	void GetBone(FILE* fp, MODEL& model);

};

#endif // _LOAD_MODEL_H_
