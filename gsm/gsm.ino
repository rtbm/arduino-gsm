#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hello GSM!");

  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    Serial.write("Available...");

    while (Serial.available() > 0) {
      Serial.write("Sending...");
      Serial1.write(Serial.read());
      delay(20);
    }

    Serial.write("\nApplying...");
    Serial1.write('\r');
  }

  if (Serial1.available() > 0) {
    Serial.write("\nSerial 1:\n");

    String buffer = "";
    char ch = 0;
    int i = 0;

    while (Serial1.available() > 0) {
      ch = Serial1.read();
      buffer += ch;
      i++;      
      delay(20);
    }

    lcd.setCursor(0, 0);

    if (buffer.indexOf("RING") > 0) {
      lcd.print("Ringing...");
      
      int startAt = buffer.indexOf("+CLIP: \"") + 8;

      if (startAt > 0) {
        String caller = "";
        char ch = 0;
        
        for(;;) {
          ch = buffer[startAt];
          
          if (ch == '"') {
            break;
          }
          
          caller += ch;
          startAt++;
        }

        lcd.setCursor(0, 1);
        lcd.print(caller);

        Serial.println(caller);
      }
      
    }
    else if (String(buffer).indexOf("NO CARRIER") > 0) {
      lcd.print("Missed...");
    }

    Serial.println(buffer);
  }
}

