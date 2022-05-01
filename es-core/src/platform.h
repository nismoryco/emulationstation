#pragma once
#ifndef ES_CORE_PLATFORM_H
#define ES_CORE_PLATFORM_H

#include <string>

enum QuitMode
{
	RESTART = 0,
	SHUTDOWN = 1,
	REBOOT = 2,
	SHELL = 3
};

int runSystemCommand(const std::string& cmd_utf8); // run a utf-8 encoded in the shell (requires wstring conversion on Windows)
int quitES(QuitMode mode = QuitMode::RESTART);
int processQuitMode();

#endif // ES_CORE_PLATFORM_H
