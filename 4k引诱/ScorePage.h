#pragma once
#include "Page.h"

class ScorePage : public Page {
public:
	ScorePage(Application& app, int score);

	void Paint() override;
private:
	int _score;
};

