/* Programa para PAC II:
 * Monitor o volume na sua caixa d'·Ågua
 * Biblioteca da Placa: "esp8266 by ESP8266 Community vers„o 2.3.0"
 * Placa: "NodeMCU 1.0 (ESP-12E Module)"
 * Upload Speed: "115200"
 * CPU Frequency: "160MHz"
 * //https://dl.espressif.com/dl/package_esp32_index.json
*/
//=====================================================================
// --- Inclus„o de bibliotecas ---
#include <WiFi.h> 
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
// ======================================================================
// --- Dados de Acesso do seu roteador ---
#define WLAN_SSID       "12345678" // InformaÁ„o da SSID do seu roteador
#define WLAN_PASS       "12345678"   // senha de acesso do seu roteador
// ======================================================================
// --- Dados de Acesso da Plataforma Adafruit IO ---
#define AIO_SERVER      "io.adafruit.com"     // manter fixo
#define AIO_SERVERPORT  1883                  // manter fixo
#define AIO_USERNAME    "marcelomoresco"        // sua informaÁ„o
#define AIO_KEY         "projeto-pac" // sua informaÁ„o
// ======================================================================
// --- Mapeamento de Hardware ---
#define trigPin 22  //D1 - PINO DE TRIGGER PARA SENSOR ULTRASSONICO
#define echoPin 23  //D2 - PINO DE ECHO PARA SENSOR ULTRASSONICO
 
WiFiClient client; // cria uma classe WiFiClient com o ESP8266 para conex„o com o servidor MQTT
 
// Configura a classe MQTT passando para o WiFi cliente e Servidor MQTT os detalhes do login
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
// O endereÁamento para enviar os arquivos seguem o formato: <username>/feeds/<feedname>
Adafruit_MQTT_Publish volume = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/volume");
 
// ======================================================================
// --- Vari·veis Globais ---
long duracao = 0;
float distancia = 0;

// ======================================================================
// --- Void Setup ---
void setup() {
   
  Serial.begin(9600); // inicia comunicaÁ„o serial com velocidade 9600

   
  Serial.println(F("Monitorar Volume Caixa D'agua - Adafruit MQTT")); // escreve na serial   
  // Conecta ao ponto de acesso WiFi
  Serial.println(); Serial.println();
  Serial.print("Conectando ao ");
  Serial.println(WLAN_SSID);
 
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
 
  Serial.println("WiFi Conectado");
  Serial.println("IP endere√ßo: "); Serial.println(WiFi.localIP());
   
// ======================================================================
// --- ConfiguraÁ„o IO ---
  pinMode(trigPin, OUTPUT); // pino D1 como saÌda para TRIGGER
  pinMode(echoPin, INPUT);  // pino D2 como entrada para ECHO
}
// ======================================================================
// --- void loop ---
void loop() {
  MQTT_connect();   // chama funÁ„o para conex„o com MQTT server
   
  digitalWrite(trigPin, LOW);    // Limpa trigPin
  delayMicroseconds(2);          // aguarda 2 microsegundos
 
  digitalWrite(trigPin, HIGH);   // Seta trigPin HIGH aguarda 
  delayMicroseconds(10);         // aguada 10 microsegundos
  digitalWrite(trigPin, LOW);    // Seta trigPin LOW 
   
  // Leitura do echoPin, retorna a onda de som em microsegundos
  duracao = pulseIn(echoPin, HIGH);
  distancia= duracao*0.034/2;
  distancia = distancia*10; // converter cm para mm
 
  if (distancia > 96){  // leitura minima. ReservatÛrio vazio
    distancia = 96;
  }
    if (distancia < 37){  // leitura m·xima. ReservatÛrio vazio
    distancia = 37;
  }
 int volumevar = map(distancia, 37, 96, 1000, 0); 
  /* Remapeia o range de leitura
   * Ao invÈs de ler de 37 a 96, ler· de 1000 a 0*/
 Serial.print("distancia:"); // imprime "distancia:"
  Serial.println(distancia);  // imprime a variavel distancia
  Serial.print("volume:");    // imprime "volume:"
  Serial.println(volumevar);  // imprime a variavel volume
   
  volume.publish(volumevar);     // publica variavel "distancia" em no feed "volume"
  // nossa saÌda ser· em mL
   
 
   
  delay(3000); // aguarda 3 segundos
  /* ObservaÁ„o: A plataforma Adafruit IO sÛ permite 30 publicaÁıes/minuto
   * na conta gratuita. Ent„o È importante n„o ultrapassar ou sua leitura 
   * na dashboard ser· inst·vel e incorreta.*/
}
 
// ======================================================================
// --- FunÁ„o para Conex„o / Reconex„o MQTT ---
  void MQTT_connect() {
    int8_t ret;
   
    // para de tentar reconex„o quando consegue
    if (mqtt.connected()) {
      return;
    }
    Serial.print("Conectando ao MQTT... "); // imprime na serial
    // tenta conex„o 5 vezes. Depois WatchDogTime!
    uint8_t retries = 5;
    while ((ret = mqtt.connect()) != 0) { // conectar· quando retornar 0
      Serial.println(mqtt.connectErrorString(ret));
      Serial.println("Nova tentativa de conex√£o MQTT em 5 segundos...");
      mqtt.disconnect();
      delay(5000);  // aguarda 5 segundos
      retries--;
      if (retries == 0) { // trava e espera reset
        while (1);
      }
    }
    Serial.println("MQTT Conectado!"); // imprime na serial
  }
// ======================================================================
// --- FIM ---
