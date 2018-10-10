#pragma once

#include "../comp_base.h"
#include "../camera/comp_camera.h"

class CTexture;
class CMaterial;
class CRenderToTexture;
class TCompLightSpot;

class TCompDestoyableSon : public TCompBase {

	DECL_SIBLING_ACCESS();

public:

	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);

};