#include <WiFi.h>
#include <WebServer.h>

#include <ArduinoOTA.h>


const char* ssid = "yourWIFI";
const char* password = "YourPassword";
WebServer server(80);

byte incomingByte;
byte ArrayBatt[100];
byte Arraycell[100];
int i = 0;
float currentpower; 
float currentvolt;
int SOC;
int Strom;
float Batt_A;

float B_Cell1_V;
float B_Cell2_V;
float B_Cell3_V;
float B_Cell4_V;
float B_Cell5_V;
float B_Cell6_V;
float B_Cell7_V;
float B_Cell8_V;
float B_Cell9_V;
float B_Cell10_V;
float B_Cell11_V;
float B_Cell12_V;
float B_Cell13_V;
float B_Cell14_V;
float B_Cell15_V;
float B_Cell16_V;


unsigned long previousMillis_WIFI = 0;
unsigned long interval_WIFI = 30000;
unsigned long previousMillis_Batt = 0;
unsigned long interval_Batt = 5000;
unsigned long previousMillis_Cell = 0;
unsigned long interval_Cell = 10000;



void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("ESP32 BMS-decoder ");
  WiFi.mode(WIFI_STA);
  WiFi.hostname("BMS_Controller");
  WiFi.begin(ssid, password);
 
if (WiFi.waitForConnectResult() != WL_CONNECTED) {
Serial.printf("WiFi Failed!\n");
 return;
} 

ArduinoOTA.setHostname("BMS");
Serial.print("IP Address: ");
Serial.println(WiFi.localIP());


server.on("/", handleRoot);     


ArduinoOTA.begin();
ArduinoOTA.setPassword((const char *)"password");
Serial.println("OTA1 server started"); 
server.begin();                 
Serial.println("HTTP server started");  
}





void handleRoot() {
BattPara();
CellVolt();
SOC = ArrayBatt[22];
long rssi = WiFi.RSSI();
String rx = String(rssi);

 String buff = "<!DOCTYPE html><html lang='de'>";
 buff += "<head>";
 buff += "<meta http-equiv='refresh' content='3'>";
 buff += "<title>BMS Werte</title>";
  
 buff += "</head> ";
 buff += "<body>";

 buff += "<font size='5'><u><b>BMS Controller</b></u></font><br><br>";

 buff += "SOC: ";
 buff += SOC;
 buff += "  %<br><br>";
 
 buff += "Strom: ";
 buff += String(Batt_A,1);
 buff += "  A<br><br>"; 
 
 buff += "Spannung: ";
 buff += currentvolt;
 buff += "  V<br><br><br>"; 

 buff += "<font size='3'><u><b>Zellspannung</b></u></font><br><br>";

 buff += "Cell_1: ";
 buff += String(B_Cell1_V,3);
 buff += "  V<br><br>";
 
 buff += "Cell_2: ";
 buff += String(B_Cell2_V,3);
 buff += "  V<br><br>";

 buff += "Cell_3: ";
 buff += String(B_Cell3_V,3);
 buff += "  V<br><br>";

 buff += "Cell_4: ";
 buff += String(B_Cell4_V,3);
 buff += "  V<br><br>";

 buff += "Cell_5: ";
 buff += String(B_Cell5_V,3);
 buff += "  V<br><br>";

 buff += "Cell_6: ";
 buff += String(B_Cell6_V,3);
 buff += "  V<br><br>";

 buff += "Cell_7: ";
 buff += String(B_Cell7_V,3);
 buff += "  V<br><br>";

 buff += "Cell_8: ";
 buff += String(B_Cell8_V,3);
 buff += "  V<br><br>";

 buff += "Cell_9: ";
 buff += String(B_Cell9_V,3);
 buff += "  V<br><br>";

 buff += "Cell_10: ";
 buff += String(B_Cell10_V,3);
 buff += "  V<br><br>";

 buff += "Cell_11: ";
 buff += String(B_Cell11_V,3);
 buff += "  V<br><br>";

 buff += "Cell_12: ";
 buff += String(B_Cell12_V,3);
 buff += "  V<br><br>";

 buff += "Cell_13: ";
 buff += String(B_Cell13_V,3);
 buff += "  V<br><br>";

 buff += "Cell_14: ";
 buff += String(B_Cell14_V,3);
 buff += "  V<br><br>";

 buff += "Cell_15: ";
 buff += String(B_Cell15_V,3);
 buff += "  V<br><br>";

 buff += "Cell_16: ";
 buff += String(B_Cell16_V,3);
 buff += "  V<br><br>";

 buff += "Temp_1: ";
 buff += (Arraycell[35] << 8 | Arraycell[36]);
 buff += " C<br><br>";

 buff += "Temp_2: ";
 buff += (Arraycell[37] << 8 | Arraycell[38]);
 buff += " C<br><br>";

 buff += "Temp_3: ";
 buff += (Arraycell[39] << 8 | Arraycell[40]);
 buff += " C<br><br>";

 buff += "Temp_4: ";
 buff += (Arraycell[41] << 8 | Arraycell[42]);
 buff += " C<br><br>"; 
 
 buff += "</font></body></html>";
  
  server.send(200, "text/html", buff);

}




