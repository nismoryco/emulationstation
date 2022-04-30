#pragma once
#ifndef ES_APP_SYSTEM_SCREEN_SAVER_H
#define ES_APP_SYSTEM_SCREEN_SAVER_H

#include "Window.h"
#include <thread>

// Screensaver implementation for main window
class SystemScreenSaver : public Window::ScreenSaver
{
public:
	SystemScreenSaver(Window* window);
	virtual ~SystemScreenSaver();

	virtual void startScreenSaver();
	virtual void stopScreenSaver();
	virtual void renderScreenSaver();
	virtual void update(int deltaTime);
	virtual bool isScreenSaverActive();

private:
	void input(InputConfig* config, Input input);

	enum STATE {
		STATE_INACTIVE,
		STATE_FADE_OUT,
		STATE_ACTIVE
	};

private:
	Window*						mWindow;
	STATE						mState;
	float						mOpacity;
};

#endif // ES_APP_SYSTEM_SCREEN_SAVER_H
