#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10,1,52,58);
EthernetServer server(80);
EthernetClient client;
int Light=13;
int Sensor=A0;

void setup() {
  Serial.begin(115200);
  //初始化Ethernet通信
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(Light,OUTPUT);
  Serial.print( "Server is at " );
  Serial.println(Ethernet.localIP());
}

 
void loop() {
  String readString= "" ;
  // 監聽連入的客戶端
  client = server.available();
  if (client) {
    Serial.println( "new client" );
    boolean currentLineIsBlank = false ;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        readString += c;
        if (c == '\n' ) {
          Serial.println(readString);
          //檢查收到的信息中是否有”on”，有則開燈
          if (readString.indexOf( "?on" ) >0) {
            digitalWrite(Light, HIGH);
            Serial.println( "Led On" );
            break ;
          }
          //檢查收到的信息中是否有”off”，有則關燈
          if (readString.indexOf( "?off" ) >0) {
            digitalWrite(Light, LOW);
            Serial.println( "Led Off" );
            break ;
          }
          //檢查收到的信息中是否有”getBrightness”，有則讀取光敏模擬值，並返回給瀏覽器
          if (readString.indexOf( "?getBrightness" ) >0) {
            client.println(analogRead(Sensor));
            break ;
          }
          //發送HTML文本
          SendHTML();
          break ;
        }       
      }
    }
    delay(1);
    client.stop();
    Serial.println( "client disonnected" );
    readString= "" ;
  }
}
 
// 用於輸出HTML文本的函數
void SendHTML()
{
  client.println( "HTTP/1.1 200 OK" );
  client.println( "Content-Type: text/html" );
  client.println( "Connection: close" );
  client.println();
  client.println( "<!DOCTYPE HTML>" );
  client.println( "<html><head><meta charset=\"UTF-8\"><title>OpenJumper!Arduino Web Server</title><script type=\"text/javascript\">" );
  client.println( "function send2arduino(){var xmlhttp;if (window.XMLHttpRequest)xmlhttp=new XMLHttpRequest();else xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");element=document.getElementById(\"light \");if (element.innerHTML.match(\"Turn on\")){element.innerHTML=\"Turn off\"; xmlhttp.open(\"GET\",\"?on\",true );}else{ element.innerHTML=\"Turn on\";xmlhttp.open(\"GET\",\"?off\",true); }xmlhttp.send();}" );
  client.println( "function getBrightness(){var xmlhttp;if (window.XMLHttpRequest)xmlhttp=new XMLHttpRequest();else xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");xmlhttp.onreadystatechange=function(){if (xmlhttp.readyState==4 && xmlhttp.status==200)document.getElementById(\"brightness\").innerHTML=xmlhttp.responseText;};xmlhttp.open(\"GET\",\"?getBrightness\" ,true); xmlhttp.send();}window.setInterval(getBrightness,1000);</script>" );
  client.println( "</head><body><div align=\"center\"><h1>Arduino Web Server</h1><div>brightness:</div><div id=\"brightness\" >" ); 
  client.println(analogRead(Sensor));
  client.println( "</div><button id=\"light\" type=\"button\" onclick=\"send2arduino()\">Turn on</button><button type=\"button\" onclick=\"alert('OpenJumper Web Server')\">About</button></div></body></html>" );
}
