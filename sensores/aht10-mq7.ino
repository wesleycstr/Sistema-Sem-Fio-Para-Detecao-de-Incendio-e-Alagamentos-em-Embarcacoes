#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Adafruit_AHTX0.h>

const char* ssid = "Net_Wifi_2G";
const char* password = "VU2E,dSAtJ";

const int ledPin = LED_BUILTIN;
const int externalLedPin = D3;
const int mq7Pin = A0;

Adafruit_AHTX0 aht;
Adafruit_Sensor *aht_humidity = nullptr;
Adafruit_Sensor *aht_temp = nullptr;

bool aht_ok = false;

// =========================
// ENVIO HTTP
// =========================
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
      digitalWrite(externalLedPin, HIGH);
      delay(1000);

      for (int i = 0; i < 2; i++) {
        digitalWrite(ledPin, HIGH);
        digitalWrite(externalLedPin, LOW);
        delay(100);
        digitalWrite(ledPin, LOW);
        digitalWrite(externalLedPin, HIGH);
        delay(100);
      }

      digitalWrite(ledPin, HIGH);
      digitalWrite(externalLedPin, LOW);
    }

  } else {
    Serial.printf("[HTTP] POST request failed, error: %s\n",
                  http.errorToString(httpCode).c_str());

    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPin, LOW);
      digitalWrite(externalLedPin, HIGH);
      delay(500);
      digitalWrite(ledPin, HIGH);
      digitalWrite(externalLedPin, LOW);
      delay(500);
    }
  }

  http.end();
}

// =========================
// INICIALIZAÇÃO DO AHT
// =========================
void iniciarAHT() {
  int tentativas = 0;
  const int max_tentativas = 5;

  while (!aht_ok && tentativas < max_tentativas) {
    Serial.println("Tentando iniciar sensor AHT10...");

    if (aht.begin()) {
      aht_ok = true;
    } else {
      Serial.println("Falha ao iniciar sensor AHT10");
      enviar_mensagem("?status=3");
      tentativas++;
      delay(2000);
    }
  }

  if (aht_ok) {
    Serial.println("AHT10/AHT20 iniciado!");
    enviar_mensagem("?status=4");

    aht_temp = aht.getTemperatureSensor();
    aht_humidity = aht.getHumiditySensor();
  } else {
    Serial.println("Sensor AHT não encontrado. Continuando sem ele...");
    enviar_mensagem("?status=5");
  }
}

// =========================
// SETUP
// =========================
void setup(void) {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(externalLedPin, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");

    digitalWrite(ledPin, LOW);
    digitalWrite(externalLedPin, HIGH);
    delay(500);
    digitalWrite(ledPin, HIGH);
    digitalWrite(externalLedPin, LOW);
    delay(500);
  }

  Serial.println("Connected to WiFi");
  enviar_mensagem("?status=1");

  Serial.println("Iniciando sensor AHT10");
  enviar_mensagem("?status=2");

  iniciarAHT();
}

// =========================
// LOOP
// =========================
void loop() {

  // -------------------------
  // RECONEXÃO WIFI
  // -------------------------
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

  // -------------------------
  // TENTAR RECUPERAR AHT
  // -------------------------
  if (!aht_ok) {
    Serial.println("Tentando recuperar sensor AHT...");
    if (aht.begin()) {
      Serial.println("Sensor AHT recuperado!");
      enviar_mensagem("?status=6");

      aht_ok = true;
      aht_temp = aht.getTemperatureSensor();
      aht_humidity = aht.getHumiditySensor();
    }
  }

  float temperatura = 0;
  float umidade = 0;

  // -------------------------
  // LEITURA DO AHT (SE EXISTIR)
  // -------------------------
  if (aht_ok && aht_temp && aht_humidity) {
    sensors_event_t humidity;
    sensors_event_t temp;

    aht_humidity->getEvent(&humidity);
    aht_temp->getEvent(&temp);

    temperatura = temp.temperature;
    umidade = humidity.relative_humidity;

    Serial.print("Humidity: ");
    Serial.println(umidade);

    Serial.print("Temperature: ");
    Serial.println(temperatura);
  } else {
    Serial.println("Sensor AHT indisponível");
  }

  // -------------------------
  // LEITURA MQ-7
  // -------------------------
  int mq7Value = analogRead(mq7Pin);
  float voltage = mq7Value * (3.3 / 1024.0);
  float gasConcentration = voltage * 1000;

  Serial.print("Valor do MQ-7: ");
  Serial.print(mq7Value);
  Serial.print("\tCO (ppm): ");
  Serial.println(gasConcentration);

  // -------------------------
  // MONTAGEM DA MENSAGEM
  // -------------------------
  String mensagem = "?sensor=caixinha";
  mensagem += "&temperatura=" + String(temperatura);
  mensagem += "&umidade=" + String(umidade);
  mensagem += "&gas=" + String(gasConcentration);

  enviar_mensagem(mensagem);

  delay(5000);
}
