//-----------------------------------------------------------------------------
//
// サウンド処理 [Sound.cpp]
//
//-----------------------------------------------------------------------------
#include "Sound.h"
#include "../GameSystems.h"
#include "../../Windows/Windows.h"
#include "../../Scene/BaseScene.h"
#include "../Loading.h"

// 初期化処理
HRESULT Sound::Init(void)
{
	Windows* windows = systems_->GetWindow();
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&pXAudio2_, 0);
	if(windows->ErrorMessage("XAudio2オブジェクトの作成に失敗", "エラー", hr))
	{
		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = pXAudio2_->CreateMasteringVoice(&pMasteringVoice_);
	if(windows->ErrorMessage("マスターボイスの生成に失敗", "エラー", hr))
	{
		ReleasePtr(pXAudio2_);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	int size = (int)Base::MAX;
	for (int i = 0; i < size; ++i)
	{
		hr = LoadSound(baseFileName[i], i);
		if (FAILED(hr)) { return E_FAIL; }
	}

	return S_OK;
}

// 終了処理
Sound::~Sound(void)
{
	Release(true);
	
	// マスターボイスの破棄
	pMasteringVoice_->DestroyVoice();
	pMasteringVoice_ = nullptr;
	
	ReleasePtr(pXAudio2_);
	
	// COMライブラリの終了処理
	CoUninitialize();
}

int	Sound::SetUpLoading(Loading* loading, int sceneNum)
{
	loading_ = loading;

	switch ((SceneList)sceneNum)
	{
	case SceneList::TITLE:
		return (int)Title::MAX - (int)Base::MAX;
	case SceneList::CAMP:
		return (int)Camp::MAX - (int)Base::MAX;
	case SceneList::BUTTLE:
		return (int)Buttle::MAX - (int)Base::MAX;
	case SceneList::RESULT:
		return (int)Result::MAX - (int)Base::MAX;
	default: break;
	}
	return 0;
}

HRESULT Sound::Load(int sceneNum)
{
	sceneNum_ = sceneNum;
	int size = 0;
	const SOUNDPARAM* fileName = nullptr;
	switch ((SceneList)sceneNum)
	{
	case SceneList::TITLE:
		size = (int)Title::MAX;
		fileName = &titleFileName[0];
		break;
	case SceneList::CAMP:
		size = (int)Camp::MAX;
		fileName = &campFileName[0];
		break;
	case SceneList::BUTTLE:
		size = (int)Buttle::MAX;
		fileName = &buttleFileName[0];
		break;
	case SceneList::RESULT:
		size = (int)Result::MAX;
		fileName = &resultFileName[0];
		break;
	default: break;
	}

	if (!fileName) { return E_FAIL; }
	for (int i = 0; i < size; ++i)
	{
		if (i < (int)Base::MAX) { continue; }
		HRESULT hr = LoadSound(fileName[i - (int)Base::MAX], i);
		if (FAILED(hr)) { return E_FAIL; }
		loading_->AddLoading();
	}

	return S_OK;
}

HRESULT Sound::LoadSound(Sound::SOUNDPARAM param, int i)
{
	UNREFERENCED_PARAMETER(i);

	Windows* windows = systems_->GetWindow();
	HANDLE hFile;
	DWORD  dwChunkSize = 0;
	DWORD  dwChunkPosition = 0;
	DWORD  dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER       buffer;

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	string error;
	string temp = param.pFilename;
	hFile = CreateFile(temp.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		error = temp + "の生成に失敗(1)";
		windows->ErrorMessage(error.c_str(), "エラー", E_FAIL);
		return HRESULT_FROM_WIN32(GetLastError());
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動

		error = temp + "の生成に失敗(2)";
		windows->ErrorMessage(error.c_str(), "エラー", E_FAIL);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// WAVEファイルのチェック
	error = temp + "のチェックに失敗(1)";
	HRESULT hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (windows->ErrorMessage(error.c_str(), "エラー", hr))
	{
		return S_FALSE;
	}
	error = temp + "のチェックに失敗(2)";
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (windows->ErrorMessage(error.c_str(), "エラー", hr))
	{
		return S_FALSE;
	}
	if (dwFiletype != 'EVAW')
	{
		error = temp + "のチェックに失敗(3)";
		windows->ErrorMessage(error.c_str(), "エラー", E_FAIL);
		return S_FALSE;
	}

	// フォーマットチェック
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (windows->ErrorMessage("フォーマットチェックに失敗(1)", "エラー", hr))
	{
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (windows->ErrorMessage("フォーマットチェックに失敗(2)", "エラー", hr))
	{
		return S_FALSE;
	}

	SOUNDDATA tempAudio;
	tempAudio.loop = param.cntLoop;
	// オーディオデータ読み込み
	hr = CheckChunk(hFile, 'atad', &tempAudio.size, &dwChunkPosition);
	if (windows->ErrorMessage("オーディオデータ読み込みに失敗(1)", "エラー", hr))
	{
		return S_FALSE;
	}
	tempAudio.data = (byte*)malloc(tempAudio.size);
	hr = ReadChunkData(hFile, tempAudio.data, tempAudio.size, dwChunkPosition);
	if (windows->ErrorMessage("オーディオデータ読み込みに失敗(2)", "エラー", hr))
	{
		return S_FALSE;
	}

	// ソースボイスの生成
	hr = pXAudio2_->CreateSourceVoice(&tempAudio.sourceVoice, &(wfx.Format));
	if (windows->ErrorMessage("ソースボイスの生成に失敗", "エラー", hr))
	{
		return S_FALSE;
	}

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = tempAudio.size;
	buffer.pAudioData = tempAudio.data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = param.cntLoop;

	// オーディオバッファの登録
	tempAudio.sourceVoice->SubmitSourceBuffer(&buffer);

	sound_.emplace_back(tempAudio);

	return S_OK;
}

void Sound::Release(bool uninit)
{
	int size = 0;
	switch ((SceneList)sceneNum_)
	{
	case SceneList::TITLE:
		size = (int)Title::MAX;
		break;
	case SceneList::CAMP:
		size = (int)Camp::MAX;
		break;
	case SceneList::BUTTLE:
		size = (int)Buttle::MAX;
		break;
	case SceneList::RESULT:
		size = (int)Result::MAX;
		break;
	default: break;
	}

	int max = (int)Base::MAX;
	if (uninit) { max = 0; }
	for (int i = size - 1; i >= max; --i)
	{
		ReleaseSound(i);
	}
}

void Sound::ReleaseSound(int i)
{
	if (i >= (int)sound_.size()) { return; }
	auto& sound = sound_[i];

	if (sound.sourceVoice)
	{
		// 一時停止
		sound.sourceVoice->Stop(0);

		// ソースボイスの破棄
		sound.sourceVoice->DestroyVoice();
		sound.sourceVoice = nullptr;

		// オーディオデータの開放
		free(sound.data);
		sound.data = nullptr;
	}

	for (auto itr = sound_.begin(); itr != sound_.end();)
	{
		if (&(*itr) == &sound)
		{
			itr = sound_.erase(itr);		//配列削除
			break;
		}
		else
		{
			itr++;
		}
	}
	sound_.shrink_to_fit();
}

// セグメント再生(再生中なら停止)
HRESULT Sound::Play(int label) const
{
	const auto& sound = sound_[label];
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = sound.size;
	buffer.pAudioData = sound.data;
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = sound.loop;

	// 状態取得
	sound.sourceVoice->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		sound.sourceVoice->Stop(0);

		// オーディオバッファの削除
		sound.sourceVoice->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	sound.sourceVoice->SubmitSourceBuffer(&buffer);

	// 再生
	sound.sourceVoice->Start(0);

	return S_OK;
}

// セグメント停止(ラベル指定)
void Sound::Stop(int label) const
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	const auto& voice = sound_[label].sourceVoice;

	voice->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		voice->Stop(0);

		// オーディオバッファの削除
		voice->FlushSourceBuffers();
	}
}

// セグメント停止(全て)
void Sound::Stop(void) const
{
	int size = 0;
	switch ((SceneList)sceneNum_)
	{
	case SceneList::TITLE:
		size = (int)Title::MAX;
		break;
	case SceneList::CAMP:
		size = (int)Camp::MAX;
		break;
	case SceneList::BUTTLE:
		size = (int)Buttle::MAX;
		break;
	case SceneList::RESULT:
		size = (int)Result::MAX;
		break;
	default: break;
	}

	// 一時停止
	for(int i = 0; i < size; ++i)
	{
		if(sound_[i].sourceVoice)
		{
			// 一時停止
			sound_[i].sourceVoice->Stop(0);
		}
	}
}

// チャンクのチェック
HRESULT Sound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

// チャンクデータの読み込み
HRESULT Sound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

