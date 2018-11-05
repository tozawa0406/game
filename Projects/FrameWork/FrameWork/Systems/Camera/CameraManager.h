//-----------------------------------------------------------------------------
//
//	�J�����Ǘ�[CameraManager.h]
//	Auther : ���V�đ�
//																	2018/01/24
//-----------------------------------------------------------------------------
#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

#include "../../Define/Define.h"
#include "../../Scene/SceneManager.h"
#include "Camera.h"
#include "../GameSystems.h"

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class MoveCamera;
class AtObjCamera;
class MobileSuits;
class CameraManager
{
	friend class SceneManager;
public:
	// �J�����̌�
	static constexpr int MAX_CAMERA = 2;

	// �f�X�g���N�^
	~CameraManager(void);

	int Update(void);		// �X�V����
	void Draw(void);

	// Getter
		   Camera*       GetCamera(void);
	inline MATRIX&       GetView(void)         { return mtxView_;    }
	inline MATRIX&       GetProj(void)         { return mtxProj_;    }
	inline SceneManager* GetSceneManager(void) { return parent_;     }
	inline int           GetMainNum(void)      { return mainCamera_; }
	inline bool          GetLook(void)         { return look_;       }

	// �f�o�b�O�J�����ւ̈ڍs
	void DebugMove(void);

	// �Ǐ]�J�����̍쐬
	Camera* CreateAtObjCamera(Object* obj, int i);
	// �Ǐ]�J�����̍폜
	void	     DestroyObjCamera(Camera* obj);

private:
	// �R���X�g���N�^
	CameraManager(SceneManager* parent);

	// �s��ϊ�
	void CreateMatrix(void);

	// �q
	Camera* camera_[MAX_CAMERA];
	Camera* debugCamera_;

	// �s��
	MATRIX  mtxView_;
	MATRIX  mtxProj_;

	// ���C���J����
	int  mainCamera_;
	int  cameraNum_;

	// �f�o�b�O���Ɏg�p
	bool look_;
	int  frame_;
	int  mainOld_;
	VECTOR3 atOld_;
	VECTOR3 posOld_;
	VECTOR3 diff_;
	VECTOR3 diffPos_;

	SceneManager* parent_;
};

#endif // _CAMERA_H_