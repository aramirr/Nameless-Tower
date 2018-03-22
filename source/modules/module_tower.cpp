#include "mcv_platform.h"
#include "module_tower.h"


bool CModuleTower::start()
{
	return true;
}

void CModuleTower::update(float delta)
{
}

void CModuleTower::render()
{
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