#include "mcv_platform.h"
#include "gui_omnidash_arrow_controller.h"
#include "components/player/comp_player_controller.h"
#include "components/camera/comp_camera.h"
#include "render/render_objects.h"

namespace GUI
{
  void COmnidashArrowController::update(float delta)
  {
    if (active) {
      VEC2 position;
      float angle;
			//EngineUI.getCamera().setOrthographic(cb_globals.global_first_resolution_X, cb_globals.global_first_resolution_Y);
      CWidget* od = EngineUI.getWidget("OmnidashImage", true);

      CEntity* player = (CEntity*)getEntityByName("The Player");
      TCompPlayerController* player_c = player->get<TCompPlayerController>();
      CEntity* e_camera = EngineCameras.getOutputCamera();
      TCompCamera* c_camera = e_camera->get< TCompCamera >();
      TCompTransform *c_my_transform = player->get<TCompTransform>();
      const Input::TInterface_Mouse& mouse = EngineInput.mouse();
      VEC3 my_pos = c_my_transform->getPosition();
      VEC3 player_position;
      my_pos.y += 0.55f;
      c_camera->getScreenCoordsOfWorldCoord(my_pos, &player_position);
      player_c->player_position = player_position;
      //std::string str1 = std::to_string(my_pos.x) + ", " + std::to_string(my_pos.y) + " GUI \n";
      //std::string str2 = std::to_string(player_position.x) + ", " + std::to_string(player_position.y) + " GUI \n";
			std::string str2 = std::to_string(mouse._position.x) + ", " + std::to_string(mouse._position.y) + " GUI \n";
      //dbg(str1.c_str());
      dbg(str2.c_str());
      angle = (float)(atan2(/*(*/mouse._position.y /** cb_globals.global_first_resolution_Y / cb_globals.global_resolution_Y)*/ - player_position.y, /*(*/mouse._position.x /** cb_globals.global_first_resolution_X / cb_globals.global_resolution_X)*/ - player_position.x));
			
      //Sacamos la otra esquina de la imagen
      float x = (od->getSize().x) * cos(angle - 0.5f);
      float y = (od->getSize().x) * sin(angle - 0.5f);

      /*if (angle < 0) {
        angle += 360;
      }*/

      VEC2 vec = VEC2(x - player_position.x, y - player_position.y);

      position = VEC2((player_position.x * cb_globals.global_first_resolution_Y / cb_globals.global_resolution_Y) + /*(vec.y * -1)*/x, 
				(player_position.y * cb_globals.global_first_resolution_X / cb_globals.global_resolution_X) + y/*vec.x * -1*/);


      //position.x += od->getSize().x / 2;
      //position.y += od->getSize().y / 2;

      od->setPosition(position);
      od->setRotation(angle);
			//EngineUI.getCamera().setOrthographic(cb_globals.global_first_resolution_X, cb_globals.global_first_resolution_Y);
    }
  }
  void COmnidashArrowController::setActive(bool _active)
  {
    active = _active;
    if (active) {
      EngineUI.activateWidget("Omnidash");
    }
    else {
      EngineUI.desactivateWidget("Omnidash");
    }
  }
}
