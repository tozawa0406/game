//-----------------------------------------------------------------------------
//
// �T�E���h���� [Sound.cpp]
//
//-----------------------------------------------------------------------------
#include "Sound.h"
#include "../GameSystems.h"
#include "../../Windows/Windows.h"
#include "../../Scene/BaseScene.h"
#include "../../Scene/SceneManager.h"
#include "../Loading.h"

// ����������
HRESULT Sound::Init(void)
{
	if (!systems_) { return E_FAIL; }
	Windows* windows = systems_->GetWindow();
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&pXAudio2_, 0);
	if (windows->ErrorMessage("XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s", "�G���[", hr)) { return E_FAIL; }
	
	// �}�X�^�[�{�C�X�̐���
	hr = pXAudio2_->CreateMasteringVoice(&pMasteringVoice_);
	if(windows->ErrorMessage("�}�X�^�[�{�C�X�̐����Ɏ��s", "�G���[", hr))
	{
		ReleasePtr(pXAudio2_);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	const ResourceInfoManager& resource = systems_->GetResource();
	int size = 0, max = 0;
	const auto& fileName = resource.LoadSound(SceneList::MAX, size, max);
	for (int i = 0; i < size; ++i)
	{
		hr = LoadSound(fileName[i], i);
		if (FAILED(hr)) { return E_FAIL; }
	}

	return S_OK;
}

