#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Renderer/Model/Model.h"
#include "DebugSetting/GUI.h"

struct LightInfo
{
	MATERIAL color;
	VECTOR3  position;
	VECTOR3  at;
	VECTOR3  direction;
};

class Systems;
class Light : public GUI
{
public:
	Light(Systems* systems);
	Light(Systems* systems, VECTOR3 position, VECTOR3 at = VECTOR3(0, 0, 0), COLOR color = COLOR(1, 1, 1, 1));

	LightInfo& GetLightInfo(void) { return light_; }

	void Update(void);
	void SetLight(void);

	void GuiUpdate(void) override;

private:
	LightInfo light_;
	float angle_;

	Systems* systems_;
};

#endif // _LIGHT_H_