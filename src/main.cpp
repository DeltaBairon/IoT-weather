#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WebServer.h>
#include <ThingSpeak.h>

WiFiClient client; // Cliente para ThingSpeak
unsigned long myChannelNumber =3125092;
const char * myWriteAPIKey = "QFWCU0T7VP0WQ921";


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Cambia estos datos por tu red WiFi
const char* ssid = "SAENZ";
const char* password = "4107480A";

WebServer server(80);
String city = "Esperando...";
String weather = "--";
String temp = "--";

void handleUpdate() {
  if (server.hasArg("city") && server.hasArg("weather") && server.hasArg("temp")) {
    city = server.arg("city");
    weather = server.arg("weather");
    temp = server.arg("temp");

    Serial.printf("Ciudad: %s | Clima: %s | Temp: %s°C\n",
                  city.c_str(), weather.c_str(), temp.c_str());

    // --- Mostrar en pantalla OLED ---
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 5);
    display.println(city);
    display.setCursor(0, 20);
    display.println(weather);
    display.setTextSize(2);
    display.setCursor(0, 35);
    display.print(temp);
    display.println("°C");
    display.display();

    // --- ✅ Respuesta correcta al navegador (con CORS) ---
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", "OK");

    // --- 🌐 Enviar datos a ThingSpeak ---
    WiFiClient client;
    const char* host = "api.thingspeak.com";
    String apiKey = "QFWCU0T7VP0WQ921";  // 🔑 Reemplaza con tu clave de escritura

    // 🔒 Codificar datos para URL (evita errores con espacios o acentos)
    String safeCity = city;
    safeCity.replace(" ", "%20");
    String safeWeather = weather;
    safeWeather.replace(" ", "%20");
    String safeTemp = temp;
    safeTemp.replace(" ", "%20");

    // Construir la URL con los campos del canal
    String url = "/update?api_key=" + apiKey +
                 "&field1=" + safeCity +
                 "&field2=" + safeWeather +
                 "&field3=" + safeTemp;

    Serial.println("[ThingSpeak] Enviando datos a: " + url);

    if (client.connect(host, 80)) {
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
      Serial.println("[ThingSpeak] Datos enviados con éxito ✅");
    } else {
      Serial.println("[ThingSpeak] ❌ Error al conectar con ThingSpeak");
    }

    delay(1500);  // ⏱️ Espera necesaria entre solicitudes (ThingSpeak la requiere)
    client.stop();

  } else {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(400, "text/plain", "Faltan parámetros");
  }
}




void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Conectado a WiFi!");
  Serial.print("🌐 IP del ESP32: ");
  Serial.println(WiFi.localIP());

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ Error inicializando OLED");
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Esperando datos...");
  display.display();

  server.on("/update", handleUpdate);
  server.begin();

  Serial.println("🌍 Servidor web iniciado");
}

void loop() {
  server.handleClient();
}
