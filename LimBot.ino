
//////////////////////////      ძრავების სამართავი პინები    ///////////////////////

// ვაერთებთ H-bridge მოდულთან
#define motor1A 3  // მოდულის IN2 პინი
#define motor1B 9  // მოდულის IN1 პინი
#define motor2A 10  // მოდულის IN4 პინი
#define motor2B 11  // მოდულის IN3 პინი
////////////////////////////////////////////////////

String command, speedString;
char index;
byte speed, angle;
byte direction = 2;
boolean stop = true;

//////////////////////////////  ფუნქციები  /////////////////////////

void initialize();  // საწყისი პარამეტრების მითითების ფუნქცია
void controlMotors();  // ძრავების მართვის ფუნქცია
void forward();  // წინ წასვლის ფუნქცია
void backward(); // უკან წასვლა
void left();  // მარცხნივ მოხვევა
void right();  // მარჯვნივ მოხვევა
/////////////////////////////////////////////////////////////////////

void setup() {
  initialize();
}

void loop()
{
  if (Serial.available() > 0) // თუ სერიალური კავშირი დამყარებულია
  {
    command = Serial.readString();    // სერიალური კავშირით String ტიპის ინფორმაციის წაკითხვა Bluetooth მოდულიდან
    Serial.println(command);    // წაკითხული ინფორმაციის Serial Monitor -ში ბეჭდვა
    index = command.charAt(0);    // ინფორმაციის პირველი სიმბოლოს შენახვა, გვიჩვენებს მიმართულებას
    speedString = command.substring(1, command.length());    // დანარჩენი სიმბოლოები წარმოადგენენ სიჩქარის მნიშვნელობას

    controlMotors();
  }
}

void initialize() {

  // H-bridge -ში შემავალი პინების გამოცხადება გამომავალ პინებად
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);

  // სერიალური კავშირის დაწყება
  Serial.begin(9600);
  // ინფორმაციის მიღების ლოდინის პერიოდი მილიწამებში
  Serial.setTimeout(5);
  // ძრავების გაჩერება
  analogWrite(motor1A, 0);
  analogWrite(motor1B, 0);
  analogWrite(motor2A, 0);
  analogWrite(motor2B, 0);

  // ძრავების სამართავ პინებზე PWM -ის სიხშირის დაყენება 31250 ჰერცზე
  TCCR1B = (TCCR1B & 0b11111000) | 0x01;
  TCCR2B = (TCCR2B & 0b11111000) | 0x01;
}

void controlMotors() {

  switch (index) {

    case 'f':
      forward();
      direction = 1;
      stop = false;
      break;
    case 'b':
      backward();
      direction = 0;
      stop = false;
      break;
    case 'l':
      if (!stop) {
        if (speed != 0) {
          angle = speedString.toInt();   // მობრუნების კუთხის მნიშვნელობა
          left();
        }
      }
      break;
    case 'r':
      if (!stop) {
        if (speed != 0) {
          angle = speedString.toInt();
          right();
        }
      }
      break;
    case 'v':
      speed = speedString.toInt();   // სიჩქარის მნიშვნელობა
      if (speed == 0) {
        stop = true;
        forward();
      }
      if (!stop) {
        if (direction == 1) {
          forward();
        } else if (direction == 0) {
          backward();
        }
      }
      break;
    case 's':
      if (!stop) {
        if (direction == 1) {
          forward();
        } else if (direction == 0) {
          backward();
        }
      }
  }
}

void left()
{
  if (direction) {            // თუ წინ ვუხვევთ
    analogWrite(motor1A, speed);  // დაატრიალე ძრავა speed სიჩქარით
    analogWrite(motor2A, speed - angle);   // დაატრიალე ძრავა speed - angle სიჩქარით
    analogWrite(motor1B, 0);    // შესაბამისად, რობოტი დაიწყებს მარცხნივ მოხვევას
    analogWrite(motor2B, 0);
  } else {                    // თუ უკან ვუხვევთ
    analogWrite(motor1A, 0);
    analogWrite(motor2A, 0);
    analogWrite(motor1B, speed);
    analogWrite(motor2B, speed - angle);
  }
}

void right()
{
  if (direction) {
    analogWrite(motor1A, speed - angle);
    analogWrite(motor2A, speed);
    analogWrite(motor1B, 0);
    analogWrite(motor2B, 0);
  } else {
    analogWrite(motor1A, 0);
    analogWrite(motor2A, 0);
    analogWrite(motor1B, speed - angle);
    analogWrite(motor2B, speed);
  }
}

void forward()
{
  analogWrite(motor1A, speed);
  analogWrite(motor2A, speed);
  analogWrite(motor1B, 0);
  analogWrite(motor2B, 0);

}

void backward()
{
  analogWrite(motor1B, speed);
  analogWrite(motor2B, speed);
  analogWrite(motor1A, 0);
  analogWrite(motor2A, 0);
}

