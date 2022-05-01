#include "platform.h"

#include <SDL_events.h>
#include <unistd.h>
#include <fcntl.h>

#include "Log.h"

int runSystemCommand(const std::string& cmd_utf8)
{
	return system(cmd_utf8.c_str());
}

QuitMode quitMode = QuitMode::RESTART;

int quitES(QuitMode mode)
{
	quitMode = mode;

	SDL_Event *quit = new SDL_Event();
	quit->type = SDL_QUIT;
	SDL_PushEvent(quit);
	return 0;
}

void touch(const std::string& filename)
{
	int fd = open(filename.c_str(), O_CREAT|O_WRONLY, 0644);
	if (fd >= 0)
		close(fd);
}

int processQuitMode()
{
	switch (quitMode)
	{
	case QuitMode::RESTART:
		LOG(LogInfo) << "Restarting EmulationStation";
		break;
	case QuitMode::SHUTDOWN:
		LOG(LogInfo) << "Shutting system down";
		break;
	case QuitMode::REBOOT:
		LOG(LogInfo) << "Rebooting system";
		break;
	case QuitMode::SHELL:
		LOG(LogInfo) << "Quitting to root shell";
		break;
	}
	return quitMode;
}
