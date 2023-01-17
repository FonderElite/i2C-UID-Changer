/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example to change UID of changeable MIFARE card.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * This sample shows how to set the UID on a UID changeable MIFARE card.
 * 
 * @author Tom Clement
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 */

#include <LiquidCrystal_I2C.h>
#include <String.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   9     // Configurable, see typical pin layout above
#define SS_PIN    10    // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
String input;
/* Set your new UID here! */
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card

  /*LCD*/
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  lcd.clear();
  lcd.init();    
  lcd.backlight();      // Make sure backlight is on
  lcd.setCursor(0, 0);
  lcd.print("Current UID: ");
  
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle. And if present, select one.
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  
 
Serial.begin(9600);
if(Serial.available()){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("New UID: ");
 byte newUid[] = NEW_UID;
if ( mfrc522.MIFARE_SetUid(newUid, (byte)4, true) ) {
    Serial.println("Wrote new UID to card.");
}
 mfrc522.PICC_HaltA();
  MFRC522::Uid uid = mfrc522.uid;

    lcd.setCursor(0, 1);   //Move cursor to position (0,0)
    lcd.print("ID:");
    for (byte i = 0; i < uid.size; i++) {
        lcd.print(uid.uidByte[i] < 0x10 ? " 0" : " ");
        lcd.print(uid.uidByte[i], HEX);
    }
    mfrc522.PICC_HaltA();
}else{
   MFRC522::Uid uid = mfrc522.uid;
 
    lcd.setCursor(0, 1);   //Move cursor to position (0,0)
    lcd.print("ID:");
    for (byte i = 0; i < uid.size; i++) {
        lcd.print(uid.uidByte[i] < 0x10 ? " 0" : " ");
        lcd.print(uid.uidByte[i], HEX);
    }
    mfrc522.PICC_HaltA();
    
}
}
