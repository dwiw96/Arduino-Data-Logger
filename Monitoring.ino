#include <max6675.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750FVI.h>
#include <SPI.h>
#include <SD.h>


//-------------THERMOCOUPLE-------------//
//pin for first thermocouple
int pinSO1 = 26;
int pinCS1 = 24;
int pinSCK1 = 22;

//pin for second thermocouple
int pinSO2 = 27;
int pinCS2 = 25;
int pinSCK2 = 37; //23;

//pin for third thermocouple
int pinSO3 = 34;
int pinCS3 = 32;
int pinSCK3 = 30;

//pin for fourth thermocouple
int pinSO4 = 35;
int pinCS4 = 33;
int pinSCK4 = 31;


MAX6675 thermocouple1(pinSCK1, pinCS1, pinSO1);
MAX6675 thermocouple2(pinSCK2, pinCS2, pinSO2);
MAX6675 thermocouple3(pinSCK3, pinCS3, pinSO3);
MAX6675 thermocouple4(pinSCK4, pinCS4, pinSO4);

//------------------DHT-----------------//
#define DHT1PIN 2
#define DHT2PIN 3
#define DHT3PIN 4
#define DHT4PIN 5
#define DHT5PIN 7

#define DHT1TYPE DHT11
#define DHT2TYPE DHT22

DHT dht1(DHT1PIN, DHT2TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);
DHT dht3(DHT3PIN, DHT2TYPE);
DHT dht4(DHT4PIN, DHT2TYPE);
DHT dht5(DHT5PIN, DHT1TYPE);

//-----------------BH1750---------------//
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
File DataSensor;

//-----------------LOGGER---------------//
const int chipSelect = 53;

void setup() {
  Serial.begin(9600);  

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  /*if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }*/
  Serial.println("card initialized.");
  
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
  //pinMode(chipSelect, OUTPUT);
  
  Serial1.begin(115200);
  LightSensor.begin();
  
}

void loop() {
  float C1 = thermocouple1.readCelsius();
  float C2 = thermocouple2.readCelsius();
  float C3 = thermocouple3.readCelsius();
  float C4 = thermocouple4.readCelsius();
  
  Serial.print("C1 = ");
  Serial.println(C1);
  Serial.print("C2 = ");
  Serial.println(C2);
  Serial.print("C3 = ");
  Serial.println(C3);
  Serial.print("C4 = ");
  Serial.println(C4);
  delay(500);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float t1 = dht1.readTemperature();
  float t2 = dht2.readTemperature();
  float t3 = dht3.readTemperature();
  float t4 = dht4.readTemperature();
  float t5 = dht5.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t1)) {
    Serial.println("Failed to read from DHT #1");
  } else {
    Serial.print("Temperature 1: "); 
    Serial.print(t1);
    Serial.println(" *C");
    delay(500);
  }
  if (isnan(t2)) {
    Serial.println("Failed to read from DHT #2");
  } else {
    Serial.print("Temperature 2: "); 
    Serial.print(t2);
    Serial.println(" *C");
    delay(500);
  }
  if (isnan(t3)) {
    Serial.println("Failed to read from DHT #3");
  } else {
    Serial.print("Temperature 3: "); 
    Serial.print(t3);
    Serial.println(" *C");
    delay(500);
  }
  if (isnan(t4)) {
    Serial.println("Failed to read from DHT #4");
  } else {
    Serial.print("Temperature 4: "); 
    Serial.print(t4);
    Serial.println(" *C");
    delay(500);
  }
  if (isnan(t5)) {
    Serial.println("Failed to read from DHT #5");
  } else {
    Serial.print("Temperature 5: "); 
    Serial.print(t5);
    Serial.println(" *C");
    delay(500);
  }
  Serial.println();

//-----------------BH1750---------------//
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Light: ");
  Serial.println(lux);
  Serial.print(" lux");
  delay(500);

//-----------------LOGGER---------------//
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(C1);
    dataFile.print(",");
    dataFile.print(C2);
    dataFile.print(",");
    dataFile.print(C3);
    dataFile.print(",");
    dataFile.print(C4);
    dataFile.print(",");
    dataFile.print(t1);
    dataFile.print(",");
    dataFile.print(t2);
    dataFile.print(",");
    dataFile.print(t3);
    dataFile.print(",");
    dataFile.print(t4);
    dataFile.print(",");
    dataFile.print(t5);
    dataFile.print(",");
    dataFile.print(lux);
    dataFile.println();
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
