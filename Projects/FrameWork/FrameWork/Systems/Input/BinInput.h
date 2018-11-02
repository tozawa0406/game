//-----------------------------------------------------------------------------
//
//	バイナリファイルのInput[BinInput.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _BININPUT_H_
#define _BININPUT_H_

#include "PS4Input.h"
#include "../../Define/Define.h"

static const string FILE_NAME = Define::ResourceDirectoryName +  "Data/ctrlData";
static const string FILE_EX = ".bin";

//-----------------------------------------------------------------------------
// クラス宣言
//-----------------------------------------------------------------------------
class BinInput : public BaseInput
{
	friend Input;

	friend class Enemy;

	struct BIN_EXCHANGE
	{
		WORD button;
		WORD axis;
	};

public:
	static constexpr int TIME_FRAME = 10800;

	void GuiUpdate(void) override;

	void WriteBin(WORD button, WORD axis);

	BinInput(string filename, bool read);
	~BinInput(void);
private:
	void Update(INPUT_STATE& state) override;
	void ReadBin(void);
	void WriteUninit(void);

	void GuiCheck(const WORD& button, const WORD& axis);

	int  AxisJudge(WORD& gamepad, WORD& axis);

	INPUT_BUTTON button_;
	INPUT_BUTTON axis_;

	string filename_;
	bool end_;
	bool read_;
	int frameCnt_;
	BIN_EXCHANGE data_[TIME_FRAME];
};

#endif