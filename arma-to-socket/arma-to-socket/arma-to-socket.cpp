// threaded_example.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

#include <fstream>

#include <boost/program_options.hpp>

#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTimeFormatter.h>
#include "Poco/Net/DNS.h"
#include <Poco/Exception.h>

#include <Poco/ThreadPool.h>
#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <Poco/Mutex.h>

#include <Poco/Logger.h>

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
using Poco::Net::Socket;


#define BUFFERLEN 8192
//using namespace std;


// mySockets: Map containing Identifier & socketPointer pair.
//
std::map<std::string, StreamSocket*> mySockets;
std::map<std::string, DatagramSocket*> myUDPSockets;

struct Data {
	bool ready = false;
	std::string params = "";
	std::string result = "";
	std::vector<std::string> resultsArray;
	bool isLong = false;
	int lastSent = -1;
	int PacketCount = 0;
};

std::unordered_map<long int, Data> tickets;
std::mutex mtx;
std::mutex UDPMtx;
std::mutex scktsMtx;
//deliminator: Function call deliminator, escaped by "\"
char* deliminator = ":";
int loadOptions(std::string, std::string*, int*, int*);
std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::atomic<bool> worker_working(false);
long int id = 0; // global ticket id
long int cur_id = 0;// current ticket id
std::string newSocket(std::vector<std::string>&);
std::string TCPSend(std::vector<std::string>&);
std::string TCPRecv(std::vector<std::string>&);
std::string TCPSRecv(std::vector<std::string>&);
std::string TCPClose(std::vector<std::string>&);

std::string UDPNew(std::vector<std::string>&);
std::string UDPSend(std::vector<std::string>&);
std::string UDPRecv(std::vector<std::string>&);
std::string UDPSRecv(std::vector<std::string>&);
std::string UDPClose(std::vector<std::string>&);
int maxLen;

extern "C" { __declspec (dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function); }

