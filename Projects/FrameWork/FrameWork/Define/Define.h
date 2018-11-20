/*
 * @file	Define.h
 * @brief	�萔�A�\���́A�֐���`�w�b�_
 * @author	���V�đ�
 * @date	2018/10/12
 */
#ifndef _DEFINE_H_
#define _DEFINE_H_

// �A�v���p�Ƀf�o�b�O�@�\��Release�œf���o�����߂�_DEBUG�ł͂����Ȃ�
// �f�o�b�O�@�\��f���o�������Ȃ����ɏ���
// �f�o�b�O�p�ł�����Ȃ��@�\�͂��̂܂܁u_DEBUG�v
#define _SELF_DEBUG

#include <Windows.h>

#include <type_traits>
#include <XInput.h>
#include "Vector.h"
#include "Matrix.h"
#include "Color.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Xinput.lib")

//-----------------------------------------------------------------------------
//	�萔��`
//-----------------------------------------------------------------------------
namespace Define
{
	// @def ���\�[�X�f�B���N�g���ւ̃p�X
	static const string ResourceDirectoryName = "./Resource/";
};

typedef unsigned char uint8;
typedef unsigned int  uint;

static constexpr float PI = 3.14f;

//-----------------------------------------------------------------------------
//	�\���̒�`
//-----------------------------------------------------------------------------
struct VERTEX2D
{
	VECTOR4 position;
	COLOR   color;
	VECTOR2 texcoord;
};

struct VERTEX3D
{
	VECTOR3 position;
	VECTOR3 normal;
	COLOR   color;
	VECTOR2 texcoord;
};
struct VERTEXPOINT
{
	VECTOR3	position;
	float	size;
	COLOR	color;
};
struct VERTEX
{
	VECTOR3 position;
	VECTOR3 normal;
	VECTOR3 tangent;
	COLOR   color;
	VECTOR2 texcoord;
	VECTOR4 boneIndex;
	VECTOR4 weight;
};

//-----------------------------------------------------------------------------
//	�֐���`
//-----------------------------------------------------------------------------
/* @fn		Half
 * @brief	����
 * @param	(origin)	���̒l
 * @return	�����ɂ����l				*/
template<typename Num>
inline Num Half(Num origin) { return (Num)(origin * 0.5f); };
/* @fn		Quarter
 * @brief	1/4�{
 * @param	(origin)	���̒l
 * @return	�v�Z����					*/
template<typename Num>
inline Num Quarter(Num origin) { return (Num)(origin * 0.25f); };
/* @fn		Abs
 * @brief	��Βl
 * @param	(origin)	���̒l
 * @return	��Βl						*/
template<typename Num>
inline Num Abs(Num origin) { return origin >= 0 ? origin : -origin; };

/* @fn		DeletePtr
 * @brief	�Z�[�tdelete 
 * @param	(ptr)	�|�C���^
 * @return	���������s��		*/
template<class Ptr>
inline bool DeletePtr(Ptr& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
		return true;
	}
	return false;
};

/* @fn		UninitDeletePtr
 * @brief	�Z�[�tdelete�A&���̑O�Ɍ㏈��
 * @param	(ptr)	�|�C���^
 * @return	���������s��		*/
template<class Ptr>
inline bool UninitDeletePtr(Ptr& ptr)
{
	if (ptr)
	{
		ptr->Uninit();
		delete ptr;
		ptr = nullptr;
		return true;
	}
	return false;
};

/* @fn		ReleaseDeletePtr
 * @brief	�Z�[�tdelete�A&���̑O��Release����
 * @param	(ptr)	�|�C���^
 * @return	���������s��		*/
template<class Ptr>
inline bool ReleasePtr(Ptr& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
		return true;
	}
	return false;
};

/* @fn		RemoveVectorPtr
 * @brief	vector����erase�A&���̌�delete����
 * @param	(obj)		vector
 * @param	(_this)		���o���z��
 * @return	���������s��		*/
template<class T, class U>
inline bool RemoveVectorPtr(T& obj, U& _this)
{
	for (auto itr = obj.begin(); itr != obj.end();)
	{
		if ((*itr) == _this)
		{
 			auto temp = *itr;
			itr = obj.erase(itr);		//�z��폜
			delete temp;
			temp = nullptr;
			break;
		}
		else
		{
			itr++;
		}
	}
	if (_heapchk() != _HEAPOK)
		DebugBreak();

	obj.shrink_to_fit();
	return true;
}

/* @fn		RemoveVector
 * @brief	vector����erase�Adelete�͂��Ȃ�
 * @param	(obj)		vector
 * @param	(_this)		���o���z��
 * @return	���������s��		*/
template<class T, class U>
inline bool RemoveVector(T& obj, U& _this)
{
	for (auto itr = obj.begin(); itr != obj.end();)
	{
		if ((*itr) == _this)
		{
			itr = obj.erase(itr);		//�z��폜
			break;
		}
		else
		{
			itr++;
		}
	}

	obj.shrink_to_fit();
	return true;
}

/* @fn		Swap
 * @brief	�z�����ւ�����
 * @param	(obj1, obj2)	����ւ���l		*/
template<class T>
inline void Swap(T& obj1, T& obj2)
{
	T temp = obj1;
	obj1 = obj2;
	obj2 = temp;
}

/* @fn		BitCheck
 * @brief	�����Z
 * @param	(var)	�t���O
 * @param	(bit)	�`�F�b�N�t���O
 * @return	�^�Ȃ�true				*/
template<class T>
inline bool BitCheck(const T& var, const T& bit) { return (var & bit) ? true : false; }

/* @fn		BitSub
 * @brief	XOR���Z
 * @param	(var)	�t���O
 * @param	(bit)	�`�F�b�N�t���O
 * @return	�^�Ȃ�true				*/
template<class T>
inline void BitSub(T& var, const T& bit) { (var & bit) ? var ^= bit : var; }

/* @fn		BitAdd
* @brief	OR���Z
* @param	(var)	�t���O
* @param	(bit)	�`�F�b�N�t���O
* @return	�^�Ȃ�true				*/
template<class T>
inline void BitAdd(T& var, const T& bit) { var |= bit; }


#endif //_DEFINE_H_
