//biblioteca Modbus
#include <SimpleModbusSlave.h>
boolean ativa = false;
boolean um=false, dois_1=false, dois_3=false, tres_2=false,tres_4=false, quatro_3=false, quatro_5=false, cinco_4=false;
long tempo;
const int   SENSOR_TEMP=A1;
float temperatura;
float T1;

//int objeto=0;
int obj=9;
int botao1 = 3;
int botao2 = 4;
int botao3 = 5;
int botao4 = 6;
int botao5 = 7;
int botao6 = 8;

int rele=10;
int y=0 ;

int NUM, NUM1;

long previousMillis = 0;        // will store last time LED was updated
long previousMillis1 = 0;        // will store last time LED was updated
 
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 5000;           // interval at which to blink (milliseconds)
/* 
   SimpleModbusSlaveV10 supports function 3, 6 & 16.
   
   This example code will receive the adc ch0 value from the arduino master. 
   It will then use this value to adjust the brightness of the led on pin 9.
   The value received from the master will be stored in address 1 in its own
   address space namely holdingRegs[].
   
   In addition to this the slaves own adc ch0 value will be stored in 
   address 0 in its own address space holdingRegs[] for the master to
   be read. The master will use this value to alter the brightness of its
   own led connected to pin 9.
   
   The modbus_update() method updates the holdingRegs register array and checks
   communication.

   Note:  
   The Arduino serial ring buffer is 64 bytes or 32 registers.
   Most of the time you will connect the arduino to a master via serial
   using a MAX485 or similar.
 
   In a function 3 request the master will attempt to read from your
   slave and since 5 bytes is already used for ID, FUNCTION, NO OF BYTES
   and two BYTES CRC the master can only request 58 bytes or 29 registers.
 
   In a function 16 request the master will attempt to write to your 
   slave and since a 9 bytes is already used for ID, FUNCTION, ADDRESS, 
   NO OF REGISTERS, NO OF BYTES and two BYTES CRC the master can only write
   54 bytes or 27 registers.
 
   Using a USB to Serial converter the maximum bytes you can send is 
   limited to its internal buffer which differs between manufactures. 
*/

#define  LED 13  

// Using the enum instruction allows for an easy method for adding and 
// removing registers. Doing it this way saves you #defining the size 
// of your slaves register array each time you want to add more registers
// and at a glimpse informs you of your slaves register layout.

//////////////// registers of your slave ///////////////////
enum 
{     
  // just add or remove registers and your good to go...
  // The first register starts at address 0
  TEMP,   //0
  VENTI,  //1
  BOTAO1, //2
  BOTAO2, //3
  BOTAO3, //4
  BOTAO4, //5
  BOTAO5, //6
  CHAMADA1,//7
  CHAMADA2, //8
  CHAMADA3, //9
  CHAMADA4,//10
  CHAMADA5, //11
  ANDAR_ATUAL,  //12
  AA,//13
  door, //14
  porta, //15
  BOTAO6, //16
  RELE_A, //17
  FIMDECURSO1,//18
  FIMDECURSO2,//19
  FIMDECURSO3,//20
  FIMDECURSO4,//21
  FIMDECURSO5,//22
  setpoint,//23
  mais, //24
  menos, //25
//  VALOR_1,     
//  VALOR_2,
//  VALOR_3,
//  VALOR_4,        
  HOLDING_REGS_SIZE // leave this one
  // total number of registers for function 3 and 16 share the same register array
  // i.e. the same address space
};

unsigned int holdingRegs[HOLDING_REGS_SIZE]; // function 3 and 16 register array
////////////////////////////////////////////////////////////

