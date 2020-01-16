#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <stdint.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>
#include <Wire.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line: (El arduino uno wifi rev2 tiene hardware serial)
#define mySerial Serial1

// SSID
#define ssid "ATC3" // your network SSID (name)

// Contraseña
#define pass "INFLAB229" // your network password

// Dirección IP del servidor con la página PHP (ip del ordenador)
#define server "192.168.2.246"

uint8_t codHuella;
int contconexion = 0;
int status = WL_IDLE_STATUS;  // the Wifi radio's status

// Inicializar instancia de la libreria WiFiClient
WiFiClient client;

//Variables necesarias para leer de la base de datos
String codigo;        // Aquí se almacena la respuesta del servidor
String nombre;        // Aquí se almacena el nombre que recuperamos de MySQL
String apellido1;     // Aquí se almacena el primer apellido que recuperamos de MySQL
String apellido2;     // Aquí se almacena el segundo apellido
String pago;          // Aquí se almacena si el alumno a pagado o no
String descripcion;   // Aquí se almacena las alergias del alumno
String descripcionDiaria;  // Aquí se almacena la las alergias del día

String alergias[14];  // Aquí se almacena las alergias por separado
String alergiasDiarias[14]; // Aquí se almacena las alergias diarias por separado

boolean fin = false;

// para master and slave
int num = 0;
byte x[2];

void setup() {
  Tft.TFTinit();  // init TFT library
  Serial.begin(9600); // comentar para usar con batería extraible

  Serial.println("Bienvenido al comedor!!!");
  Tft.drawString("Bienvenido ", 0, 50, 4, WHITE);
  Tft.drawString("al", 80, 150, 4, WHITE);
  Tft.drawString("comedor!!", 10, 240, 4, WHITE);

  Wire.begin(1); // join i2c bus with address #1
  Wire.onReceive(receiveEvent); // register event

  // INICIO PARTE WIFI
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 2 seconds for connection:
    delay(2000);
  }
  // Conected
  Serial.print("You're connected to the network \n");
  // FIN PARTE WIFI

  muestraMensaje();
  codHuella = x[0];
}

int prevHuella = 0;

void loop()                     // run over and over again
{
  Tft.fillScreen();

  while ((codHuella == prevHuella)) {
    muestraMensaje();
    while (Wire.available() > 0) { // loop through all but the last
      delay(1000);
      x[num] = Wire.read();    // receive byte as an integer
      num++;
    }

    codHuella = x[0];

    if (x[1] == 1) { // huella no reconocida
      Tft.fillScreen();
      Tft.drawString("Alumno", 40, 50, 4, WHITE);
      Tft.drawString("no", 90, 150, 4, WHITE);
      Tft.drawString("registrado", 0, 240, 4, WHITE);
      delay (3000);
      Tft.fillScreen();
    }
    Tft.fillScreen();
  }

  prevHuella = codHuella;

  Serial.print("Tu ID es: ");
  Serial.println(codHuella);
  delay(50); //don't need to run this at full speed.
  int identificador = codHuella;
  httpRequest(identificador);
  //}
}

// Parte de comunicación

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (Wire.available() > 0) { // loop through all but the last
    x[num] = Wire.read();    // receive byte as an integer
    num++;
  }
  num = 0;
}
// Fin parte comunicación

void muestraMensaje() {
  Serial.println("Introduzca huella");
  Tft.fillScreen();
  Tft.drawString("Introduzca ", 0, 100, 4, WHITE);
  Tft.drawString("huella ", 40, 180, 4, WHITE);
  delay(1500);
}

