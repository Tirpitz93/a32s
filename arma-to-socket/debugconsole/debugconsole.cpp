// debugconsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//A2S_PACKET_SIZE: maximum return size for extension
#define A2S_PACKET_SIZE 80
#include <windows.h>
#include <iostream> 
#include <string>




/* Define a function pointer for our imported
* function.
* This reads as "introduce the new type f_funci as the type:
*                pointer to a function returning an int and
*                taking no arguments.
*
* Make sure to use matching calling convention (__cdecl, __stdcall, ...)
* with the exported function. __stdcall is the convention used by the WinAPI
*/
typedef int(*f_funci)();
//extern "C" int funci();
typedef  void(__stdcall *RVExtension_f)(char*, int , const char* );

int main()
{
	std::cout << "============================================" << std::endl;
	std::cout << "Arma-to-socket debug console" << std::endl;
	std::cout << "============================================" << std::endl;

	char output[A2S_PACKET_SIZE] = "nothing here";
	//system("pause");
	HINSTANCE hGetProcIDDLL = LoadLibrary(L".\\a32s.dll");

	if (!hGetProcIDDLL) {
		std::cout << "Could not load the Extension" << std::endl;
		system("pause");
		return EXIT_FAILURE;
	
	}
	else
	{
		std::cout << "Extension Loaded..." << std::endl;
	}

	
	RVExtension_f extension = (RVExtension_f)GetProcAddress(hGetProcIDDLL, "_RVExtension@12");

	if (!extension) {
		std::cout << "Could not locate the function: RVExtension (_RVExtension@12)" << std::endl;
		std::cout << GetLastError();

		system("pause");
		return EXIT_FAILURE;
	}
	else
	{
		std::cout << "Function loaded..." << std::endl;
	}

	
	extension(output, A2S_PACKET_SIZE, "version");
	std::cout << "Extension version: " << output << std::endl;
	
	extension(output, A2S_PACKET_SIZE, "APIVersion");
	std::cout << "API Version: " << output << std::endl;
	std::cout << "============================================" << std::endl;
	std::cout << "============================================" << std::endl;
	extension(output, A2S_PACKET_SIZE, "s:udp:new:udp");
	std::cout << "New udp socket: " << output << std::endl;
	extension(output, A2S_PACKET_SIZE, "s:udp:send:udp:hello world 123");
	std::cout << "UDP Send: " << output << std::endl;
	extension(output, A2S_PACKET_SIZE, "s:udp:recv:udp");
	std::cout << "udp:recv: " << output << std::endl;
	while (true){
		std::string function;
		std::cout << "===========Please Enter API Call============" << std::endl;
		getline(std::cin, function);
		extension(output, A2S_PACKET_SIZE, function.c_str());
		char* test = output;

		std::cout << "A32s returned:" << std::endl
			<< "--------------" << std::endl
			<< output << std::endl
			<< "--------------" << std::endl;
		//system("pause");
	}
	return EXIT_SUCCESS;
}