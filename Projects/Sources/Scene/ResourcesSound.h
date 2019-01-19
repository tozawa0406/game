/*
 * @file		ResourcesSound.h
 * @brief		���\�[�X�Ǘ��t�@�C��
 * @author		���V�đ�
 * @data		2018/01/17
 */
#ifndef _RESOURCES_SOUND_H_
#define _RESOURCES_SOUND_H_

#include <FrameWork/Define/Define.h>

struct SOUNDPARAM
{
	string pFilename;	// �t�@�C����
	int cntLoop;		// ���[�v�J�E���g
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

			MAX
		};

		enum class Buttle : int8
		{
			BGM_GAME = (int)Base::MAX,

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
			{ SEDirectoryName + "enter.wav"	, 0 },
			{ SEDirectoryName + "cancel.wav"	, 0 },
		};
		const SOUNDPARAM titleFileName[(int)Title::MAX - (int)Base::MAX]
		{
			{ BGMDirectoryName + "Title/bgm_maoudamashii_fantasy08.wav", -1 },
		};
		const SOUNDPARAM campFileName[(int)Camp::MAX - (int)Base::MAX]
		{
			{ BGMDirectoryName + "Game/bgm_maoudamashii_orchestra24.wav"	, -1 },
		};
		const SOUNDPARAM buttleFileName[(int)Buttle::MAX - (int)Base::MAX]
		{
			{ BGMDirectoryName + "Game/bgm_maoudamashii_orchestra24.wav"	, -1 },
		};
		const SOUNDPARAM resultFileName[(int)Result::MAX - (int)Base::MAX]
		{
			{ BGMDirectoryName + "Result/bgm_maoudamashii_orchestra14.wav"	, -1 },
		};
	};
}

#endif // _RESOURCES_SOUND_H_