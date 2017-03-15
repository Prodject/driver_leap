# Leap Motion + External orientation driver for SteamVR
### Note that this is all work in progress, and experimental. The goal was to make something, that can deliver a usable emulation of the Vive controllers, to be able to develop for it, without the real thing. 
## What is this?
This is basically a modification of cbuchner1's leap motion driver for SteamVR (https://github.com/cbuchner1/driver_leap). The main difference is, that here, the leap motion only provides the positional data. Instead of the gesture based controls, I used a custom built controller, with a built-in IMU, to provide orientation data. With this I have more accurate and much smoother rotation, especially with closed hands.

## Some notes about the code
If you look at the code, keep in mind, that this project is basically an experiment. What do I mean by that? The code is working, but it's not pretty, not well documented and it's far from optimal.
Also, as it's based on an existing driver, which was based on the very not up-to-date Razer Hydra driver, published by Valve, it's still using OpenVR 1.0. Currently, it's working with the latest SteamVR beta, but it's possible, that with time something will break the backwards compatibility. I planned to update the OpenVR library under the driver, but the current version is very different from the original, and I didn't have the time to do it.

# The controllers
- The first prototype was an Android application, which used UDP packets to send the data to the PC. This application can be found here:
https://github.com/peter10110/Android-SteamVR-controller
- After the prototype was complete, and it worked, I started to build two custom, Arduino-based controllers. These are using USB to send the data on serial ports. These are working much better, than the phone-based prototype. The source code, and some photos of the build process can be found here:
https://github.com/peter10110/Arduino-SteamVR-controller/

## Installation of this driver.

- Get the Leap Motion Orion Beta runtimes https://developer.leapmotion.com/get-started
- Install the Visual C++ 2015 Update 2 redistributables (32 and 64 bits) https://www.microsoft.com/en-us/download/details.aspx?id=51682
- Download the source and compile it for yourself, or use the files in the "Compiled" folder. Just copy the "leap" folder into the "Steam\steamapps\common\SteamVR\drivers\" folder.
- Configure the driver's settings in the Steam/config/steamvr.vrsettings file. (the settings are detailed below)
- Start the Leap Motion service, then SteamVR. You should be good to go.

## Configuration
You have to add a new section, called "combinedLeap" to the existing steamvr.vrsettings file. It's also very important to add       "activateMultipleDrivers" : true to the "steamvr" section in the config file.
For example, here is my current vrsettings file:

```
#!json
{
   "audio" : {
      "onRecordDevice" : "{0.0.1.00000000}.{06b9c968-1d16-44e1-9143-5f619d64a142}"
   },
   "collisionBounds" : {
      "CollisionBoundsColorGammaA" : 147
   },
   "combinedLeap" : {
      "comPort1" : "\\\\.\\COM12",
      "comPort2" : "COM5",
      "useDeviceRotation" : true,
      "useLeftController" : true,
      "useRightController" : true,
      "useUdpController" : false
   },
   "compositor" : {
      "renderTargetMultiplier" : 1
   },
   "keyboard" : {
      "TutorialCompletions" : 1
   },
   "modelskins" : {
      "stage" : "",
      "stage_PublishedFileId" : "",
      "vr_controller_vive_1_5" : "C:\\Program Files (x86)\\Steam\\steamapps\\workshop\\content\\250820\\725853315\\vr_controller_vive_1_5\\vr_controller_vive_1_5.obj",
      "vr_controller_vive_1_5_PublishedFileId" : "725853315"
   },
   "steamvr" : {
      "activateMultipleDrivers" : true,
      "allowReprojection" : true,
      "background" : "C:\\Program Files (x86)\\Steam\\steamapps\\workshop\\content\\250820\\679490100\\Tron2.png",
      "defaultMirrorView" : 2,
      "directModeEdidPid" : 4121,
      "directModeEdidVid" : 53838,
      "mirrorViewGeometry" : "2168 0 880 948",
      "playAreaColor" : "#FFFFFF4F",
      "showMirrorView" : true
   }
}
```
The fields explained:
*"comPort1" : "COM3", // The COM port for the first serial controller.
*"comPort2" : "COM5", // The COM port for the second serial controller.
*"gripAngleOffset" : 45, // The default angle of the controller. Not implemented yet.
*"useDeviceRotation" : true, // If set to true, the driver will use the orientation from the controllers, instead of the one, measured by the Leap Motion.
*"useLeftController" : true, // Use a real controller, instead of gestures on the left hand.
*"useRightController" : true, // Use a real controller, instead of gestures on the right hand.
*"useUdpController" : false // Listen for UDP controller packets, on the 5555 port.

### Games/Experiences I've tested.
- The Lab (Some experiences work. The main problem is the limited FOV of the Leap Motion.)
- Raw Data (Some tricks needed, for eg. to get the sword from your back, but with pistol, it's quite good.)
- The Soulus Project (There are problems with the touchpad, but it's completely playable)

# Special thanks
Thanks to Florian Maurer, for inspiring my procect, by his very similar VR controller. His project can be found here:
http://flrnmrr.com/2016/11/30/arduino-vive-controller-emulation

## Building from Sourcecode (Developers only)

### Install Dependencies

1. Install SteamVR.
2. Install "Leap Motion Orion".
3. Fetch the OpenVR SDK 1.0.0 from https://github.com/ValveSoftware/openvr.

The solution and project files are for Visual Studio 2015.

### Configure Paths (copied from the original Leap Motion driver)

Under "Property Manager" in Visual Studio, expand any of the configurations and find "Paths".  Right click and select "Properties" and then "User Macros".  Modify the paths to match your setup.  InstallDir will be created, and will be configured as an external driver path for SteamVR.

### Build

You will probably want to build Release x86.  You can also build x64.  The post-build step will install the binaries and copy the resources to the configured InstallDir and register that path with SteamVR.

## Preapring The Leap Motion Driver for use (Developers only)

After building, the InstallDir should be a complete binary distribution.  To use it:

1. Register it with the SteamVR runtime via "vrpathreg adddriver $(InstallDir)".  This is done automatically by a Post-Build step, but if you copy the files elsewhere you will have to do it by hand.
2. Edit your config/steamvr.vrsettings to enable "activateMultipleDrivers".  This is what allows the hydra driver to co-exist with any HMD.  **Be sure to mind your commas.** Check vrserver.txt log to see if there were parse errors.  Many of the settings are described at https://developer.valvesoftware.com/wiki/SteamVR/steamvr.vrsettings .
```{
	...
	"steamvr" : {
        "activateMultipleDrivers" : true
   }
}```
3. If you are trying to use the Hydra driver without an HMD, you might want to enable driver_null (no HMD) or set "requireHmd": false.

After starting SteamVR, you should see controllers blinking in the status window until you move your hands into the field of view.

You can use "vrcmd" (with no arguments) to see a list of devices to verify things are working.
use "vrcmd" to verify things are loading:

```...
Driver leap : 2 displays
        leap (Serial number leap0_lefthand)
        leap (Serial number leap0_righthand)
...
```

You can also use "vrcmd --pollposes" (followed by an index number to limit the output) to see if things are working.

## Licenses

The code in this distribution is distributed under the terms of the LICENSE file in the root directory.

The compiled driver and the install directory use the Leap Motion Orion SDK.  Use subject to the terms of the Leap Motion SDK Agreement available at
https://developer.leapmotion.com/sdk_agreement.
