//-----------------------------------------------------------------------------
//
//	スカイドーム[SkyDome.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _SKY_DOME_H_ 
#define _SKY_DOME_H_

#include "../../Define/Define.h"
#include "Model/MeshRenderer.h"
#include "../GameSystems.h"

class SkyDome
{
public:
	SkyDome(Systems* systems);

	void Update(void);

private:
	Systems* systems_;

	Transform    transform_;
	MeshRenderer mesh_;
};

#endif // _SKY_DOME_H_