//________________________________________________LOOP_______________________________________________________________________________________
void loop() {

  server.handleClient();
  ArduinoOTA.handle();





unsigned long currentMillis_WIFI = millis();
// if WiFi is down, try reconnecting
if ((WiFi.status() != WL_CONNECTED) && (currentMillis_WIFI - previousMillis_WIFI >=interval_WIFI)) {
  Serial.print(millis());
  Serial.println("Reconnecting to WiFi...");
  WiFi.disconnect();
  WiFi.reconnect();
  previousMillis_WIFI = currentMillis_WIFI;
}





}



void BattPara() {

byte message[8] = {0x01, 0x03, 0x13, 0x02, 0x00, 0x0A, 0x60, 0x89};
Serial2.write(message, 8);
delay(200);


i = 0;

  while (Serial2.available())
  {
    incomingByte = Serial2.read();
    ArrayBatt[i] = incomingByte;
    i++;
  }

    Serial.print("SOC: ");
    Serial.print(ArrayBatt[22],DEC);
    Serial.println(" %");

    Serial.print("Temp: ");
    Serial.print(ArrayBatt[20],DEC);
    Serial.println(" C");


Batt_A = (ArrayBatt[13] << 8 | ArrayBatt[14]); 
Serial.println(Batt_A);
if (Batt_A > 60000) {
Batt_A = Batt_A - 65535;
}

Batt_A = (Batt_A * 0.1);
    Serial.print("Strom: ");
    Serial.print(Batt_A);
    Serial.println(" A");

currentvolt = (ArrayBatt[11] << 8 | ArrayBatt[12]);
currentvolt = (currentvolt * 0.01);
    Serial.print("Spannung ");
    Serial.print(currentvolt);
    Serial.println(" V");

    
}


