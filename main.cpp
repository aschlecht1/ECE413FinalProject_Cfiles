// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_VEML6070.h>

// This #include statement was automatically added by the Particle IDE.
#include <AssetTracker.h>

/*******************************************************
 * Button hit logger
 * Embedded Systems Design Lab 10/26/2017            **/

// assign SETUP button's pin
int button = BTN;
//Calling constructor from AssetTracker.cpp
AssetTracker locationTracker = AssetTracker();
//Calling constructor from Adafruit_VEML6070.cpp
Adafruit_VEML6070 ultraVData = Adafruit_VEML6070();

//float longitude = 0;
//float latitude = 0;
//float speed = 0;
//uint16_t uv = 0;

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
    // read button and if it is pressed
    if (digitalRead(button) == 0) { // pulldown resistor, 0: Pressed
        // Store longitude off of GPS
        // = locationTracker.readLon();
        // Store latitude off of GPS
        //latitude = locationTracker.readLat();
        // Store speed off of GPS
        //speed = locationTracker.getSpeed();
        // Store speed off of GPS
        // uv = ultraVData.readUV();


        // You'll need to run this every loop to capture the GPS output
        locationTracker.updateGPS();
        // Construct json string if we have gps fix
        String data = String::format("{\"longitude\": \"%f\", \"latitude\": \"%f\", \"speed\": \"%s\", \"uv\": \"%u\" }",
        locationTracker.readLon(), locationTracker.readLat(), locationTracker.getSpeed(), ultraVData.readUV());
        // Construct json string if we have NO gps fix
        //String nodata = String::format("{\"longitude\": \"longitude\", \"latitude\": \"latitude\", \"speed\": \"speed\", \"uv\": \"uv\" }");
        // Log to serial console
       // Serial.println("button pressed!");
       // if (!locationTracker.gpsFix()) {
            //Particle.publish("FinalProjTest", nodata, PRIVATE);
       // }
       // else {
        // Publish to webhook
        Particle.publish("FinalProjTest", data, PRIVATE);
       // }
    }
    // delay .5 second to block continuous input
    delay(500);
}
// When obtain response from the publish
void myHandler(const char *event, const char *data) {
  // Formatting output
  String output = String::format("Response from Post:\n  %s\n", data);
  // Log to serial console
  Serial.println(output);
}