void worker() {
	while (worker_working = id > cur_id) // next ticket exists?
	{
		mtx.lock();
		Data ticket = tickets[++cur_id];// copy ticket
		mtx.unlock();
		string input = ticket.params; // get input
		string output = "output: " + input; // process input
		//Sleep(10); //sleep for 0.01 sec (FOR TESTING PURPOSES ONLY)


		std::map < std::string, int > systemCalls;
		systemCalls.insert(pair<string, int>("version", 1));
		systemCalls.insert(pair<string, int>("APIVersion", 2));
		systemCalls.insert(pair<string, int>("tcp", 3));
		systemCalls.insert(pair<string, int>("udp", 4));

		std::map < std::string, int > mgmntCalls;

		mgmntCalls.insert(pair<string, int>("count", 1));
		mgmntCalls.insert(pair<string, int>("list", 2));
		mgmntCalls.insert(pair<string, int>("cleanup", 3));
		mgmntCalls.insert(pair<string, int>("rmall", 4));

		std::map < std::string, int > useCalls;
		useCalls.insert(pair<string, int>("send", 1));
		useCalls.insert(pair<string, int>("recv", 2));
		useCalls.insert(pair<string, int>("sRecv", 3));

		std::map < std::string, int > tcp;
		tcp.insert(pair<string, int>("new", 1));
		tcp.insert(pair<string, int>("send", 2));
		tcp.insert(pair<string, int>("recv", 3));
		tcp.insert(pair<string, int>("srecv", 4));
		tcp.insert(pair<string, int>("close", 5));
		tcp.insert(pair<string, int>("srecv", 6));

		vector<string> vect;
		vect = split(input, *A2S_DELIMINATOR);
		//std::transform(vect[1].begin(), vect[1].end(), vect[1].begin(), ::tolower);
		//try {
		int length = vect.size();
		//std::cout << length << std::endl;
		switch (systemCalls[vect[0]]){
			case 1:
			{
				//std::wcout << "version....." << std::endl;
				output = A2S_VERSION;
				break;
			}
			case 2:
			{
				output = A2S_API_VERSION;
				break;
			}
				//TCP
			case 3:
			{
				if (length < 3){
					output = "TCP calls requires at least 3 arguments. You tried: '" + input + "'";
					break;
				}
				else {}

				switch (tcp[vect[1]])
				{
					//create new socket
					case 1:
					{
						//std::cout << "creating socket" << std::endl;
						if (length < 3){
							output = "create call requires at least 3 arguments. You tried: '" + input + "'";
							break;
						}
						else
						{
							output = newSocket(vect);
						}

						break;
					}
						//send
					case 2:
					{
						if (length < 4){
							output = "send requires at least 4 arguments. You tried: '" + input + "'";
							break;
						}
						else{
							output = TCPSend(vect);
						}


						break;

					}
						//recieve
					case 3:
					{

						if (length < 3){
							output = "recieve requires at least 2 arguments. You tried: '" + input + "'";
							break;
						}
						else {
							output = TCPRecv(vect);
						}

						break;
					}

						//send recieve
					case 4:
					{
						//std::cout << "recieving" << std::endl;
						if (length < 4){
							output = "recieve requires at least 4 arguments. You tried: '" + input + "'";
							break;
						}
						else {
							TCPSend(vect);
							output = TCPRecv(vect);
						}
						break;
					}

						//close
					case 5:
					{
						if (length < 2){
							output = "recieve requires at least 2 arguments. You tried: '" + input + "'";
							break;
						}


						break;
					}

					default:
					{
						break;
					}

						break;
				}
				break;

				default:
				{
					output = "error:invalid call was made";
					break;
				}

			}
			// UDP
			case 4:
			{
				if (length < 3){
					output = "TCP calls requires at least 3 arguments. You tried: '" + input + "'";
					break;
				}
				else {}

				switch (tcp[vect[1]])
				{
					//create new socket
				case 1:
				{
					//std::cout << "creating socket" << std::endl;
					if (length < 3){
						output = "create call requires at least 3 arguments. You tried: '" + input + "'";
						break;
					}
					else
					{
						output = UDPNew(vect);
					}

					break;
				}
				//send
				case 2:
				{
					if (length < 4){
						output = "send requires at least 4 arguments. You tried: '" + input + "'";
						break;
					}
					else{
						output = UDPNew(vect);
					}


					break;

				}
				//recieve
				case 3:
				{

					if (length < 3){
						output = "recieve requires at least 2 arguments. You tried: '" + input + "'";
						break;
					}
					else {
						output = UDPRecv(vect);
					}

					break;
				}

				//send recieve
				case 4:
				{
					//std::cout << "recieving" << std::endl;
					if (length < 4){
						output = "recieve requires at least 4 arguments. You tried: '" + input + "'";
						break;
					}
					else {
						UDPSend(vect);
						output = UDPRecv(vect);
					}
					break;
				}

				//close
				case 5:
				{
					if (length < 2){
						output = "recieve requires at least 2 arguments. You tried: '" + input + "'";
						break;
					}


					break;
				}
			
				}
				break;

			}
		}


		//}
		/*catch (...){
			output = "an error occured unrecognized call was made please check the wiki for help. You tried: '" + input + "'";
		}*/
		//output = "unhandled call as of yet";
		int outLength = output.length();
		int bodyLength = maxLen - A2S_HEADER_SIZE;
		if (outLength > maxLen){
			ticket.PacketCount = 1 + ((outLength - 1) / bodyLength);
			ticket.isLong = true;

			for (int x = 0; x < ticket.PacketCount; x ++){

				ticket.resultsArray.push_back(output.substr(x*bodyLength, bodyLength));

			}

		}
		else
		{
			ticket.PacketCount = 1;
		}
		ticket.result = output; // prepare result
		ticket.ready = true; // notify about result

		mtx.lock();
		tickets[cur_id] = ticket; // copy back the result
		mtx.unlock();
	}
};

