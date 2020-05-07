# FAQ

Q. Where did you get such a crazy idea?  
A. It was an inside joke with my Melee friends in high school too outperform everyone in tournament using only OEM bongos. I have no sense of humor and took it in the most literal sense possible. In the end, I created [the most optimal controller](#pros-and-cons) to play Smash Bros.  

Q. Do you do commissions/are you taking orders/will you ever sell this?  
A. No, I did it for the meme.  

Q. Can I do this mod myself?  
A. Absolutely, there are resources on this wiki to help you do that. Once I add the R button to the controller, I will release a full guide detailing the process from start to finish including a video and list of materials.  

Q. How much do the materials cost?  
A. Bongos $2, Wii Nunchuck $2, the rest is pocket change or spare parts. It is astronomically more cost efficient than a GameCube controller.  

Q. How long does it take to do?  
A. Took me, a soldering veteran, no longer than an hour to do the entire mod.  

Q. Why not use an Arduino for Bongos?  
A. Several reasons:

1. Buffering causes poll drifting
2. No good way to map four buttons and a microphone to six axes
3. It has the opportunity to provide an unfair advantage in tournament

Q. Why use the regular Bongo mobo?  
A. It has the same IC as a regular GameCube controller mobo, for that reason:

1. There is no buffering, the sensors are connected directly to memory
2. The sensors are already mapped to the same location as the GameCube controller
3. It is an asthetic, electrical modification to a tournament legal controller that does not reduce hand motions so there is no unfair advatage

Q. How much controller delay is there with an Arduino?  
A. Upwards of 360 µs, the amount of time it takes to poll a controller.  

Q. What is buffering?  
A. When the controller receives a packet from the controller it is stored in memory. That region memory is called the buffer. The Auduino then retrieves the packet from the buffer to send to the console.  

Q. How is buffering different from the way the controller operates normally?  
A. The controller has its sensors mapped to memory, there is no buffer between the sensors and the IC.  

Q. What is poll drifting?  
A. Poll drifting occurs when an increase in the delay between the console's request and the controllers reply eventually causes the packet to be received between a frame or into the next frame of input. This means that the packet is dropped by the game because the input buffer on the console is subsequently overwritten by the packet received for the next frame. This is a normal phenomena for asynchronous serial protocals and it can occur on an OEM system. Similary, the Arduino has a buffer too, providing a second oppurtunity for packets to be dropped.  

Q. Is wiring the sensors on the controller to the Arduino faster?  
A. No, it would take at least 360 µs to read the sensors and packet the data.  

Q. Would polling more frequently reduce drifting?  
A. No, while it would reduce drifting from the Arduino, it would cause the Arduino to drift from the console more, unless you implement interrupts to allow the console to poll the Arduino.  

Q. Would implementing interrupts reduce drifting?  
A. Yes, but they cost upwards of 20 µs delay at 16 MHz and block the controller from being polled. Multithreading can fix this by using two threads, one for the console and one for the controller, to reduce the delay.  

Q. What is multithreading on Arduino?  
A. Having two PLCs running in parallel. For example, an Arduino Uno has both an ATMega 328p and 16u2 which can be accessed by flashing a custom bootloader such as Hoodloader2.  

Q. Does multithreading reduce drifting?  
A. No, it adds a second buffer.  

Q. Will a faster Arduino reduce drifting?  
A. Depends on the Arduino. Teensy 3.x will not, because it has a multi-stage pipeline that optimizes blocking opcode out during runtime. You can prevent this by executing volatile inline assembly code, however, that blocks interrupts which are required to respond to the console and controller without drifting. Teensy cannot multithread standalone, it has one MPU on board.  

## Pros and cons

Pros:

- It functions exactly the same as an OEM GameCube controller because it is an OEM GameCube controller
- You wont get permanent joint damage from banging on a drum
- You can fit a six pack inside it or two entire GameCube controllers plus snacks and a stick of deodorant
- The button membranes are the exact height for light shield, perfect for trigger mods
- Greater surface area for painting designs or putting stickers to decorate your controller
- They make everyone watching happy
- They stop everyone else from being salty
- No one you play against can John
- They make you the coolest smasher at tourney
- They teach you soldering, electrical engineering, computer architecture, computer networking, software engineering, how to play drums, and musical theory all at once

Cons:

- There are no cons
