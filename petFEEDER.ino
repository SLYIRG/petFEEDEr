#include <DS3231.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8, 9 };
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

RTC_DS3231 rtc;
Servo servo_test;
LiquidCrystal lcd(A0, A1, A2, 11, 12, 13);

int feedHour = 0;
int feedMinute = 0;
boolean feed = true;
char key;
int r[6];

void setup() {
  servo_test.attach(10);
  Wire.begin();
  rtc.begin();
  lcd.begin(16, 2);
  servo_test.write(55);
  Serial.begin(9600);
  pinMode(A3, INPUT);  // 设置A3为输入
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  // 初始化 r 数组
  for (int i = 0; i < 6; i++) {
    r[i] = 0;
  }
}

void loop() {
  lcd.setCursor(0, 0);
  int buttonPress;
  buttonPress = analogRead(A3) > 500; // 使用阈值500，你可能需要根据实际情况调整

  if (buttonPress == 1)
    setFeedingTime();

  lcd.print("Time:  ");
  DateTime now = rtc.now();
  int t1 = now.hour();
  int t2 = now.minute();
  int t3 = now.second();

  // 显示小时、分钟和秒
  lcd.print(String(t1));
  lcd.print(":");
  lcd.print(t2 < 10 ? "0" : "");
  lcd.print(String(t2));
  lcd.print(":");
  lcd.print(t3 < 10 ? "0" : "");
  lcd.print(String(t3));

  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());

  // Debugging output
  Serial.print("Current Time: ");
  Serial.print(t1);
  Serial.print(":");
  Serial.print(t2);
  Serial.print(":");
  Serial.println(t3);

  Serial.print("User-set Time: ");
  Serial.print(r[0]);
  Serial.print(":");
  Serial.println(r[1]);

  Serial.print("Feed Status: ");
  Serial.println(feed);

  // Adjusted time comparison
  if (t1 == r[0] && t2 == r[1] && feed == true) {
    feedPet(); // 在特定时间触发喂食
    feed = false; // 防止在同一时间多次触发喂食
  }
}

void setFeedingTime() {
  feed = true;
  int i = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set feeding Time");
  lcd.print("HH:MM");
  lcd.setCursor(0, 1);

  int j = 0;

  while (1) {
    key = kpd.getKey();

    if (key != NO_KEY) {
      lcd.setCursor(j, 1);
      lcd.print(key);
      r[i] = key - '0';
      i++;
      j++;

      if (j == 2) {
        lcd.print(":");
        j = 3;
      }

      delay(500);
    }

    if (key == 'D') {
      key = 0;
      break;
    }
  }

  // 打印用户输入的时间
  Serial.print("User input time: ");
  Serial.print(r[0]);
  Serial.print(":");
  Serial.println(r[1]);

  // 这里你可以加入其他逻辑，比如保存用户输入的时间

  lcd.clear();
}

void feedPet() {
  // 在这里添加喂食的逻辑
servo_9.attach(9, 500, 2500);
  // sweep the servo from 0 to 180 degrees in steps
  // of 1 degrees
  for (pos = 0; pos <= 180; pos += 1) {
    // tell servo to go to position in variable 'pos'
    servo_9.write(pos);
    // wait 15 ms for servo to reach the position
    delay(10); // Wait for 10 millisecond(s)
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    // tell servo to go to position in variable 'pos'
    servo_9.write(pos);
    // wait 15 ms for servo to reach the position
    delay(1); // Wait for 1 millisecond(s)

  }
