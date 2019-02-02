#include "Dragon.h"
#include <FrameWork/Object/ObjectManager.h>
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Graphics/DirectX11/Dx11Wrapper.h>

#include "Attack/DragonScream.h"
#include "Attack/DragonBite.h"
#include "Attack/DragonWingAttack.h"
#include "Attack/DragonTakeOff.h"
#include "Attack/DragonHit.h"
#include "Attack/DragonRush.h"

//! @def	大きさ
static constexpr float SCALE = 0.9f;
//! @def	ライフ
static constexpr int MAX_LIFE = 1500;

//! @def	飛行フラグ
static constexpr uint IS_FLY = 0x0002;

const BONE_COLLISION Dragon::BONE_COLLISION_OFFSET[static_cast<int>(Collision::MAX)] = 
{
//	BODY
	{ "Spine2"			, VECTOR3(-4   ,  1.5f ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3(18,  9   , 15   ) },
//	HEAD							 				  		          	       						        	   
	{ "Head"			, VECTOR3( 5   ,  3    ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 6,  6   ,  6   ) },
//	NECK1							  	  		         	  	       	     						    		   
	{ "Neck2"			, VECTOR3( 1   ,  2    ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 8,  8   , 10   ) },
//	NECK2							  	  		         	  	       	     						        	   
	{ "Neck4"			, VECTOR3( 3   ,  1    ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3(10,  5   ,  5   ) },
//	WING_LL							 		  		  		  											    	   
	{ "Wing2_L"			, VECTOR3(-2   ,  0    ,  0   ), VECTOR3(0.05f,  0.02f , -0.01f ), VECTOR3( 9, 10   ,  4   ) },
//	WING_LC							 									  								    	   
	{ "WingDetail10_L"	, VECTOR3( 0   , -1    ,  3.5f), VECTOR3(0    ,  0     ,  0     ), VECTOR3(20,  4   , 10   ) },
//	WING_LR							  							     								        	   
	{ "WingDetail7_L"	, VECTOR3(-5   ,  0    , -2   ), VECTOR3(0    , -0.015f, -0.005f), VECTOR3(30,  7   ,  2   ) },
//	WING_L_CLAW						  							     								        	   
	{ "WingClaw2_L"		, VECTOR3( 0   , -1.5f , -5   ), VECTOR3(0    ,  0.05f ,  0     ), VECTOR3( 7,  2   ,  2   ) },
//	WING_RL							  																        	   
	{ "Wing2_R"			, VECTOR3(-2   , -0.5f ,  0   ), VECTOR3(0.03f, -0.02f , -0.01f ), VECTOR3( 9,  4   , 10   ) },
//	WING_RC							  																        	   
	{ "WingDetail10_R"	, VECTOR3( 0   ,  1    , -3.5f), VECTOR3(0.05f,  0.01f , -0.01f ), VECTOR3(20, 10   ,  4   ) },
//	WING_RR							 		   			    											    
	{ "WingDetail7_R"	, VECTOR3(-5   ,  0    ,  2   ), VECTOR3(0    ,  0.015f, -0.005f), VECTOR3(30,  7   ,  2   ) },
//	WING_R_CLAW						 		   			    	     								        	   
	{ "WingClaw2_R"		, VECTOR3( 0   ,  2    ,  4.5f), VECTOR3(0    ,  0.04f ,  0     ), VECTOR3( 7,  2   ,  2   ) },
//	ARM_L_UP						 		   		   	 		          							        	   
	{ "UpperArm_L"		, VECTOR3(-2   ,  0    ,  0.5f), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 8,  7   ,  5   ) },
//	ARM_L_UP2						 							        	     	       			        	   
	{ "UpperArm_L"		, VECTOR3( 4   ,  0.75f,  0.5f), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 5,  5   ,  5   ) },
//	ARM_L_DOWN						 							        	     	       			        	   
	{ "LowerArm_L"		, VECTOR3( 2   ,  0    ,  1   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 8,  3   ,  3   ) },
//	ARM_R_UP						 		   		   	       	              						        	   
	{ "UpperArm_R"		, VECTOR3(-2   ,  0    ,  0.5f), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 8,  7   ,  5   ) },
