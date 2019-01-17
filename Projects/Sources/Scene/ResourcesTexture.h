/*
 * @file		ResourcesTexture.h
 * @brief		リソース管理ファイル
 * @author		戸澤翔太
 * @data		2018/01/17
 */
#ifndef _RESOURCES_TEXTURE_H_
#define _RESOURCES_TEXTURE_H_

#include <FrameWork/Define/Define.h>

namespace Resources
{
	class Texture
	{
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

		const string baseFileName[(int)Base::MAX]
		{
			{ directoryName + "white.jpg"				},
			{ directoryName + "UI/number.png"			},
			{ directoryName + "UI/ssUI.jpg"				},
			{ directoryName + "UI/load.png"				},
		};

		//const string titleFileName[(int)Title::MAX - (int)Base::MAX]
		//{		  
		//};

		const string campFileName[(int)Camp::MAX - (int)Base::MAX]
		{
			{ directoryName + "field.jpg"				},
			{ directoryName + "slash.jpg"				},
			{ directoryName + "effect.jpg"				},

			{ directoryName + "UI/game/hpbar.png"		},
			{ directoryName + "UI/game/itemlist.png"	},

			{ directoryName + "UI/timer.png"			},
		};

		const string buttleFileName[(int)Buttle::MAX - (int)Base::MAX]
		{
			{ directoryName + "field.jpg"				},
			{ directoryName + "slash.jpg"				},
			{ directoryName + "effect.jpg"				},

			{ directoryName + "UI/game/hpbar.png"		},
			{ directoryName + "UI/game/itemlist.png"	},

			{ directoryName + "UI/timer.png"			},
			{ directoryName + "UI/clear_failed.png"		},
		};

		//const string resultFileName[(int)Result::MAX - (int)Base::MAX]
		//{
		//};
	};
}

#endif // _RESOURCES_TEXTURE_H_
