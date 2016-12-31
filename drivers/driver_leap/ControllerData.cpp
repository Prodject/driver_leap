#include "ControllerData.h"

namespace CustomController
{
	using namespace std;

	vr::HmdQuaternion_t ControllerData::rotate_around_axis(float axisX, float axisY, float axisZ, const float &a)
	{
		// Here we calculate the sin( a / 2) once for optimization
		float factor = sinf(a* 0.01745329 / 2.0);

		// Calculate the x, y and z of the quaternion
		float x = axisX * factor;
		float y = axisY * factor;
		float z = axisZ * factor;

		// Calcualte the w value by cos( a / 2 )
		float w = cosf(a* 0.01745329 / 2.0);

		float mag = sqrtf(w*w + x*x + y*y + z*z);

		vr::HmdQuaternion_t result = { w / mag, x / mag, y / mag, z / mag };
		return result;
	}

	ControllerData::ControllerData()
	{
		useControllerOrientation = false;
		leftState.ResetCenter();
		rightState.ResetCenter();
	}

	ControllerData::~ControllerData()
	{
	}

	vr::HmdQuaternion_t operator*(const vr::HmdQuaternion_t& a, const vr::HmdQuaternion_t& b)
	{
		vr::HmdQuaternion_t tmp;

		tmp.w = (b.w * a.w) - (b.x * a.x) - (b.y * a.y) - (b.z * a.z);
		tmp.x = (b.w * a.x) + (b.x * a.w) + (b.y * a.z) - (b.z * a.y);
		tmp.y = (b.w * a.y) + (b.y * a.w) + (b.z * a.x) - (b.x * a.z);
		tmp.z = (b.w * a.z) + (b.z * a.w) + (b.x * a.y) - (b.y * a.x);

		return tmp;
	}

	void ControllerData::ParseStringSerial(char* serialString)
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
		rightState.btn_trigger = (buttonState & 16) > 0;
		rightState.btn_touchpadPress = (buttonState & 8) > 0;
		rightState.btn_menu = (buttonState & 4) > 0;
		rightState.btn_system = (buttonState & 2) > 0;
		rightState.btn_grip = (buttonState & 1) > 0;
		rightState.touchpadX = stof(parsed[1]);
		rightState.touchpadY = stof(parsed[2]);
	}

	void ControllerData::ParseStringUdp(string packetString)
	{
		ButtonStates* targetState;
		if (packetString[0] == 'L' && packetString[1] == '#')
		{
			targetState = &leftState;

		}
		else if (packetString[0] == 'R' && packetString[1] == '#')
		{
			targetState = &rightState;
		}
		else
		{
			// Wrong packet format
			return;
		}

		packetString.erase(0, 2);
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
				targetState->timestamp = stoll(parsed[i]);
			}
			else if (parsed[i] == FUSED_ORIENTATION_TAG)
			{
				i++;
				targetState->orientation.x = stof(parsed[i]);
				i++;
				targetState->orientation.y = stof(parsed[i]);
				i++;
				targetState->orientation.z = stof(parsed[i]);

			}
			else if (parsed[i] == FUSED_QUATERNION_TAG)
			{
				i++;
				targetState->orientationQuat.w = stof(parsed[i]);
				i++;
				targetState->orientationQuat.x = stof(parsed[i]);
				i++;
				targetState->orientationQuat.y = stof(parsed[i]);
				i++;
				targetState->orientationQuat.z = stof(parsed[i]);

				//normalizeQauternion(&(targetState->orientationQuat));

			}
			else if (parsed[i] == setCenter_TAG)
			{
				i++;
				bool reset = stoi(parsed[i]) == 1;
				if (reset)
				{
					targetState->SetCurrentAsCenter();
				}
			}
			else if (parsed[i] == HMD_CORRECTION_QUAT_TAG)
			{
				//TODO: remove
				i++;
				//targetState->hmdCorrectionQuat.w = stof(parsed[i]);
				i++;
				//targetState->hmdCorrectionQuat.x = stof(parsed[i]);
				i++;
				//targetState->hmdCorrectionQuat.y = stof(parsed[i]);
				i++;
				//targetState->hmdCorrectionQuat.z = stof(parsed[i]);

			}
			else if (parsed[i] == BUTTON_TAG)
			{
				i++;
				int buttonState = stoi(parsed[i]);
				targetState->btn_trigger = (buttonState & 16) > 0;
				targetState->btn_touchpadPress = (buttonState & 8) > 0;
				targetState->btn_menu = (buttonState & 4) > 0;
				targetState->btn_system = (buttonState & 2) > 0;
				targetState->btn_grip = (buttonState & 1) > 0;
			}
			else if (parsed[i] == TRACKPAD_TAG)
			{
				i++;
				targetState->trackpad_touched = !(parsed[i] == "0");
				i++;
				targetState->touchpadX = stof(parsed[i]);
				i++;
				targetState->touchpadY = -stof(parsed[i]);
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
	}

	vr::HmdQuaternion_t ControllerData::GetLeftOrientation()
	{
		// TODO: optimize
		//vr::HmdQuaternion_t temp = leftState.orientationQuat * leftState.centerQuaternion;
		//normalizeQauternion(&temp);
		//return temp;
		return leftState.orientationQuat;
	}

	vr::HmdQuaternion_t ControllerData::GetRightOrientation()
	{
		// TODO: optimize
		//vr::HmdQuaternion_t temp = rightState.orientationQuat * rightState.centerQuaternion;
		//vr::HmdQuaternion_t temp = rightState.orientationQuat;
		//normalizeQauternion(&temp);
		//auto angleCorrection = rotate_around_axis(1.0f, 0.0f, 0.0f, 90.0f);
		//temp = angleCorrection * temp;
		//temp.w = temp.w * -1.0f;
		//temp = temp * rightState.centerQuaternion;
		//return temp;
		return rightState.orientationQuat;
	}

	vr::HmdQuaternion_t* ControllerData::normalizeQauternion(vr::HmdQuaternion_t * quat)
	{
		double mag = quat->w * quat->w + quat->x * quat->x + quat->y * quat->y + quat->z * quat->z;
		quat->w /= mag;
		quat->x /= mag;
		quat->y /= mag;
		quat->z /= mag;
		return quat;
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

	void ButtonStates::ResetCenter()
	{
		centerQuaternion.w = 1.0f;
		centerQuaternion.x = 0.0f;
		centerQuaternion.y = 0.0f;
		centerQuaternion.z = 0.0f;
	}

	void ButtonStates::SetCurrentAsCenter()
	{
		//centerQuaternion = orientationQuat;
		//double d = centerQuaternion.w * centerQuaternion.w + centerQuaternion.x * centerQuaternion.x
		//	+ centerQuaternion.y * centerQuaternion.y + centerQuaternion.z * centerQuaternion.z;
		//centerQuaternion.w = (-1.0f * centerQuaternion.w) / d;
		//centerQuaternion.x = (-1.0f * centerQuaternion.x) / d;
		//centerQuaternion.y = (-1.0f * centerQuaternion.y) / d;
		//centerQuaternion.z = (-1.0f * centerQuaternion.z) / d;

		//centerQuaternion.w = centerQuaternion.w * -1.0f;
	}

	std::string Vector3::ToString()
	{
		return to_string(x) + ", " + to_string(y) + ", " + to_string(z);
	}

}