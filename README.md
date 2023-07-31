# CPE 648 Lab Exercise 4: ESP32 Web Server

This repository contains the Arduino code for Laboratory Exercise 4: Creating ESP32 Web Server for the subject CPE 648 - Microcontrollers and Embedded Computing Systems. 

## Overview

The project involves creating a web server on an ESP32 that serves an HTML interface to control the state of 8 LEDs. There are 5 different light sequences to choose from, and the sequences can be triggered from the web interface, causing the LEDs to light up in the selected sequence.

The repository contains two sketches:

- `sketch.ino` is the basic web server with buttons for the various LED sequences.
- `sketch2.ino` extends `sketch.ino` by adding a Piezo Buzzer.

## Setup and Usage

1. Connect your ESP32 to your computer and open the Arduino IDE.
2. Select the correct board (ESP32) and COM port from the Tools menu.
3. Open either `sketch.ino` or `sketch2.ino` from this repository in the IDE, depending on whether you want to use the Piezo Buzzer.
4. Replace the `WIFI_SSID` and `WIFI_PASSWORD` constants in the sketch with your WiFi network's SSID and password.
5. Upload the sketch to your ESP32.
6. After the sketch has been uploaded, open the Serial Monitor (Tools > Serial Monitor).
7. The ESP32 will connect to your WiFi network and print its IP address in the Serial Monitor. Copy this IP address.
8. Paste the IP address into the address bar of a web browser on a device connected to the same WiFi network.
9. You should now see the HTML interface for controlling the LEDs. Click the buttons to trigger the different LED sequences.

## Team Members

- [Engr. John Julius Baldia](https://github.com/imjjb9)
- Engr. Rynefel B. Elopre

Feel free to reach out to us if you have any questions or need further assistance with the code.
