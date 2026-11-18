
#include <WiFi.h>
#include <DHT.h>

// =================================
// 1. KONFIGURASI WIFI
// =================================
const char* ssid = "Nufa";
const char* password = "08050631";

// =================================
// 2. KONFIGURASI DHT11
// =================================
#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// =================================
// 3. KONFIGURASI PIR
// =================================
#define PIR_PIN 14

// =================================
// 4. PENGATURAN WAKTU
// =================================
unsigned long waktuSensor = 0;
unsigned long waktuCobaWiFi = 0;

const unsigned long intervalSensor = 2500;
const unsigned long intervalWiFi = 10000;

// =================================
// 5. SETUP
// =================================
void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  dht.begin();

  Serial.println();
  Serial.println("================================");
  Serial.println(" SISTEM MONITORING RUANGAN IoT");
  Serial.println("================================");

  // Menghubungkan ke WiFi
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  int percobaan = 0;

  while (WiFi.status() != WL_CONNECTED && percobaan < 30) {
    delay(500);
    Serial.print(".");
    percobaan++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi berhasil terhubung!");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi belum terhubung.");
    Serial.println("ESP32 akan mencoba kembali.");
  }

  Serial.println("Sistem monitoring siap!");
  delay(2000);
}

// =================================
// 6. LOOP
// =================================
void loop() {

  // Memeriksa dan menghubungkan ulang WiFi
  if (WiFi.status() != WL_CONNECTED) {

    if (millis() - waktuCobaWiFi >= intervalWiFi) {
      waktuCobaWiFi = millis();

      Serial.println("Mencoba menghubungkan WiFi kembali...");
      WiFi.reconnect();
    }

  }

  // Membaca sensor setiap 2,5 detik
  if (millis() - waktuSensor >= intervalSensor) {

    waktuSensor = millis();

    float suhu = dht.readTemperature();
    float kelembapan = dht.readHumidity();

    int gerakan = digitalRead(PIR_PIN);

    Serial.println();
    Serial.println("========== DATA SENSOR ==========");

    // Menampilkan data DHT11
    if (isnan(suhu) || isnan(kelembapan)) {
      Serial.println("DHT11: Gagal membaca sensor!");
    } else {
      Serial.print("Suhu: ");
      Serial.print(suhu, 1);
      Serial.println(" C");

      Serial.print("Kelembapan: ");
      Serial.print(kelembapan, 1);
      Serial.println(" %");
    }

    // Menampilkan data PIR
    if (gerakan == HIGH) {
      Serial.println("PIR: Gerakan Terdeteksi!");
    } else {
      Serial.println("PIR: Tidak Ada Gerakan");
    }

    // Menampilkan status WiFi
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi: Terhubung");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("WiFi: Terputus");
    }

    Serial.println("=================================");
  }
}