// Con esta función hacemos la conexión con el servidor
int httpRequest(int identificador) {
  bool hayAlergiasDiarias = false;

  // Comprobar si hay conexión
  if (client.connect(server, 80)) {
    Serial.println("Conectado a la base de datos");
    // Enviar la petición HTTP
    //Dirección del archivo php dentro del servidor
    client.print("GET /comedor/select.php?id=");
    //Mandamos la variable junto a la línea de GET
    client.print(identificador);
    client.println(" HTTP/1.0");
    //IP del servidor
    client.print("Host: ");
    client.println (server); // dirección ip del ordenador
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
  }
  else {
    // Si no conseguimos conectarnos
    Serial.println("Conexión fallida");
    Serial.println("Desconectando");
    client.stop();
  }

  delay(500);
  //Comprobamos si tenemos respuesta del servidor y la
  //almacenamos en el string ----> codigo.
  while (client.available()) {
    char c = client.read();
    codigo += c;
    //Habilitamos la comprobación del código recibido
    fin = true;
  }
  //Si está habilitada la comprobación del código entramos en el IF
  if (fin)  {

    //Analizamos la longitud del código recibido
    int longitud = codigo.length();


    //Buscamos en que posición del string se encuentra nuestra variable
    int posicion1 = codigo.indexOf("Nombre:");
    int posicion2 = codigo.indexOf("Apellido1:");
    int posicion3 = codigo.indexOf("Apellido2:");
    int posicion4 = codigo.indexOf("Pago:");
    int posicion5 = codigo.indexOf("Alergias:");
    int posicion6 = codigo.indexOf("Alergias Diarias:");


    //Borramos lo que haya almacenado en los strings
    nombre = "";
    apellido1 = "";
    apellido2 = "";
    pago = "";
    descripcion = "";
    descripcionDiaria = "";

    //Analizamos el código obtenido y almacenamos el nombre en el string nombre
    for (int i = posicion1 + 8; i < longitud; i++) {
      if (codigo[i] == ';') i = longitud;
      else nombre += codigo[i];
    }

    if (nombre == "") {
      Tft.drawString("Alumno no", 0, 30, 4, WHITE);// draw string: nombre, (0, 30), size: 3, color: White
      Tft.drawString("dado de", 0, 80, 4, WHITE);// draw string: apellido1, (0, 70), size: 3, color: White
      Tft.drawString("alta en", 0, 130, 4, WHITE);// draw string: apellido2, (0, 70), size: 3, color: White
      Tft.drawString("la BD", 0, 180, 4, WHITE);// draw string: pago, (0, 70), size: 3, color: White
      delay(3000);
    }
    else {
      //Analizamos el código obtenido y almacenamos el apellido1 en el string apellido1
      for (int i = posicion2 + 11; i < longitud; i++) {
        if (codigo[i] == ';') i = longitud;
        else apellido1 += codigo[i];
      }

      //Analizamos el código obtenido y almacenamos el apellido2 en el string apellido2
      for (int i = posicion3 + 11; i < longitud; i++) {
        if (codigo[i] == ';') i = longitud;
        else apellido2 += codigo[i];
      }

      //Analizamos el código obtenido y almacenamos el pago en el string pago
      for (int i = posicion4; i < longitud; i++) {
        if (codigo[i] == ';') i = longitud;
        else pago += codigo[i];
      }
      //Analizamos el código obtenido y almacenamos la descripcion de las alergias en el string descripcion
      for (int i = posicion5 + 10; i < longitud; i++) {
        if (codigo[i] == ';') i = longitud;
        else descripcion += codigo[i];
      }

      //Analizamos el código obtenido y almacenamos la descripcion de las alergias diarias en el string descripcionDiaria
      for (int i = posicion6 + 18; i < longitud; i++) {
        if (codigo[i] == ';') i = longitud;
        else descripcionDiaria += codigo[i];
        if (longitud == 0)
          hayAlergiasDiarias = false;
        else
          hayAlergiasDiarias = true;

      }

      //Deshabilitamos el análisis del código
      fin = false;

      //Imprimir los datos obtenidos
      Serial.println("Nombre: " + nombre);
      Serial.println("Apellido 1: " + apellido1);
      Serial.println("Apellido 2: " + apellido2);
      Serial.println("Pago: " + pago);
      Serial.println("Alergias: " + descripcion);

      int iCount = StringSplit(descripcion, ' ', alergias, 14);
      int iCount2 = StringSplit(descripcionDiaria, ' ', alergiasDiarias, 14);
      // print the extracted paramters
      for (int i = 0; i < iCount; i++) {
        Serial.print(alergias[i]);
        Serial.println(" ");
      }
      Serial.println("");

      // Chars para mostrar por la pantalla TFT
      char nombreBuf[20];
      char apellido1Buf[40];
      char apellido2Buf[40];
      char pagoBuf[20];
      char alergiaBuf[20];
      char alergiaDBuf[20];

      // Convertimos los strings en arrays de char
      nombre.toCharArray(nombreBuf, 20);
      apellido1.toCharArray(apellido1Buf, 40);
      apellido2.toCharArray(apellido2Buf, 40);
      pago.toCharArray(pagoBuf, 40);

      Tft.drawString(nombreBuf, 0, 30, 4, WHITE);// draw string: nombre, (0, 30), size: 3, color: White
      Tft.drawString(apellido1Buf, 0, 80, 4, WHITE);// draw string: apellido1, (0, 70), size: 3, color: White
      Tft.drawString(apellido2Buf, 0, 130, 4, WHITE);// draw string: apellido2, (0, 70), size: 3, color: White
      Tft.drawString(pagoBuf, 0, 180, 4, WHITE);// draw string: pago, (0, 70), size: 3, color: White
      delay(5000);

      Tft.fillScreen();

      if (pago == "Pago: SI") {

        Tft.drawString("Alergias: ", 0, 0, 3, WHITE);// draw string: Alergia:, (0, 70), size: 3, color: White

        // Si no caben las alergias en la pantalla, se ponen las restantes en una nueva pantalla
        if (iCount <= 7) {
          for (int i = 0; i < iCount; i++) {
            alergias[i].toCharArray(alergiaBuf, 20);
            Tft.drawString(alergiaBuf, 0, 40 * (i + 1), 3, WHITE); // draw string: alergias, (0, 70), size: 3, color: White
          }
        }
        else {
          for (int i = 0; i < 6; i++) {
            alergias[i].toCharArray(alergiaBuf, 20);
            Tft.drawString(alergiaBuf, 0, 40 * (i + 1), 3, WHITE); // draw string: alergias, (0, 70), size: 3, color: White
          }
          Tft.drawString("...", 0, 280, 3, WHITE);
          delay (5000);
          Tft.fillScreen();
          Tft.drawString("Alergias: ", 0, 0, 3, WHITE);// draw string: Alergia:, (0, 70), size: 3, color: White
          for (int i = 6; i < iCount; i++) {
            alergias[i].toCharArray(alergiaBuf, 20);
            Tft.drawString(alergiaBuf, 0, 40 * (i - 6 + 1), 3, WHITE); // draw string: alergias, (0, 70), size: 3, color: White
          }
        }
        delay (5000);
        Tft.fillScreen();

        // Si el alumno no tiene alergias o no es alérgico al menú del día
        if (descripcion == "NINGUNA" || !hayAlergiasDiarias) {
          Tft.drawString("MENU ", 50, 100, 5, GREEN);// draw string: Menu:, (0, 40), size: 4, color: Green
          Tft.drawString("NORMAL ", 35, 180, 5, GREEN);// draw string: normal:, (0, 100), size: 4, color: Green
        }
        else {
          Tft.drawString("Menu ", 0, 0, 3, WHITE);// draw string: Menu:, (0, 0), size: 3, color: White
          Tft.drawString("especial sin:", 0, 40, 3, WHITE);// draw string: normal:, (0, 40), size: 3, color: White
          for (int i = 0; i < iCount2; i++) {
            alergiasDiarias[i].toCharArray(alergiaDBuf, 20);
            Tft.drawString(alergiaDBuf, 0, 40 * (i + 2), 3, WHITE); // draw string: alergias, (0, 70), size: 3, color: White
          }
        }
      }
    }
    delay(3000);
    //Cerrar conexión
    client.stop();
  }
  //Borrar código y salir de la función
  codigo = "";
  return 1;
}

// Esta función separa el string de alergias
int StringSplit(String sInput, char cDelim, String sParams[], int iMaxParams)
{
  int iParamCount = 0;
  int iPosDelim, iPosStart = 0;

  do {
    // Searching the delimiter using indexOf()
    iPosDelim = sInput.indexOf(cDelim, iPosStart);
    if (iPosDelim > (iPosStart + 1)) {
      // Adding a new parameter using substring()
      sParams[iParamCount] = sInput.substring(iPosStart, iPosDelim);
      iParamCount++;
      // Checking the number of parameters
      if (iParamCount >= iMaxParams) {
        return (iParamCount);
      }
      iPosStart = iPosDelim + 1;
    }
  } while (iPosDelim >= 0);
  if (iParamCount < iMaxParams) {
    // Adding the last parameter as the end of the line
    sParams[iParamCount] = sInput.substring(iPosStart);
    iParamCount++;
  }
  return (iParamCount);
}
