//sketch created by Akshay Joseph
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>


const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

const int SHIFT_AUTH = 5;
const int SHIFT_NEW_PIN = 9;

const int SCREEN_AUTH = 1;
const int SCREEN_NEW_PIN = 2;
const int SCREEN_WELCOME = 3;

const String PIN_DEFAULT = "1234";

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

struct myConfig{
  char pin[5];
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

myConfig cfg = {""};

int charCnt       = 0;
String inputPin   = "";
String pinCode    = PIN_DEFAULT;
int auth          = 0;
int cursorBlink   = 1;
int allowEdit     = 1;
int enterShift    = 4;
int currentScreen = SCREEN_AUTH;

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

LiquidCrystal_I2C lcd(0x27, 16, 2);

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup()                                                                                      
{
  // myConfig test = {{'1', '2', '3', '4'}};
//  Serial.begin(9600);
  // EEPROM.put(0,test);
  

  EEPROM.get(0, cfg);
  
  if ((cfg.pin != NULL) && (cfg.pin[0] == '\0')) {
    Serial.println("Empty");
    pinCode.toCharArray(cfg.pin,5);
  } else {
//    Serial.println("Start:");
//    Serial.println(cfg.pin[2]);
//    Serial.println(cfg.pin[0], DEC);  // print as an ASCII-encoded decimal
//    Serial.println(cfg.pin[0], HEX);  // print as an ASCII-encoded hexadecimal
//    Serial.println(cfg.pin[0], OCT);  // print as an ASCII-encoded octal
//    Serial.println(cfg.pin[0], BIN);  // print as an ASCII-encoded binary
//    Serial.println(String(cfg.pin));
//    Serial.println("End.");
  }
  
//  Serial.println("Start:");
//  Serial.println(cfg.pin);
//  Serial.println(String(cfg.pin));
//  Serial.println("End.");
    
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PIN: ");
  enterShift = SHIFT_AUTH;
}

void loop()
{
  if (cursorBlink == 1)
    lcd.blink();
  
  char key = keypad.getKey();
  if (key){
    if (allowEdit == 1 && key == '#') {
      charCnt--;
      
      if (charCnt >= 0) {
        inputPin.remove(charCnt);
        lcd.setCursor(enterShift + charCnt, 0);
        lcd.print(" ");
        lcd.setCursor(enterShift + charCnt, 0);
      } else {
        inputPin = "";
        charCnt = 0;
      }
    } else if (auth == 1 && key == 'A') {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Change PIN");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("New PIN: ");
      inputPin = "";
      allowEdit = 1;
      cursorBlink = 1;
      currentScreen = SCREEN_NEW_PIN;
      enterShift = SHIFT_NEW_PIN;
    } else if (auth == 1 && key == 'C') {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Logout");
      delay(1000);
      allowEdit = 1;
      cursorBlink = 1;
      inputPin = "";
      auth = 0;
      enterShift = SHIFT_AUTH;
      currentScreen = SCREEN_AUTH;
      lcd.blink();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PIN: ");      
    } else if (auth == 1 && key == 'D') {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reboot");
      for (int i = 5; i >= 1; i--) {
        lcd.setCursor(0,1);
        lcd.print(i);
        delay(1000);
      }
      resetFunc();
    } else if(key == '*') {
      Serial.print("cfg.pin = '");
      Serial.print(cfg.pin);
      Serial.println("'");
      Serial.print("inputPin = '");
      Serial.print(inputPin);
      Serial.println("'");
      
      switch (currentScreen) {
        case SCREEN_AUTH:
          if (inputPin == String(cfg.pin)) {
            lcd.setCursor(0,1);
            lcd.print("SUCCESS!        ");
            auth = 1;
            lcd.noBlink();
            cursorBlink = 0;
            allowEdit = 0;
            delay(1000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Welcome!");
          } else {
            lcd.setCursor(0,1);
            lcd.print("Incorrect PIN   ");
            lcd.setCursor(0,0);
            lcd.print("PIN:            ");
            lcd.setCursor(enterShift,0); 
          }  
          break;
        case SCREEN_NEW_PIN:
          cursorBlink = 0;
          allowEdit = 0;
          if (inputPin == String(cfg.pin)){
            lcd.setCursor(0,0);
            lcd.print("Same PIN        ");
            lcd.setCursor(0,1);
            lcd.print("No changes      ");
          } else {
            lcd.setCursor(0,0);
            lcd.print("Saving...       ");
            lcd.backlight();

//            Serial.println(cfg.pin);    
//            inputPin.toCharArray(cfg.pin, 5);
//            Serial.println(cfg.pin);
            
            EEPROM.put(0,cfg);
            
            delay(1000);
            lcd.backlight();
            lcd.setCursor(0,1);
            lcd.print("PIN changed     ");
          }
          lcd.noBlink();
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Welcome!");
          break;
      }
        
      charCnt = 0;
      inputPin = ""; 
    } else {
      if (allowEdit == 1){
        charCnt++;
        if (charCnt > 4) {
          charCnt--;
        } else {
          lcd.print(key);
          inputPin += key;
        }
      }
    }
//    Serial.println(key);
  }
}