void setup()
{

    
  /* parameters(HardwareSerial* SerialPort,
                long baudrate, 
    unsigned char byteFormat,
                unsigned char ID, 
                unsigned char transmit enable pin, 
                unsigned int holding registers size,
                unsigned int* holding register array)
  */
  
  /* Valid modbus byte formats are:
     SERIAL_8N2: 1 start bit, 8 data bits, 2 stop bits
     SERIAL_8E1: 1 start bit, 8 data bits, 1 Even parity bit, 1 stop bit
     SERIAL_8O1: 1 start bit, 8 data bits, 1 Odd parity bit, 1 stop bit
     
     You can obviously use SERIAL_8N1 but this does not adhere to the
     Modbus specifications. That said, I have tested the SERIAL_8N1 option 
     on various commercial masters and slaves that were suppose to adhere
     to this specification and was always able to communicate... Go figure.
     
     These byte formats are already defined in the Arduino global name space. 
  */
    pinMode(botao1, INPUT);
    pinMode(botao2, INPUT);
    pinMode(botao3, INPUT);
    pinMode(botao4, INPUT);
    pinMode(botao5, INPUT);
    pinMode(botao6, INPUT);
    pinMode(obj, INPUT); //Pino ligado ao coletor do fototransistor  
    pinMode( rele,OUTPUT);
  modbus_configure(&Serial, 9600, SERIAL_8N1, 1, 2, HOLDING_REGS_SIZE, holdingRegs);

  // modbus_update_comms(baud, byteFormat, id) is not needed but allows for easy update of the
  // port variables and slave id dynamically in any function.
  modbus_update_comms(9600, SERIAL_8N1, 1);
  
  pinMode(LED, OUTPUT);
  holdingRegs[ANDAR_ATUAL]=0;
//  holdingRegs[NUM]=0; 
   holdingRegs[door]=0;
   holdingRegs[RELE_A]=10;
   holdingRegs[porta]=1;
   holdingRegs[setpoint]=30;
} 

