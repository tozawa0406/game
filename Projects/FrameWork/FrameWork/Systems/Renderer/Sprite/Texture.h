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
		UNOWN = -1,

		WHITE = 0,
		NUMBER,
		SSUI,
		LOAD,

		MAX
	};

	enum class Title : int
	{
		MAX = (int)Base::MAX
	};

	enum class Camp
	{
		FIELD = (int)Base::MAX,
		SLASH,
		EFFECT,

		HP_BAR,
		ITEM_UI,

		TIMER_UI,

		CLEAR_FAILED_UI,

		MAX
	};

	enum class Buttle
	{
		FIELD = (int)Base::MAX,
		SLASH,
		EFFECT,

		HP_BAR,
		ITEM_UI,

		TIMER_UI,

		CLEAR_FAILED_UI,

		MAX
	};

	enum class Result
	{
		MAX = (int)Base::MAX
	};

private:
	const string baseFileName[(int)Base::MAX]
	{
		{ directoryName + "white.jpg"					},
		{ directoryName + "UI/number.png"				},
		{ directoryName + "UI/ssUI.jpg"					},
		{ directoryName + "UI/load.png"					},
	};

	//const string titleFileName[(int)Title::MAX - (int)Base::MAX]
	//{		  
	//};

	const string campFileName[(int)Camp::MAX - (int)Base::MAX]
	{
		{ directoryName + "field.jpg"					},
		{ directoryName + "slash.jpg"					},
		{ directoryName + "effect.jpg"					},

		{ directoryName + "UI/game/hpbar.png"			},
		{ directoryName + "UI/game/itemlist.png"		},

		{ directoryName + "UI/timer.png"				},
		{ directoryName + "UI/clear_failed.png"			},
	};

	const string buttleFileName[(int)Buttle::MAX - (int)Base::MAX]
	{
		{ directoryName + "field.jpg"					},
		{ directoryName + "slash.jpg"					},
		{ directoryName + "effect.jpg"					},

		{ directoryName + "UI/game/hpbar.png"			},
		{ directoryName + "UI/game/itemlist.png"		},

		{ directoryName + "UI/timer.png"				},
		{ directoryName + "UI/clear_failed.png"			},
	};

	//const string resultFileName[(int)Result::MAX - (int)Base::MAX]
	//{
	//};


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