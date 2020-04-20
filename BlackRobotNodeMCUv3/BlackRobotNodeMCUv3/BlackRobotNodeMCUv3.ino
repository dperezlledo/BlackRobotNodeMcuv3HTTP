#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

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

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged\r\n";       // a string to send back
unsigned int localPort = 3030;      // local port to listen on
bool sw = true;
String comando, accion;
WiFiUDP Udp;
Servo myservo;  // create servo object to control a servo

void handleRoot(String msg) {  
    comando = msg.substring(0,2);
    comando.toUpperCase(); 
    accion = msg.substring(2); // 0 apagar 1 encender 
   
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
    else if (comando.substring(0,1).equals("X")) // Servo camara cabeza
       camaraWeb(msg);
    else if (comando.equals("CL")){
       if (accion.equals("1")) 
         claxonON(); 
       else  
         claxonOFF();  
    }      
    else if (comando.equals("LU")) {
       if (accion.equals("1")) 
         luzON(); 
       else  
         luzOFF();  
    }     
    Serial.println("Comando:"+comando + " Accion:"+accion);
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
  
  myservo.attach(D4);  // attaches the servo on GIO2 to the servo object
  myservo.write(87);
  
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
}

void loop(void) {
   // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
                  packetSize,
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                  Udp.destinationIP().toString().c_str(), Udp.localPort(),
                  ESP.getFreeHeap());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    handleRoot(packetBuffer);
  }
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

void claxonON() {
  digitalWrite(CLAXON, HIGH);
  delay(50);
  digitalWrite(CLAXON,LOW);
}

void claxonOFF() {
  digitalWrite(CLAXON,LOW);
}

void camaraWeb(String msg) {
  int pos = msg.substring(1,3).toInt();
  pos = (pos*180)/99;
  myservo.write(pos);
  Serial.println(pos);
}
