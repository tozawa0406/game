/*
 * @file		ResourcesModel.h
 * @brief		リソース管理ファイル
 * @author		戸澤翔太
 * @data		2018/01/17
 */
#ifndef _RESOURCES_MODEL_H_
#define _RESOURCES_MODEL_H_

#include <FrameWork/Define/Define.h>

struct ANIMATION_INFO
{
	string	fileName;
	int		parent;
};

namespace Resources
{
	class Model
	{
		const string directoryName = Define::ResourceDirectoryName + "Model/";
	public:
		enum class Base : int8
		{
			UNOWN = -1,
			SKY,
			MAX,
		};

		enum class Title : int8
		{
			MAX = (int)Base::MAX,
		};

		enum class Camp : int8
		{
			CUBE = (int)Base::MAX,
			UNITY_CHAN,
			SWORD,
			BOX,
			ROCK_1,
			ROCK_4,
			ROCK_6,
			ROCK_10,
			ROCK_12,
			ROCK_14,
			CART,
			CANNON,
			CANNON_BARREL,
			CANNON_BULLET,
			TENT,
			MAX
		};

		enum class Buttle : int8
		{
			CUBE = (int)Base::MAX,
			UNITY_CHAN,
			SWORD,
			DRAGON,
			ROCK_1,
			ROCK_4,
			ROCK_6,
			ROCK_10,
			ROCK_12,
			ROCK_14,
			MAX
		};

		enum class Result : int8
		{
			MAX = (int)Base::MAX,
		};

		class Animation
		{
		public:
			enum class Base : int8
			{
				UNOWN = -1,
				MAX,
			};

			enum class Title : int8
			{
				MAX = (int)Base::MAX,
			};

			enum class Camp : int8
			{
				WAIT = (int)Base::MAX,
				WAIT_TIME1,
				WAIT_TIME2,
				WALK,
				RUN,
				ROLL,
				DIVE,
				KNOCK_BACK,
				KNOCK_OUT,
				DIE,
				EARPLUG,
				STANDUP,
				HEAL,
				WALK_SETUP,
				SETUP,
				SETUP_DRAWN,
				SETUP_WAIT,
				SETUP_RUN,

				SRASH_1,
				SRASH_2,
				SRASH_3,

				MAX
			};

			enum class Buttle : int8
			{
				WAIT = (int)Base::MAX,
				WAIT_TIME1,
				WAIT_TIME2,
				WALK,
				RUN,
				ROLL,
				DIVE,
				KNOCK_BACK,
				KNOCK_OUT,
				DIE,
				EARPLUG,
				STANDUP,
				HEAL,
				WALK_SETUP,
				SETUP,
				SETUP_DRAWN,
				SETUP_WAIT,
				SETUP_RUN,

				SRASH_1,
				SRASH_2,
				SRASH_3,

				D_WAIT1,
				D_WAIT2,
				D_WALK,
				D_RUN,
				D_SCREAM,
				D_HIT,
				D_DIE,
				D_BITE,
				D_WING_ATTACK,
				D_TAKEOFF,
				D_FLY_IDLE,

				MAX
			};

			enum class Result : int8
			{
				MAX = (int)Base::MAX,
			};
		};

	const string baseFileName[(int)Base::MAX]
	{
		{ directoryName + "sky/sky2.bmx" },
	};

	const string campFileName[(int)Camp::MAX - (int)Base::MAX]
	{
		{ directoryName + "cube/cube2.bmx"									},
		{ directoryName + "unitychan/unitychan.bmx"							},
		{ directoryName + "wapon/kohaku_sword/kohaku_sword_notangent.bmx"	},
		{ directoryName + "box/TreasureChest_notangent.bmx"					},
		{ directoryName + "wallRocks/Rock1/Rock1.bmx"						},
		{ directoryName + "wallRocks/Rock4/Rock4.bmx"						},
		{ directoryName + "wallRocks/Rock6/Rock6.bmx"						},
		{ directoryName + "wallRocks/Rock10/Rock10.bmx"						},
		{ directoryName + "wallRocks/Rock12/Rock12.bmx"						},
		{ directoryName + "wallRocks/Rock14/Rock14.bmx"						},
		{ directoryName + "cart/cart.bmx"									},
		{ directoryName + "cannon/cannon.bmx"								},
		{ directoryName + "cannon/cannon_barrel.bmx"						},
		{ directoryName + "cannon/cannon_bullet.bmx"						},
		{ directoryName + "tent/tent.bmx"									},
	};

	const string buttleFileName[(int)Buttle::MAX - (int)Base::MAX]
	{
		{ directoryName + "cube/cube2.bmx"									},
		{ directoryName + "unitychan/unitychan.bmx"							},
		{ directoryName + "wapon/kohaku_sword/kohaku_sword_notangent.bmx"	},
		{ directoryName + "terror_dragon/terror_dragon2.bmx"				},
		{ directoryName + "wallRocks/Rock1/Rock1.bmx"						},
		{ directoryName + "wallRocks/Rock4/Rock4.bmx"						},
		{ directoryName + "wallRocks/Rock6/Rock6.bmx"						},
		{ directoryName + "wallRocks/Rock10/Rock10.bmx"						},
		{ directoryName + "wallRocks/Rock12/Rock12.bmx"						},
		{ directoryName + "wallRocks/Rock14/Rock14.bmx"						},

	};

	//const string resultFileName[(int)Result::MAX - (int)Base::MAX]
	//{
	//};

	const ANIMATION_INFO animationCampFileName[(int)Animation::Camp::MAX]
	{
		{ directoryName + "unitychan_animation/unitychan_WAIT00.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT01.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT02.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WALK00_F.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_RUN00_F.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Roll.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_dive.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_KnockBack.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_KnockOut.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Die.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_ear.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Standup.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_heal.bamx"					, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_walksetup.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Setup.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupDrawn.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWait.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWalk.bamx"			, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash1.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash2.bamx"				, (int)Camp::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash3.bamx"				, (int)Camp::UNITY_CHAN },
	};
	
	const ANIMATION_INFO animationButtleFileName[(int)Animation::Buttle::MAX]
	{
		{ directoryName + "unitychan_animation/unitychan_WAIT00.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT01.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WAIT02.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_WALK00_F.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_RUN00_F.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Roll.bamx"					, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_dive.bamx"					, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_KnockBack.bamx"			, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_KnockOut.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Die.bamx"					, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_ear.bamx"					, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Standup.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_heal.bamx"					, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_walksetup.bamx"			, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Setup.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupDrawn.bamx"			, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWait.bamx"			, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_SetupWalk.bamx"			, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash1.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash2.bamx"				, (int)Buttle::UNITY_CHAN },
		{ directoryName + "unitychan_animation/unitychan_Slash3.bamx"				, (int)Buttle::UNITY_CHAN },

		{ directoryName + "terror_dragon_animation/terror_dragon_idle1.bamx"		, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_idle2.bamx"		, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_walk.bamx"			, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_run.bamx"			, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_scream.bamx"		, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_hit.bamx"			, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_die.bamx"			, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_bite.bamx"			, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_wing_attack.bamx"	, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_takeoff.bamx"		, (int)Buttle::DRAGON		},
		{ directoryName + "terror_dragon_animation/terror_dragon_fly_idle.bamx"		, (int)Buttle::DRAGON		},
	};

	};
}

#endif // _RESOURCES_MODEL_H_
