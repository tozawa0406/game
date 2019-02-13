#include "DragonScream.h"
#include <random>
#include "../Dragon.h"
#include "../../../Player/Player.h"

static constexpr uint8 VOICE1 = 0x01;
static constexpr uint8 VOICE2 = 0x02;

DragonScream::DragonScream(void) : 
	collider_(nullptr)
	, voiceFlag_(0)
{
}

DragonScream::~DragonScream(void)
{
}

void DragonScream::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);

	collider_ = new Collider3D::Sphere(monster);
	if (collider_)
	{
		collider_->SetSize(125);
		collider_->SetEnable(false);
	}
}

void DragonScream::Uninit(void)
{
	DeletePtr(collider_);
}

void DragonScream::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	voiceFlag_ = 0;

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = DEFAULT_ANIMATION_SPEED;
	meshAnim.animation   = static_cast<int>(Dragon::Animation::SCREAM);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
}

bool DragonScream::Update(void)
{
	if (!monster_) { return true; }

	monster_->SetVelocity(VECTOR3(0));

	if (collider_)
	{
		const auto& hits = collider_->HitCollider();
		for (auto& hit : hits)
		{
			if (hit->GetParentTag() == ObjectTag::PLAYER &&
				hit->GetColliderTag() == ColliderTag::DEFENSE)
			{
				if (attackManager_->CheckList(attackID_))
				{
					Player* player = static_cast<Player*>(hit->GetParent());
					player->Earplug(attackID_);
				}
			}
		}
	}

	Sound*			sound	= nullptr;
	RenderTarget*	rtv		= nullptr;
	if (const auto& sys = Systems::Instance())
	{
		if (const auto& gra = sys->GetGraphics())
		{
			rtv = gra->GetRenderTarget();
		}
		sound = sys->GetSound();
	}

	if (!rtv || !sound) { return false; }

	auto& meshAnim = monster_->GetMeshAnimation();
	float pattern = meshAnim.mesh.GetPattern();
	if (pattern > 70)
	{
		if (rtv->IsFeedbackBlur())
		{
			rtv->FeedbackBlur(30);
		}
	}
	else if (pattern > 50)
	{
		if (!BitCheck(voiceFlag_, VOICE2))
		{
			sound->Play(static_cast<int>(Resources::Sound::Buttle::DRAGON_ROAR));
			BitAdd(voiceFlag_, VOICE2);
		}
	}
	else if (pattern > 20)
	{
		rtv->FeedbackBlur(-1);
		if (collider_)
		{
			collider_->SetEnable(true);
		}
		if (!BitCheck(voiceFlag_, VOICE1))
		{
			sound->Play(static_cast<int>(Resources::Sound::Buttle::DRAGON_ROAR));
			BitAdd(voiceFlag_, VOICE1);
		}

		meshAnim.animSpeed = 0.5f;
	}

	if (monster_->IsEndAnim())
	{
		meshAnim.animSpeed = DEFAULT_ANIMATION_SPEED;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME5, true);
		return true;
	}
	return false;
}

void DragonScream::EndMove(void)
{
	if (collider_)
	{
		collider_->SetEnable(false);
	}
}

void DragonScream::GuiUpdate(void)
{
	if (!monster_) { return; }
	auto& meshAnim = monster_->GetMeshAnimation();
	ImGui::Text("pattern : %.2f", meshAnim.mesh.GetPattern());

	MonsterAttack::GuiUpdate();
}

