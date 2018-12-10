/*
 * @file		CanvasRendererImage.h
 * @brief		UIテクスチャクラス
 * @author		戸澤翔太
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
		/* @brief	コンストラクタ		*/
		Image(void);
		/* @brief	デストラクタ		*/
		~Image(void);

		/* @brief	初期化
		 * @param	(priority)	描画順
		 * @param	(texNum)	テクスチャ
		 * @return	なし				*/
		void Init(byte priority, int texNum);
		/* @brief	後処理				*/
		void Uninit(void) override;

		/* @brief	テクスチャ取得				*/
		inline int				GetTexNum(void)			const { return texNum_;			}
		/* @brief	スケール取得				*/
		inline const VECTOR2&	GetScale(void)			const { return	scale_;			}
		/* @brief	スケールのオフセット取得	*/
		inline const VECTOR2&	GetScaleOffset(void)	const { return scaleOffset_;	}
		/* @brief	回転角度取得				*/
		inline float			GetAngle(void)			const { return angle_;			}
		/* @brief	回転のオフセット取得		*/
		inline const VECTOR2&	GetRotationOffset(void) const { return rotationOffset_;	}
		/* @brief	使用するシェーダー取得		*/
		inline Shader::ENUM		GetShader(void)			const { return shader_;			}
		/* @brief	アニメーション数取得		*/
		inline float			GetPattern(void)		const { return pattern_;		}
		/* @brief	分割数取得					*/
		inline const VECTOR2&	GetSplit(void)			const { return split_;			}

		/* @brief	頂点バッファ取得			*/
		inline uint			GetBuffer(void)				const { return buffer_;			}
		/* @brief	プリミティブの数取得		*/
		inline byte			GetPrimitiveNum(void)		const { return primitiveNum_;	}
		/* @brief	プリミティブの種類取得		*/
		inline Wrapper::PRIMITIVE::TYPE GetPrimitiveType(void) const { return primitiveType_; }

		/* @brief	テクスチャ設定				*/
		inline void	SetTexNum(int texNum)						{ texNum_			= texNum;	}
		/* @brief	スケール設定				*/
		inline void	SetScale(const VECTOR2& scale)				{ scale_			= scale;	}
		/* @brief	スケールのオフセット設定	*/
		inline void	SetScaleOffset(const VECTOR2& offset)		{ scaleOffset_		= offset;	}
		/* @brief	回転角度設定				*/
		inline void	SetAngle(float angle)						{ angle_			= angle;	}
		/* @brief	回転のオフセット設定		*/
		inline void	SetRotationOffset(const VECTOR2& offset)	{ rotationOffset_	= offset;	}
		/* @brief	使用するシェーダー設定		*/
		inline void	SetShader(Shader::ENUM shader)				{ shader_			= shader;	}
		/* @brief	アニメーション数設定		*/
		inline void	SetPattern(float pattern)					{ pattern_			= pattern;	}
		/* @brief	分割数設定					*/
		inline void	SetSplit(const VECTOR2& split)				{ split_			= split;	}


		/* @brief	アニメーション
		 * @param	(add)	アニメーション速度
		 * @return	アニメーション終了ならtrue	*/
		bool Animation(float add);

	private:
		void CreateVertexBuffer(void);

		int				texNum_;			//! テクスチャ
		VECTOR2			scale_;				//! スケール
		VECTOR2			scaleOffset_;		//! スケールのオフセット
		float			angle_;				//! 回転角度
		VECTOR2			rotationOffset_;	//! 回転のオフセット
		Shader::ENUM	shader_;			//! 使用するシェーダー

		float			pattern_;			//! アニメーション数
		VECTOR2			split_;				//! テクスチャ分割数

		uint			buffer_;			//! 頂点バッファ
		byte			primitiveNum_;		//! プリミティブ数
		Wrapper::PRIMITIVE::TYPE primitiveType_;	//! プリミティブの種類
	};
}

#endif // _CANVAS_RENDERER_IMAGE_H_