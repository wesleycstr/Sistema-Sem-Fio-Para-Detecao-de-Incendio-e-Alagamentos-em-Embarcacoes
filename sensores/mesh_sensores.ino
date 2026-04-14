#include <painlessMesh.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_AHTX0.h>

// ---------- CONFIG MESH ----------
#define   MESH_PREFIX     "RedeMeshSensores"
#define   MESH_PASSWORD   "12345678"
#define   MESH_PORT       5555

Scheduler userScheduler;
painlessMesh mesh;

// Defina se este nó será gateway
#define IS_GATEWAY true   // <<< MUDE para false nos outros nós

// ---------- PINOS ----------
const int ledPin = LED_BUILTIN;
const int externalLedPin = D3;
const int mq7Pin = A0;

// ---------- SENSOR ----------
Adafruit_AHTX0 aht;
Adafruit_Sensor *aht_humidity, *aht_temp;

// ---------- SERVIDOR ----------
String serverUrl = "http://192.168.0.41/recebe_temperatura.php";

// ---------- FUNÇÃO HTTP ----------
void enviarHTTP(String mensagem) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverUrl + mensagem);
  int httpCode = http.POST("");

  Serial.println("Enviando ao servidor...");

  if (httpCode > 0) {
    Serial.printf("HTTP OK: %d\n", httpCode);
  } else {
    Serial.printf("Erro HTTP: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

// ---------- RECEBIMENTO MESH ----------
void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("Mensagem recebida de %u: %s\n", from, msg.c_str());

  // Se for gateway, envia ao servidor
  if (IS_GATEWAY) {
    enviarHTTP(msg);
  }
}

// ---------- ENVIO PARA MESH ----------
void enviarMesh(String mensagem) {
  mesh.sendBroadcast(mensagem);
  Serial.println("Enviado para mesh: " + mensagem);
}

// ---------- LEITURA SENSOR ----------
void enviarDadosSensores() {
  sensors_event_t humidity, temp;
  aht_humidity->getEvent(&humidity);
  aht_temp->getEvent(&temp);

  int mq7Value = analogRead(mq7Pin);
  float voltage = mq7Value * (3.3 / 1024.0);
  float gas = voltage * 1000;

  StaticJsonDocument<256> doc;

  doc["sensor"] = mesh.getNodeId();
  doc["temperatura"] = temp.temperature;
  doc["umidade"] = humidity.relative_humidity;
  doc["gas"] = gas;

  String mensagem;
  serializeJson(doc, mensagem);

  enviarMesh(mensagem);
}

// ---------- TASK ----------
Task taskSendData(5000, TASK_FOREVER, &enviarDadosSensores);

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(externalLedPin, OUTPUT);

  // Inicializa mesh
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);

  userScheduler.addTask(taskSendData);
  taskSendData.enable();

  // Sensor
  if (!aht.begin()) {
    Serial.println("Erro no AHT!");
  }

  aht_temp = aht.getTemperatureSensor();
  aht_humidity = aht.getHumiditySensor();

  Serial.println("Mesh iniciada!");
}

// ---------- LOOP ----------
void loop() {
  mesh.update();
}
