# finger-fracture-physical-therapy-device

### Note that this whole project was made by me from the Reverse-Engineering phase to the design to the fabrication of the final product

## **Abstract**

### This project was for helping the physical therapist to help aid the patiets that had a broken or fractured finger
### The device mainly uses a potentiometer to send the data of the the finger's angle to LCD and to the phone through a bluetooth module
### The device has Arduino Nano as it's main controller, Potentiometer to send the Angle Data, LCD to show the data, Bluetooth module to send the data to the phone , 2 push buttons one for the hold of the readings, the second is for getting the angles in degree and minute format

### The project went throgh four phases

## Phase 1 Idea
### The idea was to replicate the behaviour of a digital protractor device that was already in the market to invent something that may help the patients with broken fingers
## Phase 2 Reverse-Engineering
### So i started Reverse-Engineering the Digital protractor as shown below 
![protractor](https://github.com/ENG-MohamedEssam/finger-fracture-physical-therapy-device/blob/main/images/reverse-engineered-protractor.jpeg)
### When i understood it's behaviour i started to find similar components in the Egyptian market 
### the components that i didn't find was the induction encoder so i had to find a replacement
### i thought of three things, Potentiometer, Normal Encoder, IMU
### I even wrote codes for the three sesors.
## Problems 
### IMU was too expensive for such a project but it kind of worked correctly
### The encoders in Egypt had too little pulses so the error in Degrees was not acceptable
### The potentiometer was the cheapest sensor so in mass production that was the ideal choice but it gave an error in the reading as well that was not acceptable and the Arduino Nano was only 10 bits in ADC

## Scenario to fix the Potentiameter's problem
### To fix the problem in the potentiometer there were two solutions either i use the algorithm over sampling and decimation or i use the sliding window filter
### so i went along with the over sampling and decimation technique, it was by having many readings in an array then get the average of it many times, that makes the reading more accurate.
### The error after that algorithm was +/- 0.02 and that was more than acceptable.

## Phase 3 Design
### In this phase i started to design the circuit on protues such as follows
![sim](https://github.com/ENG-MohamedEssam/finger-fracture-physical-therapy-device/blob/main/images/sim.jpeg)
### After i made sure it works fine i turned to try the components connections on a bread board as follows 
![breadboard](https://github.com/ENG-MohamedEssam/finger-fracture-physical-therapy-device/blob/main/images/breadboard.PNG)
### Then i designed the circuit on Eagle-Cad program 
### I even designed one of the three parts on SolidWorks as you can see below 
![Part](https://github.com/ENG-MohamedEssam/finger-fracture-physical-therapy-device/blob/main/images/sld-prt.png)
### the other two parts were open source 

## Phase 4 fabrication
### in this phase i fabricated the PCB and tested it and it worked fine, below are some images of the final product (The band)
![band-img1](https://github.com/ENG-MohamedEssam/finger-fracture-physical-therapy-device/blob/main/images/img%20(2).jpeg)
![band-img2](https://github.com/ENG-MohamedEssam/finger-fracture-physical-therapy-device/blob/main/images/device%20band.jpeg)
### The following is a video of me explaining how the whole process works
[Explanation](https://drive.google.com/drive/folders/1u0ruocD8NN2b_IU45vWWJVkj_FHNx0_M?usp=share_link)