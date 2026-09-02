#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>


const char* SSID_WIFI = "Nicolalau";
const char* SENHA_WIFI = "itxe1914" ;


const char* URL_API = "http://192.168.18.16:8080/api/temperatura";


const int PINO_DS18B20 = 4; // Pino D4
OneWire oneWire(PINO_DS18B20);
DallasTemperature sensors(&oneWire);


unsigned long ultimoEnvio = 0;
const unsigned long INTERVALO_ENVIO = 10000; 

void setup() {
  Serial.begin(115200);
  delay(1000);

 
  pinMode(PINO_DS18B20, INPUT_PULLUP);
  delay(100);


  sensors.begin();


  Serial.print("Buscando sensores DS18B20... Dispositivos encontrados: ");
  Serial.println(sensors.getDeviceCount());

  Serial.println();
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(SSID_WIFI);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_WIFI, SENHA_WIFI);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Conectado!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address do ESP32: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  if (millis() - ultimoEnvio >= INTERVALO_ENVIO) {
    ultimoEnvio = millis();

    if (WiFi.status() == WL_CONNECTED) {
   
      sensors.requestTemperatures();
      float temperatura = sensors.getTempCByIndex(0);

  
      if (temperatura == DEVICE_DISCONNECTED_C) {
        Serial.println("Erro: Sensor DS18B20 nao encontrado ou desconectado!");
        return;
      }

      String macAddress = WiFi.macAddress();

      Serial.print("Leitura: ");
      Serial.print(temperatura);
      Serial.println(" °C");

   
      String jsonPayload = "{\"macAddress\":\"" + macAddress + "\",\"temperatura\":" + String(temperatura, 2) + "}";

      
      HTTPClient http;
      http.begin(URL_API);
      http.addHeader("Content-Type", "application/json");

      int httpCode = http.POST(jsonPayload);

   
      if (httpCode > 0) {
        Serial.print("POST enviado com sucesso! Resposta HTTP: ");
        Serial.println(httpCode);
      } else {
        Serial.print("Falha na requisicao POST. Erro: ");
        Serial.println(http.errorToString(httpCode).c_str());
      }

      http.end(); 
    } else {
      Serial.println("Wi-Fi desconectado! Tentando reconectar...");
      WiFi.reconnect();
    }
  }
}