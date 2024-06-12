/** 
 * This is the original Author's code written for the miniquad driver board software using an Arduino Nano & Arduino IDE.
 * Original author 'goffrobb': https://www.instructables.com/DIY-High-Current-Motor-Driver-h-bridge/
 * JB Note: I did not test this code, so there may be errors. I just cleaned up the formatting for readability.
 * This is here for my own reference, research, and learning.
*/

// Analog Inputs
const int throttle = A0;
int throttleOut = 0;
int lastthrottle;
int throtarray[10];
const int batsense = A1;
int batval = 0;
int Batstate;
const int highset = A4;
byte highval = 0;
const int lowset = A3;
byte lowval = 0;

// Digital Inputs
const byte footpedal = A2;
byte drive_EN = 0;
const byte forwardtoggle = A6;
const byte reversetoggle = A5; 
byte Direction;
byte LOW_A_state = HIGH;
byte LOW_B_state = HIGH;
const byte reserve = A7; // currently reserved (no purpose)
byte reserveval;

// Outputs
const byte Enable = 2;
const byte HIGH_A = 9;
const byte HIGH_B = 10;
const byte LOW_A = 5;
const byte LOW_B = 6;

// Random variables
byte i;
bool setbatled = false;
unsigned long ledtimer = 0;
unsigned long battimer = 0;
unsigned long buttontimer[3];
bool flash= true;
byte toflash;
const byte led1 = 11, led2 = 12, led3 = 8, led4 = 7, led5 = 4;

// Performance test var
void setup() {

  pinMode(Enable,OUTPUT); digitalWrite(Enable,LOW);
  pinMode(HIGH_A,OUTPUT); digitalWrite(HIGH_A,LOW);
  pinMode(HIGH_B,OUTPUT); digitalWrite(HIGH_B,LOW);
  pinMode(LOW_A,OUTPUT); digitalWrite(LOW_A,LOW);
  pinMode(LOW_B,OUTPUT); digitalWrite(LOW_B,LOW);
    
  Serial.begin(9600);
  for(i = 0; i < 10; i++) { battery_check(); }   // get battery voltage
  for(i = 0; i < 25; i++) { throttle_tuning(); } // get throttle tuning
  for(i = 0; i < 15; i++) { GET_THROTTLE(); }    // get throttle tuning
  pinMode(led1,OUTPUT); pinMode(led2,OUTPUT); pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT); pinMode(led5,OUTPUT); pinMode(footpedal,INPUT); // input pullup for testing set as input for testing
  pinMode(forwardtoggle, INPUT); pinMode(reversetoggle,INPUT);
  direction_init();

}

void loop() {

  if (millis() >= battimer) { battery_check(); } // checks battery 2x per second & gives indication of voltage over last 10 seconds
  if (setbatled == true) {
		setbatled = false; // if the battery state has changed then we need to change the led state
		battery_display(); 
	}
  if (flash == true) {
		if (millis() >= ledtimer) { ledflash(); } // Change LED state if needed.
	}
  // Serial.println(batval);
  READBUTTONS();
  throttle_tuning();
  GET_THROTTLE();
  if(throttleOut != lastthrottle) { DRIVE(); } // if throttle not changed then we skip a pointless step

}

