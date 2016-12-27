#include "SocketReaderPlugin.h"

namespace SocketReaderPlugin
{
	UdpSocket::UdpSocket()
	{
		udpPort = DEFAULT_PORT;
	}

	UdpSocket::UdpSocket(u_short port)
	{
		udpPort = port;
	}

	UdpSocket::~UdpSocket()
	{
		if (listeningEnabled)
		{
			listeningEnabled = false;
			closesocket(mySocket);
			WSACleanup();
		}
	}

	void UdpSocket::Init()
	{
		InitializeWinsock();
		CreateSocket();
	}

	void UdpSocket::Open()
	{
		//controller = CustomController::ControllerData(); //DEBUG
		//StartListening();
	}

	void UdpSocket::Close()
	{
		if (listeningEnabled)
		{
			listeningEnabled = false;
			closesocket(mySocket);
			WSACleanup();
		}
	}

	int UdpSocket::InitializeWinsock()
	{
		// Winsock initialization
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed: %d\n", iResult);
			exit(EXIT_FAILURE);
		}
		return iResult;
	}

	void UdpSocket::CreateSocket()
	{
		mySocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (mySocket == INVALID_SOCKET)
		{
			printf("Unable to create socket\n");
			exit(EXIT_FAILURE);
		}
		printf("Socket created\n");

		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(udpPort);

		// Bind
		if (bind(mySocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			std::cout << "Bind failed with error code: " << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		printf("Bind done\n");
	}
	void UdpSocket::StartListening()
	{
		// Listening for data
		listeningEnabled = true;
		printf("Listening...\n");

		char result[INET_ADDRSTRLEN];

		while (listeningEnabled)
		{
			fflush(stdout);

			// Clear the buffer by filling null. May have previously received data.
			memset(socketBuffer, '\0', BUFLEN);

			// Try to receive data. Blocking call!
			if (receiveLength = recvfrom(mySocket, socketBuffer, BUFLEN, 0, (sockaddr*)&socket_other, &slen) == SOCKET_ERROR)
			{
				std::cout << "recvfrom() failed with error code: " << WSAGetLastError() << std::endl;
				exit(EXIT_FAILURE);
			}

			// Print the details of the received data.
			//InetNtop(AF_INET, &socket_other.sin_addr, (PWSTR)result, sizeof(result));
			//printf("Received packet from: {0}:{1}", inet_ntoa(socket_other.sin_addr), ntohs(socket_other.sin_port));
			//std::cout << "Received packet from : " << std::string(result, sizeof(result)) << std::endl;
			//std::cout << "Packet data: " << socketBuffer << std::endl;
			//controller.ParseString(socketBuffer).PrintToConsole();
		}
	}

	char* UdpSocket::ReadFromSocket()
	{
		//char result[INET_ADDRSTRLEN];

		fflush(stdout);

		// Clear the buffer by filling null. May have previously received data.
		memset(socketBuffer, '\0', BUFLEN);

		// Try to receive data. Blocking call!
		if (receiveLength = recvfrom(mySocket, socketBuffer, BUFLEN, 0, (sockaddr*)&socket_other, &slen) == SOCKET_ERROR)
		{
			std::cout << "recvfrom() failed with error code: " << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}

		// Print the details of the received data.
		//InetNtop(AF_INET, &socket_other.sin_addr, (PWSTR)result, sizeof(result));
		//printf("Received packet from: {0}:{1}", inet_ntoa(socket_other.sin_addr), ntohs(socket_other.sin_port));
		//std::cout << "Received packet from : " << std::string(result, sizeof(result)) << std::endl;
		//std::cout << "Packet data: " << socketBuffer << std::endl;
		return socketBuffer;
	}
}