// �I������
Sound::~Sound(void)
{
	Release(true);
	
	// �}�X�^�[�{�C�X�̔j��
	if (pMasteringVoice_)
	{
		pMasteringVoice_->DestroyVoice();
		pMasteringVoice_ = nullptr;
	}
	
	ReleasePtr(pXAudio2_);
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

int	Sound::SetUpLoading(Loading* loading, int sceneNum)
{
	sceneNum_ = sceneNum;
	loading_  = loading;

	if (!systems_) { return 0; }
	int size = 0, max = 0;	
	systems_->GetResource().LoadSound(static_cast<SceneList>(sceneNum), size, max);

	return size;
}

HRESULT Sound::Load(int sceneNum)
{
	sceneNum_ = sceneNum;

	if (!systems_) { return E_FAIL; }
	int size = 0, max = 0;
	const auto& fileName = systems_->GetResource().LoadSound(static_cast<SceneList>(sceneNum), size, max);

	if (!fileName) { return E_FAIL; }
	for (int i = 0; i < max; ++i)
	{
		if (i < static_cast<int>(Resources::Sound::Base::MAX)) { continue; }
		HRESULT hr = LoadSound(fileName[i - static_cast<int>(Resources::Sound::Base::MAX)], i);
		if (FAILED(hr)) { return E_FAIL; }
		loading_->AddLoading();
	}

	return S_OK;
}

HRESULT Sound::LoadSound(SOUNDPARAM param, int i)
{
	UNREFERENCED_PARAMETER(i);

	Windows* windows = systems_->GetWindow();
	HANDLE hFile;
	DWORD  dwChunkSize = 0;
	DWORD  dwChunkPosition = 0;
	DWORD  dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER       buffer;

	// �o�b�t�@�̃N���A
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// �T�E���h�f�[�^�t�@�C���̐���
	string error;
	string temp = param.pFilename;
	hFile = CreateFile(temp.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		error = temp + "�̐����Ɏ��s(1)";
		windows->ErrorMessage(error.c_str(), "�G���[", E_FAIL);
		return HRESULT_FROM_WIN32(GetLastError());
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�

		error = temp + "�̐����Ɏ��s(2)";
		windows->ErrorMessage(error.c_str(), "�G���[", E_FAIL);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// WAVE�t�@�C���̃`�F�b�N
	error = temp + "�̃`�F�b�N�Ɏ��s(1)";
	HRESULT hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (windows->ErrorMessage(error.c_str(), "�G���[", hr))
	{
		return S_FALSE;
	}
	error = temp + "�̃`�F�b�N�Ɏ��s(2)";
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (windows->ErrorMessage(error.c_str(), "�G���[", hr))
	{
		return S_FALSE;
	}
	if (dwFiletype != 'EVAW')
	{
		error = temp + "�̃`�F�b�N�Ɏ��s(3)";
		windows->ErrorMessage(error.c_str(), "�G���[", E_FAIL);
		return S_FALSE;
	}

	// �t�H�[�}�b�g�`�F�b�N
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (windows->ErrorMessage("�t�H�[�}�b�g�`�F�b�N�Ɏ��s(1)", "�G���[", hr))
	{
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (windows->ErrorMessage("�t�H�[�}�b�g�`�F�b�N�Ɏ��s(2)", "�G���[", hr))
	{
		return S_FALSE;
	}

	SOUNDDATA tempAudio;
	tempAudio.loop = param.cntLoop;
	// �I�[�f�B�I�f�[�^�ǂݍ���
	hr = CheckChunk(hFile, 'atad', &tempAudio.size, &dwChunkPosition);
	if (windows->ErrorMessage("�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s(1)", "�G���[", hr))
	{
		return S_FALSE;
	}
	tempAudio.data = (byte*)malloc(tempAudio.size);
	hr = ReadChunkData(hFile, tempAudio.data, tempAudio.size, dwChunkPosition);
	if (windows->ErrorMessage("�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s(2)", "�G���[", hr))
	{
		return S_FALSE;
	}

	// �\�[�X�{�C�X�̐���
	hr = pXAudio2_->CreateSourceVoice(&tempAudio.sourceVoice, &(wfx.Format));
	if (windows->ErrorMessage("�\�[�X�{�C�X�̐����Ɏ��s", "�G���[", hr))
	{
		return S_FALSE;
	}

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = tempAudio.size;
	buffer.pAudioData = tempAudio.data;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = param.cntLoop;

	// �I�[�f�B�I�o�b�t�@�̓o�^
	tempAudio.sourceVoice->SubmitSourceBuffer(&buffer);

	sound_.emplace_back(tempAudio);

	return S_OK;
}

void Sound::Release(bool uninit)
{
	if (!systems_) { return; }
	int size = 0, max = 0;
	systems_->GetResource().LoadSound(static_cast<SceneList>(sceneNum_), size, max);

	int baseMax = static_cast<int>(Resources::Sound::Base::MAX);
	if (uninit) { baseMax = 0; }
	for (int i = max - 1; i >= baseMax; --i)
	{
		ReleaseSound(i);
	}
}

void Sound::ReleaseSound(int i)
{
	if (i >= static_cast<int>(sound_.size())) { return; }
	auto& sound = sound_[i];

	if (sound.sourceVoice)
	{
		// �ꎞ��~
		sound.sourceVoice->Stop(0);

		// �\�[�X�{�C�X�̔j��
		sound.sourceVoice->DestroyVoice();
		sound.sourceVoice = nullptr;

		// �I�[�f�B�I�f�[�^�̊J��
		free(sound.data);
		sound.data = nullptr;
	}

	for (auto itr = sound_.begin(); itr != sound_.end();)
	{
		if (&(*itr) == &sound)
		{
			itr = sound_.erase(itr);		//�z��폜
			break;
		}
		else
		{
			itr++;
		}
	}
	sound_.shrink_to_fit();
}

// �Z�O�����g�Đ�(�Đ����Ȃ��~)
HRESULT Sound::Play(int label) const
{
	const auto& sound = sound_[label];
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = sound.size;
	buffer.pAudioData = sound.data;
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = sound.loop;

	// ��Ԏ擾
	sound.sourceVoice->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		sound.sourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		sound.sourceVoice->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	sound.sourceVoice->SubmitSourceBuffer(&buffer);

	// �Đ�
	sound.sourceVoice->Start(0);

	return S_OK;
}

// �Z�O�����g��~(���x���w��)
void Sound::Stop(int label) const
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	const auto& voice = sound_[label].sourceVoice;

	voice->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		voice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		voice->FlushSourceBuffers();
	}
}

// �Z�O�����g��~(�S��)
void Sound::Stop(void) const
{
	if (!systems_) { return; }
	const ResourceInfoManager& resource = systems_->GetResource();
	int size = 0, max = 0;
	resource.LoadSound(static_cast<SceneList>(sceneNum_), size, max);

	// �ꎞ��~
	for(int i = 0; i < max; ++i)
	{
		if(sound_[i].sourceVoice)
		{
			// �ꎞ��~
			sound_[i].sourceVoice->Stop(0);
		}
	}
}

// �`�����N�̃`�F�b�N
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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

// �`�����N�f�[�^�̓ǂݍ���
HRESULT Sound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

