//-----------------------------------------------------------------------------
//
//	�I�u�W�F�N�g�Ǘ�[ObjectManager.h]
//	Auther : ���V�đ�
//																	2017/09/05
//-----------------------------------------------------------------------------
#ifndef _OBJECT_MANAGER_H_ 
#define _OBJECT_MANAGER_H_

#include "../Scene/BaseScene.h"

class Object;
class ObjectManager : public BaseManager<Object>
{
public:
	ObjectManager(BaseScene* parent);

	HRESULT Init(void)   override { return S_OK; }
	void    Uninit(void) override;		// �I�u�W�F�N�g�̌㏈��
	void    Update(void) override;		// �I�u�W�F�N�g�̍X�V����
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
	void DestroyCheck(void);

	//! ���g�̑��݂���V�[��
	BaseScene* scene_;
};

#endif // _OBJECT_H_