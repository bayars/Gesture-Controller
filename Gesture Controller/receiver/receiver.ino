#include <RH_ASK.h>
// rf433 baglannti kutuphaneleri.
#include <SPI.h>

String str_out;
String x,y;
// Deneme icin laserpin veya led
int laserpin = 13;

// Motor pinleri
int out1,out2,out3,ou4;

RH_ASK rf_driver;


void setup(){
  // RF 433 un ayarlanmasini saglar.
  rf_driver.init();
  // Serial monitorun aktif hale gelmesi.
  Serial.begin(9600);
  pinMode(laserpin,OUTPUT);
  pinMode(out1,OUTPUT);
  pinMode(out2,OUTPUT);
  pinMode(out3,OUTPUT);
  pinMode(out4,OUTPUT);
}

void loop(){
      // Mesajin boyutu icin buf degiskeninin tanimlanmasi.
    uint8_t buf[11];
    uint8_t buflen = sizeof(buf);
    // bufer boyutunun yeterli miktarda olup olmadigini kontrol etmektedir.
    if (rf_driver.recv(buf, &buflen))
    {
      // Bufer'i String degerinde almasini saglamak icin donusum yapilmakta.
      str_out = String((char*)buf);

      // Gelen mesaji virgul ile ayirmistim. Burada string mesajin ayrilmasini saglamaktadir.
      for (int i = 0; i < str_out.length(); i++) {
      if (str_out.substring(i, i+1) == ",") {
      x = str_out.substring(0, i);
      y = str_out.substring(i+1);
      break;
    }

    }
      // Donusturulecek degerlerin tekrardan 0'a esitlenmesi gerekiyordu. Nedeni cevirirken bozuk karakterin
      //  olusmasini engellemek icin yapildi.
      int xint = 0, yint = 0;

      // Ayrilmis degerlerin string'ten integera donusturulmasidir.
      xint = x.toInt();
      yint = y.toInt();

      if ((xint < 1 && xint > -1) && (yint < 1 && yint > -1 )) {
        digitalWrite(out1,LOW);
        digitalWrite(out2,LOW);         // STOP
        digitalWrite(out3,LOW);
        digitalWrite(out4,LOW);
      } else {
        if((xint > 1 && xint < 10 ) && (yint > 1 &&  yint < 10) ){
          digitalWrite(out1,HIGH);
          digitalWrite(out2,LOW);         // FORWARD
          digitalWrite(out3,HIGH);
          digitalWrite(out4,LOW);
        }
        if (( xint < 0  && xint > -10 ) && (yint < 0 && yint > -10 )) {
          digitalWrite(out1,LOW);
          digitalWrite(out2,HIGH);        // BACKWARD
          digitalWrite(out3,LOW);
          digitalWrite(out4,HIGH);
        }
        if ((xint < 0 && xint > 10) && ( yint > 0 && yint < 10 )) {
          digitalWrite(out1,HIGH);
          digitalWrite(out2,LOW);         // LEFT OR RIGHT
          digitalWrite(out3,LOW);
          digitalWrite(out4,LOW);
        }
        if ((xint < 0 && xint > 10) && ( yint > 0 && yint < 10 )) {
          digitalWrite(out1,LOW);
          digitalWrite(out2,LOW);         // LEFT OR RIGHT
          digitalWrite(out3,HIGH);
          digitalWrite(out4,LOW);
        }
      }

      // Degerlerin test edilmesi icin degerlerin ekrana bastirilmasidir.
      Serial.print("X:");
      Serial.print(x);
      Serial.print("  - Y: ");
      Serial.println(y);
}}