//loadOptions: loads options for selected socket
//std::string name: socket identifier.
//std::string* address: socket address pointer.
//int* timeout: timeout pointer.
//int* protocol: protocol identifier pointer.
int loadOptions(std::string name, std::string* address, int* timeout, int* protocol){
	char cName = *name.c_str();

	//unsigned int timeout = 0;
	//std::string address = "address";
	//std::string port = "port";
	//unsigned int protocol = 10000;
	namespace po = boost::program_options;
	std::string timeoutStr = ".timeout";
	std::string addressStr = ".address";
	std::string portStr = ".port";
	std::string protocolStr = ".protocol";
	timeoutStr = name + timeoutStr;
	char *timeoutCStr = &timeoutStr[0u];
	addressStr = name + addressStr;
	char *addressCStr = &addressStr[0u];
	portStr = name + portStr;
	char *portCStr = &portStr[0u];
	protocolStr = name + protocolStr;
	char *protocolCStr = &protocolStr[0u];

	typedef std::vector< std::string > plugin_names_t;
	plugin_names_t plugin_names;
	std::string settings_type;

	//std::cout << protocolCStr << "\n";
	// Setup options.
	po::options_description desc("Options");
	desc.add_options()
			(timeoutCStr,
			 po::value<int >(timeout),
			 "Socket timeout (ms)")

			(addressCStr,
			 po::value< std::string >(address),
			 "Socket address (uri)")

			/*(portCStr,
            po::value< std::string >(&port),
            "Socket port")
    */
			(protocolCStr,
			 po::value< int >(protocol),
			 "Socket protocol (1:TCP,2:UDP)")
			;
	// Load setting file.
	po::variables_map vm;
	std::ifstream settings_file("a32s.ini", std::ifstream::in);
	po::store(po::parse_config_file(settings_file, desc, true), vm);
	settings_file.close();
	po::notify(vm);

	// Print settings.
	//typedef std::vector< std::string >::iterator iterator;
	/*for (plugin_names_t::iterator iterator = plugin_names.begin(),
		end = plugin_names.end();
		iterator < end;
	++iterator)
	{
		std::cout << "plugin.name: " << *iterator << std::endl;
	}*/
//	std::cout << name << "\n";
//	std::cout << std::to_string(*protocol) << "\n";
	//std::cout << port << "\n";
//	std::cout << *address << "\n";
//	std::cout << *timeout << "\n";
	return 0;

};

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

//RVExtension: Function called by arma, contains core threading framework,
//Parameters:
//			char *output: pointer to output character array
//			int outputSize: maximum output length
//			const char *function: string input from arma
void __stdcall RVExtension(char *output, int outputSize, const char *function)
{
	maxLen = outputSize;
	std::string str(function);
	std::string v = "version";
	std::string av = "APIVersion";
	std::string ol = "maxLength";
	if (str == v){
		strncpy_s(output, outputSize, A2S_VERSION, _TRUNCATE);
		return;
	}
	else if (str == av){
		strncpy_s(output, outputSize, A2S_API_VERSION, _TRUNCATE);
		return;

	}
	else if (str == ol){

		strncpy_s(output, outputSize, std::to_string(outputSize).c_str(), _TRUNCATE);
		return;

	}


	if (!strncmp(function, "r:", 2)) // detect checking for result
	{
		long int num = atol(&function[2]); // ticket number or 0
		if (tickets.find(num) != tickets.end()) // ticket exists
		{
			mtx.lock();
			if (tickets[num].ready) // result is ready
			{
				std::string header;
				if (!tickets[num].isLong){
					header = "0:";
					std::string rtn = header + tickets[num].result;
					strncpy_s(output, outputSize, (rtn.c_str()), _TRUNCATE);// result
					tickets.erase(num); // get rid of the read ticket

				}
				else
				{
					header = "1:" +
							 std::to_string(tickets[num].PacketCount) + A2S_DELIMINATOR +
							 std::to_string(tickets[num].lastSent + 2) + A2S_DELIMINATOR;
					std::string rtn = header + tickets[num].resultsArray[tickets[num].lastSent + 1].c_str();
					strncpy_s(output, outputSize, rtn.c_str(), _TRUNCATE);// result
					tickets[num].lastSent++;
					if (tickets[num].lastSent >= (tickets[num].PacketCount-1)){
						tickets.erase(num); // get rid of the read ticket

					}
				}
				mtx.unlock();
				return;
			}
			mtx.unlock();
			strncpy_s(output, outputSize, "WAIT", _TRUNCATE);    // result is not ready
			return;
		}
		strncpy_s(output, outputSize, "EMPTY", _TRUNCATE); // no such ticket
	}
	else if (!strncmp(function, "s:", 2)) // detect ticket submission
	{
		Data data; data.params = string(&function[2]); // extract params
		mtx.lock(); tickets.insert(pair<long int, Data>(++id, data)); // add ticket to the queue
		mtx.unlock(); if (!worker_working) // if worker thread is finished, start another one
		{
			worker_working = true;
			std::thread worker(worker);
			worker.detach(); // start parallel process
		}
		strncpy_s(output, outputSize, std::to_string(id).c_str(), _TRUNCATE); // ticket number
	}
	else
	{
		strncpy_s(output, outputSize, "INVALID COMMAND", _TRUNCATE); // other input
	}
}

