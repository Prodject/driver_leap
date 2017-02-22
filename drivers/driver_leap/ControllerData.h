#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "openvr_driver.h"
#include "madgwick_algorithm_c\MahonyAHRS\MahonyAHRS.h"


namespace CustomController
{
#define DEGREE_TO_RADIAN 0.0174532925
#define MAX_CONTROLLER_STRING_ELEMENTS 13
#define TIMESTAMP_TAG "TMST"
#define FUSED_ORIENTATION_TAG "FO"
#define FUSED_QUATERNION_TAG "FQ"
#define setCenter_TAG "SCT"
#define HMD_CORRECTION_QUAT_TAG "HMDC"
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
		bool btn_magicMove = false;
		bool trackpad_touched = false;
		Vector3 orientation; //TODO: remove the euler orientation.
		vr::HmdQuaternion_t orientationQuat;
		vr::HmdQuaternion_t centerQuaternion;
		vr::HmdQuaternion_t zeroQuaternion;
		void PrintToConsole();
		void ResetCenter();
		void ResetZero();
		void SetAsCenter(vr::HmdQuaternion_t hmdRot);
		void SetAsZero(vr::HmdQuaternion_t zeroQuat);
	};

	class ControllerData
	{
	private:
		vr::HmdQuaternion_t currentHmdRotation;
		vr::HmdQuaternion_t deg90_angleCorrection;
	public:
		ButtonStates rightState;
		ButtonStates leftState;
		bool useControllerOrientation;
		bool useLeftController;
		bool useRightController;

	public:
		ControllerData();
		~ControllerData();
		void ParseStringSerial(char * serialString);
		void ParseStringUdp(std::string packetString);
		vr::HmdQuaternion_t GetLeftOrientation();
		vr::HmdQuaternion_t GetRightOrientation();
		vr::HmdQuaternion_t* normalizeQauternion(vr::HmdQuaternion_t* quat);
		vr::HmdQuaternion_t ControllerData::rotate_around_axis(float angleX, float angleY, float angleZ, const float &a);
		void SetCurrentHMDOrientation(vr::HmdQuaternion_t hmdRot);
	};
}
