# particle-mon
IoT project with hardware and visualization using Particle (and gratuitous TMBG reference)

The hardware is a Particle Photon with the following hardware:

* BMP180 - temperature and pressure
* DHT11 - temperature and humidity
* CdS photocell - light levels

The Particle Photon is connected to the local wireless network and a webhook is set up to publish events to an external web page. The data is being stored in a flat file now, soon in an SQLite database. The wiring diagram will be in the folder for the Particle code since that code is closest to the hardware itself.

The next steps are to create visualizations for the data in a web page, create an Android widget for real-time monitoring, and a Slack bot for others to query the status.