std::string newSocket(std::vector<std::string>& vect)
{
	std::string rtn;
	try {
		scktsMtx.lock();
		if (mySockets.find(vect[2]) == mySockets.end()) {
			scktsMtx.unlock();
//			std::cout << "Socket Not found...creating" << std::endl;
			std::string addrStr = "127.0.0.1:9999";
			int timeout;
			int protocol;
			int iResult = loadOptions(vect[2], &addrStr, &timeout, &protocol);
			if (iResult == 0){
//				std::cout << timeout;
				SocketAddress address(addrStr);
//				std::cout << addrStr;
				StreamSocket* mySocket = new StreamSocket(address);
				Poco::Timespan timeoutSpan(timeout);
				mySocket->setReceiveTimeout(timeoutSpan);
				scktsMtx.lock();
				mySockets.insert(pair<std::string, StreamSocket*>(vect[2], mySocket));
				scktsMtx.unlock();
				rtn = "socket created";
			}
			else
			{
				rtn = "There was an error loading config file";
			}


		}
		else {
			rtn = "the socket specified already exists";
		}
	}
	catch (Poco::Exception &e){
		rtn = e.displayText();
	}
	catch (boost::program_options::validation_error &e)
	{
		rtn = e.what();
	}
	catch (...){

		rtn = "Error: an unhandled error occured";
	}
	return rtn;

}

std::string TCPSend(std::vector<std::string>& vect)
{
	std::string rtn;

	try{

		Poco::FIFOBuffer sendData(BUFFERLEN, false);
		sendData.copy(vect[3].c_str(), vect[3].size());
		int iResult = mySockets[vect[2]]->sendBytes(sendData);
		rtn = "data sent, sendBytes returned: " + std::to_string(iResult);
	}
	catch (Poco::Exception &e){
		rtn = e.displayText();
	}

	catch (boost::program_options::validation_error &e)
	{
		rtn = e.what();
	}
	catch (...){

		rtn = "Error: an unhandled error occured";
	}
	return rtn;
}

std::string TCPRecv(std::vector<std::string>& vect)
{
	std::string rtn;

//	std::cout << "Recieving data" << std::endl;
	//Recieve
	if (mySockets.find(vect[2]) == mySockets.end()) {
		rtn = "socket Not found";

	}
	else {
		try {

			Poco::FIFOBuffer recvData(BUFFERLEN);

			int iResult = mySockets[vect[2]]->receiveBytes(recvData);
			char result[A2S_PACKET_SIZE] = "";

			recvData.read(result, recvData.size());
			rtn = result;


		}
		catch (Poco::Exception &e)
		{
			rtn = e.displayText();
//			std::cout << e.displayText() << std::endl;

		}



		catch (boost::program_options::validation_error &e)
		{
			rtn = e.what();
		}
		catch (std::exception &e){
			rtn = e.what();
		}
		catch (...){

			rtn = "Error: an unhandled error occured";
		}


	}
	return rtn;
}

std::string TCPClose(std::vector<std::string>& vect)
{
	std::string rtn;
//	std::cout << "closing data" << std::endl;
	//Recieve
	if (mySockets.find(vect[2]) == mySockets.end()) {
		rtn = "socket Not found";

	}
	else {
		try {
			mySockets[vect[2]]->shutdown();
			mySockets.erase(vect[2]);

		}
		catch (Poco::Exception &e)
		{
			mySockets[vect[2]]->close();
			mySockets.erase(vect[2]);
			rtn = e.displayText();
//			std::cout << e.displayText() << std::endl;


		}


		catch (boost::program_options::validation_error &e)
		{
			rtn = e.what();
		}
		catch (std::exception &e){
			rtn = e.what();
		}
		catch (...){

			rtn = "Error: an unhandled error occured";
		}
	}
	return rtn;
}

