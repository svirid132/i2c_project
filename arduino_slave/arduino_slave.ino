#include <Wire.h>               // подключаем библиотеку
#include <SPI.h>
#include <SD.h>

File myFile;

const int BUFFER_SIZE = 50;
int rlen = 0;
int reg = -1;
int currentIndex = 0;
char buf[BUFFER_SIZE];

struct SimpleStruct {
	unsigned char control = 0; // для записи 0x02, для чтения 0x00 - управление
	unsigned char def = 0x00; // для записи, необходимо инициализировать 0xFF
	//0x03 для чтения, 8-битное значение напряжения аккумулятора с определённым коэффициентом
} structData;

void setup() {
  Serial.begin(9600);
  
  Wire.begin(0x54);                // запускаем шину на адресе 0x54
  Wire.onRequest(requestEvent); // назначаем функцию отправки данных (чтение)
  Wire.onReceive(dataRcv);    // register an event handler for received data (запись)
  
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  
  myFile = SD.open("205F97~1/RO_.RAW", O_READ);
}

void loop() {
  
}

void sendData() {
	if (rlen == currentIndex && myFile.available() > 0) {
	    rlen = myFile.readBytes(buf, BUFFER_SIZE);
      currentIndex = 0;
	}
	
  if (rlen == 0 ) {
    	Wire.write(0);
  }

  Serial.println((unsigned char)(buf[currentIndex]) + 0);

	Wire.write(buf[currentIndex]);
	currentIndex += 1;
}

void requestEvent() {     // фенкция, вызывается автоматически при получении запроса от мастера (get)

    Serial.println("requestEvent. ");

    Serial.print("reg: ");
    Serial.print(reg + 0);
    switch (reg) {
      case 0x00:
        Wire.write(structData.control);
      break;
      case 0x01:
        Wire.write(structData.def);
      break;
      case 0x02:
        Wire.write(structData.control);
      break;
      case 0x03:
          Serial.println("reg: 0x03");
          // Wire.write(0x05);
         sendData();
      break;
    }
}

void dataRcv(int howMany) { // (set)

  reg = Wire.read();

    if (howMany < 2) {
      return;
    }

    Serial.println("dataRcv. ");

    char val = Wire.read();

    switch (reg) {
      case 0x01:
        structData.def = (unsigned char)val;
      break;
      case 0x02:
        structData.control = (unsigned char)val;
      break;
    }

}
