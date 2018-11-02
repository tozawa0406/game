#include "Light.h"
#include "../Scene/SceneManager.h"

Light::Light(Systems* systems) : systems_(systems)
{
	auto& c = light_.color;
	c.diffuse  = COLOR(1, 1, 1, 1);
	c.ambient  = COLOR(0.3f, 0.3f, 0.3f, 0.3f);
	c.emission = COLOR(0, 0, 0, 0);
	c.specular = COLOR(1, 1, 1, 1);
	c.power    = 0;

	c.texture = 0;
	c.textureName = "";

	light_.position  = VECTOR3(0, 100, -150);
	light_.at		 = VECTOR3(0, -10, 0);
	light_.direction = light_.position - light_.at;
	light_.direction = VecNorm(light_.direction);

	SetLight();
}

Light::Light(Systems* systems, VECTOR3 position, VECTOR3 at, COLOR color) : systems_(systems)
{

	auto& c = light_.color;
	c.diffuse  = color;
	c.ambient  = COLOR(0.1f, 0.1f, 0.1f, 0.1f);
	c.emission = COLOR(0, 0, 0, 0);
	c.specular = COLOR(1, 1, 1, 1);
	c.power    = 0;

	c.texture = 0;
	c.textureName = "";

	light_.position  = position;
	light_.at        = at;
	light_.direction = light_.position - light_.at;
	light_.direction = VecNorm(light_.direction);

	SetLight();
}

void Light::SetLight(void)
{
	systems_->GetRenderer()->GetWrapper()->SetLight(light_);
}
