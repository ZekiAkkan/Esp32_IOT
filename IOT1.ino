#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "-";
const char* password = "-";

WebServer server(80);

bool durum = false;
unsigned long sonYazma = 0;

void anaSayfa() {
  String html = "<html><body style='text-align:center'>";
  html += "<h1>Maker Center LED</h1>";
  html += "<h2>LED: ";
  html += durum ? "ACIK" : "KAPALI";
  html += "</h2>";
  html += "<a href='/ac'><button>AC</button></a>";
  html += "<a href='/kapat'><button>KAPAT</button></a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void ac() {
  digitalWrite(4, HIGH);
  durum = true;
  anaSayfa();
}

void kapat() {
  digitalWrite(4, LOW);
  durum = false;
  anaSayfa();
}

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
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