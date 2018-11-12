// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_VEML6070.h>

// This #include statement was automatically added by the Particle IDE.
#include <AssetTracker.h>

// assign SETUP button's pin
int button = BTN;
//Calling constructor from AssetTracker.cpp
AssetTracker locationTracker = AssetTracker();
//Calling constructor from Adafruit_VEML6070.cpp
Adafruit_VEML6070 ultraVData = Adafruit_VEML6070();

// Setup 
void setup() {
    // Setup serial port
    Serial.begin(9600);
    // Initialize the gps and turn it on  
    locationTracker.begin();
    locationTracker.gpsOn();
    // Initialize the UV Sensor and turn it on
    ultraVData.begin(VEML6070_1_T);
    // Setup pin mode for button
    pinMode(button, INPUT);
    // Setup webhook subscribe
    Particle.subscribe("hook-response/FinalProjTest", myHandler, MY_DEVICES);
}
// main loop
void loop() {
    // You'll need to run this every loop to capture the GPS output
        locationTracker.updateGPS();
    // read button and if it is pressed
    if (digitalRead(button) == 0) { // pulldown resistor, 0: Pressed

        Serial.println(locationTracker.readLonDeg());
        Serial.println(locationTracker.readLatDeg());
        Serial.println(locationTracker.getSpeed());
        Serial.println(ultraVData.readUV());

        String data = "";
        
        if (locationTracker.gpsFix()) {
                data = String::format("{ \"longitude\": \"%f\", \"latitude\": \"%f\", \"speed\": \"%f\", \"uv\":\"%u\"}", 
                       locationTracker.readLonDeg(), locationTracker.readLatDeg(), locationTracker.getSpeed(), ultraVData.readUV());
        }
        else {
                data = String::format("{ \"longitude\": \"%f\", \"latitude\": \"%f\", \"speed\": \"%f\", \"uv\":\"%u\"}", 
                                         -110.987420, 32.248820, 2.33333, 5);
        }
        
        // Publish to webhook
        Particle.publish("FinalProjTest", data, PRIVATE);
    }
        //delay .5 second to block continuous input
        delay(500);
}
// When obtain response from the publish
void myHandler(const char *event, const char *data) {
  // Formatting output
  String output = String::format("Response from Post:\n  %s\n", data);
  // Log to serial console
  Serial.println(output);
}