/* OLD: DIDNT WORK RIGHT
void DRIVE() {
  if (drive_EN == 1) {
    switch(Direction) {
      // Forward
      case 1: 
				if (LOW_A_state == HIGH) {digitalWrite(LOW_A,LOW); digitalWrite(HIGH_B,LOW); LOW_A_state=LOW;delay(20);} // send HB,LA low
        if (LOW_B_state != HIGH) {digitalWrite(LOW_B,HIGH); LOW_B_state=HIGH; Serial.print("LOW_A_state"); Serial.println(LOW_A_state);}
        // analogWrite(HIGH_A,throttleOut):
        break;
      
      // Reverse
      case 0: 
				if (LOW_B_state == HIGH) {digitalWrite(LOW_B,LOW); digitalWrite(HIGH_A,LOW); LOW_B_state=LOW;delay(20);} // send HA,LB low
        if (LOW_A_state != HIGH) {digitalWrite(LOW_A,HIGH); LOW_A_state=HIGH; Serial.print("LOW_A_state"); Serial.println(LOW_A_state);}
        // analogWrite(HIGH_B,throttleOut):
        break;
      }
    } else { digitalWrite(Enable,LOW); digitalWrite(HIGH_A,LOW); digitalWrite(HIGH_B,LOW); digitalWrite(LOW_A,LOW); digitalWrite(LOW_B,LOW);
    }
}
*/
void DRIVE() {

  if (drive_EN == 1) {
    switch(Direction) {
			// EVERY TIME THE THROTTLE CHANGES OR DIRECTION CHANGES WE ENSURE THE OPPOSITE MOSFETS ARE INDEED OFF!
      case 1: // Forward: send HB,LA low: 
				digitalWrite(LOW_A,LOW); digitalWrite(HIGH_B,LOW); LOW_A_state = LOW; delay(2); // INCLUDE SHORT DELAY TO LET THEM TURN OFF
        digitalWrite(LOW_B,HIGH); LOW_B_state = HIGH; 
				// Serial.print("LOW_A_state"); Serial.println(LOW_A_state);
        analogWrite(HIGH_A,throttleOut);
        break;

      case 0: // Reverse
				digitalWrite(LOW_B,LOW); digitalWrite(HIGH_A,LOW); LOW_B_state = LOW; delay(2); //send HA,LB low
        digitalWrite(LOW_A,HIGH); LOW_A_state=HIGH; 
				// Serial.print("LOW_A_state"); Serial.println(LOW_A_state);
        analogWrite(HIGH_B,throttleOut);
        break;
    }
  } else { digitalWrite(Enable,LOW); digitalWrite(HIGH_A,LOW); 
					 digitalWrite(HIGH_B,LOW); digitalWrite(LOW_A,LOW);+ 
					 digitalWrite(LOW_B,LOW);
  }
}

void direction_init() {   // ENSURES THE DIRECTION IS KNOWN AT STARTUP
 
 	// byte readforward = digitalRead(forwardtoggle);
  byte readreverse = digitalRead(reversetoggle);
  
	/* if (readforward == HIGH) { // UNUSED DUE TO DODGY BOARD IN DEV PHASE.....
				Direction = 1;
				Serial.println("forwardinit");
			} */
  
	if (readreverse == HIGH) { 
		Direction = 0; 
		Serial.println("reverseinit");
	} else { 
		Direction = 1; 
		Serial.println("forwardinit");
	}
}

void READBUTTONS() {
  static byte  lastfoot;
  static byte lastforward;
  static byte lastreverse;
  byte readfoot=digitalRead (footpedal);
  byte readforward=digitalRead (forwardtoggle);
  byte readreverse=digitalRead (reversetoggle);
  static long foottime;
  static long forwardtime;
  static long reversetime;

  if (readfoot != lastfoot && millis() >= foottime ) { // DEBOUNCE
    foottime = millis() + 100;
    if (readfoot == LOW) { // DISABLE ALL
      digitalWrite( Enable,LOW);
      digitalWrite( HIGH_A,LOW);
      digitalWrite( HIGH_B,LOW);
      digitalWrite( LOW_A,LOW);
      digitalWrite( LOW_B,LOW);
      drive_EN = 0;
      Serial.println("motor disableR");
    } else {  
      digitalWrite( Enable,HIGH); // ALLOW MOTOR TO RUN AGAIN (THROTTLE DEPENDANT)
      drive_EN = 1;
      Serial.println("motor enableR");
    }
    lastfoot = readfoot;
}

/*
// DEAD PIN ON BOARD USING SINGLE PIN DIRECTION INSTEAD
if (readforward != lastforward && millis() >= forwardtime ) {
	forwardtime = millis() + 100;
	if (readforward == HIGH){ Direction = 1; Serial.println("forward");     }
	lastforward = readforward;
}
*/

	if (readreverse != lastreverse && millis() >= reversetime ) { // DEBOUNCE
		reversetime = millis() + 100;
		if (readreverse == HIGH) { // SETTING MOTOR DIRECTION 
			Direction = 0; 
			Serial.println("REVERSE"); 
		}
	} else { 
		Direction = 1; 
		Serial.println("FORWARD");   
		lastreverse = readreverse;
	}
}

