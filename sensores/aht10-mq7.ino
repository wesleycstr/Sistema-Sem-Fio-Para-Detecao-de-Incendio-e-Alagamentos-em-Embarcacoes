#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Adafruit_AHTX0.h>

const char* ssid = "Net_Wifi_2G";
const char* password = "VU2E,dSAtJ";

const int ledPin = LED_BUILTIN;
const int externalLedPin = D3;  // Pino onde o LED externo está conectado
const int mq7Pin = A0;  // Pino analógico onde o MQ-7 está conectado

Adafruit_AHTX0 aht;
Adafruit_Sensor *aht_humidity, *aht_temp;

void enviar_mensagem(String mensagem) {
  String url = "http://192.168.0.41/recebe_temperatura.php";
  String urlCompleta = url + mensagem;
  Serial.println("url gerada:");
  Serial.println(urlCompleta);
  
  HTTPClient http;
  WiFiClient client;

  http.begin(client, urlCompleta);
  int httpCode = http.POST("");

  if (httpCode > 0) {
    Serial.printf("[HTTP] POST request status: %d\n", httpCode);
    if (httpCode == 200) {
      digitalWrite(ledPin, LOW);
      digitalWrite(externalLedPin, HIGH);  // Acende o LED externo
      delay(1000);
      for (int i = 0; i < 2; i++) {
        digitalWrite(ledPin, HIGH);
        digitalWrite(externalLedPin, LOW);  // Apaga o LED externo
        delay(100);
        digitalWrite(ledPin, LOW);
        digitalWrite(externalLedPin, HIGH);  // Acende o LED externo
        delay(100);
      }
      digitalWrite(ledPin, HIGH);
      digitalWrite(externalLedPin, LOW);  // Apaga o LED externo
    }
  } else {
    Serial.printf("[HTTP] POST request failed, error: %s\n", http.errorToString(httpCode).c_str());
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPin, LOW);
      digitalWrite(externalLedPin, HIGH);  // Acende o LED externo
      delay(500);
      digitalWrite(ledPin, HIGH);
      digitalWrite(externalLedPin, LOW);  // Apaga o LED externo
      delay(500);
    }
  }
}

void setup(void) {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(externalLedPin, OUTPUT);  // Configura o pino do LED externo como saída

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    digitalWrite(ledPin, LOW);
    digitalWrite(externalLedPin, HIGH);  // Acende o LED externo
    delay(500);
    digitalWrite(ledPin, HIGH);
    digitalWrite(externalLedPin, LOW);  // Apaga o LED externo
    delay(500);
  }
  Serial.println("Connected to WiFi");
  enviar_mensagem("?status=1");

  Serial.println("Iniciando sensor AHT10");
  enviar_mensagem("?status=2");

  while (!aht.begin()) {
    Serial.println("Falha ao iniciar sensor AHT10");
    enviar_mensagem("?status=3");
    delay(5000);
  }

  Serial.println("AHT10/AHT20 iniciado!");
  enviar_mensagem("?status=4");
  aht_temp = aht.getTemperatureSensor();
  aht_temp->printSensorDetails();
  aht_humidity = aht.getHumiditySensor();
  aht_humidity->printSensorDetails();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado. Tentando reconectar...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Tentando reconectar ao WiFi...");
    }
    Serial.println("Reconectado ao WiFi");
    enviar_mensagem("?status=1");
  }

  sensors_event_t humidity;
  sensors_event_t temp;
  aht_humidity->getEvent(&humidity);
  aht_temp->getEvent(&temp);

  Serial.print("\t\tHumidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" % rH");
  Serial.print("\t\tTemperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degrees C");

  // Leitura do valor do sensor MQ-7
  int mq7Value = analogRead(mq7Pin);
  float voltage = mq7Value * (3.3 / 1024.0);
  float gasConcentration = voltage * 1000;

  Serial.print("Valor do MQ-7: ");
  Serial.print(mq7Value);
  Serial.print("\tConcentração estimada de CO (ppm): ");
  Serial.println(gasConcentration);

  String sensor = "?sensor=portable";
  String Temperatura = "&temperatura=";
  String valor_temperatura = Temperatura + temp.temperature;

  String Umidade = "&umidade=";
  String valor_umidade = Umidade + humidity.relative_humidity;

  String Gas = "&gas=";
  String valor_gas = Gas + gasConcentration;

  String mensagem = sensor + valor_temperatura + valor_umidade + valor_gas;
  enviar_mensagem(mensagem);

  delay(5000);
}
