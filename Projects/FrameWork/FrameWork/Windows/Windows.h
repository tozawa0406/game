/*
 * @file		Windows.h
 * @brief		Window�̏����S��
 * @author		���V�đ�
 * @data		2018/05/10
 */
#ifndef _WINDOWS_H_
#define _WINDOWS_H_

#include "../Define/Define.h"
#include "../Graphics/Graphics.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Xinput.lib")

class Systems;
class Windows
{
	//! @def	�E�B���h�E�̖��O
	static constexpr char*	CLASS_NAME		= "Dragon Hunter";
	//! @def	�E�B���h�E�̖��O
	static constexpr char*	WINDOW_NAME		= "Dragon Hunter";
	//! @def	�E�B���h�E�X�^�C��
	static constexpr int	WINDOW_STYLE	= WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX;
	//! �O���t�B�b�N�X�̃^�C�v(�_�C�A���O�Ŏw��(����͂��Ȃ�))
	Graphics::Type			graphicsType_	= Graphics::Type::DirectX11;

public:
	//! @def	�t���X�N���[���ݒ�
	static constexpr bool FULL_SCREEN	= false;
	//! @def	FPS�̐ݒ�
	static constexpr int  FPS			= 60;
	//! @def	�E�B���h�E���T�C�Y
	static constexpr int  WIDTH			= 1280;
	//! @def	�E�B���h�E�c�T�C�Y
	static constexpr int  HEIGHT		= 720;

	/* @brief	�R���X�g���N�^		*/
	Windows(void);
	/* @brief	�f�X�g���N�^		*/
	~Windows(void);

	/* @brief	FPS�̎擾����		*/
	inline float		GetFps(void)		const { return fps_;		}
	/* @brief	HWND�̎擾����		*/
	inline HWND			GetHWND(void)		const { return hWnd_;		}
	/* @brief	MSG�̎擾����		*/
	inline MSG			GetMsg(void)		const { return msg_;		}
	/* @brief	HINSTANCE�̎擾����	*/
	inline HINSTANCE	GetHInstance(void)	const { return hInstance_;	}
	/* @brief	�O���t�B�b�N�X�̎�ގ擾	*/
	inline Graphics::Type GetGraphicsType(void) const { return graphicsType_; }

	/* @brief	Graphics�̎擾����	*/
	inline Graphics*	GetGraphics(void)	{ return graphics_;	}
	/* @brief	Systems�̎擾����	*/
	inline Systems*		GetSystems(void)	{ return systems_;	}



	/* @brief	����������
	 * @sa		main()
	 * @param	(Instancs)	HINSTANCE
	 * @param	(cmdShow)	cmdShow
	 * @return	�������s			*/
	HRESULT Init(HINSTANCE Instance, int cmdShow);

	/* @brief	�Q�[�����[�v����
	 * @sa		main()
	 * @param	(fps)	�ݒ肵����FPS
	 * @return	�Ȃ�				*/
	void    GameLoop(DWORD fps);

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	MSG.wParam�̒l		*/
	WPARAM  Uninit(void);



	/* @brief	�G���[���b�Z�[�W�E�B���h�E�̏o��
	 * @param	(message)	�G���[���e
	 * @param	(error)		�E�B���h�E��(�G���[�̃W������)
	 * @param	(hr)		�������s�̔���
	 * @return	hr�����s�Ȃ�true�F�����Ȃ�false		*/
	bool ErrorMessage(const char* message, char* error, HRESULT hr);

private:
	/* @brief	�E�B���h�E���̐ݒ�
	 * @sa		Init()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void    SetWindowInfo(void);

	/* @brief	�E�B���h�E�̐���
	 * @sa		Init()
	 * @param	(cmdShow)	cmdShow
	 * @return	�Ȃ�				*/
	void    SetWindow(int cmdShow);

	/* @brief	���b�Z�[�W�v���Z�X
	 * @sa		GameLoop()
	 * @param	�Ȃ�
	 * @return	�E�B���h�E�������true	*/
	bool    MsgProcess(void);

	/* @brief	�X�V����
	 * @sa		GameLoop()
	 * @param	�Ȃ�
	 * @return	Graphics��Systems�̏������Ɏ��s������true		*/
	bool    Update(void);

	/* @brief	�`�揈��
	 * @sa		GameLoop()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void    Draw(void);

	/* @brief	�Q�[���n�̏���������
	 * @param	(type)	�O���t�B�b�N�X�̎��
	 * @return	�������Ɏ��s������true		*/
	bool	InitGame(Graphics::Type type);

	/* @brief	Windows�̒��S��Ԃ�
	 * @sa		SetWindow()
	 * @param	(dr)	�E�B���h�E�̉E
	 * @param	(wr)	�����ʒu�̉E
	 * @return	���S				*/
	inline LONG WindowCenter(LONG dr, LONG wr) { return Half(dr - wr) > 0 ? Half(dr - wr) : 0; }

	Graphics*	graphics_;		//! �O���t�B�b�N�XAPI�n�̏���
	Systems*	systems_;		//! �V�X�e���n

	HWND		hWnd_;
	HINSTANCE	hInstance_;
	MSG			msg_;
	WNDCLASSEX	wcex_;

	int			timeNow_;		//! ���ݎ���
	int			timeOld_;		//! �O�̎���
	int			timeFPS_;		//! FPS�J�E���g���̎���
	int			fpsCnt_;		//! FPS�v���J�E���^
	float		fps_;			//! FPS

	/* @brief	�E�B���h�E�v���V�[�W��		*/
	static LRESULT CALLBACK WndProc(HWND hWnd, uint uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // _WINDOWS_H