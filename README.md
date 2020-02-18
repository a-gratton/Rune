# Rune
A flexible gesture motion controller for IoT devices, created for MakeUofT 2020 in 24 hours. The system operates wirelessly through MQTT and is capable of discerning between 10 discrete gestures in real-time using a machine learning model.

This project was built collaboratively with [Felix Yang](https://github.com/YangF0917) and [Di Yang](https://github.com/diyang100)

**Hardware** 
* [Qualcomm DragonBoard 410c](https://developer.qualcomm.com/hardware/dragonboard-410c)
* [ESP32](https://www.espressif.com/en/products/hardware/esp32/overview)
* [MPU 9250 9-axis IMU](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/)


**Software**
* Python
* C
* Linux
* TensorFlow
* MQTT Messaging Protocol (Mosquitto Client)


## Project Design

### Gesture Sensing Unit
Movement is detected through the the ESP32 development board and the IMU. When the user presses an onboard button, the movement data is transfered to the ESP32 using I2C. This data is then parsed into a single string and published as an MQTT message to the DragonBoard broker. Position values are submitted every 25ms until the button is released.

### Networking
The DragonBoard acts as the broker for MQTT communication. Messages which are published from the sensing unit are recieved by the board, where they can then be interpreted by the subscribed client computer running our machine learning algorithim. The entire networking system is built on Debian Linux and runs natively; no external connections are required between any modulue of the project.

### Gesture Determination
Gestures are classified into 10 types by a machine learning model: positive and neagative movement along each cartesian axis, CW/CCW  complete rotations, and CW/CCW half rotations. A Python script is used to parse data from the MQTT broker and submit it into the model. The model was trained duing the event using data gathered from the gesture sensing unit, allowing precise compensation for inherant sensor defects. 

## Potential Expansions
* The machine learning model could be ran on the DragonBoard itself, removing the necessity of an external computer
* The gesture sensing unit could be miniaturized to fit into a small watch-type package
* More gestures could be added, including the possibility of custom user-trainable controls
* An API could be developed for easily interfacing Rune with other IoT devices


## Images

**Gesture Sensing Unit**
![NodeMCU and IMU](https://github.com/a-gratton/Rune/blob/master/images/ESP32.jpeg)

**Qualcomm DragonBoard**
![Qualcomm DragonBoard](https://github.com/a-gratton/Rune/blob/master/images/DragonBoard.jpeg)





## License
This project is licensed under the terms of the MIT license.

