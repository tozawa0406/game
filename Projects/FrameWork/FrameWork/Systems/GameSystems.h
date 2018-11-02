//-----------------------------------------------------------------------------
//
//	システム系[GameSystems.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _SYSTEMS_H_
#define _SYSTEMS_H_

#include "../Define/Define.h"

#include "DebugSetting/Debug.h"
#include "Input/Input.h"
#include "Renderer/Sprite/Texture.h"
#include "Renderer/ObjectRendererManager.h"
#include "Renderer/Sprite/CanvasRendererManager.h"
#include "Renderer/Model/Model.h"
#include "Renderer/Shader/ShaderManager.h"
#include "Sound/Sound.h"

#include "Collider/Collider2DManager.h"
#include "Collider/Collider3DManager.h"
#include "Collider/ColliderRenderer.h"

#include "Particle/ParticleManager.h"

#include "BaseManager.h"

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class Graphics;
class SceneManager;
class Systems
{
	enum class SystemsNum
	{
		DEBUG = 0,
		INPUT,
		COLLISIONDRAW,
		PARTICLE,
		OBJECTRENDERER,
		CANVASRENDERER, 
		MODEL,
		TEXTURE,
		SHADER,
		SOUND,
		COLLISION2D,
		COLLISION3D,
		SCENE,
		SCREENSHOT,
		MAX
	};

	// Systemsクラスは他クラスからの参照が多いため
	// Init等の機能を他クラスから参照させないため
	friend class Windows;
	static Systems* system_;
public:
	static Systems* Instance(void)		{ return system_;   }
	Graphics*       GetRenderer(void)	{ return graphics_; }
	Windows*        GetWindow(void)		{ return window_;   }

	SceneManager*			 GetSceneManager(void)				{ return (SceneManager*)			manager_[(int)SystemsNum::SCENE];          }
	Debug*					 GetDebug(void)						{ return (Debug*)					manager_[(int)SystemsNum::DEBUG];          }
	Input*					 GetInput(void)						{ return (Input*)					manager_[(int)SystemsNum::INPUT];          }
	Texture*				 GetTexture(void)			const	{ return (Texture*)					manager_[(int)SystemsNum::TEXTURE];        }
	CanvasRendererManager*	 GetCanvasRenderer(void)			{ return (CanvasRendererManager*)	manager_[(int)SystemsNum::CANVASRENDERER]; }
	ObjectRendererManager*	 GetObjectRenderer(void)			{ return (ObjectRendererManager*)	manager_[(int)SystemsNum::OBJECTRENDERER]; }
	Model*					 GetModel(void)				const	{ return (Model*)					manager_[(int)SystemsNum::MODEL];		   }
	ShaderManager*			 GetShader(void)					{ return (ShaderManager*)			manager_[(int)SystemsNum::SHADER];		   }
	Sound*					 GetSound(void)				const	{ return (Sound*)					manager_[(int)SystemsNum::SOUND];          }
	Collision2DManager*		 GetCollision2D(void)				{ return (Collision2DManager*)		manager_[(int)SystemsNum::COLLISION2D];    }
	Collision3DManager*		 GetCollision3D(void)				{ return (Collision3DManager*)		manager_[(int)SystemsNum::COLLISION3D];    }
	ColliderRendererManager* GetColliderRendererManager(void)	{ return (ColliderRendererManager*) manager_[(int)SystemsNum::COLLISIONDRAW];  }
	ParticleManager*		 GetParticleManager(void)			{ return (ParticleManager*)			manager_[(int)SystemsNum::PARTICLE];	   }

private:
	// 自分自身が使う
	template<class T>
	bool Create(int i, string error);

	// Windowsクラスのみが使用
	Systems(Windows* window);
	HRESULT Init(void);
	void    Uninit(void);
	void    Update(void);
	void    Draw(void);

	Graphics*  graphics_;
	Windows*   window_;
	// インスタンス
	Interface* manager_[(int)SystemsNum::MAX];
};

#endif // _SYSTEMS_H_
