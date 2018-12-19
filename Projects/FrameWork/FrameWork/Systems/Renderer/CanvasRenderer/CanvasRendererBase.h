/*
 * @file		CanvasRendererBase.h
 * @brief		UI�`��̊��N���X
 * @author		���V�đ�
 * @data		2018/12/10
 */
#ifndef _CANVAS_RENDERER_BASE_H_
#define _CANVAS_RENDERER_BASE_H_

#include "../RendererBase.h"

class CanvasRendererBase : public RendererBase
{
public:
	//! @enum	UI�`��̎��
	enum class CanvasType : uint8
	{
		Image = 0,
		Text,

		MAX
	};

	/* @brief	�R���X�g���N�^	*/
	CanvasRendererBase(CanvasType type) : RendererBase(RendererType::Canvas), position_(VECTOR2(0)), size_(VECTOR2(0)), color_(COLOR(1)), priority_(0), manager_(nullptr), CanvasType_(type) {}
	/* @brief	�f�X�g���N�^	*/
	virtual ~CanvasRendererBase(void) {}

	/* @brief	�㏈��			*/
	virtual void Uninit() = 0;

	/* @brief	�ʒu�擾(��ʍ��W)		*/
	inline const VECTOR2&	GetPosition(void)		const { return position_;	}
	/* @brief	�傫���擾(��ʍ��W)	*/
	inline const VECTOR2&	GetSize(void)			const { return size_;		}
	/* @brief	�F�̎擾				*/
	inline const COLOR&		GetColor(void)			const { return color_;		}
	/* @brief	�`�揇�擾				*/
	inline		 byte		GetPriority(void)		const { return priority_;	}
	/* @brief	��ނ̎擾				*/
	inline		 CanvasType	GetCanvasType(void)		const { return CanvasType_;	}

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

	VECTOR2		position_;		//! �ʒu(��ʍ��W)
	VECTOR2		size_;			//! �傫��(��ʍ��W)
	COLOR		color_;			//! �F
	byte		priority_;		//! �`�揇

	CanvasRendererManager* manager_;	//! �}�l�[�W���[�ւ̃|�C���^

private:
	CanvasType	CanvasType_;			//! ���
};

#endif // _CANVAS_RENDERER_BASE_H_