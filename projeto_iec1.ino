#include <Servo.h>

String ssid     	= "Simulator Wifi";  	
String password 	= ""; 				 	
String host     	= "api.thingspeak.com"; 
const int httpPort  = 80;					
String uri 			= "/update?api_key=TE4HT8QM3U9SFN3H&field3=";

int setupESP8266(void) {
  Serial.begin(115200);   
  Serial.println("AT");   
  delay(10);        
  if (!Serial.find("OK")) return 1;
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);      
  if (!Serial.find("OK")) return 2;
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);    
  if (!Serial.find("OK")) return 3;
  
  return 0;
}
 
void enviaTemperaturaESP8266(void) {
  
  int temp = map(analogRead(A0),20,358,-40,125); 
  String httpPacket = "GET " + uri + String(temp) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10);
  Serial.print(httpPacket);
  delay(10); 
  if (!Serial.find("SEND OK\r\n")) return;

}


Servo roda,roda2, eixo;
int valor, valor2, angulo, vel, vel2;
void setup()
{
  setupESP8266();
  Serial.begin(9600);
  roda.attach(8, 500, 2500);
  roda2.attach(7, 500, 2500);
  eixo.attach(9);
  eixo.write(92);
  delay(1500);
}

void loop()
{ 
   valor = digitalRead(13);
   valor2 = digitalRead(2);
   if(valor==HIGH)
   {
     vel=95;
     vel2=85;
   }
   if(valor2==HIGH)
   {
     vel=85;
     vel2=95;
   }
   if(valor==LOW && valor2==LOW)
   {
     vel=90;
     vel2=90;
   }
   if(analogRead(A1)>=750)
   {
     angulo=43;
   }
   if(analogRead(A1)<=273)
   {
     angulo=137;
   }
   if(analogRead(A1)>273 && analogRead(A1)<750)
   {
     angulo=92;
   }  
   Serial.println(analogRead(A1));
   eixo.write(angulo);
   roda.write(vel);
   roda2.write(vel2);
   enviaTemperaturaESP8266();
   Serial.println(analogRead(A0));
}