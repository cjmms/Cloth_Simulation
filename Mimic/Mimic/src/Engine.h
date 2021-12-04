#pragma once
#include "Scene.h"


class Engine
{


public:
	void init();
	void run();
	void close();

public:
	Scene *scene;
};

