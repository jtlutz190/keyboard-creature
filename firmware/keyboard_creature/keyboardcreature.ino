#include <Arduino.h>
#include <ESP32Servo.h>

// Outputs
#define DC_MOTOR_PIN 18
#define SERVO_PIN    27
#define BUZZER_PIN   33

// Inputs
#define BUTTON_PIN   4
#define TRIG_PIN     14
#define ECHO_PIN     35

// SN74HC595
#define DATA_PIN     19
#define CLOCK_PIN    25
#define LATCH_PIN    32

#define DISTANCE_LIMIT_CM 50

#define IDLE_BLINK_INTERVAL_MS 500
#define SELECTION_TIMEOUT_MS   4000
#define DISTANCE_READ_MS       100
#define BUTTON_BEEP_MS         80
#define FINISH_MOTOR_MS        4500
#define CELEBRATION_LED_MS     90

enum StudyState {
  IDLE,
  SELECT_TIME,
  COUNTDOWN,
  FINISHED
};

StudyState state = IDLE;
Servo fanServo;

const int optionCount = 4;
const int optionLEDs[optionCount] = {2, 4, 6, 8};
const unsigned long optionTimesMs[optionCount] = {300000, 600000, 900000, 1200000};

int selectedOption = -1;

bool idleBlinkState = false;
bool buzzerActive = false;

unsigned long countdownTotalMs = 0;
unsigned long countdownRemainingMs = 0;

unsigned long lastIdleBlinkTime = 0;
unsigned long lastSelectionPressTime = 0;
unsigned long lastCountdownTickTime = 0;
unsigned long lastDistanceReadTime = 0;
unsigned long buzzerStartTime = 0;
unsigned long finishedStartTime = 0;
unsigned long lastCelebrationLEDTime = 0;

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long lastButtonChangeTime = 0;
const unsigned long debounceDelayMs = 40;

float distanceCm = 999;
bool personPresent = false;

// Completion tune
const int melodyLength = 13;
const int melodyFreq[melodyLength] = {
  523, 659, 784, 1047,
  784, 1047, 1319, 0,
  988, 1047, 1175, 1319, 1568
};

const int melodyDur[melodyLength] = {
  120, 120, 120, 220,
  120, 120, 260, 80,
  120, 120, 120, 120, 360
};

int melodyIndex = 0;
unsigned long noteStartTime = 0;
bool tuneDone = false;

// LED celebration patterns
const byte celebrationPatterns[] = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001,
  0b11111111,
  0b00000000,
  0b10101010,
  0b01010101,
  0b11111111
};

const int celebrationPatternCount =
  sizeof(celebrationPatterns) / sizeof(celebrationPatterns[0]);

int celebrationStep = 0;

void writeLEDs(byte value) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, value);
  digitalWrite(LATCH_PIN, HIGH);
}

byte ledCountToPattern(int ledCount) {
  byte pattern = 0;

  for (int i = 0; i < ledCount; i++) {
    pattern |= (1 << i);
  }

  return pattern;
}

void showLEDCount(int ledCount) {
  writeLEDs(ledCountToPattern(ledCount));
}

void startButtonBeep() {
  if (state == FINISHED) return;

  buzzerActive = true;
  buzzerStartTime = millis();
  ledcWriteTone(BUZZER_PIN, 2200);
}

void updateBuzzer() {
  if (state == FINISHED) return;

  if (buzzerActive && millis() - buzzerStartTime >= BUTTON_BEEP_MS) {
    buzzerActive = false;
    ledcWriteTone(BUZZER_PIN, 0);
  }
}

float readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return 999;
  }

  return duration * 0.0343 / 2.0;
}

bool buttonPressedEvent() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonChangeTime = millis();
    lastButtonReading = reading;
  }

  if (millis() - lastButtonChangeTime > debounceDelayMs) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      if (stableButtonState == LOW) {
        return true;
      }
    }
  }

  return false;
}

void updateDistance() {
  unsigned long now = millis();

  if (now - lastDistanceReadTime >= DISTANCE_READ_MS) {
    lastDistanceReadTime = now;

    distanceCm = readDistanceCM();
    personPresent = distanceCm <= DISTANCE_LIMIT_CM;
  }
}

void fanOff() {
  fanServo.write(90);
}

void fanOn() {
  fanServo.write(180);
}

void enterIdle() {
  state = IDLE;

  selectedOption = -1;
  countdownTotalMs = 0;
  countdownRemainingMs = 0;
  idleBlinkState = false;
  buzzerActive = false;

  digitalWrite(DC_MOTOR_PIN, LOW);
  fanOff();
  ledcWriteTone(BUZZER_PIN, 0);
  writeLEDs(0x00);

  Serial.println("State: IDLE");
}

void enterSelection() {
  state = SELECT_TIME;

  selectedOption = 0;
  lastSelectionPressTime = millis();

  digitalWrite(DC_MOTOR_PIN, LOW);
  fanOff();

  showLEDCount(optionLEDs[selectedOption]);

  Serial.println("State: SELECT_TIME | 5 seconds");
}

