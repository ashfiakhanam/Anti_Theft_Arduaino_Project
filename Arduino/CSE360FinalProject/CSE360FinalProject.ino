#include <SoftwareSerial.h>
SoftwareSerial SIM900(8, 9);
SoftwareSerial LOGGER(10, 11);

const int echoPin = 2; // Echo Pin of Ultrasonic Sensor to Arduino
const int pingPin = 3; // Trigger Pin of Ultrasonic Sensor to Arduino
const int PIRPin = 4; // PIR pin to Arduino
const int BuzzerPin = 5; //initializing Buzer pin to Arduino
const int LEDPin = 7; // LED pin to Arduino

int PIRVal; //initializing PIR sensor value
int alert = 1; // alert variable for UltraSonic
int alert2 = 1; // alert variable for PIR
                    
void setup() 
{
  Serial.begin(4800); // Starting Serial Communication
  pinMode(pingPin, OUTPUT); // initialising pin 3 as output
  pinMode(echoPin, INPUT); // initialising pin 2 as input
  pinMode(LEDPin, OUTPUT); // initialising pin 7 as output
  pinMode(PIRPin, INPUT); // initialising pin 4 as input
  pinMode(BuzzerPin, OUTPUT); // initialising pin 5 as output

  SIM900.begin(2400);
  LOGGER.begin(9600);
  Serial.println("Starting the GSM module. Please Wait.");
  delay(100); // give time to log on to network.
  Serial.println("Ready!");
  
}

void loop()
{ //Ultrasonic Part
  long duration, inches, cm;
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);  
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);  
  digitalWrite(pingPin, LOW);  
  duration = pulseIn(echoPin, HIGH); // using pulsin function to determine total time
  inches = microsecondsToInches(duration); // calling method
  LOGGER.println(inches);
  
  if (inches > 10){   
    if (alert == 1){
      Serial.println();
      Serial.println();
      Serial.println("Your precious thing displaced!!!");
      

      
      SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message      
      Serial.println();
      SIM900.println("AT + CMGS = \"+8801788339917\"");// recipient's mobile number
      Serial.println("AT + CMGS = \"+8801788339917\""); 

      SIM900.println("Your precious thing displaced!!!"); // message to send
      Serial.println("Sending an SMS to the mobile number.");

      SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
      
;
      SIM900.println();

      Serial.println("SMS Sent Successfully");
      
      alert = 0;
      }   
  }
  else{
    alert = 1;
  }
  


  //PIR Part
  PIRVal = digitalRead(PIRPin);
  if (PIRVal == 1){
    LOGGER.println("1");
    if (alert2 == 1){
      digitalWrite(BuzzerPin, HIGH);
      digitalWrite(LEDPin, HIGH);
      Serial.println();
      Serial.println();
      Serial.println("Motion is detected by PIR!!!");

      
      SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message      
      Serial.println();
      SIM900.println("AT + CMGS = \"+8801788339917\"");// recipient's mobile number
      Serial.println("AT + CMGS = \"+8801788339917\""); 

      SIM900.println("Motion is detected by PIR!!!"); // message to send
      Serial.println("Sending an SMS to the mobile number.");

      SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
      

      SIM900.println();

      Serial.println("SMS Sent Successfully");
      
      alert2 = 0;
      }
    

  }
  else {
    LOGGER.println("0");
    alert2 = 1;
    

    
  //LED Part
  if (inches > 10){
    digitalWrite(LEDPin, HIGH);
    digitalWrite(BuzzerPin, HIGH); 
    }
  else{
    digitalWrite(BuzzerPin, LOW);
    digitalWrite(LEDPin, LOW);
    }    

  }
  delay(200);
}

long microsecondsToInches(long microseconds) // method to covert microsec to inches 
{
 return microseconds / 74 / 2;
}
