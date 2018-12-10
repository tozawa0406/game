/*
 * @file		CanvasRendererImage.h
 * @brief		UI�e�N�X�`���N���X
 * @author		���V�đ�
 * @data		2018/12/10
 */
#ifndef _CANVAS_RENDERER_IMAGE_H_
#define _CANVAS_RENDERER_IMAGE_H_

#include "CanvasRendererBase.h"

namespace CanvasRenderer
{
	class Image : public CanvasRendererBase
	{
	public:
		/* @brief	�R���X�g���N�^		*/
		Image(void);
		/* @brief	�f�X�g���N�^		*/
		~Image(void);

		/* @brief	������
		 * @param	(priority)	�`�揇
		 * @param	(texNum)	�e�N�X�`��
		 * @return	�Ȃ�				*/
		void Init(byte priority, int texNum);
		/* @brief	�㏈��				*/
		void Uninit(void) override;

		/* @brief	�e�N�X�`���擾				*/
		inline int				GetTexNum(void)			const { return texNum_;			}
		/* @brief	�X�P�[���擾				*/
		inline const VECTOR2&	GetScale(void)			const { return	scale_;			}
		/* @brief	�X�P�[���̃I�t�Z�b�g�擾	*/
		inline const VECTOR2&	GetScaleOffset(void)	const { return scaleOffset_;	}
		/* @brief	��]�p�x�擾				*/
		inline float			GetAngle(void)			const { return angle_;			}
		/* @brief	��]�̃I�t�Z�b�g�擾		*/
		inline const VECTOR2&	GetRotationOffset(void) const { return rotationOffset_;	}
		/* @brief	�g�p����V�F�[�_�[�擾		*/
		inline Shader::ENUM		GetShader(void)			const { return shader_;			}
		/* @brief	�A�j���[�V�������擾		*/
		inline float			GetPattern(void)		const { return pattern_;		}
		/* @brief	�������擾					*/
		inline const VECTOR2&	GetSplit(void)			const { return split_;			}

		/* @brief	���_�o�b�t�@�擾			*/
		inline uint			GetBuffer(void)				const { return buffer_;			}
		/* @brief	�v���~�e�B�u�̐��擾		*/
		inline byte			GetPrimitiveNum(void)		const { return primitiveNum_;	}
		/* @brief	�v���~�e�B�u�̎�ގ擾		*/
		inline Wrapper::PRIMITIVE::TYPE GetPrimitiveType(void) const { return primitiveType_; }

		/* @brief	�e�N�X�`���ݒ�				*/
		inline void	SetTexNum(int texNum)						{ texNum_			= texNum;	}
		/* @brief	�X�P�[���ݒ�				*/
		inline void	SetScale(const VECTOR2& scale)				{ scale_			= scale;	}
		/* @brief	�X�P�[���̃I�t�Z�b�g�ݒ�	*/
		inline void	SetScaleOffset(const VECTOR2& offset)		{ scaleOffset_		= offset;	}
		/* @brief	��]�p�x�ݒ�				*/
		inline void	SetAngle(float angle)						{ angle_			= angle;	}
		/* @brief	��]�̃I�t�Z�b�g�ݒ�		*/
		inline void	SetRotationOffset(const VECTOR2& offset)	{ rotationOffset_	= offset;	}
		/* @brief	�g�p����V�F�[�_�[�ݒ�		*/
		inline void	SetShader(Shader::ENUM shader)				{ shader_			= shader;	}
		/* @brief	�A�j���[�V�������ݒ�		*/
		inline void	SetPattern(float pattern)					{ pattern_			= pattern;	}
		/* @brief	�������ݒ�					*/
		inline void	SetSplit(const VECTOR2& split)				{ split_			= split;	}


		/* @brief	�A�j���[�V����
		 * @param	(add)	�A�j���[�V�������x
		 * @return	�A�j���[�V�����I���Ȃ�true	*/
		bool Animation(float add);

	private:
		void CreateVertexBuffer(void);

		int				texNum_;			//! �e�N�X�`��
		VECTOR2			scale_;				//! �X�P�[��
		VECTOR2			scaleOffset_;		//! �X�P�[���̃I�t�Z�b�g
		float			angle_;				//! ��]�p�x
		VECTOR2			rotationOffset_;	//! ��]�̃I�t�Z�b�g
		Shader::ENUM	shader_;			//! �g�p����V�F�[�_�[

		float			pattern_;			//! �A�j���[�V������
		VECTOR2			split_;				//! �e�N�X�`��������

		uint			buffer_;			//! ���_�o�b�t�@
		byte			primitiveNum_;		//! �v���~�e�B�u��
		Wrapper::PRIMITIVE::TYPE primitiveType_;	//! �v���~�e�B�u�̎��
	};
}

#endif // _CANVAS_RENDERER_IMAGE_H_