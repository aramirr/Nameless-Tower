#include "mcv_platform.h"
#include "gui_omnidash_arrow_controller.h"
#include "components/player/comp_player_controller.h"
#include "components/camera/comp_camera.h"

namespace GUI
{
  void COmnidashArrowController::update(float delta)
  {
    if (active) {
      VEC2 position;
      float angle;

      CEntity* player = (CEntity*)getEntityByName("The Player");
	    CEntity* e_camera = EngineCameras.getOutputCamera();
      TCompCamera* c_camera = e_camera->get< TCompCamera >();
      TCompTransform *c_my_transform = player->get<TCompTransform>();
      const Input::TInterface_Mouse& mouse = EngineInput.mouse();
      VEC3 my_pos = c_my_transform->getPosition();
      VEC3 player_position;
      c_camera->getScreenCoordsOfWorldCoord(my_pos, &player_position);
      
      angle = (float)(atan2(mouse._position.y - player_position.y, mouse._position.x - player_position.x));

      /*if (angle < 0) {
        angle += 360;
      }*/

      position = VEC2(player_position.x, player_position.y);

      CWidget* od = EngineUI.getWidget("OmnidashImage", true);
      od->setPosition(position);
      od->setRotation(angle);
    }
  }
  void COmnidashArrowController::setActive(bool _active)
  {
    /*active = _active;
    if (active) {
      EngineUI.activateWidget("Omnidash");
    }
    else {
      EngineUI.desactivateWidget("Omnidash");
    }*/
  }

  
}
