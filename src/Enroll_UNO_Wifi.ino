
#include <Wire.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>// print the character

void setup() {
  Tft.TFTinit();  // init TFT library
  Wire.begin(1);                // join i2c bus with address #1
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {

  int x = Wire.read();    // receive byte as an integer
  int id = Wire.read();
  
  if (x == 1) {
    Tft.fillScreen();
    Tft.drawString("Coloca", 25, 50, 5, WHITE);
    Tft.drawString("el dedo", 15, 150, 5, WHITE);
  }
  else if (x == 2) {
    Tft.fillScreen();
    Tft.drawString("Retira", 25, 50, 5, WHITE);
    Tft.drawString("el dedo", 20, 150, 5, WHITE);
  }
  else if (x == 3)
  {
    Tft.fillScreen();
    Tft.drawString("Vuelve a", 5, 100, 4, WHITE);
    Tft.drawString("colocar el", 5, 150, 4, WHITE);
    Tft.drawString("dedo", 5, 200, 4, WHITE);
  }
  else if (x == 4)
  {
    Tft.fillScreen();
    Tft.drawString("Huella", 10, 50, 4, WHITE);
    Tft.drawString("guardada", 10, 100, 4, WHITE);
    Tft.drawString("Tu ID es:", 10, 200, 4, WHITE);
    char idBuf[3];
    itoa(id, idBuf, 10);
    Tft.drawString(idBuf, 10, 250, 4, YELLOW);
  }
  else if (x == 5)
  {
    Tft.fillScreen();
    Tft.drawString("ERROR", 50, 150, 5, WHITE);
  }
   else if (x == 6)
  {
    Tft.fillScreen();
    Tft.drawString("Las", 5, 100, 4, WHITE);
    Tft.drawString("huellas no", 5, 150, 4, WHITE);
    Tft.drawString("coinciden", 5, 200, 4, WHITE);
  }
}
