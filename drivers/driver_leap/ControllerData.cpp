#include "ControllerData.h"

namespace CustomController
{
	using namespace std;

	ControllerData::ControllerData()
	{
		useControllerOrientation = false;
	}


	ControllerData::~ControllerData()
	{
	}

	ButtonStates ControllerData::ParseStringSerial(char* serialString)
	{
		stringstream sStream;
		sStream.str(serialString);
		string item;
		vector<string> parsed;
		int i = 0;
		while (getline(sStream, item, ';'))
		{
			parsed.push_back(item);
			i++;
		}

		int buttonState = stoi(parsed[0]);
		state.btn_trigger = (buttonState & 16) > 0;
		state.btn_touchpadPress = (buttonState & 8) > 0;
		state.btn_menu = (buttonState & 4) > 0;
		state.btn_system = (buttonState & 2) > 0;
		state.btn_grip = (buttonState & 1) > 0;
		state.touchpadX = stof(parsed[1]);
		state.touchpadY = stof(parsed[2]);
		return state;
	}

	ButtonStates ControllerData::ParseStringUdp(string packetString)
	{
		stringstream sStream;
		sStream.str(packetString);
		string item;
		vector<string> parsed;
		int i = 0;
		while (getline(sStream, item, ';'))
		{
			parsed.push_back(item);
			i++;
		}

		for (int i = 0; i < parsed.size(); i++)
		{
			if (parsed[i] == TIMESTAMP_TAG)
			{
				i++;
				state.timestamp = stoll(parsed[i]);
			}
			else if (parsed[i] == FUSED_ORIENTATION_TAG)
			{
				i++;
				state.orientation.x = stof(parsed[i]);
				i++;
				state.orientation.y = stof(parsed[i]);
				i++;
				state.orientation.z = stof(parsed[i]);

			}
			else if (parsed[i] == FUSED_QUATERNION_TAG)
			{
				i++;
				state.orientationQuat.w = stof(parsed[i]);
				i++;
				state.orientationQuat.x = stof(parsed[i]);
				i++;
				state.orientationQuat.y = stof(parsed[i]);
				i++;
				state.orientationQuat.z = stof(parsed[i]);

			}
			else if (parsed[i] == HMD_CORRECTION_QUAT_TAG)
			{
				i++;
				//state.hmdCorrectionQuat.w = stof(parsed[i]);
				i++;
				//state.hmdCorrectionQuat.x = stof(parsed[i]);
				i++;
				//state.hmdCorrectionQuat.y = stof(parsed[i]);
				i++;
				//state.hmdCorrectionQuat.z = stof(parsed[i]);

			}
			else if (parsed[i] == BUTTON_TAG)
			{
				i++;
				int buttonState = stoi(parsed[i]);
				state.btn_trigger = (buttonState & 16) > 0;
				state.btn_touchpadPress = (buttonState & 8) > 0;
				state.btn_menu = (buttonState & 4) > 0;
				state.btn_system = (buttonState & 2) > 0;
				state.btn_grip = (buttonState & 1) > 0;
			}
			else if (parsed[i] == TRACKPAD_TAG)
			{
				i++;
				state.trackpad_touched = !(parsed[i] == "0");
				i++;
				state.touchpadX = stof(parsed[i]);
				i++;
				state.touchpadY = -stof(parsed[i]);
			}
			else if (parsed[i] == END_TAG)
			{
				//cout << "END tag found" << endl;
			}
			else
			{
				cout << "Unknown data foud: " << parsed[i];
			}
		}
		return state;
	}

	vr::HmdQuaternion_t ControllerData::GetOrientationQuaternion()
	{
		//state.orientationQuat.w *= -1.0f;
		//state.orientationQuat.x *= -1.0f;
		//state.orientationQuat.y *= -1.0f;
		//state.orientationQuat.z *= -1.0f;
		//state.orientationQuat.x *= -1.0f;
		//float temp = state.orientationQuat.y;
		//state.orientationQuat.y = state.orientationQuat.z;
		//state.orientationQuat.z = temp;
		return state.orientationQuat;
	}

	void ButtonStates::PrintToConsole()
	{
		cout << "Timestamp: " << timestamp << endl
			<< "Orientation: " << orientation.ToString() << endl
			<< "Toucpad: " << touchpadX << ", " << touchpadY << (trackpad_touched ? " touched" : " not touched") << endl
			<< "Trigger: " << btn_trigger << endl
			<< "Touchpad press: " << btn_touchpadPress << endl
			<< "Menu: " << btn_menu << endl
			<< "System: " << btn_system << endl
			<< "Grip: " << btn_grip << endl << endl;
	}

	std::string Vector3::ToString()
	{
		return to_string(x) + ", " + to_string(y) + ", " + to_string(z);
	}

}