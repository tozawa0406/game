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
	void Save(const string& key, T value) { UNREFERENCED_PARAMETER(key); UNREFERENCED_PARAMETER(value); }
	template<>
	void Save(const string& key, int value) { intList_[key] = value; }
	template<>
	void Save(const string& key, float value) { floatList_[key] = value; }

	/* @brief	�l�̃��[�h����
	 * @param	(num)			���X�g�ԍ�
	 * @param	(defaultValue)	�L�[���Ȃ��������ɋA���Ă���l
	 * @return	�l					*/
	template<class T>
	T		Load(const string& key, T defaultValue) { return num; }
	template<>
	int		Load<int>(const string& key, int defaultValue) { return (intList_.count(key) == 0) ? defaultValue : intList_[key]; }
	template<>
	float	Load<float>(const string& key, float defaultValue) { return (floatList_.count(key) == 0) ? defaultValue : floatList_[key]; }

private:
	//! int���i�[����}�b�v
	std::unordered_map<string, int>	intList_;
	//! float���i�[����}�b�v
	std::unordered_map<string, float>	floatList_;

};

#endif // _DONT_DESTROY_ON_LOAD_H_
