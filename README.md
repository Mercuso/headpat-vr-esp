# HeadpatVR firmware
Firmware for ESP32 microcontroller to accept signals over WiFi network and move servo motors  

Works with [HeadpatVR server](https://github.com/Mercuso/headpat-vr) that accepts OSC signals from VRChat

## Prerequisites
You need [PlatformIO IDE](https://platformio.org/) to compile and upload it to your esp8266 controller
Installation instructions can be found here: https://platformio.org/install/ide?install=vscode  

## Configuration
Create the file local.ini with the following parameters:  
```toml
[wifi]
SSID = your-wifi-ssid
PASSWORD = YourWifi#Pa$$W0Rd

[servo]
SERVO_ROTATION_ANGLE = 120
```
replace `your-wifi-ssid` and `YourWifi#Pa$$W0Rd` to your wifi SSID and password  
It's recommended to keep the same rotation angle if your servo motor supports it

## Build and upload
You can find the "Build" and "Upload" buttons in the bottom bar  
Once local.ini file is ready, click on "Build". Once the build process is finished, click on "Upload"  
> [!WARNING]
> Disconnect at least one motor from the board before uploading firmware. Otherwise, the power supply over USB will be unstable and uploading may fail  

Once the upload is finished, you can click on "Serial monitor" button to see the logs