void CellVolt() {

byte message1[8] = {0x01, 0x03, 0x13, 0x2A, 0x00, 0x18, 0x60, 0x8C};
Serial2.write(message1, 8);
delay(200);
i = 0;

  while (Serial2.available())
  {
    incomingByte = Serial2.read();
    Arraycell[i] = incomingByte;
    i++;
  }


    B_Cell1_V = (Arraycell[3] << 8 | Arraycell[4]);
    B_Cell1_V = (B_Cell1_V * 0.001);
    Serial.print("Cell 1 =  ");
    Serial.print(B_Cell1_V,3);
    Serial.println(" V");

  


    B_Cell2_V = (Arraycell[5] << 8 | Arraycell[6]);
    B_Cell2_V = (B_Cell2_V * 0.001);
    Serial.print("Cell 2 =  ");
    Serial.print(B_Cell2_V,3);
    Serial.println(" V");

    B_Cell3_V = (Arraycell[7] << 8 | Arraycell[8]);
    B_Cell3_V = (B_Cell3_V * 0.001);
    Serial.print("Cell 3 =  ");
    Serial.print(B_Cell3_V,3);
    Serial.println(" V");

    B_Cell4_V = (Arraycell[9] << 8 | Arraycell[10]);
    B_Cell4_V = (B_Cell4_V * 0.001);
    Serial.print("Cell 4 =  ");
    Serial.print(B_Cell4_V,3);
    Serial.println(" V");

    B_Cell5_V = (Arraycell[11] << 8 | Arraycell[12]);
    B_Cell5_V = (B_Cell5_V * 0.001);
    Serial.print("Cell 5 =  ");
    Serial.print(B_Cell5_V,3);
    Serial.println(" V");

    B_Cell6_V = (Arraycell[13] << 8 | Arraycell[14]);
    B_Cell6_V = (B_Cell6_V * 0.001);
    Serial.print("Cell 6 =  ");
    Serial.print(B_Cell6_V,3);
    Serial.println(" V");

    B_Cell7_V = (Arraycell[15] << 8 | Arraycell[16]);
    B_Cell7_V = (B_Cell7_V * 0.001);
    Serial.print("Cell 7 =  ");
    Serial.print(B_Cell7_V,3);
    Serial.println(" V");

    B_Cell8_V = (Arraycell[17] << 8 | Arraycell[18]);
    B_Cell8_V = (B_Cell8_V * 0.001);
    Serial.print("Cell 8 =  ");
    Serial.print(B_Cell8_V,3);
    Serial.println(" V");

    B_Cell9_V = (Arraycell[19] << 8 | Arraycell[20]);
    B_Cell9_V = (B_Cell9_V * 0.001);
    Serial.print("Cell 9 =  ");
    Serial.print(B_Cell9_V,3);
    Serial.println(" V");

    B_Cell10_V = (Arraycell[21] << 8 | Arraycell[22]);
    B_Cell10_V = (B_Cell10_V * 0.001);
    Serial.print("Cell 10 = ");
    Serial.print(B_Cell10_V,3);
    Serial.println(" V");

    B_Cell11_V = (Arraycell[23] << 8 | Arraycell[24]);
    B_Cell11_V = (B_Cell11_V * 0.001);
    Serial.print("Cell 11 = ");
    Serial.print(B_Cell11_V,3);
    Serial.println(" V");

    B_Cell12_V = (Arraycell[25] << 8 | Arraycell[26]);
    B_Cell12_V = (B_Cell12_V * 0.001);
    Serial.print("Cell 12 = ");
    Serial.print(B_Cell12_V,3);
    Serial.println(" V");

    B_Cell13_V = (Arraycell[27] << 8 | Arraycell[28]);
    B_Cell13_V = (B_Cell13_V * 0.001);
    Serial.print("Cell 13 = ");
    Serial.print(B_Cell13_V,3);
    Serial.println(" V");

    B_Cell14_V = (Arraycell[29] << 8 | Arraycell[30]);
    B_Cell14_V = (B_Cell14_V * 0.001);
    Serial.print("Cell 14 = ");
    Serial.print(B_Cell14_V,3);
    Serial.println(" V");

    B_Cell15_V = (Arraycell[31] << 8 | Arraycell[32]);
    B_Cell15_V = (B_Cell15_V * 0.001);
    Serial.print("Cell 15 = ");
    Serial.print(B_Cell15_V,3);
    Serial.println(" V");

    B_Cell16_V = (Arraycell[33] << 8 | Arraycell[34]);
    B_Cell16_V = (B_Cell16_V * 0.001);
    Serial.print("Cell 16 = ");
    Serial.print(B_Cell16_V,3);
    Serial.println(" V");
  

     int B_Temp1 = (Arraycell[35] << 8 | Arraycell[36]);
    Serial.print("Temp 1 = ");
    Serial.print(B_Temp1);
    Serial.println(" C");

     int B_Temp2 = (Arraycell[37] << 8 | Arraycell[38]);
    Serial.print("Temp 2 = ");
    Serial.print(B_Temp2);
    Serial.println(" C");

     int B_Temp3 = (Arraycell[39] << 8 | Arraycell[40]);
    Serial.print("Temp 3 = ");
    Serial.print(B_Temp3);
    Serial.println(" C");

     int B_Temp4 = (Arraycell[41] << 8 | Arraycell[42]);
    Serial.print("Temp 4 = ");
    Serial.print(B_Temp4);
    Serial.println(" C");

  
}
