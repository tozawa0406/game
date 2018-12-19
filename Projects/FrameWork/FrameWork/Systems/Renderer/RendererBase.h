/*
 * @file		RendererBase.h
 * @brief		�`��̊��N���X
 * @author		���V�đ�
 * @data		2018/12/20
 */
#ifndef _RENDERER_BASE_H_
#define _RENDERER_BASE_H_

#include "../../Define/Define.h"
#include "../../Graphics/Wrapper.h"
#include "../GameSystems.h"

class RendererBase
{
public:
	//! @enum	UI�`��̎��
	enum class RendererType : uint8
	{
		Canvas = 0,
		Object,
		MAX
	};

	/* @brief	�R���X�g���N�^	*/
	RendererBase(RendererType type) : enable_(true), rendererType_(type) {}
	/* @brief	�f�X�g���N�^	*/
	virtual ~RendererBase(void) {}

	/* @brief	�㏈��			*/
	virtual void Uninit() = 0;

	/* @brief	�g�p�󋵊m�F			*/
	inline bool			GetEnable(void)			const  { return enable_;		}
	/* @brief	�`��^�C�v�m�F			*/
	inline RendererType GetRendererTypr(void)	const { return rendererType_;	}

	/* @brief	�g�p�󋵐ݒ�			*/
	inline void SetEnable(bool enable) { enable_ = enable;	}

protected:
	bool			enable_;			//! �g�p��

private:
	RendererType	rendererType_;		//! ���
};

#endif // _RENDERER_BASE_H_