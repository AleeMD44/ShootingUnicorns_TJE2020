

#ifndef STAGE_H
#define STAGE_H

class Image;
#include "input.h"
#include "Sprite.h"


class Stage
{
public:
	static std::map<std::string, Stage*> s_stages;
	static Stage* current_stage;

	static void changeStage(const char* name);

	Stage(const char* name);

	virtual void render(Image& fb);
	virtual void update(double seconds_elapsed);

};


class MenuStage : public Stage {
public:

	MenuStage() : Stage("menu") {}

	int option = 0;
	void render(Image& fb);
	void update(double seconds_elapsed);
};

class PlayStage : public Stage {
public:
	PlayStage() : Stage("play") {}
	int counter = 0;
	int nenemies = 0;
	int enemiesinscreen = 0;
	int round = 0;
	void render(Image& fb);
	void update(double seconds_elapsed);
	void generateEnemies(int num);
	void restart(const char* map);
	double currenttime = 0.0;
	double totalTimer = 0.4;
};


class TutorialStage : public Stage {
public:
	TutorialStage() : Stage("tutorial") {}
	int counter = 0;
	void render(Image& fb);
	void update(double seconds_elapsed);
};


class StartStage : public Stage {
public:
	StartStage() : Stage("start") {}
	void render(Image& fb);
	void update(double seconds_elapsed);
};


class WinStage : public Stage {
public:
	WinStage() : Stage("win") {}
	void render(Image& fb);
	void update(double seconds_elapsed);
};


class DeadStage : public Stage {
public:
	DeadStage() : Stage("dead") {}
	void render(Image& fb);
	void update(double seconds_elapsed);
};


#endif 