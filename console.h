#ifndef _CONSOLEH_
#define _CONSOLEH_

#include <Windows.h>
#include <iostream>
#include <string>
#include <time.h>

#pragma warning(disable:4996)

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define PURPLE 0x5
#define YELLOW 0x6
#define LIGHTGRAY 0x7
#define GRAY 0x8
#define LIGHTBLUE 0x9
#define LIGHTGREEN 0xA
#define LIGHTCYAN 0xB
#define LIGHTRED 0xC
#define LIGHTPURPLE 0xD
#define LIGHTYELLOW 0xE
#define WHITE 0xF

BOOL _stdcall CtrlHandler(DWORD type){
    if(type == CTRL_C_EVENT) printf("^C\n");
    return 1;
}

namespace Console{

    HANDLE h;
	void Alloc(LPCSTR title){
        AllocConsole();

        h = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleCtrlHandler(CtrlHandler, true);
        SetConsoleTitleA(title);

        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);
	}

    void Alloc(){
        Alloc("Debug Console");
    }

    void Destroy(){
        fclose(stdout);
        fclose(stdin);
        fclose(stderr);
        FreeConsole();
    }

    void SetColor(const char& bg, const char& fg){
        SetConsoleTextAttribute(h, bg << 4 | fg);
    }

    std::string* in;
    const char* ReadLine(){
        in = new std::string();
        getline(std::cin, *in);
        if(std::cin.fail() || std::cin.eof()){
            std::cin.clear();
            return "ctrlc";
        }
        return in->c_str();
    }

    template <typename T>
    void Write(const T& msg){
        std::cout << msg; 
    }

    template <typename T>
    void WriteError(const T& msg){
        SetColor(RED, WHITE);
        std::cout << msg << "\n";
        SetColor(BLACK, LIGHTGRAY);
    }

    template <typename T>
    void WriteLine(const T& msg){ 
        std::cout << msg << "\n";
    }

    template <typename T>
    void Log(const T& msg, int level = 0){

        char buffer[16];
        time_t t = time(nullptr);
        strftime(buffer, 16, "[%T]", localtime(&t));

        Write(buffer);

        switch(level){
            case 0:
                Write("[INFO]: ");
                break;
            case 1:
                SetColor(RED, WHITE);
                Write("[ERROR]");
                SetColor(BLACK, LIGHTGRAY);
                Write(": ");
                break;
            default:
                Write("[DEBUG]: ");
                break;
        }

        WriteLine(msg);
    }

	template<typename ... Args>
	void Log(const char* format, int level, Args ... args) {
		char* buf = new char[snprintf(nullptr, 0, format, args...) + 1];
		sprintf(buf, format, args...);
		Log(buf, level);
		delete[] buf;
	}
}

#endif