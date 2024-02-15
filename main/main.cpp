#include "includes/includes.hpp"
#include <iostream>
#include <stdio.h>

#include <Windows.h>
#include <string>
#include "includes/utils.hpp"
#include <urlmon.h>
#include <tchar.h>

#include "general.h"
#include <random>
#include "protect/auth.hpp"

const wchar_t* ٽيسٽ = (L"C:\\Windows\\System32\\Drivers\\cht4redx.sys");
const wchar_t* بنياد = (L"C:\\Windows\\System32\\Drivers\\cht4red.sys");




static std::time_t string_to_timet(std::string timestamp) {


	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {


	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);

std::string tm_to_readable_time2(tm ctx) {


	std::time_t now = std::time(nullptr);
	std::time_t expiry = std::mktime(&ctx);

	double remainingSeconds = std::difftime(expiry, now);

	if (remainingSeconds >= 60 * 60 * 24) {
		int remainingDays = static_cast<int>(remainingSeconds / (60 * 60 * 24));
		return std::to_string(remainingDays) + " day(s).";
	}
	else if (remainingSeconds >= 60 * 60) {
		int remainingHours = static_cast<int>(remainingSeconds / (60 * 60));
		return std::to_string(remainingHours) + " hour(s).";
	}
	else {
		int remainingMinutes = static_cast<int>(remainingSeconds / 60);
		return std::to_string(remainingMinutes) + " minute(s).";
	}
}

void DeleteKey(std::ifstream& File)
{

	std::string regfile("key.txt");
	std::ofstream(regfile, std::ios::trunc);
	File.setstate(std::ios::failbit);
	remove(regfile.c_str());
}

void sleepMilliseconds(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

std::string readFileIntoString(const std::string& path) {


	auto ss = std::ostringstream{};
	std::ifstream input_file(path);
	if (!input_file.is_open()) {
		std::cerr << E("Could Not Open License Key File") << std::endl;
		exit(EXIT_FAILURE);
	}
	ss << input_file.rdbuf();
	return ss.str();
}

HWND windowid = NULL;


void Logo()
{

	std::cout << R"(                                                                                                                              


             |                              ____.......__
             |\      .'           _.--""''``             ``''--._
             | \   .'/      ..--'`                             .-'`
      .._    |  \.' /  ..-''                                .-'
       '.`'"-:  '  .-'`                                  .-'
         '.             __...----""""""""""--..           \
         -         ..-''                       ``""-._     \
       .'  _.      \                                  `"-   \
      _.-'` |  /-.  \                                    `-. \
            | /   `. \                                      `.\
            |/      `-\                                       `.
            |
                                              
)" << '\n';

}

void Logo2()
{
	std::cout << R"(    

	$$\                          $$\              $$$$$$\  $$\            $$\     $$\                     $$\ 
	$$ |                         $$ |            $$  __$$\ $$ |           $$ |    $$ |                    $$ |
	$$ |     $$\   $$\  $$$$$$\  $$ |  $$\       $$ /  \__|$$ | $$$$$$\ $$$$$$\ $$$$$$\    $$$$$$\   $$$$$$$ |
	$$ |     $$ |  $$ |$$  __$$\ $$ | $$  |      \$$$$$$\  $$ |$$  __$$\\_$$  _|\_$$  _|  $$  __$$\ $$  __$$ |
	$$ |     $$ |  $$ |$$ |  \__|$$$$$$  /        \____$$\ $$ |$$ /  $$ | $$ |    $$ |    $$$$$$$$ |$$ /  $$ |
	$$ |     $$ |  $$ |$$ |      $$  _$$<        $$\   $$ |$$ |$$ |  $$ | $$ |$$\ $$ |$$\ $$   ____|$$ |  $$ |
	$$$$$$$$\\$$$$$$  |$$ |      $$ | \$$\       \$$$$$$  |$$ |\$$$$$$  | \$$$$  |\$$$$  |\$$$$$$$\ \$$$$$$$ |
	\________|\______/ \__|      \__|  \__|       \______/ \__| \______/   \____/  \____/  \_______| \_______|
	 
                                              
)" << '\n';

}


void Logo1()
{
	std::cout << R"(    
        ~+

                 *       +
           '                  |
       ()    .-.,="``"=.    - o -
             '=/_       \     |
          *   |  '=._    |
               \     `=./`,        '
            .   '=.__.=' `='      *
   +                         +
        O      *        '       .
	                                    
)" << '\n';

}



int choice;


bool fileExists(const std::string& filename) {
	std::ifstream file(filename.c_str());
	return file.good();
}

void slowPrint(const std::string& text, std::chrono::milliseconds delay) {
	for (char c : text) {
		std::cout << c << std::flush;
		std::this_thread::sleep_for(delay);
	}
}
void rndmTitle() {
	constexpr int length = 25;
	const auto characters = (("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	TCHAR title[length + 1]{};

	while (true) {
		for (int j = 0; j != length; j++) {
			title[j] = characters[rand() % 55 + 1];
		}

		SetConsoleTitle(title);

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
std::thread titleThread;

BOOL WINAPI MyHandlerRoutine(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT)
	{
		//HWND handle = FindWindowA(NULL, "Untitled - Paint");
		//ShowWindow(handle, SW_SHOW);
		system(E("taskkill /F /T /IM mspaint.exe"));
	}
	return FALSE;
}

auto main() -> int
{
	mouse_interface();

	Logo1();

	/*if (!Utilities->debug)
	{

		LI_FN(SetConsoleTextAttribute)(GetStdHandle(STD_OUTPUT_HANDLE), 0x5);*/

	//	std::thread security(security_loop);


	//	titleThread = std::thread(rndmTitle);

		if (Cheat_shi.River)
		{
			Cheat_shi.Cheat_name = E("River");
			Cheat_shi.Cheat_Version = E("Private");
			Cheat_shi.branding_type = 2;
		}

	{
		//LI_FN(system)(skCrypt("cls"));

		mem::find_driver();

		LI_FN(Sleep)(1000);
		LI_FN(printf)(skCrypt("\n \033[0m[\033[0m\033[1;34m!\033[0m]"));
		std::cout << (skCrypt(" Waiting For Game.")) << std::flush;


		while (windowid == NULL)

		{
			XorS(wind, "Fortnite  ");
			windowid = FindWindowA_Spoofed(0, wind.decrypt());

		}
		LI_FN(Sleep)(1000);
		//LI_FN(system)(skCrypt("cls"));

		globals->hwnd = FindWindowA_Spoofed(NULL, "Fortnite  ");

		LI_FN(Sleep)(1000);
		std::cout << (skCrypt("[+] Found Game.")) << std::flush;
		LI_FN(Sleep)(1000);
		//LI_FN(system)(skCrypt("cls"));

		mem::find_process("FortniteClient-Win64-Shipping.exe");

		virtualaddy = mem::find_image();

		if (virtualaddy)
		{
			LI_FN(Sleep)(1000);
			std::cout << (skCrypt(" Got Base Address -> ")) << virtualaddy << std::flush;
			LI_FN(Sleep)(1000);
			//LI_FN(system)(skCrypt("cls"));


		}
		else
		{
			std::cout << (skCrypt(" Failed To Get Base Address")) << std::flush;
			LI_FN(Sleep)(1000);
			LI_FN(exit)(0);

		}


		std::cout << virtualaddy << std::endl;


		globals->width = GetSystemMetrics_Spoofed(SM_CXSCREEN); globals->height = GetSystemMetrics_Spoofed(SM_CYSCREEN);


		if (!Utilities->debug)
		{

			//ShowWindow_Spoofed(GetConsoleWindow(), SW_HIDE);

		}


		if (Render->Setup() != RENDER_INFORMATION::RENDER_SETUP_SUCCESSFUL) ExitProcess(0);


		if (Hook->Setup() != HOOK_INFORMATION::HOOKS_SUCCESSFUL) ExitProcess(0);

		if (Game->Setup() != GAME_INFORMATION::GAME_SETUP_SUCCESSFUL) ExitProcess(0);

		Render->Render();
	}
}

