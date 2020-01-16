import processing.serial.*;
Serial mySerial;
PrintWriter output;
void setup() {
   mySerial = new Serial( this, Serial.list()[1], 9600 );
   output = createWriter( "backup.txt" );
}
void draw() {
    if (mySerial.available() > 0 ) {
         String value = mySerial.readString();
         if ( value != null ) {
              output.println( value );
         }
    }
}

void keyPressed() {
    output.flush();  // Writes the remaining data to the file
    output.close();  // Finishes the file
    exit();  // Stops the program
}
//https://www.researchgate.net/post/How_can_I_save_data_directly_in_the_pc_using_an_Arduino_UNO
