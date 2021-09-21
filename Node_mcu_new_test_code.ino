#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
//const char* ssid     = "ATITHI GF";// replace subscribe with your WiFi SSID(Name)
//const char* pass = "atithigf";//replace with Your Wifi Password name
//const char* host = "api.thingspeak.com";
//const char* writeAPIKey = "H7F8HNL93IYYCQB7"; //copy yout ThingSpeak channel API Key.

String apiKey = "H7F8HNL93IYYCQB7";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "Chachaji";     // replace with your wifi ssid and wpa2 key
const char *pass =  "9643632885";
const char* server = "api.thingspeak.com";
WiFiClient client;
int LDR;
double H,T,l1,l2,l3,l4,v;
char i[10],j[10],k[10],l[20],m[10],n[10],o[10],p[10],q[10];

void setup() {
Serial.begin(115200);
       delay(10);
       
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED)
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
while (!Serial) {
}
}

void loop() {
 
if (Serial.available()) {
//Serial.write(Serial.read());
String x = Serial.readString();
sscanf(x.c_str(),"coming from arduino: H= %s T= %s ldrstatus= %s PIRstatus= %s Room 1 current: %s Room 2 current: %s Room 3 current: %s Room 4 current: %s Home Voltage : %s",i,j,k,l,m,n,o,p,q);
LDR = atoi(k);
H = atof(i);
T = atof(j);
l1 = atof(m);
l2 =atof(n);
l3 = atof(o);
l4 = atof(p);
v = atof(q);
Serial.println("\nStored Values: \n");
Serial.println("\nHumidity: ");
Serial.print(H);
Serial.println("\nTemperature:  ");
Serial.print(T);
Serial.println("\nLDR_Status: ");
Serial.print(LDR);
Serial.println("\nPIR_Status: ");
Serial.print(l);
Serial.println("\nCurrent_Room1: ");
Serial.print(l1);
Serial.println("\nCurrent_Room2: ");
Serial.print(l2);
Serial.println("\nCurrent_Room3: ");
Serial.print(l3);
Serial.println("\nCurrent_Room4: ");
Serial.print(l4);

Serial.println("\nVoltage_Building: ");
Serial.print(v);

}
  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                           
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(T);
                             postStr +="&field2=";
                             postStr += String(H);
                             postStr +="&field3=";
                             postStr += String(LDR);
                             postStr +="&field4=";
                             postStr += String(l);
                             postStr +="&field5=";
                             postStr += String(l1);
                             postStr +="&field6=";
                             postStr += String(l2);
                             postStr +="&field7=";
                             postStr += String(l3);
                             postStr +="&field8=";
                             postStr += String(l4);
                             postStr +="&field9=";
                             postStr += String(v);
                             
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
         
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
 
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(3000);
}
