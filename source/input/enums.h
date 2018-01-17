#pragma once

namespace Input
{
	enum class PlayerId : int
	{
		PLAYER_1 = 0, PLAYER_2, PLAYER_3, PLAYER_4
	};

	static const int NUM_PLAYERS = 4;
	static const int NUM_KEYBOARD_KEYS = 256;
};
