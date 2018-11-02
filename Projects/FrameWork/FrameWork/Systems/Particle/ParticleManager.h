//-----------------------------------------------------------------------------
//
//	パーティクル管理[ParticleManager.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _PARTICLE_MANAGER_H_ 
#define _PARTICLE_MANAGER_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"

class Systems;
class Particle;
class ParticleManager : public BaseManager<Particle>
{
public:
	ParticleManager(Systems* systems) : BaseManager(systems) {}
	HRESULT Init(void)   override { return S_OK; }
	void	Uninit(void) override;		// 後処理
	void	Update(void) override;		// 更新処理
	void	Draw(void)   override;		// 描画処理

private:
	void Sort(void);
	void DestroyCheck(void);

	friend Particle;
};

#endif // _OBJECT_H_