void GET_THROTTLE() {
  // THROTTLE USES THE AVERAGE OF 15 READINGS IN AN ARRAY 
  // The INTENTION HERE IS TO AVERAGE OUT THE READING AND INTRODUCE SLIGHT HYSTERESIS
  static int throtarray[15];
  static byte ithrot;
  static int throttotal;
  analogRead(throttle);
  delay(1);													 // Wait for adc to stabilize then take the reading 
  int reading = analogRead(throttle);// Serial.println(reading);
  if (reading <= 50) { 
		throtarray[ithrot] = 0; 				 // IF THROTTLE READING TOO LOW (NO INPUT) SET DATA TO 0 (DEAD ZONE}
  	throttleOut = 0;								 // set output to zero
  	ithrot++;
    if(ithrot >= 15) {							 // RESET COUNTER
			ithrot = 0;
		}
    throttotal = throttotal - throtarray[ithrot]; // REMOVE OLDEST READING
  } else {
    throtarray[ithrot] = map(reading, 50, 1023, lowval, highval); // MAP THROTTLE TO PWM OUT VALUE BETWEEN MINIMUM TRIMM VALUE AND MAX TRIMMER VALUE (RANGE)
    throttotal = throttotal + throtarray[ithrot];
    throttleOut = throttotal / 15;
    ithrot++;
    if(ithrot >= 15) { ithrot = 0; }              // RESET COUNTER
    throttotal = throttotal - throtarray[ithrot]; // REMOVE OLDEST READING
  }
    lastthrottle = throttleOut;  // Serial.println(throttleOut);
}

/**
 * TUNING IS USED TO SET THE MINIMUM AND MAXIMUM 
 * PWM VALUES. THIS IS MEANT TO PREVENT MOTOR STALL AND OVER SPEED
 */
void throttle_tuning() { 
	
	static int higharray[25];
	static int lowarray[25];
	static byte itune;
	static int hightotal;
	static int lowtotal;
	analogRead(highset);
  higharray[itune] = map(analogRead(highset), 5, 1019, 130, 255); //map higher % of output range
  analogRead(lowset);
  lowarray[itune] =  map(analogRead(lowset) , 5, 1019, 20, 120); // map to lower % of range with 8% minimum cut off
  
  hightotal = hightotal + higharray[itune];
  lowtotal = lowtotal + lowarray[itune];
  highval = hightotal / 25;
  lowval = lowtotal / 25;
  itune++;
  
	if(itune >= 25) {itune = 0;}
  hightotal = hightotal - higharray[itune];
  lowtotal = lowtotal - lowarray[itune];
  //Serial.println(lowval);
   
}

// Test battery voltage
void battery_check() {
  // BATTERY TEST IS INTENDED TO GIVE AN ESTIMATE OF BATTERY CHARGE REMAINING.
  // IT'S NOT ACCURATE BUT SHOULD GIVE A ROUGH INDICATION.
  const byte ratio = 6 ;  // (15000 + 3000) / 3000; // ratio = (R1 + R2) / R2
  static int batarray[20] ;
  static byte ibat;
  static int battotal;
  static byte oldstate;
  analogRead(batsense);
  delay(1);
  batarray[ibat] = analogRead(batsense) * ratio * 0.441  ; // last val is ref voltage
  battotal = battotal + batarray[ibat];
  batval = battotal / 20;
  ibat++;
  if(ibat >= 20) { ibat = 0; }
  battotal = battotal - batarray[ibat];
  Batstate = map(batval, 2000, 2750, 0, 10);
  if(Batstate <= 0 || Batstate >= 30) { Batstate = 0; }
  if(Batstate >= 11 && Batstate <= 29) { Batstate = 10; }
  if (oldstate != Batstate) { oldstate = Batstate;setbatled = true; }
  // Serial.println(Batstate);
  // Serial.println(oldstate);
  battimer=millis()+500;

}

