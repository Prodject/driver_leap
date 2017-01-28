#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

namespace SocketReaderPlugin
{
#define COM_PORT L"COM15"
#define ARDUINO_WAIT_TIME 2000
#define MESSAGE_SEPARATOR '|'

	class SerialReader
	{
	private:
		//Serial comm handler
		HANDLE hSerial;
		//Connection status
		bool connected;
		//Get various information about the connection
		COMSTAT status;
		//Keep track of last error
		DWORD errors;
	public:
		SerialReader();
		~SerialReader();
		void Init(LPCSTR portName);
		int SerialReader::ReadData(char *buffer, unsigned int nbChar);
		bool isConnected();
		static bool FindWholeMessage(char* data, int length, int *startIndex, int *endIndex);
	};
}

