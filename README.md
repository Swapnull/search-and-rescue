# Search and Rescue
This project is a search and rescue robot for the programming things module

## Installation
- Ensure you have the [Arduino IDE](http://www.arduino.cc/en/Main/Software)
- clone the project using
```
git clone https://github.com/swapnull/search-and-rescue.git
```
- Install the libraries provided if needed by putting them in your Arduino project folder.

## Usage
- Follow the hardware setup below.
- Follow the course setup below.
- Upload the code in main.ino onto your arduino.
- Place the zumo in the hallway. It would be best if you placed it on the 'wall' line, but it is not neccessary.
- Press the button on the back of the zumo.

### Hardware Setup
The shields recommended are the Arduino Uno (rev 3), pololu zumo (1.2) and an xbee shield.

- A HC-SR04 ultrasonic sensor module should be plugged with trig into pin 4 and echo into pin 5.
- A Xbee sheid should be plugged into the arduino
- Header pins should be put on the current stack.
- The Xbee and arduino stack should be plugged into the zumo.
