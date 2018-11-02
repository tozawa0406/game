//-----------------------------------------------------------------------------
//
//	テクスチャ管理[Texture.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"

class Loading;
class Texture : public Interface
{
	friend Systems;

	const string directoryName = Define::ResourceDirectoryName + "Texture/";
public:
	enum class Base : int
	{
		POINT = -7,
		LINE,
		RECTANGLE,
		FILL_TRIANGLE,
		FILL_RECTANGLE,
		FILL_CIRCLE,

		UNOWN = -1,

		WHITE = 0,
		NUMBER,
		SSUI,
		LOAD,

		MAX
	};

	enum class Title : int
	{
		PLEASE_PRESS = (int)Base::MAX,
		A,
		B,
		ENTER,
		MARU,

		TITLE,

		MAX
	};

	enum class Game
	{
		FIELD = (int)Base::MAX,
		TREE,
		EXPLOSION,
		EFFECT,
		SHADOW,
		BULLET,
		TARGET_MAKER,

		ALEAT,
		BULLET_UI,
		MISSILE_UI,

		TIMER_UI,

		COUNT,
		GO,
		READY,
		ISSUE,
		MENU,

		MAX

	};

	enum class Result
	{
		PLEASE_PRESS = (int)Base::MAX,
		A,
		B,
		ENTER,
		MARU,

		MAX
	};

private:
	const string baseFileName[(int)Base::MAX]
	{
		{ directoryName + "white.jpg"		},
		{ directoryName + "UI/number.png"	},
		{ directoryName + "UI/ssUI.jpg"		},
		{ directoryName + "UI/load.png"		},
	};

	const string titleFileName[(int)Title::MAX - (int)Base::MAX]
	{
		{ directoryName + "UI/please_press/please_press.png" },
		{ directoryName + "UI/please_press/A.png" },
		{ directoryName + "UI/please_press/B.png" },
		{ directoryName + "UI/please_press/enter.png" },
		{ directoryName + "UI/please_press/maru.png" },
		  
		{ directoryName + "UI/title_rogo.png" },
	};

	const string gameFileName[(int)Game::MAX - (int)Base::MAX]
	{
		{ directoryName + "field.jpg" },
		{ directoryName + "billboard/tree.png" },
		{ directoryName + "explosion/explosion.png" },
		{ directoryName + "effect.jpg" },
		{ directoryName + "shadow.png" },
		{ directoryName + "billboard/bullet.png" },
		{ directoryName + "billboard/target_marker.png" },

		{ directoryName + "UI/aleat.png" },
		{ directoryName + "UI/UI.png" },
		{ directoryName + "UI/UI2.png" },
		{ directoryName + "UI/timer.png" },

		{ directoryName + "UI/startUI/321.png" },
		{ directoryName + "UI/startUI/GO.png" },
		{ directoryName + "UI/startUI/ready.png" },
		{ directoryName + "UI/issue.png" },
		{ directoryName + "UI/menuUI.png" },
	};

	const string resultFileName[(int)Result::MAX - (int)Base::MAX]
	{
		{ directoryName + "UI/please_press/please_press.png" },
		{ directoryName + "UI/please_press/A.png" },
		{ directoryName + "UI/please_press/B.png" },
		{ directoryName + "UI/please_press/enter.png" },
		{ directoryName + "UI/please_press/maru.png" },
	};


public:
	~Texture(void) {};

	int		SetUpLoading(Loading* loading, int sceneNum);
	HRESULT Load(int sceneNum);
	void	Release(bool uninit = false);
	VECTOR2 GetTextureSize(int texNum) const;
	string  GetWhiteTextureFileName(void) { return baseFileName[0]; }

private:
	Texture(Systems* systems);
	HRESULT Init(void)	 override;
	void	Uninit(void) override;

	int sceneNum_;
	Loading* loading_;
};

#endif // _TEXTURE_H_