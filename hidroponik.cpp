#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Konfigurasi WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Konfigurasi MQTT
const char* mqtt_server = "broker.hivemq.com";
const char* temperature_topic = "sensor/temperature";
const char* humidity_topic = "sensor/humidity";
const char* lamp_status_topic = "device/lamp_status";
const char* pump_status_topic = "device/pump_status";
const char* pump_control_topic = "device/pump_control"; // Topik untuk mengontrol pompa

WiFiClient espClient;
PubSubClient client(espClient);

// Konfigurasi pin DHT22 dan tipe sensor
#define DHTPIN 18   
#define DHTTYPE DHT22   

// Konfigurasi pin lampu, buzzer, dan relay pompa
#define LAMP_RED_PIN 12
#define LAMP_YELLOW_PIN 13
#define LAMP_GREEN_PIN 5
#define BUZZER_PIN 19
#define RELAY_PUMP_PIN 21

DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {
  delay(10);
  Serial.print("Menghubungkan ke WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("terhubung");
      client.subscribe(pump_control_topic);
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan diterima di topic: ");
  Serial.print(topic);
  Serial.print(". Pesan: ");
  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (String(topic) == pump_control_topic) {
    if (message == "ON") {
      digitalWrite(RELAY_PUMP_PIN, HIGH);
      client.publish(pump_status_topic, "ON");
      Serial.println("Pompa Dinyalakan (Relay: ON)");
    } else if (message == "OFF") {
      digitalWrite(RELAY_PUMP_PIN, LOW);
      client.publish(pump_status_topic, "OFF");
      Serial.println("Pompa Dimatikan (Relay: OFF)");
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println("Mengontrol Lampu, Buzzer, dan Relay Pompa Berdasarkan DHT22");

  dht.begin();

  pinMode(LAMP_RED_PIN, OUTPUT);
  pinMode(LAMP_YELLOW_PIN, OUTPUT);
  pinMode(LAMP_GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PUMP_PIN, OUTPUT);

  digitalWrite(LAMP_RED_PIN, LOW);
  digitalWrite(LAMP_YELLOW_PIN, LOW);
  digitalWrite(LAMP_GREEN_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PUMP_PIN, LOW);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    return;
  }

  Serial.print("Kelembaban: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.println(" *C");

  client.publish(temperature_topic, String(temperature).c_str());
  client.publish(humidity_topic, String(humidity).c_str());

  digitalWrite(LAMP_RED_PIN, LOW);
  digitalWrite(LAMP_YELLOW_PIN, LOW);
  digitalWrite(LAMP_GREEN_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  if (temperature > 35) {
    digitalWrite(LAMP_RED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RELAY_PUMP_PIN, HIGH);
    client.publish(lamp_status_topic, "Red");
    client.publish(pump_status_topic, "ON");
    Serial.println("Relay Pompa: ON (Suhu > 35°C)");
  } else if (temperature >= 30 && temperature <= 35) {
    digitalWrite(LAMP_YELLOW_PIN, HIGH);
    client.publish(lamp_status_topic, "Yellow");
    client.publish(pump_status_topic, "OFF");
    Serial.println("Relay Pompa: OFF (Suhu 30-35°C)");
  } else if (temperature < 30) {
    digitalWrite(LAMP_GREEN_PIN, HIGH);
    client.publish(lamp_status_topic, "Green");
    client.publish(pump_status_topic, "OFF");
    Serial.println("Relay Pompa: OFF (Suhu < 30°C)");
  }
}
