// Yeni sensor okuma  nano'ya yuklenecek.

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// TX icin kutuphaneler
#include <RH_ASK.h>
#include <SPI.h>

// Gonderilecek stringin tanimlanmasi
String str_out,str_x,str_y;
float x,y;

// RX'te kay sifreleme objelerin tanimlanmasini sagliyor
RH_ASK rf_driver;

// Sensorun okunmasi icin struct yapisindan aktif hale getiriyoruz.
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);



void setup(void)
{


  // rf-5v'in TX modulu aktif hale getiriliyor.
  rf_driver.init();
  // Test et burasi gerekli olmayabilir.
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);

  // ADXL345 modulunun aktif hale getirilmesi
  if(!accel.begin())
  {
    /* Hata olursa bastirilacak yazi*/
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  // Standartlar asagida belirtilmis en hassas olanini sectim.
  // Nedeni modulumuz ne kadar hassas olcerse o kadara iyi sonuclar gelir ve ayarlamamiz kolay olur.
  /* Set the range to whatever is appropriate for your project */
  //accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  accel.setRange(ADXL345_RANGE_2_G);
}

void loop(void)
{
  /* Delay degeri duzgun bicimde ayarlanmalidir. RX-TX'in kac saniye araligiyla
    GOnderim yapmsini ifade ediyor.

  */
  delay(200);

  /* Sensor icin struct yapisindan secilmekte ve event degiskenine aktarilmaktadir.*/
  sensors_event_t event;
  accel.getEvent(&event);
  x = event.acceleration.x;
  y = event.acceleration.y;
  str_x = String(x);
  str_y = String(y);
  // Gonderilecek degerin string olarak ayarlanmasini saglamakatadir.
  // Gonderilecek degerin diger tarafta parse edilmesi icin arada virgul kullandim.
  str_out = str_x + "," + str_y;


  static char *msg = str_out.c_str();
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();
  // String degerlerin paketlenip Gonderilmesini ifade ediyor.

  // ADXL345'ten okunan verilerin test amaciyla bastirilmaktadir.
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
}
