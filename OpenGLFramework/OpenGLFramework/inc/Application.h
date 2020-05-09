#pragma once

class Application {
public:
	virtual bool Start() = 0;
	virtual bool Update() = 0;
	virtual void Draw() = 0;
	virtual void Exit() = 0;
};