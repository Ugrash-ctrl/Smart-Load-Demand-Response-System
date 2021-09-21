//Arduino UNO code
#include "DHT.h"
#include <SoftwareSerial.h>
#define DHTPIN 7
#define BUZZ 4
const byte sensorPin = 8;
int flag;
#define SAMPLES 300   //Number of samples you want to take everytime you loop
#define ACS_Pin A2    //ACS712 data pin analong input A0
#define ACS_Pin_1 A3    //ACS712 data pin analong input A3
#define ACS_Pin_2 A4    //ACS712 data pin analong input A0
#define ACS_Pin_3 A5    //ACS712 data pin analong input A0
int ldr=A0;

float High_peak,Low_peak;         //Variables to measure or calculate
float Amps_Peak_Peak, Amps_RMS;
float Amps_Peak_Peak2, Amps_RMS2;
float Amps_Peak_Peak3, Amps_RMS3;
float Amps_Peak_Peak4, Amps_RMS4;

#define DHTTYPE DHT11 // DHT 11

SoftwareSerial espSerial(5, 6);
DHT dht(DHTPIN, DHTTYPE);
String str;

void setup(){
Serial.begin(115200);
espSerial.begin(115200);
dht.begin();
pinMode(ldr , INPUT );
pinMode(sensorPin,INPUT);
pinMode(ACS_Pin,INPUT);        //Define pin mode
pinMode(ACS_Pin_1,INPUT);
pinMode(ACS_Pin_2,INPUT);
pinMode(ACS_Pin_3,INPUT);
pinMode(BUZZ,OUTPUT);
delay(1000);

}
void loop()
{
  float VoltagesDataRaw = 238 + random(0.78,2.5);
  Serial.println("Voltage is : ");
  Serial.println(VoltagesDataRaw);
  int ldrstatus=analogRead(ldr);  
Serial.println("ldrstatus=");
Serial.println(ldrstatus);

 if(digitalRead(sensorPin)== HIGH)
    {
      flag=1;
      Serial.println("Motion detected");

    }
    else if(digitalRead(sensorPin)== LOW)
    {
      flag=0;
      Serial.println("Motion stoped");
        }
     else{
      flag= -1;
      Serial.println("Sensor may not be working");
      }
     
  read_Amps(0);                              
  Amps_RMS = Amps_Peak_Peak*0.3536*0.06;     //Now we have the peak to peak value normally the formula requires only multiplying times 0.3536
                            //but since the values will be very big you should multiply by 0.06, you can first not use it,
                                             //do your calculations and compare them to real values measured by an Ammeter. eg: 0.06=Real value/Measured value
 
  if(Amps_RMS < 0.12){
    Amps_RMS = 0;
    Serial.println("No Current");
    }                                          
  else{
    Serial.print("Current = ");
    Serial.print(Amps_RMS);
    }

  read_Amps(1);
  Amps_RMS2 = Amps_Peak_Peak2*0.3536*0.06;     //Now we have the peak to peak value normally the formula requires only multiplying times 0.3536
                            //but since the values will be very big you should multiply by 0.06, you can first not use it,
                                             //do your calculations and compare them to real values measured by an Ammeter. eg: 0.06=Real value/Measured value
 
  if(Amps_RMS2 < 0.12){
    Amps_RMS2 = 0;
    Serial.println("No Current");
    }                                          
  else{
    Serial.print("Current = ");
    Serial.print(Amps_RMS2);
    }

read_Amps(2);                              
  Amps_RMS3 = Amps_Peak_Peak3*0.3536*0.06;     //Now we have the peak to peak value normally the formula requires only multiplying times 0.3536
                            //but since the values will be very big you should multiply by 0.06, you can first not use it,
                                             //do your calculations and compare them to real values measured by an Ammeter. eg: 0.06=Real value/Measured value
 
  if(Amps_RMS3 < 0.12){
    Amps_RMS3 = 0;
    Serial.println("No Current");
    }                                          
  else{
    Serial.print("Current = ");
    Serial.print(Amps_RMS3);
    }

  read_Amps(3);
  Amps_RMS4 = Amps_Peak_Peak4*0.3536*0.06;     //Now we have the peak to peak value normally the formula requires only multiplying times 0.3536
                            //but since the values will be very big you should multiply by 0.06, you can first not use it,
                                             //do your calculations and compare them to real values measured by an Ammeter. eg: 0.06=Real value/Measured value
 
  if(Amps_RMS4 < 0.12){
    Amps_RMS4 = 0;
    Serial.println("No Current");
    }                                          
  else{
    Serial.print("Current = ");
    Serial.print(Amps_RMS4);
    }


 
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
Serial.print("H: ");
Serial.print(h);
Serial.print("% ");
Serial.print(" T: ");
Serial.print(t);
Serial.println("C");
str =String("coming from arduino: ")+String("H= ")+String(h)+String(" T= ")+String(t)+String(" ldrstatus= ")+String(ldrstatus)+String(" PIRstatus= ")+String(flag)+String(" Room 1 current: ")+String(Amps_RMS)+String(" Room 2 current: ")+String(Amps_RMS2)+String(" Room 3 current: ")+String(Amps_RMS3)+String(" Room 4 current: ")+String(Amps_RMS4)+String(" Home Voltage : ")+String(VoltagesDataRaw);
espSerial.print(str);
delay (5000);

float threshold_value = 50;
float opcurrent = Amps_RMS + Amps_RMS2 + Amps_RMS3 + Amps_RMS4;
float power = VoltagesDataRaw * opcurrent;

if (power >= threshold_value){
  digitalWrite(BUZZ, HIGH);}
else{
  digitalWrite(BUZZ, LOW);
    }
}



