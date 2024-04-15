#include <LiquidCrystal.h>
#include <DHT.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

const int SENSOR_PIN = 34;
const int Motor_PIN = 7;
const int threshold_temp_lower = 21;
const int threshold_temp_upper = 29;
const int threshold_moisture_upper = 60;
const int threshold_moisture_lower = 20;
const int threshold_humid_lower = 40;
const int threshold_humid_upper = 60;

#define DHTPIN 35
#define DHTTYPE DHT11

// Firebase connection
#define FIREBASE_HOST "mliot-sis-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "4vcsEDT5ktvD74VB4p4sOCXU3XTgahwOMXX1h1gk"
#define WIFI_SSID "AAU-CBE-Student"
#define WIFI_PASSWORD "CBE@Student!"

FirebaseData firebaseData;

DHT dht(DHTPIN, DHTTYPE);

String path = "MLIoT_SIS";


WiFiServer server(80);

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(Motor_PIN, OUTPUT);

  Serial.begin(9600); // Initialize serial communication
  Serial.println("Irrigation System"); // Output message to the Virtual Terminal
  delay(1000);
  dht.begin(); // Initialize DHT sensor
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
     
    Serial.print(".");
    delay(300);
  }
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  server.begin();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float Sensor_Val = analogRead(SENSOR_PIN);
  float Moisture = map(Sensor_Val, 0, 1023, 0, 100);

  // Update Firebase with moisture value
  Firebase.setFloat(firebaseData, path + "/moisture", Moisture);
  Serial.print("Moisture: ");
  Serial.print(Moisture);
  Serial.println("%");

  float temp_value = dht.readTemperature(); // Read temperature from DHT11 sensor
  float humidity_value = dht.readHumidity();

  // Update Firebase with temperature value
  Firebase.setFloat(firebaseData, path + "/temperature", temp_value);
  Serial.print("Temperature: ");
  Serial.print(temp_value);
  Serial.println("C");

  // Update Firebase with humidity value
  Firebase.setFloat(firebaseData, path + "/humidity", humidity_value);
  Serial.print("Humidity: ");
  Serial.print(humidity_value);
  Serial.println("%");

  delay(1000);

  // Control motor based on thresholds
  if (Moisture < threshold_moisture_lower) {
    Serial.println("Motor: ON");
    digitalWrite(Motor_PIN, HIGH);
  } else if (Moisture > threshold_moisture_upper) {
    Serial.println("Motor: OFF");
    digitalWrite(Motor_PIN, LOW);
  } else if (temp_value > threshold_temp_upper) {
    Serial.println("Motor: ON");
    digitalWrite(Motor_PIN, HIGH);
  } else if (temp_value < threshold_temp_lower) {
    Serial.println("Motor: OFF");
    digitalWrite(Motor_PIN, LOW);
  } else if (humidity_value < threshold_humid_lower) {
    Serial.println("Motor: ON");
    digitalWrite(Motor_PIN, HIGH);
  } else if (humidity_value > threshold_humid_upper) {
    Serial.println("Motor: OFF");
    digitalWrite(Motor_PIN, LOW);
  } else {
    Serial.println("Motor: ON");
    digitalWrite(Motor_PIN, HIGH);
  }

  Serial.flush(); // Flush the serial buffer
}

