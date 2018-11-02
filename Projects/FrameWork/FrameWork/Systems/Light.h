#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Renderer/Model/Model.h"

struct LightInfo
{
	MATERIAL color;
	VECTOR3  position;
	VECTOR3  at;
	VECTOR3  direction;
};

class Systems;
class Light
{
public:
	Light(Systems* systems);
	Light(Systems* systems, VECTOR3 position, VECTOR3 at = VECTOR3(0, 0, 0), COLOR color = COLOR(1, 1, 1, 1));

	LightInfo& GetLight(void) { return light_; }

	void SetLight(void);

private:
	LightInfo light_;

	Systems* systems_;
};

#endif // _LIGHT_H_