void read_Amps(int i)            //read_Amps function calculate the difference between the high peak and low peak
{                           //get peak to peak value
    if (i ==0)
      {
      int cnt;            //Counter
      High_peak = 0;      //We first assume that our high peak is equal to 0 and low peak is 1024, yes inverted
      Low_peak = 1024;
 
      for(cnt=0 ; cnt<SAMPLES ; cnt++)          //everytime a sample (module value) is taken it will go through test
          {
            float ACS_Value = analogRead(ACS_Pin); //We read a single value from the module

       
            if(ACS_Value > High_peak)                //If that value is higher than the high peak (at first is 0)
                {
                  High_peak = ACS_Value;            //The high peak will change from 0 to that value found
                }
       
            if(ACS_Value < Low_peak)                //If that value is lower than the low peak (at first is 1024)
                {
                  Low_peak = ACS_Value;             //The low peak will change from 1024 to that value found
                }
          }                                        //We keep looping until we take all samples and at the end we will have the high/low peaks values
     
      Amps_Peak_Peak = High_peak - Low_peak;      //Calculate the difference
      }
else if (i == 1){
          int cnt;            //Counter
          High_peak = 0;      //We first assume that our high peak is equal to 0 and low peak is 1024, yes inverted
          Low_peak = 1024;
 
          for(cnt=0 ; cnt<SAMPLES ; cnt++)          //everytime a sample (module value) is taken it will go through test
            {
                float ACS_Value = analogRead(ACS_Pin_1); //We read a single value from the module

       
            if(ACS_Value > High_peak)                //If that value is higher than the high peak (at first is 0)
                {
                  High_peak = ACS_Value;            //The high peak will change from 0 to that value found
                }
       
            if(ACS_Value < Low_peak)                //If that value is lower than the low peak (at first is 1024)
                {
                  Low_peak = ACS_Value;             //The low peak will change from 1024 to that value found
                }
            }                                        //We keep looping until we take all samples and at the end we will have the high/low peaks values
     
      Amps_Peak_Peak2 = High_peak - Low_peak;      //Calculate the difference
      }

else if (i == 2){
          int cnt;            //Counter
          High_peak = 0;      //We first assume that our high peak is equal to 0 and low peak is 1024, yes inverted
          Low_peak = 1024;
 
          for(cnt=0 ; cnt<SAMPLES ; cnt++)          //everytime a sample (module value) is taken it will go through test
            {
                float ACS_Value = analogRead(ACS_Pin_2); //We read a single value from the module

       
            if(ACS_Value > High_peak)                //If that value is higher than the high peak (at first is 0)
                {
                  High_peak = ACS_Value;            //The high peak will change from 0 to that value found
                }
       
            if(ACS_Value < Low_peak)                //If that value is lower than the low peak (at first is 1024)
                {
                  Low_peak = ACS_Value;             //The low peak will change from 1024 to that value found
                }
            }                                        //We keep looping until we take all samples and at the end we will have the high/low peaks values
     
      Amps_Peak_Peak3 = High_peak - Low_peak;      //Calculate the difference
      }

else {
          int cnt;            //Counter
          High_peak = 0;      //We first assume that our high peak is equal to 0 and low peak is 1024, yes inverted
          Low_peak = 1024;
 
          for(cnt=0 ; cnt<SAMPLES ; cnt++)          //everytime a sample (module value) is taken it will go through test
            {
                float ACS_Value = analogRead(ACS_Pin_3); //We read a single value from the module

       
            if(ACS_Value > High_peak)                //If that value is higher than the high peak (at first is 0)
                {
                  High_peak = ACS_Value;            //The high peak will change from 0 to that value found
                }
       
            if(ACS_Value < Low_peak)                //If that value is lower than the low peak (at first is 1024)
                {
                  Low_peak = ACS_Value;             //The low peak will change from 1024 to that value found
                }
            }                                        //We keep looping until we take all samples and at the end we will have the high/low peaks values
     
      Amps_Peak_Peak4 = High_peak - Low_peak;      //Calculate the difference
      }
}
