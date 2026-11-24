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

_Paragraph 2: describe the other problems — anything else that went wrong during wiring, testing, or the demo, and what each one taught you._

![Figure 3: The test setup where the problem showed up](images/figure-3-test-setup.png)
_Figure 3: The test setup where the problem showed up._

## Final solution

The final approach separates the IR commands from the movement functions. Each button is mapped to a specific movement: forward, backward, left, or right. The movement functions use encoder feedback to control distance and rotation. The emergency stop is checked while the robot is moving, allowing the code to immediately halt both motors when button 5 is pressed.

_Paragraph 2: describe the result — what the robot does now, how it behaved in the demo, and how the final solution compares to the original plan._

![Figure 4: The finished robot with the IR remote](images/figure-4-finished-robot.png)
_Figure 4: The finished robot with the IR remote._

## Conclusion

_Paragraph 1: sum up what was built and what worked — the remote control, the movements, and the emergency stop._

_Paragraph 2: reflect on what you learned and what you would do differently next time._
