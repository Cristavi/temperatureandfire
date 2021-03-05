
//importing libraries

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
char auth[] = "PLyJc-4kPlzMZMfvvFQ2_K4dwHe_tYv9"; //Enter the Auth code which was send by Blynk

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "TP-LINK_6AAA";  //Enter your WIFI Name
char pass[] = "18283848";  //Enter your WIFI Password

#define DHTPIN 2          // Digital pin 4

#define DHTTYPE DHT11     // DHT 11



DHT dht(DHTPIN, DHTTYPE);

//instance creation
SimpleTimer timer;

int flag = 0;

//function of fire notification
void notifyOnFire(){

  //the button is pressed after detecting fire
  int isBtnPressed = digitalRead(D1);
  if(isBtnPressed == 1 && flag == 0){
    Blynk.notify("Fire detected");

    Serial.print("Fire detected");
    
    flag = 1;
    }

    
    else if(isBtnPressed == 0){
      flag = 0;
      }
  }

//function of getting data from DHT11
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  //if humidity and temp is null
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //sending data to Blynk app
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature

  Serial.println(t);
  Serial.println(h);
}

//main
void setup()
{
  Serial.begin(115200); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);
  pinMode(D1, INPUT_PULLUP);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  delay(10);
  timer.setInterval(1000L, notifyOnFire);
}

//arduino re-iterates the functions in a continous loop
void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
