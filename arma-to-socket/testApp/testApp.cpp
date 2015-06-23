// testApp.cpp : Defines the entry point for the console application.
//

#pragma once
//#pragma comment (lib, "boost_math_c99-vc120-mt-1_58")
#pragma comment (lib, "ssleay32")
#pragma comment (lib, "libeay32")
#pragma comment (lib, "PocoFoundation")

//#pragma comment (lib, "PocoFoundationd")
#include "stdafx.h"
//#include "Socket.h"
#define WIN32_LEAN_AND_MEAN
#include "stdafx.h" 
#include <string> 
#include <unordered_map> 
#include <thread> 
#include <mutex> 
#include <atomic> 
#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
// DatagramSocket send example

#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTimeFormatter.h>
#include <iostream>
#include <fstream>
#include <vector>


#include <boost/program_options.hpp> 
#include "Poco/Net/DNS.h"
#include <iostream>
//#include <iostream>
//#include <string>
//#include <cstring>
#include <vector>
#include <utility>
#include <cstdlib>
#include <exception>

#include <Poco/Exception.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/ThreadPool.h>
#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <Poco/Mutex.h>


using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::exception;
using Poco::Exception;
using Poco::NoThreadAvailableException;
using Poco::Net::StreamSocket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::ThreadPool;
using Poco::Thread;
using Poco::Runnable;
using Poco::Mutex;
using Poco::FIFOBuffer;


/**
Makes random requests to the server.
**/




//std::string default_timeout;
//std::string local_address;
//std::string local_timeout;
//std::string test_address;
//std::vector<std::string> fifth;
//char* deliminator = ":";
//char address[] = "127.0.0.1";
//
//static char version[] = "0.2";
//std::map<std::string, std::string> mySockets;
//int loadOptions(std::string);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
//std::string parseCall(std::string);
//void create(std::string);
//int getConfig(std::string, char*, char*, int*, int*);

int _tmain(int argc, _TCHAR* argv[])
{
	std::size_t bufLen = 1024;
	SocketAddress address("127.0.0.1:9999");
	StreamSocket mySocket(address);
	std::cout << mySocket.getReceiveTimeout().totalSeconds() << std::endl;
	std::cout << mySocket.getSendTimeout().totalSeconds() << std::endl;
	Poco::FIFOBuffer sendData(bufLen, false);
	sendData.copy( "hello world", 12);
	std::cout << mySocket.sendBytes(sendData) << std::endl;

	system("pause");
	return EXIT_SUCCESS;

	//namespace po = boost::program_options;

	//typedef std::vector< std::string > plugin_names_t;
	//plugin_names_t plugin_names;
	//std::string settings_type;

	//// setup options.
	//po::options_description desc("options");
	//desc.add_options()
	//	("default.timeout", po::value< std::string >(&default_timeout),
	//	"default_timeout");

	//// load setting file.
	//po::variables_map vm;
	//std::ifstream settings_file("a32s.ini", std::ifstream::in);
	//po::store(po::parse_config_file(settings_file, desc,true), vm);
	//settings_file.close();
	//po::notify(vm);
	//	
	//// print settings.
	///*typedef std::vector< std::string >::iterator iterator;
	//for (plugin_names_t::iterator iterator = plugin_names.begin(),
	//	end = plugin_names.end();
	//	iterator < end;
	//++iterator)
	//{
	//	std::cout << "plugin.name: " << *iterator << std::endl;
	//}*/
	//std::cout << "settings.type: " << default_timeout << std::endl;
	//loadoptions("test");

	//while (true){
	//	std::string function;
	//	
	//	std::cout << "please enter extension call";
	//	getline(std::cin, function);
	//	std::cout << "you entered: " << function << "!\n";
	//	
	//	std::cout << parsecall(function);

	//};
	//return 0;
}
//
//int loadOptions(std::string name){
//	char cName = *name.c_str();
//
//	unsigned int timeout = 0;
//	std::string address = "address";
//	std::string port = "port";
//	unsigned int protocol = 10000;
//	namespace po = boost::program_options;
//	std::string timeoutStr = ".timeout";
//	std::string addressStr = ".address";
//	std::string portStr = ".port";
//	std::string protocolStr = ".protocol";
//	timeoutStr = name + timeoutStr;
//	char *timeoutCStr = &timeoutStr[0u];
//	addressStr = name + addressStr;
//	char *addressCStr = &addressStr[0u];
//	portStr = name + portStr;
//	char *portCStr = &portStr[0u];
//	protocolStr = name + protocolStr;
//	char *protocolCStr = &protocolStr[0u];
//
//	typedef std::vector< std::string > plugin_names_t;
//	plugin_names_t plugin_names;
//	std::string settings_type;
//	
//	//std::cout << protocolCStr << "\n";
//	// Setup options.
//	po::options_description desc("Options");
//	desc.add_options()
//		(timeoutCStr,
//		po::value<unsigned int >(&timeout),
//		"Socket timeout (ms)")
//
//		(addressCStr,
//		po::value< std::string >(&address),
//		"Socket address (uri)")
//
//		(portCStr,
//		po::value< std::string >(&port),
//		"Socket port")
//
//		(protocolCStr,
//		po::value<unsigned int >(&protocol),
//		"Socket protocol (1:TCP,2:UDP)")
//		;
//	// Load setting file.
//	po::variables_map vm;
//	std::ifstream settings_file("a32s.ini", std::ifstream::in);
//	po::store(po::parse_config_file(settings_file, desc, true), vm);
//	settings_file.close();
//	po::notify(vm);
//
//	// Print settings.
//	typedef std::vector< std::string >::iterator iterator;
//	for (plugin_names_t::iterator iterator = plugin_names.begin(),
//		end = plugin_names.end();
//		iterator < end;
//	++iterator)
//	{
//		std::cout << "plugin.name: " << *iterator << std::endl;
//	}
//	std::cout << name << "\n";
//	std::cout << std::to_string(protocol) << "\n";
//	std::cout << port << "\n";
//	std::cout << address << "\n";
//	std::cout << timeout << "\n";
//	return 0;
//
//};

