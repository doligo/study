#include "Ward.h"
#include "MapDraw.h"
#pragma once

#define WARD_MAX 75

class WardManager : virtual public Ward
{
private:
	MapDraw XY;
public:
	WardManager();
	void Load_Ward(_Ward *name, int ward_max);
	void Set_xy(_Ward *name);
	void Draw_Word(_Ward *name);
	~WardManager();
};

