#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10, 1, 52, 45);
EthernetServer server(80);
EthernetClient client;
String readString = "" ;

// LAB8 - 讀取光敏電阻 (v2)

int photocellPin = A0; // 光敏電阻 (photocell) 接在 anallog pin 2
int photocellVal = 0; // photocell variable
int minLight = 40;   // 最小光線門檻值
int ledPin = 13;
int ledState = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(111800);
}

void loop() {
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
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply

        // 讀取光敏電阻並輸出到 Serial Port
        photocellVal = analogRead(photocellPin);
        Serial.println(photocellVal);

        // 光線不足時打開 LED
        if (photocellVal < minLight && ledState == 0) {
          digitalWrite(ledPin, HIGH); // turn on LED
          ledState = 1;
        }

        // 光線充足時關掉 LED
        if (photocellVal > minLight && ledState == 1) {
          digitalWrite(ledPin, LOW); // turn off LED
          ledState = 0;
        }

        delay(1000);
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
        
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

