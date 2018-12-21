/*
 * @file		Wrapper.h
 * @brief		�O���t�B�b�N�XAPI�̕`�揈��
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include "../Define/Define.h"

// �O����`
namespace CanvasRenderer { class Image; }
struct SpriteRenderer;
struct MeshRenderer;
class  Particle;
struct ColliderRenderer;
struct LightInfo;
class Shader;
class Font;

class Wrapper
{
protected:
	MATRIX inverse_;		//! view�̋t�s��

public:	
	static constexpr uint R_ERROR = UINT_MAX;		//! �G���[�l
	static constexpr uint S_NULL  = UINT_MAX - 1;	//! ���������̒l

	//! �v���~�e�B�u�̒l
	class PRIMITIVE
	{
	public:
		//! @enum	�v���~�e�B�u�̎��
		enum class TYPE
		{
			POINT = 0,
			LINE,
			LINELIST,
			TRIANGLE,
			TRIANGLE_STRIP,
			TRIANGLE_FAN,
		};
		//! ���_��
		class V
		{
		public:
			static constexpr int CIRCUMFERENCE		= 100;          //! �~��
			static constexpr int POINT				= 1;			//! �_
			static constexpr int LINE				= 2;			//! ��
			static constexpr int RECT				= 5;			//! �l�p(���蔲��)
			static constexpr int FILL_TRIANG		= 3;			//! �O�p
			static constexpr int FILL_RECT			= 4;			//! �l�p
			static constexpr int FILL_CIRCLE		= 102;			//! �~

			//! �����蔻��`��Ɏg�p
			static constexpr int COLLIDER_RECTANGLE = 5;
			static constexpr int COLLIDER_CIRCLE	= CIRCUMFERENCE + 1;
			static constexpr int COLLIDER_SEGMENT	= 2;
			static constexpr int COLLIDER_SPHERE	= CIRCUMFERENCE + 1;
		};
	};

	//! @enum	FVF
	enum class FVF
	{
		VERTEX_2D = 0,
		VERTEX_3D,
		VERTEX_POINT,
		INDEX,
	};
	//! @enum	�A���t�@�u�����h
	enum class ALFA_BREND
	{
		DEF = 0,
		ADD,
		MAX
	};

	//! �e�N�X�`�����
	struct TextureData
	{
		VECTOR2 size;
	};

	/* @brief	�R���X�g���N�^		*/
	Wrapper(void) : font_(nullptr) { inverse_.Identity(); }
	/* @brief	�f�X�g���N�^		*/
	virtual ~Wrapper(void) {}

	/* @brief	����������			*/
	virtual HRESULT Init(void) = 0;
	/* @brief	�㏈��				*/
	virtual void	Uninit(void) {}

	/* @brief	���_�o�b�t�@�̍쐬����			*/
	virtual uint    CreateVertexBuffer(const void* v, uint size, uint vnum) = 0;
	/* @brief	�C���f�b�N�X�o�b�t�@�̍쐬����	*/
	virtual uint    CreateIndexBuffer(const WORD* v, uint vnum)		 = 0;
	/* @brief	�o�b�t�@�̉��					*/
	virtual void    ReleaseBuffer(uint number, FVF fvf)    = 0;

	/* @brief	2D�`��̊J�n����				*/
	virtual void    BeginDrawCanvasRenderer(void)	= 0;
	/* @brief	3D�`��̊J�n����				*/
	virtual void    BeginDrawObjectRenderer(void)	= 0;
	/* @brief	�`��̏I������					*/
	virtual void    EndDrawRenderer(void)			= 0;

	/* @brief	�e�N�X�`���̐ݒ菈��			*/
	virtual void    SetTexture(int stage, int texNum = -1, int modelNum = -1) = 0;

	/* @brief	2D�`�揈��						*/
	virtual void    Draw(const CanvasRenderer::Image*   obj, const Shader* shader) = 0;
	/* @brief	�|���S���`�揈��				*/
	virtual void    Draw(const SpriteRenderer*   obj, const Shader* shader) = 0;
	/* @brief	���f���`�揈��					*/
	virtual void    Draw(MeshRenderer*     obj, const Shader* shader) = 0;
	/* @brief	�p�[�e�B�N���`�揈��			*/
	virtual void	Draw(const Particle*	     obj, const Shader* shader) = 0;
	/* @brief	�����蔻��`�揈��				*/
	virtual void    Draw(const ColliderRenderer* obj) = 0;

	/* @brief	��`�`�揈��					*/
	virtual void DrawQuad(VECTOR2 position, VECTOR2 size, COLOR color = COLOR(1, 1, 1, 1)) = 0;

	/* @brief	�e�N�X�`���̓ǂݍ���			*/
	virtual HRESULT LoadTexture(string fileName, int texNum, int modelNum = -1) = 0;
	/* @brief	�e�N�X�`���̉��				*/
	virtual void    ReleaseTexture(int texNum, int modelNum = -1) = 0;
	/* @brief	�e�N�X�`���̃T�C�Y�擾			*/
	virtual VECTOR2 GetTextureSize(int texNum) = 0;

	/* @brief	���f���̓ǂݍ���				*/
	virtual HRESULT LoadModel(string fileName, int modelNum) = 0;
	/* @brief	���f���A�j���[�V�����̓ǂݍ���	*/
	virtual HRESULT LoadModelAnimation(string fileName, int parent) = 0;
	/* @brief	���f���̉��					*/
	virtual void    ReleaseModel(int modelNum) = 0;

	/* @brief	���_�V�F�[�_�[�̍쐬			*/
	virtual uint	CreateVertexShader(string fileName, string method, string version, void* t = nullptr, uint elemNum = 0) = 0;
	/* @brief	���_�V�F�[�_�[�̐ݒ�			*/
	virtual HRESULT	SetVertexShader(uint number)		= 0;
	/* @brief	���_�V�F�[�_�[�̉��			*/
	virtual void	ReleaseVertesShader(uint number)	= 0;

	/* @brief	�s�N�Z���V�F�[�_�[�̍쐬		*/
	virtual uint	CreatePixelShader(string fileName, string method, string version) = 0;
	/* @brief	�s�N�Z���V�F�[�_�[�̐ݒ�		*/
	virtual HRESULT	SetPixelShader(uint number)		= 0;
	/* @brief	�s�N�Z���V�F�[�_�[�̉��		*/
	virtual void	ReleasePixelShader(uint number) = 0;

	/* @brief	���C�g�̐ݒ�					*/
	virtual void SetLight(LightInfo& light) = 0;

	/* @brief	�����`��N���X�̎擾			*/
	inline Font* GetFont(void) { return font_; }

protected:
	//! �����`��N���X
	Font* font_;
};

#endif // _WRAPPER_H