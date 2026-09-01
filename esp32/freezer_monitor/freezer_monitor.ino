#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// --- CONFIGURAÇÕES DE REDE ---
const char* SSID_WIFI = "Nicolalau";
const char* SENHA_WIFI = "itxe1914";

// IP LOCAL da sua máquina onde o Spring Boot está rodando
const char* URL_API = "http://192.168.18.16:8080/api/temperatura";

// --- CONFIGURAÇÃO DO SENSOR DS18B20 ---
const int PINO_DS18B20 = 4; // Pino D4 (GPIO 4)
OneWire oneWire(PINO_DS18B20);
DallasTemperature sensors(&oneWire);

// Intervalo entre envios (10 segundos)
unsigned long ultimoEnvio = 0;
const unsigned long INTERVALO_ENVIO = 10000; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 1. Ativa o resistor de Pull-up interno do ESP32 no GPIO 4
  pinMode(PINO_DS18B20, INPUT_PULLUP);
  delay(100);

  // 2. Inicializa o sensor de temperatura
  sensors.begin();

  // Diagnóstico: exibe se encontrou o sensor fisicamente
  Serial.print("Buscando sensores DS18B20... Dispositivos encontrados: ");
  Serial.println(sensors.getDeviceCount());

  // 3. Conecta ao Wi-Fi
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
  // Executa o envio a cada 10 segundos
  if (millis() - ultimoEnvio >= INTERVALO_ENVIO) {
    ultimoEnvio = millis();

    if (WiFi.status() == WL_CONNECTED) {
      // 1. Solicita a leitura da temperatura
      sensors.requestTemperatures();
      float temperatura = sensors.getTempCByIndex(0);

      // Validação de erro do sensor (-127.0 °C)
      if (temperatura == DEVICE_DISCONNECTED_C) {
        Serial.println("Erro: Sensor DS18B20 nao encontrado ou desconectado!");
        return;
      }

      String macAddress = WiFi.macAddress();

      Serial.print("Leitura: ");
      Serial.print(temperatura);
      Serial.println(" °C");

      // 2. Monta o JSON para a requisição
      String jsonPayload = "{\"macAddress\":\"" + macAddress + "\",\"temperatura\":" + String(temperatura, 2) + "}";

      // 3. Dispara a requisição HTTP POST para a API Spring Boot
      HTTPClient http;
      http.begin(URL_API);
      http.addHeader("Content-Type", "application/json");

      int httpCode = http.POST(jsonPayload);

      // 4. Analisa a resposta da API
      if (httpCode > 0) {
        Serial.print("POST enviado com sucesso! Resposta HTTP: ");
        Serial.println(httpCode);
      } else {
        Serial.print("Falha na requisicao POST. Erro: ");
        Serial.println(http.errorToString(httpCode).c_str());
      }

      http.end(); // Libera conexão
    } else {
      Serial.println("Wi-Fi desconectado! Tentando reconectar...");
      WiFi.reconnect();
    }
  }
}