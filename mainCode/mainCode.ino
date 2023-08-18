    
//USS init
int trigPin=2;
int echoPin=3;
int pingTravelTime;
float pingDistance;

//Buzzer init
const int buzzer = 4;


//Fob init
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


//Other variables
float packDistance;


void setup(){

 // put your setup code here, to run once:


  //buzzer
  pinMode(buzzer, OUTPUT);


//USS setup  
pinMode(trigPin,OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(115200);



//Fob setup
SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  delay(500);
  Serial.println();
}

void loop(){
  /// put your main code here, to run repeatedly:
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
{
    return;
}  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  String content2 = "";
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


while(true){
  //Check for fob
  
  if (content.substring(1) == "DB 7B 04 22") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);


    float sum=0;
    for(int i=0; i<10; i++){
      digitalWrite(trigPin,LOW);
      delayMicroseconds(10);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin,LOW);
      pingTravelTime = pulseIn(echoPin, HIGH); //Time in microseconds
      pingDistance= (343.*pingTravelTime)/2000000; //Distance in meters
      Serial.print(pingTravelTime); 
      Serial.print("   -   ");
      Serial.println(pingDistance);
      delay(100);
      sum+=pingDistance;
    }
    packDistance=sum/10;
    Serial.print(packDistance);
    delay(3000);  
    break;
  }
 
 else{
    Serial.println(" Access denied");
    delay(3000);
    return;
 }
  //If distance is less than certain amount -> activate system
  //save distance value for future checks
  //exit loop


  //Else keep running 
}


while(true){
      digitalWrite(trigPin,LOW);
      delayMicroseconds(10);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin,LOW);
      pingTravelTime = pulseIn(echoPin, HIGH); //Time in microseconds
      pingDistance= (343.*pingTravelTime)/2000000; //Distance in meters
      Serial.print(pingTravelTime); 
      Serial.print("   -   ");
      Serial.println(pingDistance);
      delay(100);


      if (mfrc522.PICC_IsNewCardPresent()){
        mfrc522.PICC_ReadCardSerial();
        content2 = "";
        byte letter2;
        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
          content2.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
          content2.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
        content2.toUpperCase();
      }


      if (pingDistance<packDistance -0.02 || pingDistance>packDistance +0.02){
          Serial.println("THIEF");
          for (int i=0;i<5;i++){
          tone(buzzer, 1000);
          delay(200);
          noTone(buzzer);
          delay(100);
          }
          break;
      }
      
        if (content2.substring(1) == "97 C9 12 34"){
        Serial.println("Package safe");
        delay(2000);
        break;
        }else if(content2.substring(1) != "97 C9 12 34" && content2.substring(1) != ""){
          Serial.println("Access denied");
        delay(2000);
        content2= "";
        }
  }


}