//	ARM_R_UP2						 							        	     	       			        	   
	{ "UpperArm_R"		, VECTOR3( 4   , -0.75f,  0.5f), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 5,  5   ,  5   ) },
//	ARM_R_DOWN						  							        	     	       			        	   
	{ "LowerArm_R"		, VECTOR3( 2   ,  0    ,  1   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 8,  3   ,  3   ) },
//	LEGS_L_UP													        	     	       			        
	{ "UpperLeg_L"		, VECTOR3( 1.5f,  1.5f ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 7,  5   ,  5.5f) },
//	LEGS_L_UP2													        	     	       
	{ "LowerLeg_L"		, VECTOR3( 3   ,  1    ,  1.2f), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 7,  3.5f,  3.5f) },
//	LEGS_L_DOWN													        	     	       		    
	{ "LowerLeg2_L"		, VECTOR3( 1.5f,  1.5f ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 6,  2.5f,  3   ) },
//	LEGS_R_UP													     	 	          			    
	{ "UpperLeg_R"		, VECTOR3( 1.5f, -1.5f ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 7,  5   ,  5.5f) },
//	LEGS_R_UP2													        	     	       		    
	{ "LowerLeg_R"		, VECTOR3( 3   , -1    ,  1.2f), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 7,  3.5f,  3.5f) },
//	LEGS_R_DOWN													        	     	       		        
	{ "LowerLeg2_R"		, VECTOR3( 1.5f, -1.5f ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 6,  2.5f,  3   ) },
//	TAIL5											  		       	  	       					        		   
	{ "Tail5"			, VECTOR3( 0   ,  1.5f ,  0   ), VECTOR3(0.01f,  0     ,  0     ), VECTOR3( 3,  3   ,  5   ) },
//	TAIL4							  		     	  				  	      					               	   
	{ "Tail4"			, VECTOR3( 1   ,  1.5f ,  0   ), VECTOR3(0    ,  0     ,  0.01f ), VECTOR3( 5,  4   ,  4   ) },
//	TAIL3							  		     	  	   	          	      					               	   
	{ "Tail3"			, VECTOR3( 1   ,  1.5f ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 7,  5   ,  5   ) },
//	TAIL2							  		     	  	   	          	       					               	   
	{ "Tail2"			, VECTOR3( 1   ,  1.5f ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 7,  5   ,  5   ) },
//	TAIL1							  		     	  	   	          	       					               	   
	{ "Tail1"			, VECTOR3( 1   ,  1.5f ,  0   ), VECTOR3(0    ,  0     ,  0     ), VECTOR3( 5,  6   ,  6   ) },
};



Dragon::Dragon(void) : GameObject(ObjectTag::ENEMY), GUI(Systems::Instance(), this, "dragon")
	, flag_(0)
	, debugMove_(false)
	, currentAttack_(nullptr)
	, moveController_(nullptr)
	, accumulation_(0)
	, cameraManager_(nullptr)
	, camera_(nullptr)
{
	meshAnim_.animation = static_cast<int>(Animation::WAIT1);
	meshAnim_.animSpeed = 0;

	for (auto& c : collision_) { c = nullptr; }
	for (auto& c : collisionDefense_) { c = nullptr; }

	for (auto& a : attack_) { a = nullptr; }

	life_ = MAX_LIFE;
}

Dragon::~Dragon(void)
{
}

