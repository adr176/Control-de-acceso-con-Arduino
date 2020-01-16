
#include <Wire.h>
#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment this line if using hardware serial
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;
byte x = 0;

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  // set the data rate for the sensor serial port
  finger.begin(57600);

  Serial.println("\n\nRegistro de huella");

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } 
  else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
}

// función que busca el primer id no asignado
uint8_t huecoLibre() {
  uint8_t cod = 1;
  bool enc = false;

  // Busca el primer hueco
  while (cod < 128 && !enc) {
    uint8_t p = finger.loadModel(cod);
    if (p != FINGERPRINT_OK) enc = true;
    else cod ++;
  }
  return cod;
}

void loop() {
  while (!getFingerprintEnroll());
  delay(500);
}

uint8_t getFingerprintEnroll() {

  id = huecoLibre();
  int p = -1;
  Serial.println("Coloca el dedo en el sensor");


  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        Wire.beginTransmission(1); // start transmitting
        x = 1;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
        delay(2000);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
       Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
       Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
       Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
    default:
      Serial.println("Unknown error");
       Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
  }

  Serial.println("Remove finger");
  Wire.beginTransmission(1);
  x = 2;
  Wire.write(x);
  Wire.endTransmission();    // stop transmitting
  delay (1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  Serial.println("Place same finger again");
  Wire.beginTransmission(1);
  x = 3;
  Wire.write(x);
  Wire.endTransmission();    // stop transmitting
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        delay(2000);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
         Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
         Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
        break;
      default:
        Serial.println("Unknown error");
         Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
       Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
       Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
       Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
       Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
    default:
      Serial.println("Unknown error");
       Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
     Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
     Wire.beginTransmission(1); // start transmitting
        x = 6;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
    return p;
  } else {
    Serial.println("Unknown error");
     Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    Wire.beginTransmission(1);
    x = 4;
    Wire.write(x);
    Wire.write(id);
    Wire.endTransmission();
    delay (5000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
     Wire.beginTransmission(1); 
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
     Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();  
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
     Wire.beginTransmission(1); 
        x = 5;
        Wire.write(x);
        Wire.endTransmission();   
    return p;
  } else {
    Serial.println("Unknown error");
     Wire.beginTransmission(1); // start transmitting
        x = 5;
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting
    return p;
  }
}
