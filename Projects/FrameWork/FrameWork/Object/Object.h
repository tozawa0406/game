/*
 * @file		Object.h
 * @brief		�I�u�W�F�N�g
 * @author		���V�đ�
 * @data		2017/09/05
 */
#ifndef _OBJECT_H_ 
#define _OBJECT_H_

#include "../Define/Define.h"
#include "../Windows/Windows.h"
#include "../Systems/GameSystems.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Renderer/Sprite/SpriteRenderer.h"
#include "../Systems/Renderer/Model/MeshRenderer.h"

#include "../Systems/DebugSetting/GUI.h"

#include "../../../Sources/Object/ObjectTag.h"

class Controller;
class BaseScene;
class GuiManager;
class Systems;
class Object
{
public:
	friend class ObjectManager;
protected:
	/* @brief	����������		*/
	virtual void Init(void)   = 0;
	/* @brief	�㏈��			*/
	virtual void Uninit(void) = 0;
	/* @brief	�X�V����		*/
	virtual void Update(void) = 0;

	/* @brief	�^�O�̐ݒ�
	 * @param	(tag)	�ݒ肵�����^�O
	 * @return	�Ȃ�			*/
	inline void SetTag(ObjectTag tag)	{ tag_	= tag; }

	/* @brief	�폜����
	 * @param	�Ȃ�
	 * @return	�Ȃ�			*/
	inline void Destroy(void)	{ destroy_ = true; }

	/* @brief	�폜��Ԏ擾
	 * @param	�Ȃ�
	 * @return	�폜���		*/
	inline bool GetDestroy(void) { return destroy_; }

	/* @brief	�R���g���[���擾
	 * @param	(i)		�R���g���[���ԍ�
	 * @return	�R���g���\���̃|�C���^		*/
	Controller* GetCtrl(int i);

	//! �p��
	Transform  transform_;
	//! �}�l�[�W���[
	ObjectManager* manager_;

public:
	/* @brief	�R���X�g���N�^		*/
	Object(ObjectTag tag);
	/* @brief	�f�X�g���N�^		*/
	virtual ~Object(void);

	/* @brief	�p���擾			*/
	inline Transform&	GetTransform(void) { return transform_; }
	/* @brief	�^�O�̎擾			*/
	inline ObjectTag	GetTag(void)       { return tag_;       }

	/* @fn		SetObejctManager
	 * @brief	�}�l�[�W���[�̐ݒ�
	 * @param	(manager)	���g�̊Ǘ���		*/
	inline void SetObjectManager(ObjectManager* manager) { manager_ = manager; }

private:
	//! �폜�t���O
	bool		destroy_;
	//! �^�O
	ObjectTag	tag_;

	friend GuiManager;
};

#endif // _OBJECT_H_