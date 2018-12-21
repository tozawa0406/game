/*
 * @file		ObjectManager.h
 * @brief		�I�u�W�F�N�g�Ǘ��N���X
 * @author		���V�đ�
 * @data		2017/09/05
 */
#ifndef _OBJECT_MANAGER_H_ 
#define _OBJECT_MANAGER_H_

#include "../Scene/BaseScene.h"

class Object;
class ObjectManager : public BaseManager<Object>
{
public:
	/* @breif	�R���X�g���N�^
	 * @param	(parent)	���������V�[��		*/
	ObjectManager(BaseScene* parent);
	/* @brief	�f�X�g���N�^	*/
	~ObjectManager(void);

	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�������s		*/
	HRESULT Init(void)   override;

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�			*/
	void    Uninit(void) override;

	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�			*/
	void    Update(void) override;

	/* @brief	�I�u�W�F�N�g�擾		*/
	std::vector<Object*>& Get(void)        { return obj_;  }

	/* @fn		GetScene
	 * @brief	�V�[���̎擾
	 * @return	���g�̑��݂���V�[��		*/
	inline BaseScene* GetScene(void) { return scene_; }


	
	/* @fn		Create
	 * @brief	�I�u�W�F�N�g�̐���
	 * @param	new�̍ۂɕK�v�Ȉ���
	 * @return	�I�u�W�F�N�g�̃|�C���^
	 * @detail	�I�u�W�F�N�g�̐����A�����Ĕz��ɒǉ����A
				�I�u�W�F�N�g�ɂ��}�l�[�W�F�[�ւ̃|�C���^��ݒ�
				�I�u�W�F�N�g�̏��������������Ă��̃|�C���^��Ԃ�		*/
	template<class T, class... Args>
	T* Create(Args... args)
	{
		const auto& temp = new T(args...);
		if (temp)
		{
			this->Add(temp);
			temp->SetObjectManager(this);
			temp->Init();
			return temp;
		}
		return nullptr;
	}


private:
	/* @brief	�I�u�W�F�N�g�폜
	 * @param	�Ȃ�
	 * @return	�Ȃ�			*/
	void DestroyCheck(void);

	//! ���g�̑��݂���V�[��
	BaseScene* scene_;
};

#endif // _OBJECT_H_