// BATTERY DISPLAY IS A STATE MACHINE WHICH CHANGES THE OUTPUT OF THE LED BATTERY
// INDICATOR USED WITH LEDFLASH WHICH IS A TAKE ON BLINK WITHOUT DELAY
void battery_display() {

const byte led1 = 11,led2 = 12,led3 = 8,led4 = 7,led5 = 4;

switch(Batstate) {  // Case 0: need to kill motorpower
    case 0: digitalWrite(led1,LOW); digitalWrite(led2,LOW); digitalWrite(led3,LOW); digitalWrite(led4,LOW); digitalWrite(led5,LOW); flash = false;      // 00000
      break;
    case 1:  digitalWrite(led1,HIGH); digitalWrite(led2,LOW); digitalWrite(led3,LOW); digitalWrite(led4,LOW); digitalWrite(led5,LOW); flash = true; toflash = led1;
      break; 
    case 2:  digitalWrite(led1,HIGH); digitalWrite(led2,LOW); digitalWrite(led3,LOW); digitalWrite(led4,LOW); digitalWrite(led5,LOW); flash = false;
      break;
    case 3:  digitalWrite(led1,HIGH); digitalWrite(led2,HIGH); digitalWrite(led3,LOW); digitalWrite(led4,LOW); digitalWrite(led5,LOW); flash = true; toflash = led2;
      break;
    case 4:  digitalWrite(led1,HIGH); digitalWrite(led2,HIGH); digitalWrite(led3,LOW); digitalWrite(led4,LOW); digitalWrite(led5,LOW); flash = false;
      break;
    case 5:  digitalWrite(led1,HIGH); digitalWrite(led2,HIGH); digitalWrite(led3,HIGH); digitalWrite(led4,LOW); digitalWrite(led5,LOW); flash = true; toflash = led3;
      break;
    case 6:  digitalWrite(led1,HIGH); digitalWrite(led2,HIGH); digitalWrite(led3,HIGH); digitalWrite(led4,LOW); digitalWrite(led5,LOW); flash = false;
      break;
    case 7:  digitalWrite(led1,HIGH); digitalWrite(led2,HIGH); digitalWrite(led3,HIGH); digitalWrite(led4,HIGH); digitalWrite(led5,LOW); flash = true; toflash = led4; 
      break;
    case 8:  digitalWrite(led1,HIGH); digitalWrite(led2,HIGH); digitalWrite(led3,HIGH); digitalWrite(led4,HIGH); digitalWrite(led5,LOW); flash = false; 
      break;
    case 9:  digitalWrite(led1,HIGH); digitalWrite(led2,HIGH); digitalWrite(led3,HIGH); digitalWrite(led4,HIGH); digitalWrite(led5,HIGH); flash = true;toflash = led5;
      break;
    case 10: digitalWrite(led1,HIGH); digitalWrite(led2,HIGH); digitalWrite(led3,HIGH); digitalWrite(led4,HIGH); digitalWrite(led5,HIGH); flash = false;
      break;
  }
}

void ledflash() {
  // LED BLINK WITHOUT DELAY
  ledtimer = millis() + 500;
  static byte ledstate;
  if (ledstate != LOW) { 
    ledstate = LOW;
  } else { 
    ledstate = HIGH;
  }
  digitalWrite(toflash,ledstate);
  // Serial.println(ledstate);
}
