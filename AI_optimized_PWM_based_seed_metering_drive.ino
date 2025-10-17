// Pin definitions
#define ENCODER_A 2
#define ENCODER_B 3
#define MOTOR_PWM 5
#define MOTOR_DIR 4

// Variables for encoder and timing
volatile long encoderCount = 0;
unsigned long prevMicros = 0;
float rpm = 0;
int motorSpeed = 0;

// Constants
const int PULSES_PER_REV = 1200;     // Encoder pulses per revolution
const unsigned long SAMPLE_INTERVAL = 500000; // 500 ms in microseconds

void setup() {
  Serial.begin(9600);

  // Set pin modes
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);

  // Interrupt on rising edge for encoder A
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), updateEncoder, RISING);

  digitalWrite(MOTOR_DIR, HIGH);
}

void loop() {
  unsigned long currentMicros = micros();
  
  // Calculate RPM every 500 ms
  if (currentMicros - prevMicros >= SAMPLE_INTERVAL) {
    noInterrupts(); // Prevent interrupt changes during calculation
    long countSnapshot = encoderCount;
    encoderCount = 0; // Reset counter for next interval
    interrupts();

    // Convert pulses to revolutions per minute
    rpm = (countSnapshot * 60.0 * 1000000.0) / (PULSES_PER_REV * SAMPLE_INTERVAL);

    // Adjust and constrain RPM
    float reducedRPM = rpm * 0.75;
    reducedRPM = constrain(reducedRPM, 0, 100);

    // Map RPM to PWM for motor speed control
    motorSpeed = map(reducedRPM, 0, 100, 0, 255);
    analogWrite(MOTOR_PWM, motorSpeed);

    Serial.print("RPM: ");
    Serial.print(rpm);
    Serial.print("\tMotor PWM: ");
    Serial.println(motorSpeed);

    prevMicros = currentMicros;
  }
}

// Interrupt Service Routine (ISR) for encoder
void updateEncoder() {
  int A = digitalRead(ENCODER_A);
  int B = digitalRead(ENCODER_B);

  // Determine rotation direction
  if (A == B) encoderCount++;
  else encoderCount--;
}
