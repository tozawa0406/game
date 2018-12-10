//-----------------------------------------------------------------------------
//
//	�X�R�A�`��[Score.h]
//	Auther : ���V�đ�
//																	2017/07/06
//-----------------------------------------------------------------------------
#include "Score.h"
#include <Windows.h>

#include "../Windows/Windows.h"
#include "../Scene/SceneManager.h"
#include "GameSystems.h"

// ������
Score::Score(SceneManager* parent, bool bZero, int dig) : systems_(Systems::Instance()), parent_(parent), bZero_(bZero), dig_(dig), color_(COLOR(1, 1, 1, 1)), scale_(1), priority_(240)
{
	//�����̌������X�R�A�\���̏���l�̐ݒ�
	for (int digMax = dig_; digMax > 0; --digMax)
	{
		//�����̌J��グ
		SCORE_MAX_ *= 10;
		//�ő剻
		SCORE_MAX_ += 9;
	}

	const auto& temp = systems_->GetTexture()->GetTextureSize((int)Texture::Base::NUMBER);
	VECTOR2 textureSize;
	textureSize.x = temp.x / 5;
	textureSize.y = temp.y / 2;

	texture_ = new CanvasRenderer::Image[dig];

	for (int i = 0; i < dig; ++i)
	{
		(texture_ + i)->Init(240, (int)Texture::Base::NUMBER);
		(texture_ + i)->SetSplit(VECTOR2(5, 2));
		(texture_ + i)->SetSize(textureSize);
		(texture_ + i)->SetEnable(true);
	}
}

Score::~Score(void)
{
	if (texture_ != nullptr)
	{
		for (int i = 0; i < dig_; ++i)
		{
			(texture_ + i)->Uninit();
		}

		delete[] texture_;
		texture_ = nullptr;
	}
}

void Score::Update(int score, VECTOR2 pos)
{
	//���l��
	if (dig_ <= 0)
	{
		//�l���ύX����邽�ߕʂ̂��̂ōs��
		int scoreDig = score;
		// dig�� 0 �ȉ��ł����v
		dig_ = 0;

		//�X�R�A������ꍇ
		for (; scoreDig > 0;)
		{
			//������������
			dig_++;
			//�X�R�A�̌������炷
			scoreDig = (int)(scoreDig * 0.1f);
		}
	}

	//�\���X�R�A������������ꍇ���
	score = min(score, SCORE_MAX_);

	float halfX = Half(texture_->GetSize().x * scale_ * dig_) - Half(texture_->GetSize().x * scale_);
	//���l����
	int num = 1;
	//�X�R�A�������
	while (score > 0)
	{
		int i = num - 1;
		// 1���ڎ��o��
		int n = score % 10;
		//�X�R�A�̌������炷
		score = (int)(score * 0.1);
		//������`��
		(texture_ + i)->SetColor(color_);
		(texture_ + i)->SetScale(scale_);
		(texture_ + i)->SetPattern((float)n);
		VECTOR2 size = (texture_ + i)->GetSize().x * scale_;
		(texture_ + i)->SetPosition(VECTOR2(pos.x + (size.x * dig_) - (size.x * num) - halfX, pos.y));
		(texture_ + i)->SetPriority(priority_);
		(texture_ + i)->SetEnable(true);
		//�\�����̊i�[
		num++;
	}

	// 0�l��
	if (bZero_)
	{
		//�\�������w�肳�ꂽ���ƈ���Ă�����
		while (num != dig_ + 1)
		{
			int i = num - 1;
			// 0��`��
			(texture_ + i)->SetColor(color_);
			(texture_ + i)->SetScale(scale_);
			(texture_ + i)->SetPattern(0);
			VECTOR2 size = (texture_ + i)->GetSize().x * scale_;
			(texture_ + i)->SetPosition(VECTOR2(pos.x + (size.x * dig_) - (size.x * num) - halfX, pos.y));
			(texture_ + i)->SetPriority(priority_);
			(texture_ + i)->SetEnable(true);
			//�\�������𑝂₷
			num++;
		}
	}
	else
	{
		while (num != dig_ + 1)
		{
			int i = num - 1;
			// 0��`��
			(texture_ + i)->SetEnable(false);
			num++;
		}
	}
}

NumBoard::NumBoard(BaseScene* parent, VECTOR2 pos, float scale)
{
	scene_    = parent;
	timeCnt_  = 0;
	position_ = pos;

	min_ = new Score(parent->GetManager(), true, 3);
	min_->SetScale(scale);

	sec_ = new Score(parent->GetManager(), true, 1);
	sec_->SetScale(Quarter(scale) * 2.75f);

	VECTOR2 size = min_->GetSize() * scale;
	VECTOR2 p = position_;
	p.x += size.x * 2 - 5;

	back_.Init(230, (int)Texture::Base::WHITE);
	back_.SetColor(COLOR::RGBA(0, 0, 100, 200));
	back_.SetPosition(p);
	back_.SetSize(VECTOR2(120, 30));
}

NumBoard::~NumBoard(void)
{
	back_.Uninit();
	DeletePtr(sec_);
	DeletePtr(min_);
}

bool NumBoard::Update(void)
{
	if (timeCnt_ >= Score::GAME_TIME * 60)
	{
#ifdef _DEBUG
		return false;
#else
		return true;
#endif
	}

	timeCnt_++;

	VECTOR2 size = sec_->GetSize();
	VECTOR2 p = position_;
	p.x += size.x * 1.25f;
	p.y += 15;

	int t = Score::GAME_TIME - timeCnt_ / 60;
	min_->Update(t, p);

	float diff = size.x * 3;
	int a = 10 - ((timeCnt_ % 60) / 6);
	if (a == 10) { a = 0; }
	sec_->Update(a, VECTOR2(p.x + diff, p.y + 7));

	return false;
}
