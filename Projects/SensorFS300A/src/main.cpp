#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_GFX.h>  
#include <Adafruit_ST7735.h>
#include <Adafruit_I2CDevice.h>
/*ST7735 TFT SPI display pins for Arduino Uno/Nano:
 * LED =   3.3V
 * SCK =   13
 * SDA =   11
 * A0 =    8
 * RESET = 9
 * CS =    10
 * GND =   GND
 * VCC =   5V
 * SPI pin names can be confusing. These are the alternative names for the SPI pins:
MOSI = DIN = R/W = SDO = DI = SI = MTSR = SDA = D1 = SDI
CS = CE = RS = SS
DC = A0 = DO = DOUT = SO = MRST
RESET = RST
SCLK = CLK = E = SCK = SCL = D0
*/
#define TFT_CS     10
#define TFT_RST    9  // You can also connect this to the Arduino reset in which case, set this #define pin to -1!
#define TFT_DC     8  // The rest of the pins are pre-selected as the default hardware SPI for Arduino Uno (SCK = 13 and SDA = 11)
// Create display:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int rele = 6;
int sensor = 2;

RTC_DS3231 reloj_rtc;
volatile int rpmcont;
int Calc;
int i=0;
int j=0;
int f=0;
float vazao=1; //Variável para armazenar o valor em L/min
float media=0; //Variável para tirar a média a cada 1 minuto
float total=0;


void rpm(){
  rpmcont++;
}

void setup() {

  //Wire.begin();
  tft.initR(INITR_MINI160x80);  // Initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_WHITE);  // Fill screen with black
  tft.setTextWrap(true);
  tft.setRotation(3); 
  tft.setCursor(10, 20);  // Set position (x,y)
  tft.setTextColor(ST7735_BLACK);  // Set color of text. First is the color of text and after is color of background
  tft.setTextSize(1);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
  tft.println("Hola");  // Print a text or value
  tft.setCursor(10, 50);  // Set position (x,y)
  //tft.setTextColor(ST7735_BLACK);  // Set color of text. We are using custom font so there is no background color supported
  tft.println("Iniciando...");  // Print a text or value
  tft.flush();
  //tft.enableDisplay(false);
  

  
  Serial.begin(9600);
  delay(1000);
  rpmcont = 0;
  attachInterrupt(1,rpm,RISING);//pin para -> 3 digital
  pinMode(rele, OUTPUT);
  digitalWrite(rele, HIGH);

 if (! reloj_rtc.begin()) {				// si falla la inicializacion del modulo
 Serial.println("Modulo RTC no encontrado !");	// muestra mensaje de error
 while (1);					// bucle infinito que detiene ejecucion del programa
 }else{
   Serial.println("Reloj Ajustado");
   //reloj_rtc.adjust(DateTime("Oct 17 2020", "13:02:00"));
 }


}

void loop() {
 //tft.enableDisplay(true);
 tft.setTextColor(ST7735_BLACK);
 rpmcont = 0;
 delay(1000);
 Serial.println("");
 Serial.print("RPM: ");
 Serial.println(rpmcont);
 Serial.println("");
 vazao = rpmcont/5.5;
 media = media + vazao;
 
 i++;
 j++;
 f++;
  Serial.print(vazao); //Imprime na serial o valor da vazão
  Serial.print(" L/min - "); //Imprime L/min
  Serial.print(i); //Imprime a contagem i (segundos)
  Serial.println("s"); //Imprime s indicando que está em segundos
  if(j == 900 ){
    digitalWrite(rele, LOW);
    Serial.println("Enciende rele");
  }
  if (f==35){
    if(vazao<4 && digitalRead(rele)<=0.2){
      digitalWrite(rele, HIGH);
      Serial.println("Se apago rele");
    }
    f=0;
  }
  
  if(i==60)  {
    total = total + media;
    media = media/60; 
    tft.setCursor(10, 40);
    Serial.print("Media por minuto = ");
    tft.print("Media por minuto = "); 
    Serial.print(media); 
    tft.print(media);
    Serial.println(" L/min - ");
    tft.print(" L/min - ");
    tft.setCursor(10, 60); 
    Serial.print("Total: ");
    tft.print("Total: ");
    Serial.print(total/60);
    tft.print(total/60);
    Serial.print(" L");
    tft.print(" L");
    media = 0; 
    i=0; 
    Serial.println("\n\nInicio\n\n"); //Imprime Inicio indicando que a contagem iniciou
  }

   DateTime fecha = reloj_rtc.now();			// funcion que devuelve fecha y horario en formato
	tft.setCursor(10, 10);  // Set position (x,y)
  	
 tft.print(fecha.dayOfTheWeek());
 Serial.print(" ");
 tft.print(" "); 				
 Serial.print(fecha.day());			// funcion que obtiene el dia de la fecha completa +"/"+fecha.month()+"/"+fecha.year()+" "+fecha.hour()+":"+fecha.minute()+":"+fecha.second()
 tft.print(fecha.day());
 Serial.print("/");				// caracter barra como separador
 tft.print("/");
 Serial.print(fecha.month());			// funcion que obtiene el mes de la fecha completa
 tft.print(fecha.month());
 Serial.print("/");				// caracter barra como separador
 tft.print("/");
 Serial.print(fecha.year());			// funcion que obtiene el año de la fecha completa
 tft.print(fecha.year());
 Serial.print(" ");				// caracter espacio en blanco como separador
 tft.setCursor(10, 20);
 Serial.print(fecha.hour());			// funcion que obtiene la hora de la fecha completa
 tft.print(fecha.hour());
 Serial.print(":");				// caracter dos puntos como separador
 tft.print(":");
 Serial.print(fecha.minute());			// funcion que obtiene los minutos de la fecha completa
 tft.print(fecha.minute());
 Serial.print(":");				// caracter dos puntos como separador
 tft.print(":");
 Serial.println(fecha.second());		// funcion que obtiene los segundos de la fecha completa
 tft.print(fecha.second());
 
 //tft.flush();
 //tft.enableDisplay(false);
 	//DateTime fecha1 = fecha;		


}