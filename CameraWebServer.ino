// Biblioteca da câmera ESP32-CAM
#include "esp_camera.h"
// Biblioteca Wi‑Fi
#include <WiFi.h>
// Biblioteca para requisições HTTP
#include <HTTPClient.h>
// Biblioteca para trabalhar com JSON
#include <ArduinoJson.h>
// Biblioteca do display LCD I2C
#include <LiquidCrystal_I2C.h>
// Comunicação I2C
#include <Wire.h>


// LCD ligado:
// SDA -> IO14
// SCL -> IO15
// (na simulação usei D14 e D15)


// ======================
// CONFIGURAÇÃO DO WIFI
// ======================
const char* ssid = "INTERNET QUARTO 2G";
const char* password = "252423@@";


// ======================
// IP DO SERVIDOR PYTHON
// ======================
// Endereço do notebook rodando Flask + YOLO
String servidor = "http://192.168.1.33:5000/detectar";


// ======================
// CONFIGURAÇÃO LCD
// ======================
// Endereço I2C 0x27
// Display 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);


// ======================
// MODELO DA CAMERA
// ======================
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"


// ===================================================
// SETUP → executa apenas uma vez ao ligar o ESP32
// ===================================================
void setup() {

  // Inicia monitor serial
  Serial.begin(115200);


  // ======================
  // CONFIGURAÇÃO DO LCD
  // ======================
  Wire.begin(14, 15); // SDA = 14 e SCL = 15

  lcd.init();         // inicia LCD
  lcd.backlight();    // liga luz do LCD

  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");


  // ======================
  // CONEXÃO WIFI
  // ======================
  WiFi.begin(ssid, password);

  Serial.print("Conectando WiFi");

  int tentativas = 0;


  // tenta conectar no Wi‑Fi
  while (WiFi.status() != WL_CONNECTED && tentativas < 30) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }


  // ======================
  // WIFI CONECTADO
  // ======================
  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("");
    Serial.println("WiFi conectado!");

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.print("WiFi OK");

  } else {

    // caso falhe conexão
    Serial.println("");
    Serial.println("Falha no WiFi");

    lcd.clear();
    lcd.print("Falha WiFi");

    return;
  }


  // ======================
  // CONFIGURAÇÃO DA CAMERA
  // ======================
  camera_config_t config;

  // canal do clock da câmera
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  // pinos da câmera
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  // frequência do clock
  config.xclk_freq_hz = 20000000;

  // formato da imagem JPEG
  config.pixel_format = PIXFORMAT_JPEG;


  // resolução da câmera
  config.frame_size = FRAMESIZE_SVGA;

  // qualidade JPEG
  // menor número = melhor qualidade
  config.jpeg_quality = 8;

  // quantidade de buffers
  config.fb_count = 1;


  // inicia câmera
  esp_err_t err = esp_camera_init(&config);


  // verifica erro
  if (err != ESP_OK) {

    Serial.println("Erro camera");

    lcd.clear();
    lcd.print("Erro Camera");

    return;
  }


  // câmera funcionando
  lcd.clear();
  lcd.print("Camera OK");

  Serial.println("Camera OK");
}


// ===================================================
// LOOP → executa infinitamente
// ===================================================
void loop() {


  // ======================
  // VERIFICA WIFI
  // ======================
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi caiu!");

    delay(2000);
    return;
  }


  // ======================
  // CAPTURA FOTO
  // ======================
  camera_fb_t *fb = esp_camera_fb_get();

  if (!fb) {

    Serial.println("Erro camera");
    return;
  }


  // ======================
  // ENVIA FOTO PARA IA
  // ======================
  HTTPClient http;

  // conecta ao servidor Flask
  http.begin(servidor);

  // define tipo de conteúdo
  http.addHeader("Content-Type", "image/jpeg");


  // envia imagem JPEG via POST
  int httpResponseCode =
      http.POST(fb->buf, fb->len);


  // mostra código HTTP
  Serial.print("HTTP: ");
  Serial.println(httpResponseCode);


  // ======================
  // RESPOSTA DO SERVIDOR
  // ======================
  if (httpResponseCode > 0) {

    // pega resposta do Flask
    String resposta = http.getString();

    Serial.println(resposta);


    // cria JSON
    DynamicJsonDocument doc(256);

    deserializeJson(doc, resposta);


    // pega nome do objeto
    String objeto = doc["objeto"];


    // ======================
    // MOSTRA NO LCD
    // ======================
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("OBJETO:");

    lcd.setCursor(0, 1);
    lcd.print(objeto);

  } else {

    // erro ao conectar
    Serial.println("Erro servidor");
  }


  // encerra HTTP
  http.end();

  // libera memória da câmera
  esp_camera_fb_return(fb);


  // espera 5 segundos
  delay(5000);
}
