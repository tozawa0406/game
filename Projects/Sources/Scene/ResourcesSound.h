/*
 * @file		ResourcesSound.h
 * @brief		リソース管理ファイル
 * @author		戸澤翔太
 * @data		2018/01/17
 */
#ifndef _RESOURCES_SOUND_H_
#define _RESOURCES_SOUND_H_

#include <FrameWork/Define/Define.h>

struct SOUNDPARAM
{
	string pFilename;	// ファイル名
	int cntLoop;		// ループカウント
};

namespace Resources
{
	class Sound
	{
		const string directoryName = Define::ResourceDirectoryName + "Sound/";
		const string BGMDirectoryName = directoryName + "BGM/";
		const string SEDirectoryName = directoryName + "SE/";
	public:
		enum class Base : int8
		{
			UNOWN = -1,
			SE_SELECT = 0,
			SE_ENTER,
			SE_CANCEL,

			MAX
		};

		enum class Title : int8
		{
			BGM_TITLE = (int)Base::MAX,

			MAX
		};

		enum class Camp : int8
		{
			BGM_GAME = (int)Base::MAX,

			UNITYCHAN_WALK,
			UNITYCHAN_DRINK,
			UNITYCHAN_DRAWN,
			UNITYCHAN_ATTACK01,
			UNITYCHAN_ATTACK02,
			UNITYCHAN_ATTACK03,
			UNITYCHAN_HIT,
			UNITYCHAN_HEAL,
			UNITYCHAN_BREATH,
			WAPON_ATTACK,

			MAX
		};

		enum class Buttle : int8
		{
			BGM_GAME = (int)Base::MAX,

			UNITYCHAN_WALK,
			UNITYCHAN_DRINK,
			UNITYCHAN_DRAWN,
			UNITYCHAN_ATTACK01,
			UNITYCHAN_ATTACK02,
			UNITYCHAN_ATTACK03,
			UNITYCHAN_HIT,
			UNITYCHAN_HEAL,
			UNITYCHAN_BREATH,
			WAPON_ATTACK,
			WAPON_HIT,

			DRAGON_ROAR,

			MAX
		};

		enum class Result : int8
		{
			BGM_RESULT = (int)Base::MAX,

			MAX
		};

		const SOUNDPARAM baseFileName[(int)Base::MAX]
		{
			{ SEDirectoryName + "select.wav"	, 0 },
			{ SEDirectoryName + "enter.wav"		, 0 },
			{ SEDirectoryName + "cancel.wav"	, 0 },
		};
		const SOUNDPARAM titleFileName[(int)Title::MAX - (int)Base::MAX]
		{
			{ BGMDirectoryName + "Title/bgm_maoudamashii_fantasy08.wav", 255 },
		};
		const SOUNDPARAM campFileName[(int)Camp::MAX - (int)Base::MAX]
		{
			{ BGMDirectoryName + "Game/bgm_maoudamashii_orchestra24.wav"	, 255 },

			{ SEDirectoryName + "unitychan/walk.wav"						,  0 },
			{ SEDirectoryName + "unitychan/drink.wav"						,  0 },
			{ SEDirectoryName + "unitychan/drawn.wav"						,  0 },
			{ SEDirectoryName + "unitychan/attack01.wav"					,  0 },
			{ SEDirectoryName + "unitychan/attack02.wav"					,  0 },
			{ SEDirectoryName + "unitychan/attack03.wav"					,  0 },
			{ SEDirectoryName + "unitychan/hit.wav"							,  0 },
			{ SEDirectoryName + "unitychan/heal.wav"						,  0 },
			{ SEDirectoryName + "unitychan/breath.wav"						,  0 },
			{ SEDirectoryName + "unitychan/attack.wav"						,  0 },
		};
		const SOUNDPARAM buttleFileName[(int)Buttle::MAX - (int)Base::MAX]
		{
			{ BGMDirectoryName + "Game/bgm_maoudamashii_orchestra24.wav"	, 255 },

			{ SEDirectoryName + "unitychan/walk.wav"						,  0 },
			{ SEDirectoryName + "unitychan/drink.wav"						,  0 },
			{ SEDirectoryName + "unitychan/drawn.wav"						,  0 },
			{ SEDirectoryName + "unitychan/attack01.wav"					,  0 },
			{ SEDirectoryName + "unitychan/attack02.wav"					,  0 },
			{ SEDirectoryName + "unitychan/attack03.wav"					,  0 },
			{ SEDirectoryName + "unitychan/hit.wav"							,  0 },
			{ SEDirectoryName + "unitychan/heal.wav"						,  0 },
			{ SEDirectoryName + "unitychan/breath.wav"						,  0 },
			{ SEDirectoryName + "unitychan/attack.wav"						,  0 },
			{ SEDirectoryName + "unitychan/attackHit.wav"					,  0 },

			{ SEDirectoryName + "monster1.wav"								,  0 },
		};
		const SOUNDPARAM resultFileName[(int)Result::MAX - (int)Base::MAX]
		{
			{ BGMDirectoryName + "Result/bgm_maoudamashii_orchestra14.wav"	, 255 },
		};
	};
}

#endif // _RESOURCES_SOUND_H_
