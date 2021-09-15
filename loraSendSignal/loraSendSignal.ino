#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>
#include "images.h"
#include <LoRa.h>


#define SCK     5    // GPIO5  -- SCK
#define MISO    19   // GPIO19 -- MISO
#define MOSI    27   // GPIO27 -- MOSI
#define SS      18   // GPIO18 -- CS
#define RST     23   // GPIO14 -- RESET (If Lora does not work, replace it with GPIO14)
#define DI0     26   // GPIO26 -- IRQ(Interrupt Request)


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



unsigned int counter = 0;

Adafruit_ADS1115 ads; 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


String rssi = "RSSI --";
String packSize = "--";
String packet ;

#define BAND 433E6


void setup() {

   Serial.begin(115200);
  
   pinMode(16,OUTPUT);
   pinMode(2,OUTPUT);
      
   digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
   delay(50); 
   digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high


   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
   }

  Serial.println();
  Serial.println("LoRa Sender Test");
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSyncWord(0xF3);
    //LoRa.onReceive(cbk);
    //LoRa.receive();
  Serial.println("init ok");
  
    display.clearDisplay(); //BORRAR DISPLAY;
    display.setTextColor(WHITE);
    display.setCursor(0,0);//POSICION
    display.drawBitmap(0,0,sequopro,128,64,BLACK,WHITE);//IMAGEN 1
    display.display();
    delay(3000);
   
  delay(1500);
}

void loop() {
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //display.drawBitmap(0, 2, cobCuatro,  16, 16, 1); 
  display.setCursor(0,0);
  
  display.println("Sending packet: ");
  display.setCursor(90,0);
  display.println(String(counter));
  Serial.println(String(counter));
  display.display();

  // send packet
  LoRa.beginPacket();
  LoRa.print("sequopro ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
