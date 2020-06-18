#include <OneWire.h>  
#include <DallasTemperature.h>  
#include <SoftwareSerial.h>    
//DS18B20 온도 센서의 데이터선인 가운데 핀을 아두이노 3번에 연결합니다.   
#define ONE_WIRE_BUS 3  
SoftwareSerial BluetoothSerial(2,5); // Software Serial RX, TX
                                     // 블루투스 TXD <-> 아두이노 RX (2)
                                     // 블루투스 RXD <-> 아두이노 TX (3)    
//1-wire 디바이스와 통신하기 위한 준비  
OneWire oneWire(ONE_WIRE_BUS);  
    
// oneWire선언한 것을 sensors 선언시 참조함.  
DallasTemperature sensors(&oneWire);  
    
//다비아스 주소를 저장할 배열 선언  
DeviceAddress insideThermometer;  

int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = true;


void setup(void)  
{  
  pinMode(fadePin,OUTPUT);  
  Serial.begin(115200);
   
  BluetoothSerial.begin(9600);  
  interruptSetup();  
  sensors.begin();  
      
     
  //버스에서 첫번째 장치의 주소를 가져온다.  
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");   
     
}  
    
    
//디바이스 주소를 출력하는 함수  
void printAddress(DeviceAddress deviceAddress)  
{  
  for (uint8_t i = 0; i < 8; i++)  
  {  
    if (deviceAddress[i] < 16) Serial.print("0");  
        //Serial.print(deviceAddress[i], HEX);  
  }  
}  
    
    
void loop(void)  
{   
    
   serialOutput() ;       
    
  if (QS == true){     // A Heartbeat Was Found
    
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
        //fadeRate = 255;         // Makes the LED Fade Effect Happen
                                // Set 'fadeRate' Variable to 255 to fade LED with pulse
        serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.     
        QS = false;                      // reset the Quantified Self flag for next time    
  }
  
  
  //sensors.requestTemperatures();   //연결되어 있는 전체 센서의 온도 값을 읽어옴  
  sensors.requestTemperaturesByIndex(0); //첫번째 센서의 온도값 읽어옴  
  //Serial.println("DONE");  
      //delay(800); 
  //센서에서 읽어온 온도를 출력 
   
  int tempC = sensors.getTempC(insideThermometer);
  String t = String(tempC);
  Serial.print(t);
  for(int i = 0; i < t.length();i++){
    BluetoothSerial.write(t[i]);
  }
    BluetoothSerial.write("\0");
  
  /*
  char c1 = t[0];
  char c2 = t[1];
  Serial.print(c1);
  Serial.println(c2);
  BluetoothSerial.write(" ");
  BluetoothSerial.write(c1);
  BluetoothSerial.write(c2);*/
}  
