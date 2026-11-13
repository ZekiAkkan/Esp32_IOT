#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "-";
const char* password = "-";

#define RGB_PIN  48   // Bazı kartlarda 38 veya 21 olabilir
#define TRIG_PIN 6    // HC-SR04 Trig
#define ECHO_PIN 7    // HC-SR04 Echo 

WebServer server(80);

float mesafe = 0;            // cm
unsigned long sonYazma = 0;

float mesafeOlc() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);     // 10 µs ses dalgası gönder
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long sure = pulseIn(ECHO_PIN, HIGH, 30000);  // yankı süresi (µs)
  if (sure == 0) return 400;                   // yankı yok: çok uzak
  return sure * 0.0343 / 2;                    // ses hızı 0.0343 cm/µs
}

void anaSayfa() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='1'>";   // 1 sn'de bir yenile
  html += "<title>Maker Center Park</title></head>";
  html += "<body style='text-align:center; font-family:sans-serif'>";
  html += "<h1>Maker Center Park Sensörü</h1>";
  html += "<h2>Mesafe: " + String(mesafe, 0) + " cm</h2>";

  if (mesafe < 10)      html += "<h2 style='color:red'>DUR!</h2>";
  else if (mesafe < 30) html += "<h2 style='color:orange'>Yaklaşıyorsun</h2>";
  else                  html += "<h2 style='color:green'>Güvenli</h2>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  neopixelWrite(RGB_PIN, 0, 0, 0);   // Core 3.x'te: rgbLedWrite(...)

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("WiFi'ye baglaniliyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Baglandi! IP: http://");
  Serial.println(WiFi.localIP());

  server.on("/", anaSayfa);
  server.begin();
}

void loop() {
  server.handleClient();
  mesafe = mesafeOlc();

  // LED rengi mesafeye göre değişir
  if (mesafe < 10) {
    neopixelWrite(RGB_PIN, 50, 0, 0);    // kırmızı
  } else if (mesafe < 30) {
    neopixelWrite(RGB_PIN, 50, 30, 0);   // sarı
  } else {
    neopixelWrite(RGB_PIN, 0, 50, 0);    // yeşil
  }

  // IP ve mesafeyi 2 saniyede bir seri porta yazdır
  if (millis() - sonYazma > 2000) {
    sonYazma = millis();
    Serial.print("IP: http://");
    Serial.print(WiFi.localIP());
    Serial.print(" | Mesafe: ");
    Serial.print(mesafe);
    Serial.println(" cm");
  }

  delay(60);  // iki ölçüm arasında kısa bekleme
}
