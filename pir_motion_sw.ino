/* 
 * //////////////////////////////////////////////////
 * //making sense of PIR sensor's output
 * //////////////////////////////////////////////////
 *
 * Switches a LED according to the state of the sensors output pin.
 * Determines the beginning and end of continuous motion sequences.
 *
 * @author: Prarinya Ekapho
 * @date:   3 November 2016 
 *
 * The sensor's output pin goes to HIGH if motion is present.
 * However, even if motion is present it goes to LOW from time to time, 
 * which might give the impression no motion is present. 
 * This program deals with this issue by ignoring LOW-phases shorter than a given time, 
 * assuming continuous motion is present during these phases.
 *  
 */

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 15; //select 15 secs        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 180000;  //delay on time about 3 min.

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 2;    //the digital pin connected to the PIR sensor's output
int ryPin = 4;    //the digital pin connected to relay module
int disPin = 12;  //indicale to green LED
int ledPin = 13;  //indicate sensing from PIR sensor


//SETUP
void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ryPin, OUTPUT);
  pinMode(disPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  digitalWrite(ryPin, LOW); //for active 'LOW' relay module
  digitalWrite(disPin, HIGH); //indicale to green LED
  //digitalWrite(ryPin, HIGH); //for active 'HIGH' relay module
  
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    delay(500);
    digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
  }

//LOOP
void loop(){

     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("#####");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec");
         digitalWrite(ryPin, LOW); //for active 'LOW' module
         digitalWrite(disPin, HIGH);//indicale to green LED
         //digitalWrite(ryPin, HIGH); //for active 'HIGH' module
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the LED visualizes the sensors output pin state

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           digitalWrite(ryPin, HIGH); //for active 'LOW' module
           digitalWrite(disPin, LOW); //
           //digitalWrite(ryPin, LOW); //for active 'HIGH' module
           delay(50);
           }
       }
  }
