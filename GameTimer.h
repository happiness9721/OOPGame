#pragma once

namespace game_framework {
	class GameTimer {
	public:
		int GetTimeCounter();
	private:
		int _timeCounter;
	};
}