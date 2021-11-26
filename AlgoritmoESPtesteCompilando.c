/* Programa para PAC II:
 * Monitor o volume na sua caixa d'água
 * Biblioteca da Placa: "esp32 dev"
 * Upload Speed: "9600"
 * CPU Frequency: "160MHz"
 * //https://dl.espressif.com/dl/package_esp32_index.json
*/
//=====================================================================
// --- Inclusão de bibliotecas ---
#include <WiFi.h> 
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "SoftwareSerial.h"
#include "AUnit.h"
// ======================================================================
// --- Dados de Acesso do seu roteador ---
#define WLAN_SSID       "12345678" // Informação da SSID do seu roteador
#define WLAN_PASS       "12345678"   // senha de acesso do seu roteador
// ======================================================================
// --- Dados de Acesso da Plataforma Adafruit IO ---
#define AIO_SERVER      "io.adafruit.com"     // manter fixo
#define AIO_SERVERPORT  1883                  // manter fixo
#define AIO_USERNAME    "vcleitoso"        // sua informação
#define AIO_KEY         "aio_zJmJ397OfZxIvm5t3LNgAIi22JpT" // sua informação
// ======================================================================
// --- Mapeamento de Hardware --- 
WiFiClient client; // cria uma classe WiFiClient com o ESP32 para conexão com o servidor MQTT
 
// Configura a classe MQTT passando para o WiFi cliente e Servidor MQTT os detalhes do login
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
// O endereçamento para enviar os arquivos seguem o formato: <username>/feeds/<feedname>
Adafruit_MQTT_Publish volume = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/volume");
// ======================================================================
// --- Confirguração Slave----
SoftwareSerial blackBoardMaster(34,35); // (RX, TX), usar portas certas.

// ======================================================================
// --- Variáveis Globais ---
float duracao;
float distancia;
float volumevar;
int conectado = 0;
// ======================================================================
// --- Funções ---
 void WifiConecta (){
   WiFi.begin(WLAN_SSID, WLAN_PASS);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }
   Serial.println();

   Serial.println("WiFi Conectado");
   Serial.println("IP endereço: "); Serial.println(WiFi.localIP());
 }
 void MQTT_connect() {
    int8_t ret;
   
    // para de tentar reconexão quando consegue
    if (mqtt.connected()) {
      return;
    }
    Serial.print("Conectando ao MQTT... "); // imprime na serial
    // tenta conexão 5 vezes. Depois WatchDogTime!
    uint8_t retries = 5;
    while ((ret = mqtt.connect()) != 0) { // conectará quando retornar 0
      Serial.println(mqtt.connectErrorString(ret));
      Serial.println("Nova tentativa de conexão MQTT em 5 segundos...");
      mqtt.disconnect();
      delay(5000);  // aguarda 5 segundos
      retries--;
      if (retries == 0) { // trava e espera reset
        while (1);
      }
    }
    int conectado = 1;
    Serial.println("MQTT Conectado!"); // imprime na serial
  }
void calculo() {

  duracao = blackBoardMaster.read();
  distancia= duracao*0.01715;
  distancia = distancia*10; // converter cm para mm
 
  if (distancia > 96){  // leitura mínima. Reservatório vazio
    distancia = 96;
  }
    if (distancia < 37){  // leitura máxima. Reservatório vazio
    distancia = 37;
  }
  volumevar = map(distancia, 37, 96, 1000, 0); 
  
}

 void publica () {
  Serial.print("distancia:"); // imprime "distancia:"
  Serial.println(distancia);  // imprime a variavel distancia
  Serial.print("volume:");    // imprime "volume:"
  Serial.println(volumevar);  // imprime a variavel volume
   
  volume.publish(volumevar);     // publica variável "distância" em no feed "volume"
  // nossa saída será em mL
   
  delay(3000); // aguarda 3 segundos
  /* Observação: A plataforma Adafruit IO só permite 30 publicações/minuto
   * na conta gratuita. Então é importante não ultrapassar ou sua leitura 
   * na dashboard será instável e incorreta.*/
  
 }

// ======================================================================
//Testes

test(wifi){
 WifiConecta();
 assertEqual(WiFi.status(),WL_CONNECTED);
}
test(MQTT){
 WifiConecta();
 MQTT_connect();
 assertEqual(conectado,1);
}
test(calc){ 
  float y;

  y= 5822*0.01715;
  y = y*10; // converter cm para mm
 
  if (y > 96){  // leitura mínima. Reservatório vazio
    y = 96;
  }
    if (y < 37){  // leitura máxima. Reservatório vazio
    y = 37;
  }
float result=96;
 assertEqual(y,result);
}


// ======================================================================
// --- Void Setup ---
void setup() {
  blackBoardMaster.begin(9600);
  Serial.begin(9600); // inicia comunicação serial com velocidade 9600

   
  Serial.println(F("Monitorar Volume Caixa D'agua - Adafruit MQTT")); // escreve na serial   
  // Conecta ao ponto de acesso WiFi
  Serial.println(); Serial.println();
  Serial.print("Conectando ao ");
  Serial.println(WLAN_SSID);
 
  WifiConecta(); 
}
   
// ======================================================================


// --- void loop ---
void loop() {
 aunit::TestRunner::run();

  calculo();
  publica();
  
  

 
}

 
// ======================================================================

// --- FIM ---
