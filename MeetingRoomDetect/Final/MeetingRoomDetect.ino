/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi

 */

#include <SPI.h>
#include <Ethernet.h>
#include <MsTimer2.h>           //Timer interrupt function library
int pbIn = digitalPinToInterrupt(2);                    // Define interrupt 0 that is digital pin 2
int ledOut = 13;                 // Define the indicator LED pin digital pin 13
int number = 0;                    //Interrupt times
float usage = 0;                    // Usage Indicator
volatile int state = LOW;         // Defines the indicator LED state, the default is not bright
String HTTP_req; // stores the HTTP request

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xF9
};
IPAddress ip(10,1,161,233);
IPAddress myDns(8, 8, 8, 8);
IPAddress gateway(10,1,160,254);
IPAddress subnet(255, 255, 254, 0);
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  // Motion Detector
  pinMode(ledOut, OUTPUT);// 
  attachInterrupt(pbIn, stateChange, CHANGE); // Set the interrupt function, interrupt pin is digital pin D2, interrupt service function is stateChange (), when the D2 power change from high to low , the trigger interrupt.
  MsTimer2::set(1000, Handle); // Set the timer interrupt function, running once Handle() function per 1000ms 
  MsTimer2::start();//Start timer interrupt function
}


void loop() {
  if(state == HIGH)  //When a moving object is detected, the ledout is automatically closed after the light 2S, the next trigger can be carried out, and No need to reset. Convenient debugging.
    {
      delay(1000);
      state = LOW;
      digitalWrite(ledOut, state);    //turn off led
    }
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if ( HTTP_req.length() < 80)
          HTTP_req += c;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          client.print("Meeting Room State: ");
          client.print(usage);
          client.println("<br />");
          //client.println("<img src=\"http://systex:xetsys@10.1.51.134/image/jpeg.cgi\" alt=\"http://systex:xetsys@10.1.51.134/image/jpeg.cgi\"></img>");
          //client.println(" <iframe src=\"http://systex:xetsys@10.1.51.134/image/jpeg.cgi\" sandbox=\"allow-scripts\"></iframe> ");
          client.println("</html>");
          break;
        }
        else if (HTTP_req.indexOf("status_api") >= 0 ) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          //client.println("<!DOCTYPE HTML>");
          //client.println("<html>");
          // output the value of each analog input pin
          client.print(usage);
          //client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(2);
    // close the connection:
    client.stop();
    HTTP_req = "";
    Serial.println("client disconnected");
  }
}

 
void stateChange()  //Interrupt service function
{  
  number++;  //Interrupted once, the number +1

}

void Handle()   //Timer service function
{
  if (usage>2)
    {
      usage = 0.95 * usage + state;
      if (usage>10)
        {usage = 10;}
    }
  else
    {usage = 0.95 * usage + state;}
  if(number>1)  //If in the set of the interrupt time the number more than 1 times, then means have detect moving objects,This value can be adjusted according to the actual situation, which is equivalent to adjust the threshold of detection speed of moving objects.
       {
                   state = HIGH;            
                   digitalWrite(ledOut, state);    //light led
                   number=0;   //Cleare the number, so that it does not affect the next trigger
       }
        else
            number=0;   //If in the setting of the interrupt time, the number of the interrupt is not reached the threshold value, it is not detected the moving objects, Cleare the number.
}

