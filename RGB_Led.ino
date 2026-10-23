
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "-";
const char* password = "-";

#define RGB_PIN 48 

WebServer server(80);
bool durum = false;
unsigned long sonYazma = 0;

void anaSayfa() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Maker Center LED</title></head>";
  html += "<body style='text-align:center; font-family:sans-serif'>";
  html += "<h1>Maker Center LED</h1>";
  html += "<h2>LED: ";
  html += durum ? "AÇIK" : "KAPALI";
  html += "</h2>";
  html += "<p><a href='/ac'><button style='font-size:24px; padding:10px 30px'>AÇ</button></a></p>";
  html += "<p><a href='/kapat'><button style='font-size:24px; padding:10px 30px'>KAPAT</button></a></p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}


void anaSayfayaDon() {
  server.sendHeader("Location", "/");
  server.send(303);
}

void ac() {
  durum = true;
  neopixelWrite(RGB_PIN, 0, 50, 0); // yeşil (R, G, B)
  anaSayfayaDon();
}

void kapat() {
  durum = false;
  neopixelWrite(RGB_PIN, 0, 0, 0); 
  anaSayfayaDon();
}

void setup() {
  Serial.begin(115200);
  delay(1500);


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
  server.on("/ac", ac);
  server.on("/kapat", kapat);

  server.begin();

  
}

void loop() {
  server.handleClient();

  // IP adresini 5 saniyede bir yazdır
  if (millis() - sonYazma > 5000) {
    sonYazma = millis();
    Serial.print("IP: http://");
    Serial.println(WiFi.localIP());
  }
}