void Dragon::Init(void)
{
	transform_.position		= VECTOR3(0, 0, 50);
	transform_.rotation.y   = PI;
	transform_.scale		= VECTOR3(SCALE);

	meshAnim_.animSpeed = 0.75f;

	meshAnim_.mesh.Init(Systems::Instance(), (int)Resources::Model::Buttle::DRAGON, &transform_);

	CreateCollision();
	
	int arrayNum = static_cast<int>(AttackPattern::SCREAM);
	attack_[arrayNum] = new DragonScream;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	arrayNum = static_cast<int>(AttackPattern::BITE);
	attack_[arrayNum] = new DragonBite;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	arrayNum = static_cast<int>(AttackPattern::WING_ATTACK);
	attack_[arrayNum] = new DragonWingAttack;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	arrayNum = static_cast<int>(AttackPattern::TAKE_OFF);
	attack_[arrayNum] = new DragonTakeOff;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	arrayNum = static_cast<int>(AttackPattern::HIT);
	attack_[arrayNum] = new DragonHit;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	arrayNum = static_cast<int>(AttackPattern::RUSH);
	attack_[arrayNum] = new DragonRush;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	moveController_ = new DragonMoveController;
	if (moveController_)
	{
		moveController_->Init(this);
	}

#ifdef _SELF_DEBUG
	// デバッグ用、敵操作時のカメラ
	// カメラマネージャーの取得
	if (manager_)
	{
		if (const auto& scene = manager_->GetScene())
		{
			if (const auto& sceneManager = scene->GetManager())
			{
				if (const auto& cameraManager = sceneManager->GetCameraManager())
				{
					cameraManager_ = cameraManager;
				}
			}
		}
	}
	assert(cameraManager_);

	// TPSカメラの生成
	if (cameraManager_)
	{
		camera_ = cameraManager_->CreateAtObjCamera(this, 1, VECTOR3(0, 25, -30));
	}
#endif

}

void Dragon::Uninit(void)
{
	UninitDeletePtr(moveController_);

	for (auto& c : collision_)
	{
		DeletePtr(c);
	}
	for (auto& c : collisionDefense_)
	{
		DeletePtr(c);
	}


	for (auto& a : attack_)
	{
		UninitDeletePtr(a);
	}

	// 生成したTPSカメラの後始末
	if (cameraManager_ && camera_)
	{
		cameraManager_->DestroyObjCamera(camera_);
	}
}

void Dragon::Update(void)
{
	meshAnim_.mesh.material.diffuse = COLOR(1, 1, 1, 1);
	if (ded_) { return; }

#ifdef _SELF_DEBUG
	if (!DebugInput())
#endif
	{
		if (moveController_)
		{
			int act = (currentAttack_) ? -1 : 0;
			moveController_->Action(act, flag_);
			if (act >= 0)
			{
				currentAttack_ = attack_[act];
				if (currentAttack_)
				{
					currentAttack_->SetMove();
				}
			}
		}
	}

	isEndAnim_ = meshAnim_.mesh.Animation(meshAnim_.animSpeed);

	if (meshAnim_.animation == static_cast<int>(Animation::TAIL_ATTACK))
	{
		velocity_ = 0;
		if (isEndAnim_)
		{
			transform_.rotation.y += 3.14f;
			transform_.position += front_ * 12.25f;
			meshAnim_.animation = static_cast<int>(Animation::WAIT1);
			meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 30, true);
		}
	}

	if (TakenDamage()) { return; }

	if (currentAttack_)
	{
		if (currentAttack_->Update())
		{
			currentAttack_->EndMove();
			currentAttack_ = nullptr;
		}
	}

	if (!BitCheck(flag_, IS_FLY) && meshAnim_.animation != static_cast<int>(Animation::TAKE_OFF))
	{
		transform_.position.y = 0;
	}
	Move();

	for (auto& c : collision_)
	{
		if (c)
		{
			c->Update();
			transform_.position += c->GetBack();
		}
	}
	for (auto& c : collisionDefense_)
	{
		if (c) { c->Update(); }
	}
}

void Dragon::CreateCollision(void)
{
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& renderer = systems->GetGraphics())
		{
			if (Dx11Wrapper* wrapper = static_cast<Dx11Wrapper*>(renderer->GetWrapper()))
			{
				const auto& model = wrapper->GetModel(meshAnim_.mesh.GetModelNum());

				int max = static_cast<int>(Collision::MAX);
				for (int i = 0; i < max; ++i)
				{
					SetCollision(i, BONE_COLLISION_OFFSET[i], model);
				}
			}
		}
	}
}

