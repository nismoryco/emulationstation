#include "guis/GuiGeneralScreensaverOptions.h"

#include "components/OptionListComponent.h"
#include "components/SliderComponent.h"
#include "components/SwitchComponent.h"
#include "guis/GuiMsgBox.h"
#include "Settings.h"

GuiGeneralScreensaverOptions::GuiGeneralScreensaverOptions(Window* window, const char* title) : GuiScreensaverOptions(window, title)
{
	// screensaver time
	auto screensaver_time = std::make_shared<SliderComponent>(mWindow, 0.f, 30.f, 1.f, "m");
	screensaver_time->setValue((float)(Settings::getInstance()->getInt("ScreenSaverTime") / Settings::ONE_MINUTE_IN_MS));
	addWithLabel("SCREENSAVER AFTER", screensaver_time);
	addSaveFunc([screensaver_time] {
		Settings::getInstance()->setInt("ScreenSaverTime", (int)Math::round(screensaver_time->getValue()) * Settings::ONE_MINUTE_IN_MS);
	});

	// Allow ScreenSaver Controls - ScreenSaverControls
	auto ss_controls = std::make_shared<SwitchComponent>(mWindow);
	ss_controls->setState(Settings::getInstance()->getBool("ScreenSaverControls"));
	addWithLabel("SCREENSAVER CONTROLS", ss_controls);
	addSaveFunc([ss_controls] { Settings::getInstance()->setBool("ScreenSaverControls", ss_controls->getState()); });

	// screensaver behavior
	auto screensaver_behavior = std::make_shared< OptionListComponent<std::string> >(mWindow, "SCREENSAVER BEHAVIOR", false);
	std::vector<std::string> screensavers;
	screensavers.push_back("dim");
	screensavers.push_back("black");
	for(auto it = screensavers.cbegin(); it != screensavers.cend(); it++)
		screensaver_behavior->add(*it, *it, Settings::getInstance()->getString("ScreenSaverBehavior") == *it);
	addWithLabel("SCREENSAVER BEHAVIOR", screensaver_behavior);
	addSaveFunc([this, screensaver_behavior] {
		if (Settings::getInstance()->getString("ScreenSaverBehavior") != "random video" && screensaver_behavior->getSelected() == "random video") {
			// if before it wasn't risky but now there's a risk of problems, show warning
			mWindow->pushGui(new GuiMsgBox(mWindow,
			"The \"Random Video\" screensaver shows videos from your gamelist.\n\nIf you do not have videos, or if in several consecutive attempts the games it selects don't have videos it will default to black.\n\nMore options in the \"UI Settings\" > \"Video Screensaver\" menu.",
				"OK", [] { return; }));
		}
		Settings::getInstance()->setString("ScreenSaverBehavior", screensaver_behavior->getSelected());
	});
}

GuiGeneralScreensaverOptions::~GuiGeneralScreensaverOptions()
{
}
