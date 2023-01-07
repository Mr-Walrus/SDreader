/*
  Скетч к проекту "Читалка SD с OLED"
  Страница проекта (схемы, описания): https://alexgyver.ru/SDreader/
  Исходники на GitHub: https://github.com/AlexGyver/SDreader/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/
*/

// Ссылка для менеджера плат: https://alexgyver.github.io/package_GyverCore_index.json
// Читать про GyverCore: https://github.com/AlexGyver/GyverCore
// Настройки платы: Clock/Internal 8 MHz, Bootloader/Without bootloader

// Настройки
#define ORIENTATION 0   // 0 - правша, 1 - левша

// пины
#define BTN_UP 3
#define BTN_SET 4
#define BTN_DWN 2
#define OLED_PWR1 5
#define OLED_PWR2 6
#define SD_PWR A1

// библиотеки
#include "buttonMinim.h"
#include <SPI.h>
#include <SD_fix.h>
#include "GyverOLED_fix.h"
#include "Vector.h"

// данные и классы
buttonMinim buttUP(BTN_UP);
buttonMinim buttSET(BTN_SET);
buttonMinim buttDWN(BTN_DWN);
GyverOLED oled(0x3C);
Sd2Card card;
SdVolume volume;
SdFile root;
File myFile;
Vector<int> Pss;

// переменные
String filenames = "";
String setName = "";
int8_t filesAmount = 0;
byte mode = 0;
int8_t fileSet = 0;
int filePage = 0;
uint32_t btnTimer;

// ================ SETUP ================
void setup() {
  pinMode(SD_PWR, OUTPUT);
  pinMode(OLED_PWR1, OUTPUT);
  pinMode(OLED_PWR2, OUTPUT);
  digitalWrite(SD_PWR, 1);
  digitalWrite(OLED_PWR1, 1);
  digitalWrite(OLED_PWR2, 1);
  delay(500);
  oledInit();
  SDinit();
  printNames();
  
  Pss.reserve(10);
 // for(int i = 0; i < 30; i++)
 //   Poses[i] = 0;
 
  /*
    // 8 МГц искусственно
    CLKPR |= (1 << CLKPCE); // разрешить изменение прескалера
    CLKPR = 0x1; // прескалер / 16 (1МГЦ)

    SPCR &= ~ ((1 << SPR1) | (1 << SPR0)); // F_cpu / 4
    SPSR |= (1 << SPI2X); // Удвоить скорость

    TWBR = 0; // макс скорость TWI
    TWSR &= ~ ((1 << TWPS1) | (1 << TWPS0)); // еще быстрее*/
}

// ================ LOOP ================
void loop() {
  if (buttDWN.clicked()) {
    if (mode == 0) nextFile();
    else if (mode == 1) nextPage();
  }
  if (buttUP.clicked()) {
    if (mode == 0) prevFile();
    else if (mode == 1) prevPage();
  }
  if (buttDWN.holding()) {
    if (millis() - btnTimer >= 300) {
      btnTimer = millis();
      if (mode == 0) nextFile();
      else if (mode == 1) nextPage();
    }
  }
  if (buttUP.holding()) {
    if (millis() - btnTimer >= 300) {
      btnTimer = millis();
      if (mode == 0) prevFile();
      else if (mode == 1) prevPage();
    }
  }

  if (buttSET.clicked()) {
    if (mode == 0) {
      mode = 1;
      myFile = SD.open(setName, FILE_READ);
      Pss.empty();
      //curPage = 0;
      oled.inverse(false);
      printFile();
    }
  }
  if (buttSET.holded()) {
    if (mode == 1) {
      mode = 0;
      myFile.close();
      printNames();
    }
  }
}

// ================ УПРАВЛЕНИЕ ================
void nextFile() {
  if (++fileSet >= filesAmount) fileSet = filesAmount;
  printNames();
}
void prevFile() {
  if (--fileSet < 0) fileSet = 0;
  printNames();
}
void nextPage() {
  printFile();
}
void prevPage() {
  uint32_t pos = 0;
  if (Pss.size() > 2) 
    pos = (myFile.position() - (Pss[Pss.size()-2]+Pss[Pss.size()-1]));
  if (pos < 0) pos = 0;
  if (Pss.size() > 1) Pss.pop_back();
  if (Pss.size() > 0) Pss.pop_back();
  myFile.seek(pos);
  printFile();
}

// ================ ПЕЧАТЬ ФАЙЛА ================
void printFile() {
  if ((byte)myFile.peek() == 255) return;
  oled.clear();
  oled.home();
  int len = 0;
  while (!oled.isEnd()) {
    if ((byte)myFile.peek() != 255) { 
      oled.print((char)myFile.read());
      len++;
    }
    else break;
  }
  //curPage++; 
  Pss.push_back(len);
}

// ================ ВЫВОД ИМЁН ================
void printNames() {
  oled.clear();
  oled.home();
  int i = 0;

  byte thisPosition = 0;
  byte thisPage = fileSet / 8;
  setName = "";
  while (true) {
    if (thisPosition == fileSet) {
      oled.inverse(true);
      if (filenames[i] != '\n') setName += filenames[i];
    } else oled.inverse(false);

    if ((thisPosition / 8) >= thisPage) {
      oled.print(filenames[i]);
    }
    if (filenames[i] == '\n') {
      thisPosition++;
      if (thisPosition > filesAmount) break;
    }
    if ((thisPosition / 8) == (thisPage + 1)) break;
    i++;
  }
}

// ================ ИНИЦИАЛИЗАЦИЯ ================
void oledInit() {
  Wire.begin();
  Wire.setClock(400000L);
  oled.init(OLED128x64);
  oled.setContrast(3);
  oled.clear();
  oled.home();
  oled.scale2X();
  oled.println("SD Reader");
  oled.scale1X();
  oled.println("v1.3 by AlexGyver");
  oled.println("feat MrWalrus");
  oled.println();
}

void SDinit() {
  oled.print("Init SD...");
  if (!card.init(SPI_HALF_SPEED, 10)) {
    oled.println(" error");
    while (1);
  } else {
    oled.println(" OK");
  }

  if (!volume.init(card)) {
    oled.println("Wrong card!");
    while (1);
  }
  root.openRoot(volume);
  filenames = root.ls_String(LS_R);

  int i = 0;
  while (true) {
    if (filenames[i] == '\n') filesAmount++;
    if (filenames[i] == '\0') break;
    i++;
  }
  filesAmount--;

  oled.print((uint32_t)filesAmount);
  oled.println(" files found");

  SD.begin(10);
  delay(1000);
  oled.clear();
  oled.home();
}
