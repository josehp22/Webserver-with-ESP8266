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


char c;
char buf2[255];
unsigned int len2;
 
//  HTML webpage contents in program memory //

//constant with the information of the main page
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
<input type="text" name="username" placeholder="Username"></br>    
<input type="submit" value="Login">
</form>
<p>Click to see <a href="msg" target="myIframe">RESPONSE</a></p>    
<p>Received message:<iframe name="myIframe" width="200" height="25" frameBorder="0"></p><br>  //``myIframe´´ is the receiving word
<hr>
</body>
</html>
)=====";
//``username´´ is the name of the variable that contain the written word
//``myIframe´´ is the text box where it is printed the ``username´´
//``msg´´ the message received


//  SSID and Password of your WiFi router //
char* ssid = "SEMI_PUBLIC";
char* password = "microelecpub";

//  Declare a global object variable from the ESP8266WebServer class  //
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
 
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================

void handleRoot() {                         // When URI is requested, send a web page. Creation of the web page
 Serial.println("You called root page");
 String s = MAIN_page;                    //Read HTML contents
 server.send(200, "text/html", s);        //Send web page
}
 
void handleLMsg() { 
 server.send(200, "text/html", buf2);     //Send the value of buf2 is (char variable)
}

void handleInComeMSG() {                         // If a POST request is made to URI /login
  Serial.println(server.arg("username"));        // Print the value in the serial port
  String help = server.arg("username");          // Catch the value of ``username´´
  len2 = help.length();                          // Store the length of ``username´´
  help.toCharArray(buf2, len2+1);                // Convert the string to a char
  Serial.print(buf2);                 
  server.send(200, "text/html", MAIN_page);      // Reload de web page

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
  
 // GET AND POST
 
  server.on("/",HTTP_GET, handleRoot);               //Which routine display the page
  server.on("/login", HTTP_POST, handleInComeMSG);   //Take out the information of the web page
  server.on("/msg", handleLMsg);                     //Send the message to the web page

 
  server.begin();                           //Start server
  Serial.println("HTTP server started");
  Serial.println("Beggining of the mensages");
}

//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();   // Listen for HTTP requests from clients
}
