# Robot remote control with IR

Assignment for TME 441 Mechatronics (extra labor) — remote control for the W robot with an IR remote.

By Joshua Klymenko — solo project.

## Introduction

This project is to allow the remote control of a robot with the implementation of an emergency  stop. The robot uses two motors in a similar fashion as to how "tank steering" works, and will have 5 main functions, driving forwards, backwards, turn left and right, and the previously mentioned emergency stop, which will halt the motors immediately once the command is received, then proceed with the next command as normal. This project was chosen due to it's important nature in the modern worlds reliance on wireless connections, and mechatronics.

By the end of this project I was able to successfully command the robot its 4 driving functions and implement the emergency stop. This project will cover how an IR receiver can be configured to work with a PWM motor, and problems that occurred due to a timing issue in the Arduino Mega. 

_Figure 1: IR Receiver and IR Remote._
<img width="512" height="512" alt="image" src="https://github.com/user-attachments/assets/8e236795-3cee-4078-b050-9fa5e9462437" />

## Specifications

We have our simplified parts table in Tab. 1. We must not any other hardware besides those to accomplish the project. Our exact project specifications include: 
1. Demonstrate remote control including driving forward and backwards, turning CW and CCW, and halting.
2. The halt function should effectively interrupt any currently-running command.

The various button and their commands:
- 2 goes forward 10cm
- 4 turns counter-clockwise 30 degrees
- 5 is the emergency stop button
- 6 turns clockwise 30 degrees
- 8 goes 10cm backwards

Table 1: Parts Summary for this project
| Part | Purpose |
| ------------- | ------------- |
| Arduino Mega 2560 | Microcontroller |
| Gear motor | Motor for Wheel Control |
| HX1838 & VS1838  |  NEC IR Wireless Remote |

_Figure 2: Wiring diagram of the motor encoder._

<img width="720" height="400" alt="image" src="https://github.com/user-attachments/assets/c5cd8f2b-bac3-4258-8798-d073a318c53d" />


## Plan

For our code solution, it is remarkably simple. For this project we used the IRremote library (https://github.com/Arduino-IRremote/Arduino-IRremote). In this code, once the IR Receiver gets a signal, it decodes it using the IrReceiver.decode() function. It then checks if the command decoded (For example, command = 0x19, which means forward), matches one of the if statements. If true, then it executes the proper function (drive or turn). Implementing the emergency stop, however, proved difficult. This will be explained in the next section.

To see if the project is a success it will, in order:
1. Drive forwards 30cm
2. Turn 60 degrees left
3. Go backwards 60cm.
4. Turn 30 degrees right.
5. Go forwards, but then halt after pressing the forward button*. If the robot halts, success.
*Note: The IR receiver, due to the electrical noise of the lab and the surroundings, sometimes misses the command sent. This means for emergency stop, it is best to spam the button.

## Problems we ran into

The first problem was that driveDistance() and rotateCCW() used blocking loops. While either function was running, the main loop could not process new IR commands. As a result, the emergency stop could not interrupt the robot during movement. The second problem was a timer conflict between IRremote and the motor control system. Both libraries relied on AVR timers, so using the same timer interfered with the motor PWM signals. This caused the motors to behave incorrectly and prevented reliable movement control.

The second issue that occurred was wavelength. When using the remote, it would only work when the total distance away from the reciever was ~1 meter away. Further research shows why this occured. The IR receiver is simply getting "blinded" by the overly bright infrared light when the remote is closer than 1 meter, which blurs the signal pulses together so your Arduino cannot decode them. There is also the aspect of electrical noise that interfere with the commanded signal.

## Final solution

The final approach separates the IR commands from the movement functions. Each button is mapped to a specific movement: forward, backward, left, or right. The movement functions use encoder feedback to control distance and rotation. The emergency stop is checked while the robot is moving, allowing the code to immediately halt both motors when button 5 is pressed.

With our current robot, it completes the path as expected. Instead of the previous 10cm, it was opted for 200cm in order to better test and verify the solution regarding the emergency stop. This however did not change the solution and how it performed, and still it stopped immediately, with only a centimeter traveling after the command was recieved.

_Figure 4: IR Remote to Arduino flow chart._


## Conclusion

Overall, the project is a success. The emergency stop works immediately once the signal is recieved, and stops the robot before it can go any further. The IR reciver paired with the Arduino Mega and two PWM motors allow the robot to move wherever needed remotely.

If I were to redo this project I would most definetely allow to combine signals together in order to have a consistent driving path. For example, I would hold on to the forward button and turn by pressing the left or right as needed. That way I do not need to stop before every instruction, but guide it in real time.
