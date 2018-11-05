/*
 * @file		Debug.h
 * @brief		�f�o�b�O����
 * @author		���V�đ�
 * @data		2018/11/05
 */
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"

class GuiManager;
class Debug : public Interface
{
public:
	Debug(Systems* systems);
	~Debug(void);
	HRESULT Init(void)   override;
	void	Uninit(void) override;
	void    Update(void) override;
	void	Draw(void)   override;
	void    On(void);
	
	// Getter
	inline bool			GetDebug(void)		{ return debug_;		}
	inline bool			GetDebugPause(void) { return debugPause_;	}
	inline GuiManager*	GetGuiManager(void) { return gui_;			}
	// Setter
	inline void SetDebugPause(bool pause) { debugPause_ = pause; }

	/* @fn		BoolToString
	 * @brief	bool�^�̕�����`��
	 * @param	(judge)		������ɂ�����bool�^
	 * @return	���ʂƂȂ镶����		*/
	inline string BoolToString(bool judge) { return (judge) ? "true" : "false"; }

private:
	GuiManager*		gui_;			//! gui�̃}�l�[�W��
	bool			debug_;			//! �f�b�o�O�����ǂ���
	bool			debugPause_;	//! �f�o�b�O�p�̃|�[�Y���s�����ǂ���
};

#endif // _DEBUG_H_