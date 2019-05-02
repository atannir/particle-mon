// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_DHT.h>

// This #include statement was automatically added by the Particle IDE.
#include <BMP180.h>

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11		// DHT 11
//#define DHTTYPE DHT22		// DHT 22 (AM2302)
//#define DHTTYPE DHT21		// DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

#define ALTITUDE 30
// meters above sea level, approximate.
// found online, corrected slightly
// Without this, the readings default to Denver, where SparkFun is based.
// I was getting about half of the expected barometric pressure, and Denver is at about 0.5 atmosphere.

#define DARKMAX 600
#define LIGHTMIN 1800

DHT dht(DHTPIN, DHTTYPE);
BMP180 bMP180;

// TODO:
// Correct pressure for altitude
// DONE: Make sensor values show in a single string
// DONE: Write php script to record to a log file
// Write php script to record and store values in a SQLite db
// Allow for changing reporting delay remotely
//


int five = 5; // used to test the Particle.variable functionality.
// Was getting weird values with old code, resolved by reimporting libraries.

int photocellPin = A5;

int dht_h = 1;
int dht_c = 2;
double dht_f = 0.3;

double bmp_c = 0.4;
double bmp_p = 0.5;

int lightRaw = 3;
// double oldLightRaw = 4;

int publishDelay = 5; // Publish a reading every X minutes. 1 = 1440, every 2 minutes means 720 events per day. 3 min = 480

double lastTransmitMillis = 123;

String statusData = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";
                //  "dh: 123 dc: 123 bc: 12345 bp: 12345 lr: 1234"
// char * pubStr;

String event_name = "location-monitor";

void setup() {
    Particle.variable("five", five);
    pinMode(photocellPin, INPUT);

    Particle.variable("dht_humidity", dht_h);
    Particle.variable("dht_temp_c", dht_c);
    Particle.variable("dht_temp_f", dht_f);
    Particle.variable("bmp_temp_c", bmp_c);
    Particle.variable("bmp_pressure", bmp_p);
    Particle.variable("lightRaw", lightRaw);
    Particle.variable("status", statusData);


    dht.begin();
    bMP180.begin();
    delay(5000);
    lastTransmitMillis = millis();
}

String getSensorValues() {
    // build string with live values
    // Save string to statusData
    // statusData = "dht_hum: dht_temp_c: bmp_temp_c: bmp_press: light_raw: " + String(lightRaw);
    // statusData = "dh: " + printf("%d") + \
    //            "dt: " + ;
    // sprintf(statusData, "dh: %d dc: %.1f bc: %.1f bp: %.1f lr: %d", \
    //                dht_h, dht_c, bmp_c, bmp_p, lightRaw);
    statusData = String::format("dh: %d dc: %d bc: %.1f bp: %.1f lr: %d", dht_h, dht_c, bmp_c, bmp_p, lightRaw);
    return statusData;
}

void loop() {

    // BMP180 pressure was coming in at about half of what it should
    // My speculation is that it is because of Sparkfun in Denver, which is approximately 1/2 atm being set default in library

    char bmpStatus;
    // double bT, bP;

    bmpStatus = bMP180.startTemperature(); // http://www.circuitbasics.com/set-bmp180-barometric-pressure-sensor-arduino/
    if (bmpStatus != 0) {
        delay(1000);
        bmpStatus = bMP180.getTemperature(bmp_c);
        if (bmpStatus != 0) {
            bmpStatus = bMP180.startPressure(1); // 0, 1, 2, 3 oversampling rate where higher numbers are slower
            if (bmpStatus != 0) {
                delay(1000);
                bmpStatus = bMP180.getPressure(bmp_p, bmp_c);
                if(bmpStatus != 0) {
                    float adjPressure = bMP180.sealevel(bmp_p, ALTITUDE);
                    bmp_p = (double) adjPressure;
                }
            }
        }
        // bMP180.startPressure(1);
    }
    delay(8000);
    lightRaw = (int) analogRead(photocellPin);

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

    dht_h = dht.getHumidity();
    dht_c = dht.getTempCelcius();
    dht_f = dht.getTempFarenheit();

    // bmp_c = bMP180.getTemperature();
    // bmp_p = bMP180.getPressure();
    // bMP180.getTemperature(bmp_c); // takes a reference to a double where temp will be stored
    // bMP180.getPressure(bmp_p, bmp_c); // takes a reference to double p to store pressure and a double where temp exists

    // Check if last value is different from current value
    // Publish. Makes for a cheap interrupt replacement, will only be 2 seconds delayed at most.

    if( (millis() - lastTransmitMillis) > publishDelay * 60 * 1000)
    {
        // Particle.publish("Sensors", getSensorValues());
        // Using new webhook:  Particle.publish("FUBAR-status", data, PRIVATE);
        // Particle.publish("myEvent", "MyData");
        Particle.publish(event_name, getSensorValues(), PRIVATE);
        lastTransmitMillis = millis();
    }

    // oldLightRaw = lightRaw;

    // publish event every 5 minutes, or when state changes, maybe.




    // Compute heat index
    // Must send in temp in Fahrenheit!
	// float hi = dht.getHeatIndex();
	// float dp = dht.getDewPoint();
	// float k = dht.getTempKelvin();

    //dht_hi = dht.getHeatIndex();
    //dht_dp = dht.getDewPoint();
    //dht_k = dht.getTempKelvin();

}
