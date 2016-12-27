#pragma once
#include <string>
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include "ControllerData.h"

namespace SocketReaderPlugin
{
#define BUFLEN 512  //Max length of buffer
#define DEFAULT_PORT 5555 // The default UDP port



	class UdpSocket
	{
	// Variables
	private:
		//static const std::string senderIP;
		u_short udpPort;
		WSADATA wsaData;
		SOCKET mySocket;
		sockaddr_in server;
		sockaddr_in socket_other;
		bool listeningEnabled = false;
		char socketBuffer[BUFLEN];
		int receiveLength;
		int slen = sizeof(socket_other);
		//CustomController::ControllerData controller; //DEBUG

	// Fucntions
	private:
		int InitializeWinsock();
		void CreateSocket();
		void StartListening();

	public:
		UdpSocket();
		UdpSocket(u_short port);
		char* ReadFromSocket();
		~UdpSocket();
		void Init();
		void Open();
		void Close();
	};
}