//std::string parseCall(std::string input){
//	using namespace std;
//	map < string, int > systemCalls;
//	systemCalls.insert(pair<string, int>("version", 1));
//	systemCalls.insert(pair<string, int>("dump", 2));
//
//	map < string, int > mgmntCalls;
//	
//	mgmntCalls.insert(pair<string, int>("count", 1));
//	mgmntCalls.insert(pair<string, int>("list", 2));
//	mgmntCalls.insert(pair<string, int>("cleanup", 3));
//	mgmntCalls.insert(pair<string, int>("rmall", 4));
//
//
//
//
//	map < string, int > useCalls;
//	useCalls.insert(pair<string, int>("send", 1));
//	useCalls.insert(pair<string, int>("recv", 2));
//	useCalls.insert(pair<string, int>("sRecv", 3));
//
//
//	map < string, int > maintCalls;
//	maintCalls.insert(pair<string, int>("create", 1));
//	maintCalls.insert(pair<string, int>("destroy", 2));
//	maintCalls.insert(pair<string, int>("shutdown", 3));
//	maintCalls.insert(pair<string, int>("init", 4));
//	string::size_type sz;
//	
//	vector<string> vect;
//	vect = split(input, *deliminator);
//	std::transform(vect[1].begin(), vect[1].end(), vect[1].begin(), ::tolower);
//	try{ 
//		
//		int length = vect.size();
//
//		switch (stoi(vect[0], &sz)){
//
//		case 0:{
//
//			if (length < 2){
//				return "unrecognized call was made please check the wiki for help. Management call requires 2 arguments. You tried: '" + input + "'";
//				break;
//			}
//
//			//managemnt calls eg clean array, count
//			switch (mgmntCalls[vect[1]]){
//
//			case 1:{
//				//count // count open sockets
//
//				return "0:" + to_string(mySockets.size());
//
//				break;
//			}
//			case 2:{
//				//list sockets and destinations
//				break;
//			}
//			case 3:{
//				//cleanup
//				if (mySockets.find(vect[2]) == mySockets.end()) {
//					std::cout << "Socket Not socket";
//					std::cout << "\n";
//				}
//				else {
//					std::cout << "socket found";
//				}
//
//				break;
//			}
//			case 4:{
//				//rmall // crash all open sockets and call cleanup
//				if (mySockets.find(vect[2]) == mySockets.end()) {
//					std::cout << "Socket Not socket";
//					std::cout << "\n";
//				}
//				else {
//					std::cout << "socket found";
//				}
//
//				break;
//			}
//			default:
//				break;
//			}
//			break;
//		}
//		case 1:{
//			if (length < 3){
//				return "unrecognized call was made please check the wiki for help. Use calls require 3 arguments. You tried: '" + input + "'";
//				break;
//			}
//			//use calls: eg send, recieve, sendRecieve (listen)/ (accept)
//			switch (useCalls[vect[1]]){
//
//			case 1: {
//				//send
//				if (mySockets.find(vect[2]) == mySockets.end()) {
//					std::cout << "Socket Not found";
//					std::cout << "\n";
//				}
//				else {
//					std::cout << "socket found";
//					std::cout << "\n";
//				}
//
//				break; }
//			case 2: {
//				//recieve
//				if (mySockets.find(vect[2]) == mySockets.end()) {
//					std::cout << "Socket Not found";
//					std::cout << "\n";
//				}
//				else {
//					std::cout << "socket found";
//					std::cout << "\n";
//				}
//
//				break;
//			}
//			case 3: {
//				//send + recieve
//				if (mySockets.find(vect[2]) == mySockets.end()) {
//					std::cout << "Socket Not found";
//					std::cout << "\n";
//				}
//				else {
//					std::cout << "socket found";
//					std::cout << "\n";
//				}
//			}
//			}
//		}
//		case 2:{
//			if (length < 3){
//				return "unrecognized call was made please check the wiki for help. Maintenance call requires 3 arguments. You tried: '" + input + "'";
//				break;
//			}
//			switch (maintCalls[vect[1]]){
//
//			case 1:{
//				//create
//				
//				break;
//			}
//			case 2:{
//				//destroy // crash
//				if (mySockets.find(vect[2]) == mySockets.end()) {
//					std::cout << "Socket Not socket";
//					std::cout << "\n";
//				}
//				else {
//					std::cout << "socket found";
//				}
//
//				break;
//			}
//			case 3:{
//				//shutdown // gracefull/ finish recieve
//				if (mySockets.find(vect[2]) == mySockets.end()) {
//					std::cout << "Socket Not socket";
//					std::cout << "\n";
//				}
//				else {
//					std::cout << "socket found";
//				}
//
//				break;
//			}
//			case 4:{
//				//init 
//				if (mySockets.find(vect[2]) == mySockets.end()) {
//					std::cout << "Socket Not socket";
//					std::cout << "\n";
//				}
//				else {
//					std::cout << "socket found";
//				}
//
//				break;
//			}
//
//			default:
//				break;
//			}
//		}
//
//		case 9:{
//			if (length < 2){
//				return "unrecognized call was made please check the wiki for help. Management call requires 2 arguments. You tried: '" + input + "'";
//				break;
//			}
//			switch (systemCalls[vect[1]]){
//			case 1:
//				//version: return version number
//
//
//				std::cout << version;
//				std::cout << "\n";
//				break;
//			case 2:{
//
//				std::cout << local_address;
//
//				break;
//			}
//
//			default:
//
//				break;
//			}
//			break;
//		default:
//
//			break;
//		}
//		}
//	}
//	catch (...){
//		return "unrecognized call was made please check the wiki for help. You tried: '" + input +"'";
//	}
//	return "unhandled call as of yet";
//};
//
//
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
};