void loop()
{
  
  modbus_update();  
      
  holdingRegs[BOTAO1]=digitalRead(botao1);
  holdingRegs[BOTAO2]=digitalRead(botao2);
  holdingRegs[BOTAO3]=digitalRead(botao3);
  holdingRegs[BOTAO4]=digitalRead(botao4);
  holdingRegs[BOTAO5]=digitalRead(botao5);
  holdingRegs[door]=digitalRead(obj);
  holdingRegs[BOTAO6]=digitalRead(botao6);
  

// if (holdingRegs[door]==0)  
//  {  
//    holdingRegs[porta]=0;  // HIGH objeto detectado - porta fica aberta
//    delay(100);
//  }  
//  else if (holdingRegs[door]==1) {  
//    holdingRegs[porta]=1;  // LOW objeto ausente - porta pode fechar
//    delay(100);    
//  }
//      
//
      
  if (holdingRegs[BOTAO1] == HIGH && holdingRegs[porta]==1) {         // check if the input is HIGH (button released)
    NUM=1;
    NUM1=0;
    
    dois_1=true;
    dois_3=true;
    tres_2=true;
    tres_4=true;
    quatro_3=true;
    quatro_5=true;
    cinco_4=true;
    //ativa=false;
    holdingRegs[CHAMADA1]= 1;
    holdingRegs[CHAMADA2]= 0;
    holdingRegs[CHAMADA3]= 0;
    holdingRegs[CHAMADA4]= 0;
    holdingRegs[CHAMADA5]= 0;         
    }  
  if (holdingRegs[BOTAO2] == HIGH && holdingRegs[porta]==1) {         // check if the input is HIGH (button released)
    NUM=2;
     NUM1=0;
    um=true;
    tres_2=true;
    tres_4=true;
    quatro_3=true;
    quatro_5=true;
    cinco_4=true;
    //ativa=false;    
    holdingRegs[CHAMADA1]= 0;
    holdingRegs[CHAMADA2]= 1;
    holdingRegs[CHAMADA3]= 0;
    holdingRegs[CHAMADA4]= 0;
    holdingRegs[CHAMADA5]= 0;


    }   
  if (holdingRegs[BOTAO3] == HIGH && holdingRegs[porta]==1) {         // check if the input is HIGH (button released)
    NUM=3;
     NUM1=0;
    um=true;
    dois_1=true;
    dois_3=true;
    quatro_3=true;
    quatro_5=true;
    cinco_4=true;    
    //ativa=false;    
    holdingRegs[CHAMADA1]= 0;
    holdingRegs[CHAMADA2]= 0;
    holdingRegs[CHAMADA3]= 1;
    holdingRegs[CHAMADA4]= 0;
    holdingRegs[CHAMADA5]= 0;
}
if (holdingRegs[BOTAO4] == HIGH && holdingRegs[porta]==1) {         // check if the input is HIGH (button released)
   NUM=4;
    NUM1=0;
    um=true;
    dois_1=true;
    dois_3=true; 
    tres_2=true;   
   tres_4=true;
    cinco_4=true;
    //ativa=false;   
    holdingRegs[CHAMADA1]= 0;
    holdingRegs[CHAMADA2]= 0;
    holdingRegs[CHAMADA3]= 0;
    holdingRegs[CHAMADA4]= 1;
    holdingRegs[CHAMADA5]= 0;
}     
  if (holdingRegs[BOTAO5] == HIGH && holdingRegs[porta]==1) {         // check if the input is HIGH (button released)
    NUM=5;
     NUM1=0;
    um=true;
    dois_1=true;
    dois_3=true;
    tres_2=true;    
   tres_4=true; 
   quatro_3=true;    
    quatro_5=true;
    //ativa=false;    
    holdingRegs[CHAMADA1]= 0;
    holdingRegs[CHAMADA2]= 0;
    holdingRegs[CHAMADA3]= 0;
    holdingRegs[CHAMADA4]= 0;
    holdingRegs[CHAMADA5]= 1;
}
if (holdingRegs[BOTAO6]==HIGH && holdingRegs[porta]==1){
  NUM1=6;
  //um=true;  
  }


   if (NUM>holdingRegs[ANDAR_ATUAL] && digitalRead(botao6)==HIGH){

    for (int x=0;x<1;x++){
    holdingRegs[ANDAR_ATUAL]=holdingRegs[ANDAR_ATUAL]+1;
        }
        delay(400);
    }
   if (NUM<holdingRegs[ANDAR_ATUAL] && digitalRead(botao6)==HIGH){
      for (int x=0;x<1;x++){
     holdingRegs[ANDAR_ATUAL]=holdingRegs[ANDAR_ATUAL]-1;
    }
     delay(400);    
   }
//
//  
// 

  if(holdingRegs[ANDAR_ATUAL]==1){    
    holdingRegs[FIMDECURSO1]=1;
    holdingRegs[FIMDECURSO2]=0;
    holdingRegs[FIMDECURSO3]=0;
    holdingRegs[FIMDECURSO4]=0;
    holdingRegs[FIMDECURSO5]=0;
    
    if((ativa==false || (NUM1==6 && um==true))&& NUM==1){
      tempo=millis();
      ativa=true;
      holdingRegs[porta]=0;
      um=false;
      }
 if (holdingRegs[door]==0 && holdingRegs[porta]==0)  
  {  
    tempo=millis();        
  }  
  if (holdingRegs[BOTAO1] == HIGH)  
  {  
    holdingRegs[porta]=0;
    tempo=millis();        
  }          
    if(millis() > (tempo+10000))
    {   holdingRegs[porta]=1;}
          }
   
   
  
    if(holdingRegs[ANDAR_ATUAL]==2){
    holdingRegs[FIMDECURSO1]=0;
    holdingRegs[FIMDECURSO2]=1;
    holdingRegs[FIMDECURSO3]=0;
    holdingRegs[FIMDECURSO4]=0;
    holdingRegs[FIMDECURSO5]=0;

    if((ativa==false || (NUM1==6 && dois_1==true)||(NUM1==6 && dois_3==true))&& NUM==2){
      tempo=millis();
      ativa=true;
      holdingRegs[porta]=0;
    dois_1=false;
    dois_3=false;
      }
 if (holdingRegs[door]==0 && holdingRegs[porta]==0)  
  {  
    tempo=millis();        
  }  
  if (holdingRegs[BOTAO2] == HIGH)  
  {  
    holdingRegs[porta]=0;
    tempo=millis();        
  }          
    if(millis() > (tempo+10000))
    {   holdingRegs[porta]=1;}        
    }
    
    if(holdingRegs[ANDAR_ATUAL]==3){
    holdingRegs[FIMDECURSO1]=0;
    holdingRegs[FIMDECURSO2]=0;
    holdingRegs[FIMDECURSO3]=1;
    holdingRegs[FIMDECURSO4]=0;
    holdingRegs[FIMDECURSO5]=0;
    if((ativa==false || (NUM1==6 && tres_2==true)||(NUM1==6 && tres_4==true))&& NUM==3){
      tempo=millis();
      ativa=true;
      holdingRegs[porta]=0;
      tres_2=false;
      tres_4=false;
      }
 if (holdingRegs[door]==0 && holdingRegs[porta]==0)  
  {  
    tempo=millis();        
  }  
  if (holdingRegs[BOTAO3] == HIGH)  
  {  
    holdingRegs[porta]=0;
    tempo=millis();        
  }          
    if(millis() > (tempo+10000))
    {   holdingRegs[porta]=1;}         
    }
    if(holdingRegs[ANDAR_ATUAL]==4){
    holdingRegs[FIMDECURSO1]=0;
    holdingRegs[FIMDECURSO2]=0;
    holdingRegs[FIMDECURSO3]=0;
    holdingRegs[FIMDECURSO4]=1;
    holdingRegs[FIMDECURSO5]=0;
    if((ativa==false || (NUM1==6 && quatro_3==true)||(NUM1==6 && quatro_5==true))&& NUM==4){
      tempo=millis();
      ativa=true;
      holdingRegs[porta]=0;
      quatro_3=false;
      quatro_5=false;
      }
 if (holdingRegs[door]==0 && holdingRegs[porta]==0)  
  {  
    tempo=millis();        
  }  
  if (holdingRegs[BOTAO4] == HIGH)  
  {  
    holdingRegs[porta]=0;
    tempo=millis();        
  }          
    if(millis() > (tempo+10000))
    {   holdingRegs[porta]=1;}         
    }    
    
    if(holdingRegs[ANDAR_ATUAL]==5){
    holdingRegs[FIMDECURSO1]=0;
    holdingRegs[FIMDECURSO2]=0;
    holdingRegs[FIMDECURSO3]=0;
    holdingRegs[FIMDECURSO4]=0;
    holdingRegs[FIMDECURSO5]=1; 
    if((ativa==false || (NUM1==6 && cinco_4==true))&& NUM==5){
      tempo=millis();
      ativa=true;
      holdingRegs[porta]=0;
      cinco_4=false;
      }
 if (holdingRegs[door]==0 && holdingRegs[porta]==0)  
  {  
    tempo=millis();        
  }  
  if (holdingRegs[BOTAO5] == HIGH)  
  {  
    holdingRegs[porta]=0;
    tempo=millis();        
  }          
    if(millis() > (tempo+10000))
    {   holdingRegs[porta]=1;}      
    } 
  holdingRegs[TEMP]= (float (analogRead(SENSOR_TEMP)));
  T1=( 5.0 *  float (analogRead(SENSOR_TEMP)) * 100.0) / 1024.0;

if (T1<holdingRegs[setpoint]){
  digitalWrite(rele,LOW);
  holdingRegs[VENTI]=0;  
  } else if(T1>holdingRegs[setpoint]){
    digitalWrite(rele,HIGH);  
    holdingRegs[VENTI]=1;
    }
if (holdingRegs[mais]==true){
  holdingRegs[setpoint]=holdingRegs[setpoint]+1;
  holdingRegs[mais]=false;
  } 
if (holdingRegs[menos]==true){
    holdingRegs[setpoint]=holdingRegs[setpoint]-1;
  holdingRegs[menos]=false;
  }  



  
  analogWrite(LED, 255); // constrain adc value from the arduino master to 255
  }

