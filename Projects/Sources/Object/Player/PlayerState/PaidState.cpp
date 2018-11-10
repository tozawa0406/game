//-----------------------------------------------------------------------------
//
//	Auther : 戸澤翔太
//-----------------------------------------------------------------------------
#include "PaidState.h"
#include "PlayerAction/PaidAction/PaidWaitAction.h"

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
PaidState::PaidState(void)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
PaidState::~PaidState(void)
{
}

void PaidState::Init(PlayerHunter* player, Controller* ctrl)
{
	PlayerState::Init(player, ctrl);

	action_ = new PaidWaitAction;
	if (action_) { action_->Init(player, ctrl); }
}

void PaidState::Update(void)
{
	if (action_)
	{
		const auto& temp = action_->Update();
		if (temp)
		{
			UninitDeletePtr(action_);
			temp->Init(player_, ctrl_);
			action_ = temp;
		}
	}
}

PlayerState* PaidState::Setup(void)
{
	return nullptr;
}

void PaidState::Move(void)
{
}

void PaidState::Attack(void)
{
}

void PaidState::Avoidance(void)
{
}

