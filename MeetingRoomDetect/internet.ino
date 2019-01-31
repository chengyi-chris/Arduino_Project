#include <SPI.h>
#include <Ethernet.h>
byte mac[]={0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};//指定網路卡 MAC 位址 (隨便設)
EthernetClient client;// 建立乙太網物件 EthernetClient
void setup() {//設定初始值
  Serial.begin(115200);//初始化序列埠
  //啟用 Ethernet 連線，預設會以 DHCP 取得 IP 位址
  if (Ethernet.begin(mac)==0) {
    Serial.println("Cannot get IP Address!");   
    for(;;);// 無法取得 IP 位址，不做任何事情
    }
  Serial.print("IP Address ");//輸出 IP 位址
  Serial.println(Ethernet.localIP());
  }
void loop() { } //無限迴圈