void Dragon::SetCollision(int arrayNum, const BONE_COLLISION& offset, const MODEL& model)
{
	collision_[arrayNum] = new Collider3D::OBB(this);
	if (collision_[arrayNum])
	{
		for (auto& bone : model.bone)
		{
			if (bone.name == offset.boneName)
			{
				collision_[arrayNum]->SetParentMtx(&model.transMtx, &bone.nowBone);
				break;
			}
		}

		auto s = transform_.scale;
		collision_[arrayNum]->SetSize(offset.size * s);
		collision_[arrayNum]->SetOffsetPosition(offset.offsetPosition * s);
		collision_[arrayNum]->SetOffsetRotation(offset.offsetRotation * s);
		collisionDefense_[arrayNum] = new Collider3D::OBB(this);
		CreateDefenseCollider(*collision_[arrayNum], collisionDefense_[arrayNum]);
	}
}

bool Dragon::TakenDamage(void)
{
	// 死亡処理
	if (life_ <= 0)
	{
		if (currentAttack_)
		{
			currentAttack_->EndMove(); 
			currentAttack_ = nullptr;
		}

		if (moveController_)
		{
			UninitDeletePtr(moveController_);
		}

		if (isEndAnim_)
		{
			meshAnim_.mesh.AnimEndPattern();
			ded_ = true;
			return true;
		}

		meshAnim_.animSpeed = 0.5f;
		meshAnim_.animation = static_cast<int>(Animation::DIE);
		meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 30, true);

		int pattern = static_cast<int>(meshAnim_.mesh.GetPattern());
		if(pattern >= 35)
		{
			meshAnim_.animSpeed = 0.5f;
		}
		else if(pattern >= 20)
		{
			meshAnim_.animSpeed = 0.25f;
		}

		for (auto& c : collision_)
		{
			DeletePtr(c);
		}

		return true;
	}

	if (life_ > Quarter(MAX_LIFE) && accumulation_ >= MAX_LIFE * 0.3f)
	{
		accumulation_ = 0;		
		if (currentAttack_) { currentAttack_->EndMove(); }
		currentAttack_ = attack_[static_cast<int>(AttackPattern::HIT)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}

	}

	return false;
}

void Dragon::Hit(int damage)
{
	life_ -= damage; 
	accumulation_ += damage;
}

