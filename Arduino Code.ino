#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 #include <LiquidCrystal.h>
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 7 
#define LED_R 6 
#define BUZZER 8 
MFRC522 mfrc522(SS_PIN, RST_PIN);  
Servo myServo; //define servo name
 LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

void setup() 
{ 
 Serial.begin(115200);  
  SPI.begin();     
  mfrc522.PCD_Init(); 
  myServo.attach(5); 
  myServo.write(0); 
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader...");
  Serial.println();
  lcd.begin(16,2);
}
void loop() 
{
  lcd.clear();
  lcd.print("Welcome ");
  lcd.setCursor(0,1);
  lcd.print("please insert!");
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "FC 1E C1 6D")
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo.write(180);
    lcd.clear();
    lcd.print("Welcome to Home");
    lcd.setCursor(0,1);
    lcd.print("Room Unlocked!");
    delay(5000);
    myServo.write(0);
    digitalWrite(LED_G, LOW);
    delay(1000);
  }
 else 
 {
    Serial.println(" Access denied");
    delay(500);
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    lcd.clear();
    lcd.print("Wrong Key");
    lcd.setCursor(0,1);
    lcd.print("Room is locked!");
    delay(2000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
    delay(1000);
  }
} 
   
