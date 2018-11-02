//-----------------------------------------------------------------------------
//
// サウンド処理 [Sound.h]
//
//-----------------------------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

#include "../../Define/Define.h"
#include <xAudio2.h>
#include "../BaseManager.h"

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class Loading;
class Sound : public Interface
{
	friend class Systems;
	const string directoryName		= Define::ResourceDirectoryName + "Sound/";
	const string BGMDirectoryName	= directoryName + "BGM/";
	const string SEDirectoryName	= directoryName + "SE/";
public:
	enum class Base : int
	{
		UNOWN = -1, 
		SE_SELECT = 0,
		SE_ENTER,
		SE_CANCEL,
		BGM_WIN,
		BGM_LOSE,

		MAX
	};

	enum class Title : int
	{
		BGM_OP = (int)Base::MAX,

		MAX
	};
	enum class Game : int
	{
		BGM_GAME = (int)Base::MAX,
		SE_SHOUT,
		SE_EXPLOSION,
		SE_ALEAT,

		MAX
	};
	enum class Result : int
	{
		MAX = (int)Base::MAX,
	};

private:
	struct SOUNDPARAM
	{
		string pFilename;	// ファイル名
		int cntLoop;		// ループカウント
	};

	const SOUNDPARAM baseFileName[(int)Base::MAX]
	{
		{ SEDirectoryName + "select.wav"	, 0 },
		{ SEDirectoryName + "enter.wav"	, 0 },
		{ SEDirectoryName + "cancel.wav"	, 0 },

		{ BGMDirectoryName + "Win/Goodbye_The_Fake_Band.wav"	, -1 },
		{ BGMDirectoryName + "Lose/新宿地下道.wav"				, -1 },
	};
	const SOUNDPARAM titleFileName[(int)Title::MAX]
	{
		{ BGMDirectoryName + "OP/Easy_Days.wav", -1 },
	};
	const SOUNDPARAM gameFileName[(int)Game::MAX]
	{
		{ BGMDirectoryName + "Game/Thunderbolt.wav"	, -1 },
		{ SEDirectoryName + "shout.wav"			,  0 },
		{ SEDirectoryName + "explosion.wav"		,  0 },
		{ SEDirectoryName + "aleat.wav"			,  0 },
	};
//	const SOUNDPARAM resultFileName[(int)Result::MAX - (int)Base::MAX];

public:
	~Sound(void);

	int		SetUpLoading(Loading* loading, int sceneNum);
	HRESULT Load(int sceneNum);
	void	Release(bool uninit = false);

	HRESULT Play(int label) const;
	void    Stop(int label) const;
	void    Stop(void)      const;

private:

	struct SOUNDDATA
	{
		IXAudio2SourceVoice*	sourceVoice;	// ソースボイス
		BYTE*					data;			// オーディオデータ
		DWORD					size;			// オーディオデータサイズ
		int						loop;
	};

	Sound(Systems* systems) : Interface(systems), pXAudio2_(nullptr), pMasteringVoice_(nullptr) {}
	HRESULT Init(void);

	HRESULT LoadSound(SOUNDPARAM param, int i);
	void	ReleaseSound(int i);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	Loading*							loading_;
	int									sceneNum_;
	IXAudio2*							pXAudio2_;				// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice*				pMasteringVoice_;		// マスターボイス
	std::vector<SOUNDDATA>				sound_;
};

#endif
