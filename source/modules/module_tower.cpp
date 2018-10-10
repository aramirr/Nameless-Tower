#include "mcv_platform.h"
#include "module_tower.h"
#include "components/juan/comp_render.h"
#include "render/render_objects.h"
#include "components/lights/comp_light_dir.h"
#include "components/lights/comp_light_point_shadows.h"
#include "components/controllers/comp_door.h"
#include "components/player/comp_player_controller.h"

bool CModuleTower::start()
{
	changeExposure = false;
	return true;
}

void CModuleTower::update(float delta)
{
	if (changeExposure) {
		if (newExposure > oldExposure) {
			cb_globals.global_exposure_adjustment += 0.01f;
			oldExposure = cb_globals.global_exposure_adjustment;
			if (oldExposure > newExposure) {
				if (oldExposure > defaultExposure)newExposure = defaultExposure;
				else changeExposure = false;
			}
		}
		else if (newExposure < oldExposure) {
			cb_globals.global_exposure_adjustment -= 0.005f;
			oldExposure = cb_globals.global_exposure_adjustment;
			if (oldExposure < newExposure) {
				changeExposure = false;
			}
		}
	}
	if (cargar) {
		newExposure = 0.f;
		oldExposure = cb_globals.global_exposure_adjustment;
		defaultExposure = cb_globals.global_exposure_adjustment;
		cargar = false;
	}
	if (bandCinematics && bandsValue < 0.15f) {
		bandsValue += 0.01f;
		cb_globals.global_bandMax_adjustment = bandsValue;
		cb_globals.global_bandMin_adjustment = bandsValue;
	}
	else if (!bandCinematics && bandsValue > 0.f) {
		bandsValue -= 0.01f;
		cb_globals.global_bandMax_adjustment = bandsValue;
		cb_globals.global_bandMin_adjustment = bandsValue;
	}
}

void CModuleTower::render()
{
}

float CModuleTower::getTowerRadius() {
	return tower_radius;
}

const VEC3 CModuleTower::getLastCheckpoint()
{
	return last_checkpoint_position;
}

const void CModuleTower::setLastCheckpoint(VEC3 checkpoint) {
	last_checkpoint_position = checkpoint;
}

const float CModuleTower::getLastCheckpointYaw()
{
	return last_checkpoint_yaw;
}

const void CModuleTower::setLastCheckpointYaw(float checkpoint_yaw) {
	last_checkpoint_yaw = checkpoint_yaw;
}

const bool CModuleTower::getLastCheckpointLeft()
{
	return last_checkpoint_looking_left;
}

const void CModuleTower::setLastCheckpointLeft(bool checkpoint_left) {
	last_checkpoint_looking_left = checkpoint_left;	
}

const void CModuleTower::disappearEntity(const std::string& name) {
  CEntity* entity = (CEntity*)getEntityByName(name);
  if (entity) {
      TCompRender* h_render = entity->get< TCompRender >();
      h_render->is_active = false;
      h_render->refreshMeshesInRenderManager();
  }
}

const void CModuleTower::appearEntity(const std::string& name) {
  CEntity* entity = (CEntity*)getEntityByName(name);
  if (entity) {
      TCompRender* h_render = entity->get< TCompRender >();
      h_render->is_active = true;
      h_render->refreshMeshesInRenderManager();
  }
}

const void CModuleTower::renderOnlyShadows(const std::string& name) {
  CEntity* entity = (CEntity*)getEntityByName(name);
  if (entity) {
      TCompRender* h_render = entity->get< TCompRender >();
      h_render->only_shadows = true;
      h_render->refreshMeshesInRenderManager();
  }
}

const void CModuleTower::renderEverything(const std::string& name) {
  CEntity* entity = (CEntity*)getEntityByName(name);
  if (entity) {
      TCompRender* h_render = entity->get< TCompRender >();
      h_render->only_shadows = false;
      h_render->refreshMeshesInRenderManager();
  }
}


const void CModuleTower::activateCinematic(const std::string& name) {
    CEntity* cinematic = (CEntity*)getEntityByName(name);
    TMsgActivateCinematic activate_cinematic;
    cinematic->sendMsg(activate_cinematic);
}

const void CModuleTower::deactivateCinematic(const std::string& name) {
    CEntity* cinematic = (CEntity*)getEntityByName(name);
    TMsgDeactivateCinematic deactivate_cinematic;
    cinematic->sendMsg(deactivate_cinematic);
}

const void CModuleTower::setAmbientAdjustment(float ambient) {
	cb_globals.global_ambient_adjustment = ambient;
}

const void CModuleTower::setExposureAdjustment(float exposure) {
    cb_globals.global_exposure_adjustment = exposure;

}
const void CModuleTower::setBandsCinematics(bool _band)
{
	bandCinematics = _band;
}
const void CModuleTower::setDirLightIntensity(const std::string& name, float intensity) {
  CEntity* entity = (CEntity*)getEntityByName(name);
  if (entity) {
      TCompLightDir* light_dir = entity->get<TCompLightDir>();
      light_dir->setIntensity(intensity);
  }
}

const void CModuleTower::setPointLightIntensity(const std::string& name, float intensity) {
  CEntity* entity = (CEntity*)getEntityByName(name);
  if (entity) {
      TCompLightPointShadows* light_point = entity->get<TCompLightPointShadows>();
      light_point->setIntensity(intensity);
  }  
}

float CModuleTower::checkAngle(float alpha, VEC3 pos1) {
  if (alpha < 0.f) alpha += deg2rad(360.f);

  float cosa = cos(alpha);
  float sina = sin(alpha);
  float aux = rad2deg(alpha);

  if (pos1.x > 0.f and pos1.z > 0.f) {
    if (cos(alpha) < 0) {
      alpha = deg2rad(180) - alpha;
    }
  }
  else if (pos1.x < 0.f and pos1.z > 0.f) {
    if (cos(alpha) > 0) {
      alpha = deg2rad(180) - alpha;
    }
  }
  else if (pos1.x < 0.f and pos1.z < 0.f) {
    if (cos(alpha) > 0) {
      alpha = deg2rad(180) + (deg2rad(360) - alpha);
    }
  }
  else if (pos1.x > 0.f and pos1.z < 0.f) {
    if (cos(alpha) < 0) {
      alpha = deg2rad(270) + (deg2rad(270) - alpha);
    }
  }

  return alpha;
}

const void CModuleTower::openDoor(const std::string& name) {
    CEntity* entity = (CEntity*)getEntityByName(name);
    TMsgOpenDoor msg;
    entity->sendMsg(msg);
}

const void CModuleTower::closeDoor(const std::string& name) {
    CEntity* entity = (CEntity*)getEntityByName(name);
    TMsgCloseDoor msg;
    entity->sendMsg(msg);
}

const void CModuleTower::activateAnim(const std::string& name) {
	CEntity* entity = (CEntity*)getEntityByName(name);
	TMsgActivateAnim msg;
	entity->sendMsg(msg);
}

void CModuleTower::setExposure(float _exposure) {
	newExposure = _exposure;
	changeExposure = true;
}