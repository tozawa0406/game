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
		EXPLOSION,
		SLASH,
		EFFECT,

		HP_BAR,
		ITEM_UI,

		TIMER_UI,

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
		{ directoryName + "white.jpg"					},
		{ directoryName + "UI/number.png"				},
		{ directoryName + "UI/ssUI.jpg"					},
		{ directoryName + "UI/load.png"					},
	};

	const string titleFileName[(int)Title::MAX - (int)Base::MAX]
	{
		{ directoryName + "UI/please_press/please_press.png" },
		{ directoryName + "UI/please_press/A.png"		},
		{ directoryName + "UI/please_press/B.png"		},
		{ directoryName + "UI/please_press/enter.png"	},
		{ directoryName + "UI/please_press/maru.png"	},
		  
		{ directoryName + "UI/title_rogo.png"			},
	};

	const string gameFileName[(int)Game::MAX - (int)Base::MAX]
	{
		{ directoryName + "field.jpg"					},
		{ directoryName + "explosion/explosion.png"		},
		{ directoryName + "slash.jpg"					},
		{ directoryName + "effect.jpg"					},

		{ directoryName + "UI/game/hpbar.png"			},
		{ directoryName + "UI/game/itemlist.png"		},

		{ directoryName + "UI/timer.png"				},

		{ directoryName + "UI/menuUI.png"				},
	};

	const string resultFileName[(int)Result::MAX - (int)Base::MAX]
	{
		{ directoryName + "UI/please_press/please_press.png" },
		{ directoryName + "UI/please_press/A.png"		},
		{ directoryName + "UI/please_press/B.png"		},
		{ directoryName + "UI/please_press/enter.png"	},
		{ directoryName + "UI/please_press/maru.png"	},
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