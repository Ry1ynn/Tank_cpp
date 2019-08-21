#include "Func.h"
#include <iostream>
#include <windows.h>

//系统功能
void SetCursorState(bool b)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);	//获取控制台光标信息
	CursorInfo.bVisible = b;					//显示/隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);	//设置控制台光标状态
}
void setColor(int ForeColor, int BackGroundColor)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);					 //获取当前窗口句柄
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);//设置颜色
}


void GotoxyAndPrint(unsigned x, unsigned y, const char* str, int ForeColor)
{
	COORD cur;
	cur.X = x * 2;
	cur.Y = y;

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, cur);
	SetConsoleTextAttribute(handle, ForeColor);//设置颜色
	std::cout << str;
}





