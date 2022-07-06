//sketch created by Akshay Joseph
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>


const int KEYPAD_ROWS    = 4;
const int KEYPAD_COLUMNS = 4;

const int SHIFT_AUTH    = 5;
const int SHIFT_NEW_PIN = 9;

const int SCREEN_AUTH = 1;
const int SCREEN_NEW_PIN = 2;
const int SCREEN_WELCOME = 3;

const String PIN_DEFAULT = "1234";

const char KEY_BACKSPACE = '#';
const char KEY_ENTER     = '*';
const char KEY_NEW_PIN   = 'A';
const char KEY_LOGOUT    = 'C';
const char KEY_REBOOT    = 'D';

char keys[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
  {'1',      '2','3',           KEY_NEW_PIN},
  {'4',      '5','6',           'B'},
  {'7',      '8','9',           KEY_LOGOUT},
  {KEY_ENTER,'0',KEY_BACKSPACE, KEY_REBOOT}
};

struct myConfig{
  char pin[5];
};

byte pin_rows[KEYPAD_ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[KEYPAD_COLUMNS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

myConfig cfg = {""};

int charCnt       = 0;
String inputPin   = "";
String pinCode    = PIN_DEFAULT;
int auth          = 0;
int cursorBlink   = 1;
int allowEdit     = 1;
int enterShift    = 4;
int currentScreen = SCREEN_AUTH;
char key; 

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, KEYPAD_ROWS, KEYPAD_COLUMNS );

LiquidCrystal_I2C lcd(0x27, 16, 2);

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void setup()                                                                                      
{
  // myConfig test = {{'1', '2', '3', '4'}};
//  Serial.begin(9600);
  // EEPROM.put(0,test);
  

  EEPROM.get(0, cfg);
  
  if ((cfg.pin != NULL) && (cfg.pin[0] == '\0')) {
//    Serial.println("Empty");
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
  screenAuth();  
}

void loop()
{
  if (cursorBlink == 1)
    lcd.blink();
  
  key = keypad.getKey();
  if (key) {
    switch (key) {
      case KEY_BACKSPACE:
        doBackspace(); 
        break;
      
      case KEY_ENTER:
        doEnter(); 
        break;
      
      case KEY_NEW_PIN:
        if(auth == 1)
          screenNewPin(); 
        break;
      
      case KEY_LOGOUT:
        if(auth == 1)
          screenLogout(); 
        break;
      
      case KEY_REBOOT:
        if(auth == 1)
          screenReboot();
        break;  
      
      default:
        doKeyBuffer();
        break;
    }
  }
}

void screenWrite(String str, int line)
{
  lcd.setCursor(0, line);
  lcd.print(str);  
}

void writeFirst(String str)
{
  screenWrite(str, 0);
}

void writeSecond(String str)
{
  screenWrite(str, 1);
}

void screenWelcome()
{
  cursorBlink = 0;
  allowEdit   = 0;
  
  lcd.noBlink();
  delay(1000);
  lcd.clear();
  writeFirst("Welcome!");
}

void screenAuth()
{
  allowEdit     = 1;
  cursorBlink   = 1;
  inputPin      = "";
  auth          = 0;
  enterShift    = SHIFT_AUTH;
  currentScreen = SCREEN_AUTH;
  
  lcd.blink();
  lcd.clear();
  writeFirst("PIN: ");    
}

void screenLogout()
{
  lcd.clear();
  writeFirst("Logout");
  delay(1000);
  screenAuth(); 
}

void screenReboot()
{
  lcd.clear();
  writeFirst("Reboot");
  for (int i = 5; i >= 1; i--) {
    writeSecond(String(i));
    delay(1000);
  }
  resetFunc();
}

void screenNewPin()
{
  inputPin      = "";
  allowEdit     = 1;
  cursorBlink   = 1;
  currentScreen = SCREEN_NEW_PIN;
  enterShift    = SHIFT_NEW_PIN;
  
  lcd.clear();
  writeFirst("Change PIN");
  delay(1000);
  lcd.clear();
  writeFirst("New PIN: ");
}

void doBackspace()
{
  if (allowEdit == 1) {
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
  }
}

void doEnter()
{
//      Serial.print("cfg.pin = '");
//      Serial.print(cfg.pin);
//      Serial.println("'");
//      Serial.print("inputPin = '");
//      Serial.print(inputPin);
//      Serial.println("'");
  
  switch (currentScreen) {
    case SCREEN_AUTH:
      if (inputPin == String(cfg.pin)) {
        writeSecond("SUCCESS!        ");
        auth = 1;
        screenWelcome();
      } else {
        writeSecond("Incorrect PIN   ");
        writeFirst("PIN:            ");
        lcd.setCursor(enterShift,0); 
      }  
      break;
    case SCREEN_NEW_PIN:
      if (inputPin == String(cfg.pin)){
        writeFirst("Same PIN        ");
        writeSecond("No changes      ");
      } else {
        writeFirst("Saving...       ");
        lcd.backlight();
        
        inputPin.toCharArray(cfg.pin, 5);
        EEPROM.put(0,cfg);
        
        delay(1000);
        writeSecond("PIN changed     ");
      }
      screenWelcome();
      break;
  }
    
  charCnt = 0;
  inputPin = ""; 
}

void doKeyBuffer()
{
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
