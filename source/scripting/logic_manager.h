
class LogicManager
{
	int playerlife;
	float px, py, pz;

public:
	int numagents;

	LogicManager();
	void RespawnPlayer();
	void TeleportPlayer(float, float, float);
	float GetPlayerLife();
	void printDbg();

	void shake_body();
	//   void GetPlayerPos(float &, float &, float &);
};