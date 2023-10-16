#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRac.h>
#include <IRutils.h>

const uint16_t kRecvPin = 14; // D4, ПИН подсоединения ИК приемника  
const uint32_t kBaudRate = 115200; // Скорость соединения с последовательным портом
const uint16_t kCaptureBufferSize = 1024; //  Размер буфера

// kTimeout - задержка в милисекундах для в получении нескольких пакетов сообщений
// ИК протоколы кондиционеров часто используют значение 20-40 мс

#if DECODE_AC
const uint8_t kTimeout = 50; // Задержка в милисекундах для получения нескольких пакетов сообщений
#else
const uint8_t kTimeout = 15;
#endif

const uint16_t kMinUnknownSize = 12; // Минимальный размер неизвестных случайных данных, позволяет снизить инфракрасный шум

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;

void setup() {
#if defined(ESP8266)
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
#else
  Serial.begin(kBaudRate, SERIAL_8N1);
#endif

  while (!Serial)  // Ожидание установления соединения с последовательным портом
 
    delay(50);
     Serial.println("ожидаем иницилизацию сом завершено");
     delay(50);
  Serial.printf("\nОжидаем ИК сигнал для его считывания, ПИН: "
                "%d\n", kRecvPin);
#if DECODE_HASH
 Serial.println("вылет из setup");
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif
  irrecv.enableIRIn();
 
}


void loop() {
  // Проверяем, есть ли ИК код
  if (irrecv.decode(&results)) {
    // Display a crude timestamp.
  //  uint32_t now = millis();
   // Serial.printf("Timestamp : %06u.%03u\n", now / 1000, now % 1000);
  
    if (results.overflow) Serial.printf( "Предупреждение: ИК код превышает буфер (>= %d). "  "Полученные значения неверные, увеличьте буфер kCaptureBufferSize.\n",   kCaptureBufferSize);
    /*Serial.println("Library : v" _IRREMOTEESP8266_VERSION_ "\n");*/
    
    Serial.print(resultToHumanReadableBasic(&results));
  
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length()) Serial.println("Description: " + description);
    yield();
    
  ///  Serial.println(resultToTimingInfo(&results)); //выводит много столбиков
  //  yield();
    
   Serial.println(resultToSourceCode(&results));
   Serial.println();
  yield();

  }
}
