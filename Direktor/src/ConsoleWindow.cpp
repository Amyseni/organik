#include "ConsoleWindow.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <filesystem>
#include <iostream>
#include <fstream>

using namespace std;

static const WORD MAX_CONSOLE_LINES = 500;

void ConsoleWindow::Init()
{
    int hConHandle;
    long lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE *fp;

    // allocate a console for this app
    AllocConsole();
    
    // Console in allows us to select text without freezing up the entire process, or so I've heard
    freopen("CONIN$", "w", stdin);

    // info and error log files
    freopen((std::filesystem::current_path() / "info.log").generic_string().c_str(), "w", stdout);
    freopen((std::filesystem::current_path() / "error.log").generic_string().c_str(), "w", stderr);

    // make sure cout and cerr are going to the files as well? I hope?
    std::ios::sync_with_stdio(true);

    std::cerr << "test message please ignore" << std::endl;
}
