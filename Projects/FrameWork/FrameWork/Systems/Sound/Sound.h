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
#include <Scene/ResourcesSound.h>

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class Loading;
class Sound : public Interface
{
	friend class Systems;
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
		byte*					data;			// オーディオデータ
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
