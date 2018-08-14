#ifndef INC_COMP_CINEMATIC_H_
#define INC_COMP_CINEMATIC_H_

#include "components/comp_base.h"
#include "camera/camera.h"
#include "comp_camera.h"

struct TCompCinematic: public TCompBase {
	float                   current_time = 0.f;
	float                   time;
	float                   blend_time;
	bool										active = false;
	std::string							spline_name;
	std::string							camera_name;
    void load(const json& j, TEntityParseContext& ctx);
    void update(float dt);
    static void registerMsgs();
    void activate(const TMsgActivateCinematic& msg);
    void deactivate_msg(const TMsgDeactivateCinematic& msg);
    void deactivate();
    DECL_SIBLING_ACCESS();
};

#endif