//void create(std::string name){
//	char cName = *name.c_str();
//
//	int timeout;
//	std::string address;
//	std::string port;
//	int protocol;
//	namespace po = boost::program_options;
//
//	typedef std::vector< std::string > plugin_names_t;
//	plugin_names_t plugin2_names;
//
//	// Setup options.
//	po::options_description desc("Options");
//	desc.add_options()
//		(cName + ".timeout",
//		po::value< int >(&timeout),
//		"Socket timeout (ms)")
//
//		(cName + ".address",
//		po::value< std::string >(&address),
//		"Socket address (uri)")
//
//		(cName + ".port",
//		po::value< std::string >(&port),
//		"Socket port")
//
//		(cName + ".protocol",
//		po::value< int >(&protocol),
//		"Socket protocol (1:TCP,2:UDP)")
//		;
//	// Load setting file.
//	po::variables_map vm;
//	std::ifstream settings_file("a32s.ini", std::ifstream::in);
//	po::store(po::parse_config_file(settings_file, desc), vm);
//	settings_file.close();
//	po::notify(vm);
//
//
//
//
//}
//
//int getConfig(std::string name, char* address, char* port, int* protocol, int* timeout){
//
//	char cName = *name.c_str();
//
//
//	namespace po = boost::program_options;
//
//	typedef std::vector< std::string > plugin_names_t;
//	plugin_names_t plugin2_names;
//
//	// Setup options.
//	try {
//
//		po::options_description desc("Options");
//		desc.add_options()
//			(cName + ".timeout",
//			po::value< int* >(&timeout),
//			"Socket timeout (ms)")
//
//			(cName + ".address",
//			po::value< char* >(&address),
//			"Socket address (uri)")
//
//			(cName + ".port",
//			po::value< char* >(&port),
//			"Socket port")
//
//			(cName + ".protocol",
//			po::value< int* >(&protocol),
//			"Socket protocol (1:TCP,2:UDP)")
//			;
//		// Load setting file.
//		po::variables_map vm;
//		std::ifstream settings_file("a32s.ini", std::ifstream::in);
//		po::store(po::parse_config_file(settings_file, desc), vm);
//		settings_file.close();
//		po::notify(vm);
//		return 0;
//	}
//	catch (...){
//		return 1;
//	}
//
//}