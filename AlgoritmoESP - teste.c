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
int duracao = 0;
float distancia = 0;

// ======================================================================
//Testes

test(NotNull) {
 assertNotEqual(duracao, 0); 
 assertNotEqual(volumevar, distancia); 
}

test(dist){
 assertEqual(calculoDistancia(5822),99.99285);
}
test(leit.Min){
 assertEqual(distanciaMINMAX(98),96);
}
test(leit.Max){
 assertEqual(distanciaMINMAX(35),37);
}
test(MQTT){
 assertEqual(conectado,1);
}
//ver com o joe se isso pode ser feito, ou então coloca o teste no loop.

test(wifi){
 WifiConecta();
 assertEqual(WiFi.status(),WL_CONNECTED);
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
  MQTT_connect();   // chama função para conexão com MQTT server
   
  calculoDistancia(duracao, distancia);
 
  distanciaMINMAX(distancia);
 
  range();

  imprimevar();
   
  publicarvar();   
 
   
  delay(3000); // aguarda 3 segundos
  /* Observação: A plataforma Adafruit IO só permite 30 publicações/minuto
   * na conta gratuita. Então é importante não ultrapassar ou sua leitura 
   * na dashboard será instável e incorreta.*/
}
 
// ======================================================================
// --- Função para Conexão / Reconexão MQTT ---
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

float calculoDistancia (duracao){
  duracao = blackBoardMaster.read(); //recebe duração do arduino uno
  distancia= duracao*0.017175;
  distancia = distancia*10; // converter cm para mm
  return distancia;

}

float distanciaMINMAX (distancia){
 if (distancia > 96){  // leitura minima. Reservatório vazio
    distancia = 96;
  }
    if (distancia < 37){  // leitura máxima. Reservatório vazio
    distancia = 37;
  }
 return distancia;
}

void range (){ 
 int volumevar = map(distancia, 37, 96, 1000, 0); 
  /* Remapeia o range de leitura
   * Ao invés de ler de 37 a 96, lerá de 1000 a 0*/

}

void imprimevar (){
  Serial.print("distancia:"); // imprime "distancia:"
  Serial.println(distancia);  // imprime a variavel distancia
  Serial.print("volume:");    // imprime "volume:"
  Serial.println(volumevar);  // imprime a variavel volume

}

void publicarvar (){
 volume.publish(volumevar);     // publica variavel "distancia" em no feed "volume"
  // nossa saída será em mL

}


// ======================================================================
// --- FIM ---
