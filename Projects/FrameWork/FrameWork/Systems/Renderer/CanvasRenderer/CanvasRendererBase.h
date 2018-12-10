/*
 * @file		CanvasRendererBase.h
 * @brief		UI�`��̊��N���X
 * @author		���V�đ�
 * @data		2018/12/10
 */
#ifndef _CANVAS_RENDERER_BASE_H_
#define _CANVAS_RENDERER_BASE_H_

#include "../../../Define/Define.h"
#include "../../../Graphics/Wrapper.h"
#include "../../GameSystems.h"

class CanvasRendererBase
{
public:
	//! @enum	UI�`��̎��
	enum class Type : uint8
	{
		Image = 0,
		Text,

		MAX
	};

	/* @brief	�R���X�g���N�^	*/
	CanvasRendererBase(Type type) : enable_(true), position_(VECTOR2(0)), size_(VECTOR2(0)), color_(COLOR(1)), priority_(0), manager_(nullptr), type_(type) {}
	/* @brief	�f�X�g���N�^	*/
	virtual ~CanvasRendererBase(void) {}

	/* @brief	�㏈��			*/
	virtual void Uninit() = 0;

	/* @brief	�g�p�󋵊m�F			*/
	inline		 bool	  GetEnable(void)	const { return enable_;	}
	/* @brief	�ʒu�擾(��ʍ��W)		*/
	inline const VECTOR2& GetPosition(void)	const { return position_;	}
	/* @brief	�傫���擾(��ʍ��W)	*/
	inline const VECTOR2& GetSize(void)		const { return size_;		}
	/* @brief	�F�̎擾				*/
	inline const COLOR&	  GetColor(void)	const { return color_;	}
	/* @brief	�`�揇�擾				*/
	inline		 byte	  GetPriority(void) const { return priority_; }
	/* @brief	��ނ̎擾				*/
	inline		 Type	  GetType(void)		const { return type_;		}

	/* @brief	�g�p�󋵐ݒ�			*/
	inline void SetEnable(bool enable)					{ enable_	= enable;	}
	/* @brief	�ʒu�ݒ�(��ʍ��W)		*/
	inline void SetPosition(const VECTOR2& positoin)	{ position_ = positoin; }
	/* @brief	�傫���ݒ�(��ʍ��W)	*/
	inline void SetSize(const VECTOR2& size)			{ size_		= size;		}
	/* @brief	�F�̐ݒ�				*/
	inline void SetColor(const COLOR& color)			{ color_	= color;	}
	/* @brief	�`�揇�ύX				*/
	void		SetPriority(byte priority)				{ priority_ = priority; if (manager_) { manager_->Sort(); } }

protected:
	/* @brief	������(�h���悩��Ăяo��)	*/
	void OnInit(void)
	{
		if (const auto& systems = Systems::Instance()) { if (const auto& manager = systems->GetCanvasRenderer()) { manager_ = manager; manager->Add(this); } }
	}
	/* @brief	�㏈��(�h���悩��Ăяo��)	*/
	void OnUninit(void)
	{
		if (manager_) { manager_->Remove(this); }
	}

	bool		enable_;		//! �g�p��
	VECTOR2		position_;		//! �ʒu(��ʍ��W)
	VECTOR2		size_;			//! �傫��(��ʍ��W)
	COLOR		color_;			//! �F
	byte		priority_;		//! �`�揇

	CanvasRendererManager* manager_;	//! �}�l�[�W���[�ւ̃|�C���^

private:
	Type		type_;			//! ���
};

#endif // _CANVAS_RENDERER_BASE_H_