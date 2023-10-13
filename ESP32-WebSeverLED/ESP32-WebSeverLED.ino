#include <WiFi.h>

const char* WIFI_NAME= "LAB TKJ 01"; 
const char* WIFI_PASSWORD = "40304839@SMKN1";

WiFiServer server(80);

String header;

String STATUS_LED_SATU = "off";
String STATUS_LED_DUA = "off";
String STATUS_LED_TIGA = "off";


const int GPIO_PIN_NUMBER_13 = 13;
const int GPIO_PIN_NUMBER_12 = 12;
const int GPIO_PIN_NUMBER_15 = 15;


void setup() {
Serial.begin(115200);
pinMode(GPIO_PIN_NUMBER_13, OUTPUT);
pinMode(GPIO_PIN_NUMBER_12, OUTPUT);
pinMode(GPIO_PIN_NUMBER_15, OUTPUT);

digitalWrite(GPIO_PIN_NUMBER_13, LOW);
digitalWrite(GPIO_PIN_NUMBER_12, LOW);
digitalWrite(GPIO_PIN_NUMBER_15, LOW);


Serial.print("Connecting to ");
Serial.println(WIFI_NAME);
WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println("Trying to connect to Wifi Network");
}
Serial.println("");
Serial.println("Successfully connected to WiFi network");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
}

void loop(){
WiFiClient client = server.available(); 
if (client) { 
Serial.println("New Client is requesting web page"); 
String current_data_line = ""; 
while (client.connected()) { 
if (client.available()) { 
char new_byte = client.read(); 
Serial.write(new_byte); 
header += new_byte;
if (new_byte == '\n') { 
         
if (current_data_line.length() == 0) 
{
            
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();
            
if (header.indexOf("LED0=ON") != -1) 
{
Serial.println("GPIO13 LED Nyala");
STATUS_LED_SATU = "on";
digitalWrite(GPIO_PIN_NUMBER_13, HIGH);
} 
if (header.indexOf("LED0=OFF") != -1) 
{
Serial.println("GPIO13 LED Padam");
STATUS_LED_SATU = "off";
digitalWrite(GPIO_PIN_NUMBER_13, LOW);
} 

if (header.indexOf("LED1=ON") != -1)
{
Serial.println("GPIO12 LED Nyala");
STATUS_LED_DUA = "on";
digitalWrite(GPIO_PIN_NUMBER_12, HIGH);
}
if (header.indexOf("LED1=OFF") != -1) 
{
Serial.println("GPIO12 LED Padam");
STATUS_LED_DUA = "off";
digitalWrite(GPIO_PIN_NUMBER_12, LOW);
}

// if (header.indexOf("LED2=ON") != -1) 
// {
// Serial.println("GPIO15 LED Nyala");
// STATUS_LED_TIGA = "on";
// digitalWrite(GPIO_PIN_NUMBER_15, HIGH);
// }
// if(header.indexOf("LED2=OFF") != -1) {
// Serial.println("GPIO15 LED Padam");
// STATUS_LED_TIGA = "off";
// digitalWrite(GPIO_PIN_NUMBER_15, LOW);
// }
            
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
client.println(".buttonON { background-color: #4CAF50; border: 2px solid #4CAF50;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
client.println(".buttonOFF { background-color: #CD6155; border: 2px solid #CD6155;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}"); 
// Web Page Heading
client.println("</style></head>");
client.println("<body><center><h1>ESP32 Web server LED</h1></center>");
client.println("<center><h3>Tekan tombol on untuk menyalakan led dan tombol off untuk mematikan led</h3></center>");
client.println("<form><center>");
//LED 1
client.println("<p> Status LED 1 : " + STATUS_LED_SATU + "</p>");
client.println("<center> <button class=\"buttonON\" name=\"LED0\" value=\"ON\" type=\"submit\">LED0 ON</button>") ;
client.println("<button class=\"buttonOFF\" name=\"LED0\" value=\"OFF\" type=\"submit\">LED0 OFF</button><br><br>");
//LED 2
client.println("<p> Status LED 2 : " + STATUS_LED_DUA + "</p>");
client.println("<center> <button class=\"buttonON\" name=\"LED1\" value=\"ON\" type=\"submit\">LED1 ON</button>") ;
client.println("<button class=\"buttonOFF\" name=\"LED1\" value=\"OFF\" type=\"submit\">LED1 OFF</button><br><br>");
client.println("</center></form></body></html>");
client.println();
break;
} 
else 
{ 
current_data_line = "";
}
} 
else if (new_byte != '\r') 
{ 
current_data_line += new_byte; 
}
}
}
// Clear the header variable
header = "";
// Close the connection
client.stop();
Serial.println("Client disconnected.");
Serial.println("");
}
}