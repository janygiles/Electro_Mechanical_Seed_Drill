
#define encoder_input_1 2
#define encoder_input_2 3

#define motor_output_1 5
#define motor_output_2 4

int Pos;
unsigned int State;
unsigned int LastState;
unsigned int current_time;
unsigned int previous_time;
unsigned int elapsed_time;
float current_value;
float previous_value;
float elapsed_value;
float rpm;
unsigned int counter =0;
int motor_speed;
int reduced_rpm;

void setup() {
  Serial.begin(9600);
  pinMode(encoder_input_1,INPUT);
  pinMode(encoder_input_2,INPUT);
  pinMode(motor_output_1,OUTPUT);
  pinMode(motor_output_2,OUTPUT);

  digitalWrite(encoder_input_1, HIGH);
  digitalWrite(encoder_input_2, HIGH);

  attachInterrupt(0,updateEncoder,HIGH);
  
  LastState = digitalRead(encoder_input_1);

  previous_time = 0;
  previous_value = 0;
}

void loop() {
  // Serial.println(counter);
  current_time = millis();
  current_value = counter;
  elapsed_time = current_time - previous_time;
  
  if (elapsed_time >= 500) {
    elapsed_value = current_value - previous_value;
    rpm = (elapsed_value / 1200) * 120;
    Serial.println(rpm);
    previous_value = current_value;
    previous_time = current_time;
  }
  
  reduced_rpm = rpm * 0.75;
  motor_speed = map(reduced_rpm, 0, 95, 0, 255);
  analogWrite(motor_output_1, motor_speed);
  digitalWrite(motor_output_2, HIGH);
}

void updateEncoder(){
   State = digitalRead(encoder_input_1);
   if(State!=LastState){
   if(digitalRead(encoder_input_2)!=State){
   counter++;
}
} 
    LastState = State;
}







