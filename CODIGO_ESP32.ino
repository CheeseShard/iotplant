/*
 * Smart Plant Monitoring with Automatic Irrigation
 * 
 * Este código implementa um sistema de monitoramento de plantas com irrigação automática
 * utilizando ESP32, sensores de umidade do solo, temperatura, umidade do ar e luminosidade,
 * com comunicação via protocolo MQTT.
 * 
 * Baseado no artigo "Smart Plant Monitoring with Automatic Irrigation da Universidade Presbiteriana Mackenzie feito por Davi Ferreira Brito Lima"
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// =============================================
// CONFIGURAÇÕES DE HARDWARE
// =============================================

// Pinos do ESP32
const int soilMoisturePin = 34;    // Pino analógico para o sensor de umidade do solo
const int ldrPin = 35;             // Pino analógico para o sensor de luminosidade (LDR)
const int relayPin = 26;           // Pino digital para o módulo relé (bomba de água)
const int dhtPin = 27;             // Pino digital para o sensor DHT11

// Definição do tipo de sensor DHT
#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

// =============================================
// CONFIGURAÇÕES DE REDE E MQTT
// =============================================

// Configurações Wi-Fi (SUBSTITUA pelos seus dados)
const char* ssid = "SUA_REDE_WIFI";
const char* password = "SUA_SENHA_WIFI";

// Configurações MQTT
const char* mqtt_server = "broker.mqtt-dashboard.com";
const int mqtt_port = 1883;  // Porta padrão MQTT
const char* mqtt_client_id = "smart_plant_esp32"; // ID único para o cliente

// Tópicos MQTT para publicação
const char* topic_soil = "plant/soil";
const char* topic_temp = "plant/temperature";
const char* topic_humidity = "plant/humidity";
const char* topic_light = "plant/light";
const char* topic_pump = "plant/pump";

// Tópicos MQTT para assinatura (receber comandos)
const char* topic_irrigate_force = "plant/irrigate/force";

// =============================================
// VARIÁVEIS GLOBAIS
// =============================================
WiFiClient espClient;
PubSubClient client(espClient);

// Limites e configurações
const int drySoilThreshold = 2000;  // Valor abaixo do qual o solo é considerado seco (ajuste conforme calibração)
const int lowLightThreshold = 1500; // Valor abaixo do qual a luz é considerada baixa (ajuste conforme calibração)
const int publishInterval = 30000;  // Intervalo de publicação MQTT (30 segundos)
const int pumpDuration = 10000;     // Duração da irrigação em ms (10 segundos)

// Variáveis de controle
unsigned long lastPublishTime = 0;
unsigned long pumpStartTime = 0;
bool pumpActive = false;
bool allowIrrigation = true;        // Controle para evitar irrigação em horários inadequados

// =============================================
// FUNÇÃO SETUP (INICIALIZAÇÃO)
// =============================================
void setup() {
  Serial.begin(115200);  // Configura comunicação serial para debug
  
  // Configura pinos
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Garante que a bomba inicia desligada
  
  // Inicializa o sensor DHT
  dht.begin();
  
  // Conecta ao Wi-Fi
  connectToWiFi();
  
  // Configura cliente MQTT
  setupMQTT();
  
  Serial.println("Sistema de monitoramento de plantas inicializado!");
}

// =============================================
// CONEXÃO WI-FI
// =============================================
void connectToWiFi() {
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// =============================================
// CONFIGURAÇÃO MQTT
// =============================================
void setupMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); // Configura função para receber mensagens
}

// =============================================
// RECONEXÃO MQTT
// =============================================
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    
    if (client.connect(mqtt_client_id)) {
      Serial.println("Conectado!");
      
      // Assina tópicos para receber comandos
      client.subscribe(topic_irrigate_force);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

// =============================================
// CALLBACK PARA MENSAGENS RECEBIDAS
// =============================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Comando para forçar irrigação
  if (String(topic) == topic_irrigate_force) {
    if (message == "1" || message.equalsIgnoreCase("on")) {
      startPump();
      Serial.println("Irrigação FORÇADA iniciada");
    } else if (message == "0" || message.equalsIgnoreCase("off")) {
      stopPump();
      Serial.println("Irrigação FORÇADA interrompida");
    }
  }
}

// =============================================
// FUNÇÕES DE CONTROLE DA BOMBA
// =============================================
void startPump() {
  digitalWrite(relayPin, HIGH);
  pumpActive = true;
  pumpStartTime = millis();
  
  // Publica estado da bomba
  client.publish(topic_pump, "1");
}

void stopPump() {
  digitalWrite(relayPin, LOW);
  pumpActive = false;
  
  // Publica estado da bomba
  client.publish(topic_pump, "0");
}

// =============================================
// LEITURA DE SENSORES
// =============================================
int readSoilMoisture() {
  // Lê o valor analógico do sensor de umidade do solo
  int value = analogRead(soilMoisturePin);
  Serial.print("Umidade do solo: ");
  Serial.println(value);
  return value;
}

int readLightLevel() {
  // Lê o valor analógico do sensor de luminosidade (LDR)
  int value = analogRead(ldrPin);
  Serial.print("Nível de luz: ");
  Serial.println(value);
  return value;
}

float readTemperature() {
  // Lê a temperatura do sensor DHT11
  float temp = dht.readTemperature();
  
  // Verifica se a leitura falhou
  if (isnan(temp)) {
    Serial.println("Falha ao ler temperatura do DHT11!");
    return 0;
  }
  
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" °C");
  return temp;
}

float readHumidity() {
  // Lê a umidade do sensor DHT11
  float humidity = dht.readHumidity();
  
  // Verifica se a leitura falhou
  if (isnan(humidity)) {
    Serial.println("Falha ao ler umidade do DHT11!");
    return 0;
  }
  
  Serial.print("Umidade do ar: ");
  Serial.print(humidity);
  Serial.println(" %");
  return humidity;
}

// =============================================
// PUBLICAÇÃO MQTT
// =============================================
void publishSensorData() {
  // Lê valores dos sensores
  int soilMoisture = readSoilMoisture();
  int lightLevel = readLightLevel();
  float temperature = readTemperature();
  float humidity = readHumidity();
  
  // Prepara mensagens para publicação
  char msg_soil[10];
  char msg_light[10];
  char msg_temp[10];
  char msg_humidity[10];
  
  // Converte valores para strings
  snprintf(msg_soil, 10, "%d", soilMoisture);
  snprintf(msg_light, 10, "%d", lightLevel);
  snprintf(msg_temp, 10, "%.1f", temperature);
  snprintf(msg_humidity, 10, "%.1f", humidity);
  
  // Publica nos tópicos MQTT
  client.publish(topic_soil, msg_soil);
  client.publish(topic_light, msg_light);
  client.publish(topic_temp, msg_temp);
  client.publish(topic_humidity, msg_humidity);
  
  Serial.println("Dados publicados via MQTT");
}

// =============================================
// LÓGICA DE IRRIGAÇÃO
// =============================================
void checkIrrigationNeeds() {
  // Lê valores dos sensores
  int soilMoisture = readSoilMoisture();
  int lightLevel = readLightLevel();
  
  // Verifica se o solo está seco e se a irrigação é permitida
  if (soilMoisture > drySoilThreshold && allowIrrigation) {
    // Verifica se não está muito escuro (evita irrigar à noite)
    if (lightLevel > lowLightThreshold) {
      Serial.println("Solo SECO - Iniciando irrigação");
      startPump();
    } else {
      Serial.println("Solo seco, mas está escuro demais para irrigar agora");
    }
  }
}

// =============================================
// LOOP PRINCIPAL
// =============================================
void loop() {
  // Verifica conexões
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
  
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Controle da bomba de água (desliga após tempo definido)
  if (pumpActive && (millis() - pumpStartTime > pumpDuration)) {
    Serial.println("Tempo de irrigação concluído");
    stopPump();
  }

  // Publica dados no MQTT periodicamente
  if (millis() - lastPublishTime > publishInterval) {
    publishSensorData();
    lastPublishTime = millis();
    
    // Verifica necessidade de irrigação após publicar dados
    if (!pumpActive) {
      checkIrrigationNeeds();
    }
  }

  delay(1000); // Pequeno delay para estabilidade
}
