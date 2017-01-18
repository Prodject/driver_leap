#include "SerialReader.h"

namespace SocketReaderPlugin
{
	SerialReader::SerialReader()
	{
		connected = false;
	}

	void SerialReader::Init(LPCSTR portName)
	{
		// Open file
		hSerial = CreateFile(portName,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		// Check if it was succesful
		if (hSerial == INVALID_HANDLE_VALUE)
		{
			std::cout << "Failed to open port " << portName << std::endl;
			return;
		}
		else
		{
			std::cout << "Port opened: " << portName << std::endl;
			DCB dcbSerialParams = { 0 };

			//Try to get the current parameters
			if (!GetCommState(this->hSerial, &dcbSerialParams))
			{
				//If impossible, show an error
				printf("Failed to get current serial parameters!");
			}
			else
			{
				// Define serial connection parameters
				dcbSerialParams.BaudRate = CBR_115200; //TODO: read baud rate from vr settings
				dcbSerialParams.ByteSize = 8;
				dcbSerialParams.StopBits = ONESTOPBIT;
				dcbSerialParams.Parity = NOPARITY;
				//reset upon establishing a connection
				dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

				//Set the parameters and check for their proper application
				if (!SetCommState(hSerial, &dcbSerialParams))
				{
					printf("ALERT: Could not set Serial Port parameters");
				}
				else
				{
					//If everything went fine we're connected
					connected = true;
					//Flush any remaining characters in the buffers 
					PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

					std::cout << "Connected" << std::endl;

					//We wait 2s as the arduino board will be reseting
					Sleep(ARDUINO_WAIT_TIME);
				}
			}
		}
	}

	int SerialReader::ReadData(char *buffer, unsigned int dataLength)
	{
		//Number of bytes we'll have read
		DWORD bytesRead;
		//Number of bytes we'll really ask to read
		unsigned int toRead;

		//Use the ClearCommError function to get status info on the Serial port
		ClearCommError(this->hSerial, &this->errors, &this->status);

		//Check if there is something to read
		if (this->status.cbInQue>0)
		{
			//If there is we check if there is enough data to read the required number
			//of characters, if not we'll read only the available characters to prevent
			//locking of the application.
			if (this->status.cbInQue>dataLength)
			{
				toRead = dataLength;
			}
			else
			{
				toRead = this->status.cbInQue;
			}

			//Try to read the require number of chars, and return the number of read bytes on success
			if (ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL))
			{
				return bytesRead;
			}

		}

		//If nothing has been read, or that an error was detected return 0
		return 0;

	}

	SerialReader::~SerialReader()
	{
		if (connected)
		{
			connected = false;
			CloseHandle(hSerial);
		}
	}

	bool SerialReader::isConnected()
	{
		return connected;
	}
}