std::string UDPNew(std::vector<std::string>& vect)
{
	std::string rtn;
	try {
		scktsMtx.lock();
		if (myUDPSockets.find(vect[2]) == myUDPSockets.end()) {
			scktsMtx.unlock();
			//			std::cout << "Socket Not found...creating" << std::endl;
			std::string addrStr = "127.0.0.1:9999";
			int timeout;
			int protocol;
			int iResult = loadOptions(vect[2], &addrStr, &timeout, &protocol);
			if (iResult == 0){
				//				std::cout << timeout;
				SocketAddress address(addrStr);
				//				std::cout << addrStr;
				DatagramSocket* mySocket = new DatagramSocket();
				Poco::Timespan timeoutSpan(timeout);
				mySocket->setReceiveTimeout(timeoutSpan);
				scktsMtx.lock();
				myUDPSockets.insert(pair<std::string, DatagramSocket*>(vect[2], mySocket));
				scktsMtx.unlock();
				rtn = "socket created";
			}
			else
			{
				rtn = "There was an error loading config file";

			}

		}

		else {
			rtn = "the socket specified already exists";
		}
	}
	catch (Poco::Exception &e){
		rtn = e.displayText();
	}
	catch (boost::program_options::validation_error &e)
	{
		rtn = e.what();
	}
	catch (...){

		rtn = "Error: an unhandled error occured";
	}
	return rtn;
}

std::string UDPSend(std::vector<std::string>& vect)
{
	std::string rtn;
	std::string addrStr = "";
	int timeout;
	int protocol;

	int iResult = loadOptions(vect[2], &addrStr, &timeout, &protocol);
	SocketAddress address(addrStr);
	try{

		Poco::FIFOBuffer sendData(BUFFERLEN, false);
		sendData.copy(vect[3].c_str(), vect[3].size());
		int iResult = myUDPSockets[vect[2]]->sendTo(vect[2].c_str(), A2S_PACKET_SIZE,address);
		rtn = "data sent, sendBytes returned: " + std::to_string(iResult);
	}
	catch (Poco::Exception &e){
		rtn = e.displayText();
	}

	catch (boost::program_options::validation_error &e)
	{
		rtn = e.what();
	}
	catch (...){

		rtn = "Error: an unhandled error occured";
	}
	return rtn;
}
std::string UDPRecv(std::vector<std::string>& vect)
{
	std::string rtn;

	//	std::cout << "Recieving data" << std::endl;
	//Recieve
	if (mySockets.find(vect[2]) == mySockets.end()) {
		rtn = "socket Not found";

	}
	else {
		try {

			Poco::FIFOBuffer recvData(BUFFERLEN);

			int iResult = mySockets[vect[2]]->receiveBytes(recvData);
			char result[A2S_PACKET_SIZE] = "";

			recvData.read(result, recvData.size());
			rtn = result;


		}
		catch (Poco::Exception &e)
		{
			rtn = e.displayText();
			//			std::cout << e.displayText() << std::endl;

		}



		catch (boost::program_options::validation_error &e)
		{
			rtn = e.what();
		}
		catch (std::exception &e){
			rtn = e.what();
		}
		catch (...){

			rtn = "Error: an unhandled error occured";
		}


	}
	return rtn;
}

std::string UDPClose(std::vector<std::string>& vect)
{
	std::string rtn;
	//	std::cout << "closing data" << std::endl;
	//Recieve
	if (mySockets.find(vect[2]) == mySockets.end()) {
		rtn = "socket Not found";

	}
	else {
		try {
			mySockets[vect[2]]->shutdown();
			mySockets.erase(vect[2]);

		}
		catch (Poco::Exception &e)
		{
			mySockets[vect[2]]->close();
			mySockets.erase(vect[2]);
			rtn = e.displayText();
			//			std::cout << e.displayText() << std::endl;


		}


		catch (boost::program_options::validation_error &e)
		{
			rtn = e.what();
		}
		catch (std::exception &e){
			rtn = e.what();
		}
		catch (...){

			rtn = "Error: an unhandled error occured";
		}
	}
	return rtn;
}