void advanceSelection() {
  selectedOption++;

  if (selectedOption >= optionCount) {
    selectedOption = 0;
  }

  lastSelectionPressTime = millis();
  showLEDCount(optionLEDs[selectedOption]);

  Serial.print("Selected: ");
  Serial.print(optionTimesMs[selectedOption] / 1000);
  Serial.println(" seconds");
}

void startCountdown() {
  state = COUNTDOWN;

  countdownTotalMs = optionTimesMs[selectedOption];
  countdownRemainingMs = countdownTotalMs;
  lastCountdownTickTime = millis();

  writeLEDs(0xFF);

  Serial.print("State: COUNTDOWN | ");
  Serial.print(countdownTotalMs / 1000);
  Serial.println(" seconds");
}

void enterFinished() {
  state = FINISHED;

  finishedStartTime = millis();
  lastCelebrationLEDTime = 0;
  celebrationStep = 0;

  melodyIndex = 0;
  noteStartTime = 0;
  tuneDone = false;
  buzzerActive = false;

  fanOff();
  digitalWrite(DC_MOTOR_PIN, HIGH);
  writeLEDs(0xFF);

  Serial.println("State: FINISHED | Celebration");
}

void updateIdle() {
  unsigned long now = millis();

  if (now - lastIdleBlinkTime >= IDLE_BLINK_INTERVAL_MS) {
    lastIdleBlinkTime = now;
    idleBlinkState = !idleBlinkState;
    writeLEDs(idleBlinkState ? 0xFF : 0x00);
  }
}

void updateSelection() {
  if (millis() - lastSelectionPressTime >= SELECTION_TIMEOUT_MS) {
    startCountdown();
  }
}

void updateCountdown() {
  unsigned long now = millis();

  if (personPresent) {
    fanOn();

    unsigned long elapsed = now - lastCountdownTickTime;

    if (elapsed >= countdownRemainingMs) {
      countdownRemainingMs = 0;
    } else {
      countdownRemainingMs -= elapsed;
    }

    lastCountdownTickTime = now;
  } else {
    fanOff();
    lastCountdownTickTime = now;
  }

  int ledsToShow = 0;

  if (countdownRemainingMs > 0) {
    ledsToShow = (countdownRemainingMs * 8 + countdownTotalMs - 1) / countdownTotalMs;
  }

  showLEDCount(ledsToShow);

  if (countdownRemainingMs == 0) {
    enterFinished();
  }
}

void updateCelebrationLEDs() {
  unsigned long now = millis();

  if (now - lastCelebrationLEDTime >= CELEBRATION_LED_MS) {
    lastCelebrationLEDTime = now;

    writeLEDs(celebrationPatterns[celebrationStep]);

    celebrationStep++;
    if (celebrationStep >= celebrationPatternCount) {
      celebrationStep = 0;
    }
  }
}

void updateFinishTune() {
  if (tuneDone) return;

  unsigned long now = millis();

  if (melodyIndex >= melodyLength) {
    tuneDone = true;
    ledcWriteTone(BUZZER_PIN, 0);
    return;
  }

  if (noteStartTime == 0) {
    noteStartTime = now;

    if (melodyFreq[melodyIndex] > 0) {
      ledcWriteTone(BUZZER_PIN, melodyFreq[melodyIndex]);
    } else {
      ledcWriteTone(BUZZER_PIN, 0);
    }
  }

  unsigned long elapsed = now - noteStartTime;
  unsigned long noteDuration = melodyDur[melodyIndex];

  if (elapsed > noteDuration * 0.8) {
    ledcWriteTone(BUZZER_PIN, 0);
  }

  if (elapsed >= noteDuration) {
    melodyIndex++;
    noteStartTime = 0;
  }
}

void updateFinished() {
  updateCelebrationLEDs();
  updateFinishTune();

  bool motorDone = millis() - finishedStartTime >= FINISH_MOTOR_MS;

  if (motorDone && tuneDone) {
    enterIdle();
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(DC_MOTOR_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  fanServo.attach(SERVO_PIN, 500, 2400);
  fanOff();

  ledcAttach(BUZZER_PIN, 2200, 8);
  ledcWriteTone(BUZZER_PIN, 0);

  enterIdle();
}

void loop() {
  updateDistance();
  updateBuzzer();

  bool pressed = buttonPressedEvent();

  if (pressed) {
    startButtonBeep();

    if (state == IDLE) {
      enterSelection();
    } else if (state == SELECT_TIME) {
      advanceSelection();
    }
  }

  if (state == IDLE) {
    updateIdle();
  } else if (state == SELECT_TIME) {
    updateSelection();
  } else if (state == COUNTDOWN) {
    updateCountdown();
  } else if (state == FINISHED) {
    updateFinished();
  }
}
