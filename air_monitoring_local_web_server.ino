//-------------------------------Include the NodeMCU ESP8266 Library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//------------------------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
#include <MQ135.h>

//-----------------------------------Include the DHT Sensor Library 
#include "DHT.h"  
#define DHTTYPE DHT11 

#include "indexpage.h" 

// Define the LEDonBoard pin number, 
//which is used to control the onboard LED of the NodeMCU ESP8266.
#define LEDonBoard 2   

//---------------------------SSID and Password of your WiFi router
// Define the SSID and password for connecting to the WiFi router
const char* ssid = "uumwifi mcm ahma";
const char* password = "0174652300abc" ;
//----------------------------------------


// Create a web server object on port 80
ESP8266WebServer server(80);  

// Define the pin connected to the DHT sensor
const int DHTPin = 2; 
DHT dht(DHTPin, DHTTYPE); 

//----------------------------------------Initialize LCD
// Initialize the LCD with the I2C address, number of columns, and number of rows
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27, 16 characters wide and 2 lines

//----------------------------------------Gas Sensor
// Define the analog pin connected to the MQ135 gas sensor
#define MQ_PIN A0   
// Define the calibrated CO2 0 level
#define co2Zero 70                        
// Initialize the gas sensor object with the analog pin
MQ135 gasSensor(MQ_PIN); 

// Define concentration thresholds
const int GOOD_THRESHOLD = 500;   // ppm
const int MODERATE_THRESHOLD = 700;  // ppm

// Function to classify CO2 concentration
String classifyCO2Concentration(int concentration) {
    // Classify the CO2 concentration into three categories
  if (concentration <= GOOD_THRESHOLD) {
    return "Good";
  } else if (concentration <= MODERATE_THRESHOLD) {
    return "Moderate";
  } else {
    return "Danger";
  }
}


//-------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot() {
  // Load the HTML page stored in the MAIN_page variable and send it to the client
  String s = MAIN_page; 
  server.send(200, "text/html", s); 
}

//----------------------------------------
//-------------------Procedure for reading the temperature value of a DHT11 sensor
void handleDHT11Temperature() {
  float t = dht.readTemperature();  
  String Temperature_Value = String(t);
 
  server.send(200, "text/plane", Temperature_Value); 
  
  if (isnan(t)) { 
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    lcd.clear();
    Serial.print("DHT11 || Temperature : ");
    Serial.print(t);
    Serial.print(" || ");
    
    // Display temperature on LCD
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.print("C");
  }
}

//----------------------------------------
//---------Procedure for reading humidity values from DHT11 sensors
void handleDHT11Humidity() {
  // Read the humidity from the DHT11 sensor
  float h = dht.readHumidity();
  // Convert the humidity value to a string
  String Humidity_Value = String(h);
 
  // Send the humidity value to the client as plain text
  server.send(200, "text/plane", Humidity_Value); 
  
  // Check if the humidity reading is NaN (not a number)
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor !");
  }
  else {
    // Print the humidity value to the serial monitor
    Serial.print("Humidity: ");
    Serial.println(h);
    
    // Display the humidity on the LCD
    lcd.setCursor(0, 1);
    lcd.print("Humi: ");
    lcd.print(h);
    lcd.print("%");
  }
}

//----------------------------------------
//---------------------------Procedure for reading the gas sensor value
void handleGasSensor() {
  int co2now[10];          // Array to store CO2 readings
  int co2raw = 0;          // Raw CO2 value
  int co2ppm = 0;          // Calculated CO2 concentration in ppm
  int zzz = 0;             // Variable for averaging

  // Sample CO2 10 times over 2 seconds
  for (int x = 0; x < 10; x++) {                   
    co2now[x] = analogRead(MQ_PIN);
    delay(200);
  }

  // Add samples together
  for (int x = 0; x < 10; x++) {                     
    zzz = zzz + co2now[x];  
  }
  
  co2raw = zzz / 10;       // Average the samples
  co2ppm = co2raw - co2Zero; // Calculate the ppm
  
  // Classify gas concentration
  String gasClassification = classifyCO2Concentration(co2ppm);
  
  // Send gas concentration and gas quality separately
  server.send(200, "text/plain", String(co2ppm) + "," + gasClassification); 

  // Print gas concentration and air quality to the serial monitor
  Serial.print("Gas: ");
  Serial.print(co2ppm);
  Serial.print(" PPM");
  Serial.print(", Air Quality :");
  Serial.println(gasClassification);
  Serial.println();

  // Display gas concentration and air quality on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(co2ppm);
  lcd.print(" PPM");

  lcd.setCursor(0, 1);
  lcd.print("Quality:");
  lcd.print(gasClassification);
  delay(2000);
}

//----------------------------------------
//----------------------------------------Setup
void setup(void){

  // Initialize LCD
  lcd.init();            // Initialize LCD display                    
  lcd.backlight();       // Turn on LCD backlight
  lcd.clear();           // Clear LCD display
  
  // Start serial communication
  Serial.begin(115200);
  delay(500);

  // Initialize DHT sensor
  dht.begin();  
  delay(500);
  
   // Set LED pin mode and turn it on
  pinMode(LEDonBoard, OUTPUT); 
  digitalWrite(LEDonBoard, HIGH); 

  // Connect to WiFi network
  WiFi.begin(ssid, password); 
  Serial.println("Connecting to WiFi...");
  lcd.print("Connecting...");
  
  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LEDonBoard, LOW);
    delay(250);
    digitalWrite(LEDonBoard, HIGH);
    delay(250);
  }
  
  Serial.println("");
  Serial.println("Successfully connected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Display connection status on LCD
  lcd.clear();
  lcd.print("Connected!");
  lcd.setCursor(0, 1);
  lcd.print("IP: ");
  lcd.print(WiFi.localIP());
  
  // Set up HTTP server routes
  server.on("/", handleRoot); 
  server.on("/readTemperature", handleDHT11Temperature);  
  server.on("/readHumidity", handleDHT11Humidity);  
  server.on("/readGasSensor", handleGasSensor);  

  // Start HTTP server
  server.begin(); 
  Serial.println("HTTP server started");
  delay(5000); // Delay to show IP address longer


  // Initialize LCD (again to ensure backlight remains on)
  lcd.init();                       
  lcd.backlight();                  
  lcd.clear();                      
}
//----------------------------------------
//----------------------------------------Loop
void loop(){
  // Handle client requests
  server.handleClient();  
}
//----------------------------------------
