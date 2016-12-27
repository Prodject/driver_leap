#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <stdlib.h>


namespace CustomController
{
#define MAX_CONTROLLER_STRING_ELEMENTS 13
#define TIMESTAMP_TAG "TMST"
#define FUSED_ORIENTATION_TAG "FO"
#define BUTTON_TAG "BTN"
#define TRACKPAD_TAG "TRK"
#define END_TAG "END"

	struct Vector3
	{
		float x = 0;
		float y = 0;
		float z = 0;

		Vector3() {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		std::string ToString();
	};

	struct ButtonStates
	{
		// Controller data
		long long timestamp = 0;
		float touchpadX = 0;
		float touchpadY = 0;
		bool btn_touchpadPress = false;
		bool btn_trigger = false;
		bool btn_menu = false;
		bool btn_system = false;
		bool btn_grip = false;
		bool trackpad_touched = false;
		Vector3 orientation;

		void PrintToConsole();
	};

	class ControllerData
	{
	public:
		struct ButtonStates state;

	public:
		ControllerData();
		~ControllerData();
		ButtonStates ParseStringSerial(char * serialString);
		ButtonStates ParseStringUdp(std::string packetString);
	};
}
