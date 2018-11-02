//-----------------------------------------------------------------------------
//
//	�I�u�W�F�N�g[Object.h]
//	Auther : ���V�đ�
//																	2017/09/05
//-----------------------------------------------------------------------------
#ifndef _OBJECT_H_ 
#define _OBJECT_H_

#include "../Define/Define.h"
#include "../Systems/GameSystems.h"
#include "../Systems/Renderer/Sprite/CanvasRenderer.h"
#include "../Systems/Renderer/Sprite/SpriteRenderer.h"
#include "../Systems/Renderer/Model/MeshRenderer.h"

#include <FrameWork/Systems/DebugSetting/GUI.h>

class Controller;
class BaseScene;
class GuiManager;
class Systems;
class Object
{
public:
	enum class Tag
	{
		UKOWN = -1,
		PLAYER = 0,
		UI,
		BILLBOARD,
		STATIC,
		ENEMY,
		WAPON,
		MAX
	};

	friend class ObjectManager;
protected:
	virtual void Init(void)   = 0;			// ����������
	virtual void Uninit(void) = 0;			// �㏈��
	virtual void Update(void) = 0;			// �X�V����
	virtual void DrawShadow(void) {}

	/* @fn		SetTag
	 * @briel	�^�O�̐ݒ�
	 * @param	(tag)	�ݒ肵�����^�O		*/
	void SetTag(Tag tag)  { tag_	 = tag; }
	void Destroy(void)	  { destroy_ = true; }
	bool GetDestroy(void) { return destroy_; }

	Controller* GetCtrl(int i);

	Transform  transform_;

	ObjectManager* manager_;

public:
	Object(Tag tag);		// �R���X�g���N�^
	virtual ~Object(void);			// �f�X�g���N�^

	Transform& GetTransform(void) { return transform_; }
	Tag        GetTag(void)       { return tag_;       }

	/* @fn		SetObejctManager
	 * @brief	�}�l�[�W���[�̐ݒ�
	 * @param	(manager)	���g�̊Ǘ���		*/
	inline void SetObjectManager(ObjectManager* manager) { manager_ = manager; }

private:
	bool destroy_;
	Tag  tag_;

	friend GuiManager;
};

#endif // _OBJECT_H_