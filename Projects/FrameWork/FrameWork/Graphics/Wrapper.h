//-----------------------------------------------------------------------------
//
//	Graphics���Ɉˑ���������[Wrapper.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class Wrapper
{
protected:
	MATRIX inverse_;		// view�̋t�s��

public:	
	static constexpr uint R_ERROR = UINT_MAX;		// �G���[�l
	static constexpr uint S_NULL = UINT_MAX - 1;

	// �v���~�e�B�u�̒l
	class PRIMITIVE
	{
	public:
		// �v���~�e�B�u�̎��
		enum class TYPE
		{
			POINT = 0,
			LINE,
			LINELIST,
			TRIANGLE,
			TRIANGLE_STRIP,
			TRIANGLE_FAN,
		};
		// ���_��
		class V
		{
		public:
			static constexpr int CIRCUMFERENCE		= 100;          // �~��
			static constexpr int POINT				= 1;			// �_
			static constexpr int LINE				= 2;			// ��
			static constexpr int RECT				= 5;			// �l�p(���蔲��)
			static constexpr int FILL_TRIANG		= 3;			// �O�p
			static constexpr int FILL_RECT			= 4;			// �l�p
			static constexpr int FILL_CIRCLE		= 102;			// �~

			// �����蔻��`��Ɏg�p
			static constexpr int COLLIDER_RECTANGLE = 5;
			static constexpr int COLLIDER_CIRCLE	= CIRCUMFERENCE + 1;
			static constexpr int COLLIDER_SEGMENT	= 2;
			static constexpr int COLLIDER_SPHERE	= CIRCUMFERENCE + 1;
		};
	};

	// FVF
	enum class FVF
	{
		VERTEX_2D = 0,
		VERTEX_3D,
		VERTEX_POINT,
		INDEX,
	};
	// �A���t�@�u�����h
	enum class ALFA_BREND
	{
		DEF = 0,
		ADD,
		MAX
	};

	// �e�N�X�`�����
	struct TextureData
	{
		VECTOR2 size;
	};

	// �R���X�g���N�^
	Wrapper(void) : font_(nullptr) { inverse_.Identity(); }
	// �f�X�g���N�^
	virtual ~Wrapper(void) {}

	virtual void Init(void)   {}		// ����������
	virtual void Uninit(void) {}		// �㏈��

	// ���_�o�b�t�@����
	virtual uint    CreateVertexBuffer(const void* v, uint size, uint vnum) = 0;
	// �C���f�b�N�X�o�b�t�@����
	virtual uint    CreateIndexBuffer(const WORD* v, uint vnum)		 = 0;
	// �o�b�t�@�̉��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�̔���͑�������FVF)
	virtual void    ReleaseBuffer(uint number, FVF fvf)    = 0;

	// 2D�`��̊J�n
	virtual void    BeginDrawCanvasRenderer(void) = 0;
	// 2D�`��̏I��
	virtual void    EndDrawCanvasRenderer(void)   = 0;

	// 3D�`��̊J�n
	virtual void    BeginDrawObjectRenderer(void) = 0;
	// 3D�`��̏I��
	virtual void    EndDrawObjectRenderer(void)   = 0;

	// �`��e�N�X�`���̃Z�b�g
	virtual void    SetTexture(int stage, int texNum = -1, int modelNum = -1) = 0;

	// �`�揈��
	virtual void    Draw(const CanvasRenderer::Image*   obj, const Shader* shader) = 0;
	virtual void    Draw(const SpriteRenderer*   obj, const Shader* shader) = 0;
	virtual void    Draw(MeshRenderer*     obj, const Shader* shader) = 0;
	virtual void	Draw(const Particle*	     obj, const Shader* shader) = 0;
	virtual void    Draw(const ColliderRenderer* obj) = 0;

	virtual void DrawQuad(VECTOR2 position, VECTOR2 size, COLOR color = COLOR(1, 1, 1, 1)) = 0;

	// �e�N�X�`���̃��[�h
	virtual HRESULT LoadTexture(string fileName, int texNum, int modelNum = -1) = 0;
	// �e�N�X�`���̉��
	virtual void    ReleaseTexture(int texNum, int modelNum = -1) = 0;
	// �e�N�X�`���T�C�Y�̎擾
	virtual VECTOR2 GetTextureSize(int texNum) = 0;

	// ���f���̓ǂݍ���
	virtual HRESULT LoadModel(string fileName, int modelNum) = 0;
	// ���f���A�j���[�V�����̓ǂݍ���
	virtual HRESULT LoadModelAnimation(string fileName, int parent) = 0;
	// ���f���̉��
	virtual void    ReleaseModel(int modelNum) = 0;

	// view�s��̐���
	virtual MATRIX  CreateViewMatrix(VECTOR3 position, VECTOR3 at, VECTOR3 up) = 0;
	// projection�s��̐���
	virtual MATRIX  CreateProjectionMatrix(int fov, float aspect, float cnear, float cfar) = 0;

	// ���_�V�F�[�_�[�̐���
	virtual uint	CreateVertexShader(string fileName, string method, string version, void* t = nullptr, uint elemNum = 0) = 0;
	// ���_�V�F�[�_�[�̃Z�b�g
	virtual HRESULT	SetVertexShader(uint number)		= 0;
	// ���_�V�F�[�_�[�̉��
	virtual void	ReleaseVertesShader(uint number)	= 0;

	// �s�N�Z���V�F�[�_�[�̐���
	virtual uint	CreatePixelShader(string fileName, string method, string version) = 0;
	// �s�N�Z���V�F�[�_�[�̃Z�b�g
	virtual HRESULT	SetPixelShader(uint number)		= 0;
	// �s�N�Z���V�F�[�_�[�̉��
	virtual void	ReleasePixelShader(uint number) = 0;

	// ���C�g�̐ݒ�
	virtual void SetLight(LightInfo& light) = 0;

	Font* GetFont(void) { return font_; }

protected:
	Font* font_;
};

#endif // _WRAPPER_H