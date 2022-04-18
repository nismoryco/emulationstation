#include "SystemScreenSaver.h"

#include "CollectionSystemManager.h"
#include "utils/FileSystemUtil.h"
#include "views/gamelist/IGameListView.h"
#include "views/ViewController.h"
#include "FileData.h"
#include "FileFilterIndex.h"
#include "Log.h"
#include "Scripting.h"
#include "Sound.h"
#include "SystemData.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <time.h>
#include <unordered_map>

#define FADE_TIME 300

SystemScreenSaver::SystemScreenSaver(Window* window) :
	mWindow(window),
	mState(STATE_INACTIVE),
	mOpacity(0.0f)
{
	mWindow->setScreenSaver(this);
}

SystemScreenSaver::~SystemScreenSaver()
{
}

bool SystemScreenSaver::allowSleep()
{
	return true;
}

bool SystemScreenSaver::isScreenSaverActive()
{
	return (mState != STATE_INACTIVE);
}

void SystemScreenSaver::startScreenSaver()
{
	mState = STATE_FADE_OUT;
	mOpacity = 0.0f;
}

void SystemScreenSaver::stopScreenSaver()
{
	mState = STATE_INACTIVE;
}

void SystemScreenSaver::renderScreenSaver()
{
	if (mState != STATE_INACTIVE)
	{
		std::string screensaver_behavior = Settings::getInstance()->getString("ScreenSaverBehavior");
		Renderer::setMatrix(Transform4x4f::Identity());
		unsigned int color = (int)((screensaver_behavior == "dim" ? 0x000000A0 : 0x000000FF)*mOpacity);
		Renderer::drawRect(0.0f, 0.0f, Renderer::getScreenWidth(), Renderer::getScreenHeight(), color, color);
	}
}

void SystemScreenSaver::update(int deltaTime)
{
	// Use this to update the fade value for the current fade stage
	if (mState == STATE_FADE_OUT)
	{
		mOpacity += (float)deltaTime / FADE_TIME;
		if (mOpacity >= 1.0f)
		{
			mOpacity = 1.0f;
			mState = STATE_ACTIVE;
		}
	}
}
