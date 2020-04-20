#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "MIWIFI_2G_6ATg"
#define STAPSK  "t447dtapfyqj"
#endif

//#define ENA 11
//#define ENB 5
#define IN1 D5
#define IN2 D6
#define IN3 D7
#define IN4 D8
#define LUZ D0
#define CLAXON D1
#define NUMCOMANDOS 7;

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

bool sw = true;
String comando, accion;

void handleRoot() {  
  if (server.args()!=2) {
     server.send(200, "text/plain", "Argumentos no válidos: Uso {AT|AV|IZ|DE|LU|CL}{0|1}");
  } else {
    comando =server.arg(0);
    accion = server.arg(1);  
    comando.toUpperCase(); 
     
    Serial.println(comando);
    Serial.println(accion);

    if (comando.equals("ST")) 
       motoresFrenar();
    else if (comando.equals("AD")) 
       motoresAdelante(255);
    else if (comando.equals("AT")) 
       motoresAtras(255);
    else if (comando.equals("IZ")) 
       motoresIzquierda(255);
    else if (comando.equals("DE")) 
         motoresDerecha(255);
    else if (comando.equals("CL")) 
       claxon(); // NO IMPLEMENTADO EL CLAXON EN ROBOT AZUL
    else if (comando.equals("LU")) {
       if (accion.equals("1")) 
         luzON(); 
       else  
         luzOFF();  
    }     
    server.send(200, "text/plain", "Comando:"+comando + " Accion:"+accion);
  }
}

void handleNotFound() {
  digitalWrite(LED_BUILTIN, HIGH);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);    
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LUZ, OUTPUT);
  pinMode(CLAXON, OUTPUT);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(LUZ, LOW);
  digitalWrite(CLAXON, LOW);
  
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}


void motoresAdelante(int motorSpeed) { 
  //analogWrite(ENA, motorSpeed);
  //analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  
}
 
void motoresAtras(int motorSpeed) {
  //analogWrite(ENA, motorSpeed);
  //analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);      
}

void motoresIzquierda(int motorSpeed) { 
  //analogWrite(ENA, motorSpeed);
  //analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  
}

void motoresDerecha(int motorSpeed) { 
  //analogWrite(ENA, motorSpeed);  
  //analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  
}

void motoresFrenar () {
  //digitalWrite(ENA, LOW);
  //digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);   
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);   
}

void luzON() {  
  Serial.println("LUZ_ON");
  digitalWrite(LUZ, HIGH);
}

void luzOFF() {  
  digitalWrite(LUZ, LOW);
}

void claxon() {
  digitalWrite(CLAXON, HIGH);
  delay(50);
  digitalWrite(CLAXON,LOW);
}
