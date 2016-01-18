# Search and Rescue
This project is a search and rescue robot for the programming things module



## Usage
- Follow the hardware setup below.
- Follow the software setup below.
- Follow the course setup below.
- Place the Zumo on a 'wall' and press the button to configure the sensors.
- Place the Zumo in the middle of the corridor and press the button to start.

### Hardware Setup
The shields used for development were the Arduino Uno (rev 3), Pololu Zumo (1.2), a Hobbietronics XBee shield and an XBee explorer.

#### XBee
The XBee's are not mandatory. If you use an XBee, it should be setup following the [Sparkfun tutorial](https://learn.sparkfun.com/tutorials/exploring-xbees-and-xctu).

#### Main Stack
- A HC-SR04 ultrasonic sensor module should be plugged with trig into pin 6 and echo into pin 4.
- A XBee shield should be attached onto the Arduino (optional).
- Header pins should be put on the current stack.
- The XBee and Arduino stack should be attached onto the Zumo.
- Using a serial cable, plug the stack into a pc and follow the installation section.

The HC-SR04 and any jumper cables used should be secured using electrical tape or something similar.


#### [Arduino IDE](http://www.arduino.cc/en/Main/Software)
- Ensure you have the [Arduino IDE](http://www.arduino.cc/en/Main/Software)
- clone the project using
```
git clone https://github.com/swapnull/search-and-rescue.git
```
- Install the libraries provided if needed by taking them from lib and putting them into your Arduino project folder.

#### [Arturo](https://github.com/scottdarch/Arturo)
- Ensure you have downloaded and installed [Arturo](https://github.com/scottdarch/Arturo).
- clone the project using
```
git clone https://github.com/swapnull/search-and-rescue.git
```
- Plug in the hardware stack shown in the hardware setup.
- Open a terminal, navigate to the project folder and run the command.
```
ano build && ano upload
```

### Course Setup
The course must be set up with the following measurements.
- The corridor width is approximately 1.2x the width of the Zumo.
- The room depth is approximately 1.5x the width of the corridor.
- The door width is approximately 2x the width of the corridor.
- The width of the wall should be around 2cm.

Using those constraints, the course can be setup however desired so long as there are no two rooms opposite each other.

### Hand-in Documents
- There is a report that is roughly 1500 words long that can be found in /report.docx.
- There are images of the zumo setup that can be found at /assets/img.
- There are videos that can be found at /assets/video. These videos contain the individual sections (moving down corridor, going in and out of rooms, turning corners, reaching the course end)