bool Dragon::DebugInput(void)
{
	if (!debugMove_) { return false; }

	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return false; }

	VECTOR2 inputDir;
	// Input
	inputDir.x = (float)ctrl->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir.y = (float)ctrl->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// 正規化
	inputDir = VecNorm(inputDir);

	float inputDash = 1;
	BitSub(flag_, IS_DASH);
	if (ctrl->Press(Input::GAMEPAD_R1, DIK_LSHIFT))
	{
		inputDash = 10;
		BitAdd(flag_, IS_DASH);
	}

	VECTOR3 front = (camera_) ? camera_->GetFrontXPlane() : front_;
	VECTOR3 right = (camera_) ? camera_->GetRightXPlane() : right_;

	velocity_ += front * inputDir.y * inputDash * MOVE_SPEED;
	velocity_ -= right * inputDir.x * inputDash * MOVE_SPEED;

	Animation tempAnim = static_cast<Animation>(meshAnim_.animation);
	if (inputDir != 0)
	{
		if(!currentAttack_)
//		if (tempAnim == Animation::WAIT1 || (tempAnim == Animation::WALK || tempAnim == Animation::RUN))
		{
			meshAnim_.animSpeed = 0.5f;
			meshAnim_.animation = static_cast<int>(Animation::WALK);
			if (BitCheck(flag_, IS_DASH))
			{
				meshAnim_.animSpeed = 0.75f;
				meshAnim_.animation = static_cast<int>(Animation::RUN);
			}
			meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 30);
		}
	}
	else
	{
		if (!currentAttack_)
		{
			if (tempAnim == Animation::WALK || tempAnim == Animation::RUN)
			{
				meshAnim_.mesh.ChangeAnimation(static_cast<int>(Animation::WAIT1), 30);
				meshAnim_.animSpeed = 0.75f;
			}
		}
	}


	if (currentAttack_) { return true; }

	// 咆哮
	if (ctrl->Trigger(Input::GAMEPAD_L2, DIK_O))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::SCREAM)];
		if (currentAttack_) 
		{
			currentAttack_->SetMove();
		}
	}

	// ダッシュ
	if (ctrl->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::RUSH)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}
	}

	if (ctrl->Trigger(Input::GAMEPAD_L3, DIK_N))
	{
		meshAnim_.animation = static_cast<int>(Animation::TAIL_ATTACK);
		meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 15);
	}

	// 被ダメ
	if (ctrl->Trigger(Input::GAMEPAD_CIRCLE, DIK_O))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::HIT)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}
	}

	// 噛みつき
	if (ctrl->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::BITE)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}
	}

	// 翼攻撃
	if (ctrl->Trigger(Input::GAMEPAD_SQUARE, DIK_H))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::WING_ATTACK)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}
	}

	// 飛行
	if (ctrl->Trigger(Input::GAMEPAD_R2, DIK_G))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::TAKE_OFF)];
		if (currentAttack_)
		{
			bool fly = BitCheck(flag_, IS_FLY);

			static_cast<DragonTakeOff*>(currentAttack_)->SetFly(fly);
			currentAttack_->SetMove();

			if(fly)
			{
				BitSub(flag_, IS_FLY);
			}
			else
			{
				BitAdd(flag_, IS_FLY);
			}
		}
	}
	return true;
}

static bool debug_nextFrame_ = false;
int Dragon::tint = static_cast<int>(Dragon::Collision::WING_R_CLAW);

void Dragon::GuiUpdate(void)
{
	BONE_COLLISION ttt;
	auto s = transform_.scale;
	ttt.offsetPosition = collision_[tint]->GetOffsetPosition() / s;
	ImGui::DragFloat3("pos  : ", ttt.offsetPosition, 0.1f);
	collision_[tint]->SetOffsetPosition(ttt.offsetPosition * s);
	ttt.offsetRotation = collision_[tint]->GetOffsetRotation() / s;
	ImGui::DragFloat3("rot  : ", ttt.offsetRotation, 0.01f);
	collision_[tint]->SetOffsetRotation(ttt.offsetRotation * s);
	ttt.size = collision_[tint]->GetSize() / s;
	ImGui::DragFloat3("size : ", ttt.size, 0.1f);
	collision_[tint]->SetSize(ttt.size * s);


	ImGui::Text("Life : %d", life_);
	if (life_ > Quarter(MAX_LIFE))
	{
		ImGui::Text("Accu : %d / %.2f", accumulation_, MAX_LIFE * 0.3f);
	}
	else
	{
		ImGui::Text("life < Quarter(MAX_LIFE)");
	}

	// デバッグ用操作の切り替え
	if (ImGui::Button("ctrl"))
	{
		debugMove_ = !debugMove_;
		if (cameraManager_)
		{
			cameraManager_->ChangeCamera((debugMove_) ? 1 : 0);
		}
	}
	ImGui::SameLine();
	ImGui::Text(" : ");
	ImGui::SameLine();
	ImGui::Text((debugMove_) ? "true" : "false");

	ImGui::Text("state : ");
	if (BitCheck(flag_, IS_DASH))
	{
		ImGui::SameLine();
		ImGui::Text("dash ");
	}
	if (BitCheck(flag_, IS_FLY))
	{
		ImGui::SameLine();
		ImGui::Text("fly ");
	}

	if (currentAttack_)
	{
		currentAttack_->GuiUpdate();
	}
}