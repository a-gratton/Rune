# Rune
A flexible gesture motion controller for IoT devices, created for MakeUofT 2020. The system is Capable of discerning between 10 discrete gestures in real-time.


**Hardware** 
* Qualcomm DragonBoard 410c
* NodeMCU ESP32
* MPU 9250 9-axis IMU


**Software**
* TensorFlow
* Numpy
* MQTT Messaging Protocol (Mosquitto Client)


## Images

**Qualcomm DragonBoard**
![Qualcomm DragonBoard](https://github.com/a-gratton/Rune/blob/master/images/DragonBoard.jpeg)

**NodeMCU and IMU**
![NodeMCU and IMU](https://github.com/a-gratton/Rune/blob/master/images/NodeMCU.jpeg)



## Architecture
Low level communication is handled via the MQTT messaging protocol with devices running on a shared wireless network. The DragonBoard subscripes to a topic using an instance of the mosquitto client, where it recieves messages pushed the the NodeMCU-IMU sensor combination.


A python script parses recieved MQTT messages and feeds them into our machine learning model, which then determines what gesture was performed. This gesture is then submitted as a POST request to a web front-end to simulate end-effector control.


## License
This project is licensed under the terms of the MIT license.

