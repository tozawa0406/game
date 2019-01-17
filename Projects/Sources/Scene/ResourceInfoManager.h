/*
 * @file		ResourceInfoManager.h
 * @brief		�V�[�����Ǘ��N���X
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _RESOURCE_INFO_MANAGER_H_
#define _RESOURCE_INFO_MANAGER_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Windows/Windows.h>
#include "00_EachScene.h"
#include "ResourcesTexture.h"
#include "ResourcesModel.h"
#include "ResourcesSound.h"

class ResourceInfoManager
{
public:
	/* @brief	�R���X�g���N�^		*/
	ResourceInfoManager(void);
	/* @brief	�f�X�g���N�^		*/
	~ResourceInfoManager(void);

	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�������s			*/
	HRESULT Init(Windows* window);

	/* @brief	�e�N�X�`���̃��[�h���̎擾
	 * @param	(state)		�V�[��
	 * @param	(size)		���̃V�[�����̃T�C�Y
	 * @param	(max)		�x�[�X���܂߂��T�C�Y
	 * @return	�e�N�X�`����		*/
	const string* LoadTexture(SceneList state, int& size, int& max) const;
	
	/* @brief	���f���̃��[�h���̎擾
	 * @param	(state)		�V�[��
	 * @param	(size)		���̃V�[�����̃T�C�Y
	 * @param	(max)		�x�[�X���܂߂��T�C�Y
	 * @return	���f����			*/
	const string* LoadModel(SceneList state, int& size, int& max) const;
	
	/* @brief	���f���A�j���[�V�����̃��[�h���̎擾
	 * @param	(state)		�V�[��
	 * @param	(size)		���̃V�[�����̃T�C�Y
	 * @param	(max)		�x�[�X���܂߂��T�C�Y
	 * @return	���f���A�j���[�V������		*/
	const ANIMATION_INFO* LoadModelAnimation(SceneList state, int& size, int& max) const;

	/* @brief	�T�E���h�̃��[�h���̎擾
	 * @param	(state)		�V�[��
	 * @param	(size)		���̃V�[�����̃T�C�Y
	 * @param	(max)		�x�[�X���܂߂��T�C�Y
	 * @return	�T�E���h����BGM/SE�敪		*/
	const SOUNDPARAM* LoadSound(SceneList state, int& size, int& max) const;

	/* @brief	���e�N�X�`���̖��O�擾
	 * @param	�Ȃ�
	 * @return	���e�N�X�`���̖��O	*/
	const string& GetWhiteTextureName(void) const;

private:
	Resources::Texture	texture_;
	Resources::Model	model_;
	Resources::Sound	sound_;
};

#endif // _RESOURCE_INFO_MANAGER_
