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
		enum class Base : int8
		{
			UNOWN = -1,

			WHITE = 0,
			NUMBER,
			SSUI,
			LOAD,

			MAX
		};

		enum class Title : int8
		{
			TITLE = (int)Base::MAX,

			MAX
		};

		enum class Camp : int8
		{
			FIELD = (int)Base::MAX,
			SLASH,
			EFFECT,

			HP_BAR,
			ITEM_UI,

			TIMER_UI,

			ITEM_RECOVERY,
			ITEM_RATION,

			UI_TUTORIAL,
			UI_BALOON,

			UI_PS4CTRL,
			UI_PS4CTRL_L3,
			UI_PS4CTRL_R3,
			UI_PS4CTRL_R1,
			UI_PS4CTRL_CROSS,
			UI_PS4CTRL_L1,
			UI_PS4CTRL_SQUARE,
			UI_PS4CTRL_CIRCLE,
			UI_PS4CTRL_TRIANGLE,

			UI_XCTRL,
			UI_XCTRL_L,
			UI_XCTRL_R,
			UI_XCTRL_RB,
			UI_XCTRL_A,
			UI_XCTRL_LB,
			UI_XCTRL_X,
			UI_XCTRL_B,
			UI_XCTRL_Y,

			UI_KEY,
			UI_KEY_A,
			UI_KEY_W,
			UI_KEY_S,
			UI_KEY_D,
			UI_KEY_I,
			UI_KEY_J,
			UI_KEY_K,
			UI_KEY_L,
			UI_KEY_LSHIFT,
			UI_KEY_M,
			UI_KEY_R,
			UI_KEY_T,
			UI_KEY_Y,
			UI_KEY_U,
			UI_KEY_H,

			MAX
		};

		enum class Buttle : int8
		{
			FIELD = (int)Base::MAX,
			SLASH,
			EFFECT,

			HP_BAR,
			ITEM_UI,

			TIMER_UI,

			ITEM_RECOVERY,
			ITEM_RATION,

			UI_TUTORIAL,

			CLEAR_FAILED_UI,

			MAX
		};

		enum class Result : int8
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

		const string titleFileName[(int)Title::MAX - (int)Base::MAX]
		{
			{ directoryName + "title.png"				},
		};

		const string campFileName[(int)Camp::MAX - (int)Base::MAX]
		{
			{ directoryName + "field.jpg"					},
			{ directoryName + "slash.jpg"					},
			{ directoryName + "effect.jpg"					},

			{ directoryName + "UI/game/hpbar.png"			},
			{ directoryName + "UI/game/itemlist.png"		},

			{ directoryName + "UI/timer.png"				},

			{ directoryName + "UI/game/item/recovery.png"	},
			{ directoryName + "UI/game/item/ration.png"		},

			{ directoryName + "UI/tutorial/ctrlset.png"		},
			{ directoryName + "UI/tutorial/baloon.png"		},

			{ directoryName + "UI/tutorial/PS4ctrl/PS4controller.png"	},
			{ directoryName + "UI/tutorial/PS4ctrl/L3stick.png"			},
			{ directoryName + "UI/tutorial/PS4ctrl/R3stick.png"			},
			{ directoryName + "UI/tutorial/PS4ctrl/R1.png"				},
			{ directoryName + "UI/tutorial/PS4ctrl/cross.png"			},
			{ directoryName + "UI/tutorial/PS4ctrl/L1.png"				},
			{ directoryName + "UI/tutorial/PS4ctrl/square.png"			},
			{ directoryName + "UI/tutorial/PS4ctrl/circle.png"			},
			{ directoryName + "UI/tutorial/PS4ctrl/triangle.png"		},

			{ directoryName + "UI/tutorial/Xctrl/Xcontroller.png" },
			{ directoryName + "UI/tutorial/Xctrl/Lstick.png" },
			{ directoryName + "UI/tutorial/Xctrl/Rstick.png" },
			{ directoryName + "UI/tutorial/Xctrl/RB.png"	},
			{ directoryName + "UI/tutorial/Xctrl/A.png"		},
			{ directoryName + "UI/tutorial/Xctrl/LB.png"	},
			{ directoryName + "UI/tutorial/Xctrl/X.png"		},
			{ directoryName + "UI/tutorial/Xctrl/B.png"		},
			{ directoryName + "UI/tutorial/Xctrl/Y.png"		},

			{ directoryName + "UI/tutorial/keyboard/keyboard.png" },
			{ directoryName + "UI/tutorial/keyboard/a.png"	},
			{ directoryName + "UI/tutorial/keyboard/w.png"	},
			{ directoryName + "UI/tutorial/keyboard/s.png"	},
			{ directoryName + "UI/tutorial/keyboard/d.png"	},
			{ directoryName + "UI/tutorial/keyboard/i.png"	},
			{ directoryName + "UI/tutorial/keyboard/j.png"	},
			{ directoryName + "UI/tutorial/keyboard/k.png"	},
			{ directoryName + "UI/tutorial/keyboard/l.png"	},
			{ directoryName + "UI/tutorial/keyboard/Lshift.png"	},
			{ directoryName + "UI/tutorial/keyboard/m.png"	},
			{ directoryName + "UI/tutorial/keyboard/r.png"	},
			{ directoryName + "UI/tutorial/keyboard/t.png"	},
			{ directoryName + "UI/tutorial/keyboard/y.png"	},
			{ directoryName + "UI/tutorial/keyboard/u.png"	},
			{ directoryName + "UI/tutorial/keyboard/h.png"	},
		};

		const string buttleFileName[(int)Buttle::MAX - (int)Base::MAX]
		{
			{ directoryName + "field.jpg"					},
			{ directoryName + "slash.jpg"					},
			{ directoryName + "effect.jpg"					},

			{ directoryName + "UI/game/hpbar.png"			},
			{ directoryName + "UI/game/itemlist.png"		},

			{ directoryName + "UI/timer.png"				},

			{ directoryName + "UI/game/item/recovery.png"	},
			{ directoryName + "UI/game/item/ration.png"		},

			{ directoryName + "UI/tutorial/ctrlset.png"		},

			{ directoryName + "UI/clear_failed.png"			},
		};

		//const string resultFileName[(int)Result::MAX - (int)Base::MAX]
		//{
		//};
	};
}

#endif // _RESOURCES_TEXTURE_H_
