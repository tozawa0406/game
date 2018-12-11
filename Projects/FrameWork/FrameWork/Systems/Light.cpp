#include "Light.h"
#include "../Scene/SceneManager.h"

Light::Light(Systems* systems) : GUI(systems, nullptr, "Light")
	, systems_(systems)
	, angle_(-1)
{
	auto& c = light_.color;
	c.diffuse  = COLOR(1, 1, 1, 1);
	c.ambient  = COLOR(0.3f, 0.3f, 0.3f, 0.3f);
	c.emission = COLOR(0, 0, 0, 0);
	c.specular = COLOR(1, 1, 1, 1);
	c.power    = 0;

	for (auto& t : c.texture) { t = 0; }
	for (auto& t : c.textureName) { t = ""; }

	light_.position  = VECTOR3(-100, 0, -150);
	light_.at		 = VECTOR3(0, -10, 0);
	light_.direction = light_.position - light_.at;
	light_.direction = VecNorm(light_.direction);

	SetLight();
}

Light::Light(Systems* systems, VECTOR3 position, VECTOR3 at, COLOR color) : GUI(systems, nullptr, "Light")
	, systems_(systems)
{

	auto& c = light_.color;
	c.diffuse  = color;
	c.ambient  = COLOR(0.1f, 0.1f, 0.1f, 0.1f);
	c.emission = COLOR(0, 0, 0, 0);
	c.specular = COLOR(1, 1, 1, 1);
	c.power    = 0;

	for (auto& t : c.texture) { t = 0; }
	for (auto& t : c.textureName) { t = ""; }

	light_.position  = position;
	light_.at        = at;
	light_.direction = light_.position - light_.at;
	light_.direction = VecNorm(light_.direction);

	SetLight();
}

void Light::Update(void)
{
	angle_ += 0.001f;
	light_.position.x = sinf(angle_) * 100;
	light_.position.y = max(cosf(angle_) * 100, -2);
}

void Light::SetLight(void)
{
	systems_->GetRenderer()->GetWrapper()->SetLight(light_);
}

void Light::GuiUpdate(void)
{
	ImGui::SliderFloat3("position", light_.position, 0.1f, 1);
}
