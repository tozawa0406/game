/*
 * @file		DontDestroyOnLoad.h
 * @brief		�V�[���Ԃɒl��ێ�����N���X
 * @author		���V�đ�
 * @data		2019/01/21
 */
#ifndef _DONT_DESTROY_ON_LOAD_H_
#define _DONT_DESTROY_ON_LOAD_H_

#include <FrameWork/Define/Define.h>
#include <unordered_map>

enum class DontDestroyList : uint8
{
	PLAYER_STATE,
	MAX_LIFE,
	MAX_STAMINE,
	CURRENT_LIFE,
	CURRENT_STAMINA,
	STAMINA_CNT,
	TIMER_TIME,
	TIMER_FRAME,
	TIMER_SECOND,
	CURRENT_ITEM,

	MAX
};

class DontDestroyOnLoad
{
public:
	/* @brief	�R���X�g���N�^		*/
	DontDestroyOnLoad(void);
	/* @brief	�f�X�g���N�^		*/
	~DontDestroyOnLoad(void);

	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void);

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void);

	/* @brief	���Z�b�g����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Reset(void);

	/* @brief	�l�̃Z�[�u����
	 * @param	(type)		�^�w��(�e���v���[�g�Ɛ�΂ɍ��킹�邱��)
	 * @param	(num)		���X�g�ԍ�(�d�����Ă����ꍇ�㏑��)
	 * @param	(value)		�ێ�����l
	 * @return	�Ȃ�				*/
	template<class T>
	void Save(DontDestroyList num, T value) {}
	template<>
	void Save(DontDestroyList num, int value) { intList_[num] = value; }	
	template<>
	void Save(DontDestroyList num, float value) { floatList_[num] = value; }

	/* @brief	�l�̃��[�h����
	 * @param	(num)		���X�g�ԍ�
	 * @return	�l					*/
	template<class T>
	T		Load(DontDestroyList num) { return num; }
	template<>
	int		Load<int>(DontDestroyList num) { return (intList_.count(num) == 0) ? 0 : intList_[num]; }
	template<>
	float	Load<float>(DontDestroyList num) { return (floatList_.count(num) == 0) ? 0 : floatList_[num]; }


private:
	//! int���i�[����}�b�v
	std::unordered_map<DontDestroyList, int>	intList_;
	//! float���i�[����}�b�v
	std::unordered_map<DontDestroyList, float>	floatList_;

};

#endif // _DONT_DESTROY_ON_LOAD_H_
