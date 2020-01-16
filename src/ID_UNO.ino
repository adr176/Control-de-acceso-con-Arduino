
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
byte x[2];

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }
  else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void loop() {

  Wire.beginTransmission(1);

  int z = getFingerprintIDez();

  Serial.println(z);

  if (z > 0) {
    x[0] = z;
  }

  else if (z == -2) {
    x[1] = 1; // señal de control de huella no reconocida
  }

  Serial.println(x[0]);

  for (int i = 0; i < 2; i++) {
    Wire.write (x[i]);
  }

  delay(50);

  x[1] = 0;
    for (int i = 0; i < 2; i++) {
    Wire.write (x[i]);
  }
  Wire.endTransmission();    // stop transmitting

  delay(500);
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  {
    if (p == FINGERPRINT_NOTFOUND) return -2;
    else
      return -1;
  }
  // found a match!

  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
