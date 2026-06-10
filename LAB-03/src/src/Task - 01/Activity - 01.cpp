#include <avr/io.h>
#include <avr/interrupt.h>

const int ROAD_RED = 8;
const int ROAD_YELLOW = 9;
const int ROAD_GREEN = 10;
const int PED_RED = 11;
const int PED_GREEN = 12;

volatile bool emergencyFlag = false;
volatile bool pedestrianFlag = false;
volatile bool maintenanceFlag = false;

volatile unsigned long lastInterruptTime = 0;
const unsigned long DEBOUNCE_DELAY = 250;

void setup() {
  pinMode(ROAD_RED, OUTPUT);
  pinMode(ROAD_YELLOW, OUTPUT);
  pinMode(ROAD_GREEN, OUTPUT);
  pinMode(PED_RED, OUTPUT);
  pinMode(PED_GREEN, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

  clearAllLEDs();

  cli();

  EICRA |= (1 << ISC01) | (1 << ISC11); 
  EICRA &= ~((1 << ISC00) | (1 << ISC10));
  EIMSK |= (1 << INT0) | (1 << INT1);

  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT20);

  sei();
}

void loop() {
  if (emergencyFlag) {
    handleEmergency();
  }
  else if (pedestrianFlag) {
    handlePedestrian();
  }
  else if (maintenanceFlag) {
    handleMaintenance();
  }
  else {
    digitalWrite(ROAD_GREEN, HIGH);
    digitalWrite(PED_RED, HIGH);
    digitalWrite(ROAD_RED, LOW);
    digitalWrite(PED_GREEN, LOW);
    controlledDelay(5000);

    if (emergencyFlag || pedestrianFlag || maintenanceFlag) return;

    digitalWrite(ROAD_GREEN, LOW);
    digitalWrite(PED_RED, LOW);
    digitalWrite(ROAD_RED, HIGH);
    digitalWrite(PED_GREEN, HIGH);
    controlledDelay(5000);
  }
}

ISR(INT0_vect) {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > DEBOUNCE_DELAY) {
    emergencyFlag = true;
    pedestrianFlag = false;
    maintenanceFlag = false;
    lastInterruptTime = interruptTime;
  }
}

ISR(INT1_vect) {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > DEBOUNCE_DELAY) {
    if (!emergencyFlag) {
      pedestrianFlag = true;
      maintenanceFlag = false;
    }
    lastInterruptTime = interruptTime;
  }
}

ISR(PCINT2_vect) {
  if ((PIND & (1 << PIND4)) == 0) {
    unsigned long interruptTime = millis();
    if (interruptTime - lastInterruptTime > DEBOUNCE_DELAY) {
      if (!emergencyFlag && !pedestrianFlag) {
        maintenanceFlag = true;
      }
      lastInterruptTime = interruptTime;
    }
  }
}

void handleEmergency() {
  clearAllLEDs();
  digitalWrite(ROAD_GREEN, HIGH);
  digitalWrite(PED_RED, HIGH);
  delay(10000);
  emergencyFlag = false;
}

void handlePedestrian() {
  digitalWrite(ROAD_YELLOW, HIGH);
  delay(5000);

  clearAllLEDs();
  digitalWrite(ROAD_RED, HIGH);
  digitalWrite(PED_GREEN, HIGH);
  delay(10000);
  
  pedestrianFlag = false;
}

void handleMaintenance() {
  clearAllLEDs();
  while (maintenanceFlag) {
    digitalWrite(ROAD_YELLOW, HIGH);
    delay(500);
    digitalWrite(ROAD_YELLOW, LOW);
    delay(500);

    if (emergencyFlag || pedestrianFlag) {
      maintenanceFlag = false;
      break;
    }
  }
}

void controlledDelay(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (emergencyFlag || pedestrianFlag || maintenanceFlag) {
      break;
    }
    delay(10);
  }
}

void clearAllLEDs() {
  digitalWrite(ROAD_RED, LOW);
  digitalWrite(ROAD_YELLOW, LOW);
  digitalWrite(ROAD_GREEN, LOW);
  digitalWrite(PED_RED, LOW);
  digitalWrite(PED_GREEN, LOW);
}