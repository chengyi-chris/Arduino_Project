// 從腳位A0讀取光敏電阻的值。
// 以腳位2控制LED。


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:

#include <SPI.h>
#include <Ethernet.h>
EthernetServer server(80);
EthernetClient client = server.available();
String   HTTP_req = "";
void setup() {
  byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xF9
  };
  IPAddress ip(10, 1, 52, 99);
  IPAddress myDns(8, 8, 8, 8);
  IPAddress gateway(10, 1, 52, 254);
  IPAddress subnet(255, 255, 255, 0);
  // Initialize the Ethernet server library
  // with the IP address and port you want to use
  // (port 80 is default for HTTP):
  EthernetServer server(80);
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  // Motion Detector
}


void loop() {
  // 以analogRead()讀取光敏電阻的值，會回傳0~1023之間的值。
  int pr = analogRead(A0);
  // 並且把值輸出到序列埠，請用手遮蔽光敏電阻，看看變化。
  Serial.println(pr);
  // 若大於這個值，熄滅LED，若小於就點亮。
  // 請視需求修改pr_min。
 
  delay(1000);
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
        client.print(analogRead);
        client.println("<br />");
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
        client.print(analogRead);
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

  Serial.println("client disconnected");
}
}
}
