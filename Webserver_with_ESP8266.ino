///////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Creation of a webserver with ESP8266.                                                                //
//  The communication of the ESP8266 with the Webserver is through WiFi communication.                   //
//                                                                                                       //
//  Generate with the help of:                                                                           //
//  https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html                             //
//  https://circuits4you.com/2018/02/05/esp8266-arduino-wifi-web-server-led-on-off-control/              //
//                                                                                                       //
//  Adaptation of José Huertas Pedroche  &  Amanda Besen Borges                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>  //Include the WebServer library

String readStrings;
char c;
char readchars;
char buf[255];
char buf2[255];
unsigned int len;
unsigned int len2;
 
//  HTML webpage contents in program memory //

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<center>
<h1>WiFi Receive message</h1>
</center>
</head>
<body>
<form action="/login" method="POST">
<input type="text" name="username" placeholder="Username"></br>    //``username´´ is the name of the written word
<input type="submit" value="Login">
</form>
<p>Click to see <a href="msg" target="myIframe">RESPONSE</a></p>
<p>Received message:<iframe name="myIframe" width="200" height="25" frameBorder="0"></p><br>  //``myIframe´´ is the receiving word
<hr>
</body>
</html>
)=====";

//  SSID and Password of your WiFi router //
char* ssid = "SEMI_PUBLIC";
char* password = "microelecpub";

//  Declare a global object variable from the ESP8266WebServer class  //
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
 
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================

void handleRoot() {                         // When URI is requested, send a web page
 Serial.println("You called root page");
 String s = MAIN_page;                    //Read HTML contents
 server.send(200, "text/html", s);        //Send web page
}
 
void handleLMsg() { 
 server.send(200, "text/html", buf2);     //Send ADC value only to client ajax request. buf2 is a char variable
}

void handleInComeMSG() {                         // If a POST request is made to URI /login
  Serial.println(server.arg("username"));
  String help = server.arg("username");
  len2 = help.length();
  help.toCharArray(buf2, len2+1);
  Serial.print(buf2);
  server.send(200, "text/html", MAIN_page);

}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  
  WiFi.begin(ssid, password);   //Connect to your WiFi router
  Serial.println("");
 
// Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());            //IP address assigned to your ESP
 
  server.on("/",HTTP_GET, handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/login", HTTP_POST, handleInComeMSG);
  server.on("/msg", handleLMsg);

 
  server.begin();                           //Start server
  Serial.println("HTTP server started");
  Serial.println("Beggining of the mensages");
}

//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();   // Listen for HTTP requests from clients
  
  readStrings="";
    while (Serial.available()) {
     delay(10);  
     if (Serial.available() >0) {
       char c = Serial.read();
       readStrings += c;
     }
    }
    len = readStrings.length();
    if (len >0) {
     readStrings.toCharArray(buf, len+1);
     Serial.println(readStrings);
     Serial.print(buf2);
     server.send(200, "text/html", MAIN_page); 
    }
}
