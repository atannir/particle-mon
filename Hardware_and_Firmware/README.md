# Hardware
* Particle Photon Wi-Fi enabled IoT platform (A Particle board, if you will)
* BMP180 Pressure / Temperature / Altitude sensor
* DHT11 Temperature / Humidity sensor
* CdS photocell (unknown resistance)
* 10K resistor
* 8.2K resistor (or similar)
* Assorted jumper wires (pin to pin)
* Small breadboard

# Firmware
* Arduino-like code with libraries ported over
* Publish an event to be handled by the Particle cloud
* Expose certain functions for debug and ad hoc status queries

# Additional Steps
* Set up a webhook in the Particle console with the name of the event that you publish in the firmware .ino file
* The webhook will point to the URL of the script that will store and process the events from the device.
* See the next folder in the sequence for more information on that step.