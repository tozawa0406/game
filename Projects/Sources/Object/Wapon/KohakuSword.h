/*
 * @file		KohakuSword.h
 * @brief		剣
 * @author		戸澤翔太
 * @data		2018/10/30
 */
#ifndef _KOHAKU_SWORD_H_
#define _KOHAKU_SWORD_H_

#include "Wapon.h"

class KohakuSword : public Wapon, public GUI
{
public:
	KohakuSword(void);
	~KohakuSword(void);

	void Init(void)   override;
	void Uninit(void) override;
	void Update(void) override;

	void GuiUpdate(void) override;

private:
	void PaidSword(void)  override;
	void DrawnSword(void) override;

	bool debug_hit_;		//! デバッグ用
};

#endif // _KOHAKU_SWORD_H_
