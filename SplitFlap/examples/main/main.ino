#include "SplitFlapUnit.h"
#include "DeepSleep.h"

#define BIAS 500      // 기본 바이어스 값
#define CARD_COUNT 64 // 카드 갯수
#define SPEED 20      // 모터 속도 (RPM 단위)

#define SPR 2048 // 모터 스텝 갯수 (steps per revolution)
#define IN1 10
#define IN2 9
#define IN3 8
#define IN4 7

#define SWITCH_PIN 4 // 다른 핀 번호로 변경

#define ID_DIP_SWITCH_0 3
#define ID_DIP_SWITCH_1 5
#define ID_DIP_SWITCH_2 6
#define ID_DIP_SWITCH_3 A4
#define ID_DIP_SWITCH_4 A3
#define ID_DIP_SWITCH_5 A2

#define POTENTIOMETER_PIN A0

int bias;
uint8_t device_id;

uint8_t packet[2] = {0x00, 0x00};
uint8_t index = 0;

// 깨어났을 때 호출될 콜백 함수
void onWakeUp()
{
    index = 0;
    digitalWrite(11, LOW);
}

// 잠들기 전에 호출될 콜백 함수
void onSleep()
{
    index = 0;
    digitalWrite(11, HIGH);
}

// 스플릿 플랩 유닛 인스턴스 생성
SplitFlapUnit unit(SPR, IN1, IN2, IN3, IN4, SWITCH_PIN);

// DeepSleep 클래스 인스턴스 생성 (D2 핀 사용)
// INTERRUPT_RISING, INTERRUPT_FALLING, INTERRUPT_CHANGE, INTERRUPT_LOW 가능
DeepSleep deepSleepManager(2, onWakeUp, onSleep, DeepSleep::INTERRUPT_FALLING);

void setup()
{
    // 11번핀을 출력핀으로 설정
    pinMode(11, OUTPUT);
    digitalWrite(11, LOW);

    Serial.begin(115200);
    delay(500);

    // 핀 모드 설정
    pinMode(ID_DIP_SWITCH_0, INPUT_PULLUP);
    pinMode(ID_DIP_SWITCH_1, INPUT_PULLUP);
    pinMode(ID_DIP_SWITCH_2, INPUT_PULLUP);
    pinMode(ID_DIP_SWITCH_3, INPUT_PULLUP);
    pinMode(ID_DIP_SWITCH_4, INPUT_PULLUP);
    pinMode(ID_DIP_SWITCH_5, INPUT_PULLUP);

    // ID 결정
    device_id = digitalRead(ID_DIP_SWITCH_0) +
                digitalRead(ID_DIP_SWITCH_1) * 2 +
                digitalRead(ID_DIP_SWITCH_2) * 4 +
                digitalRead(ID_DIP_SWITCH_3) * 8 +
                digitalRead(ID_DIP_SWITCH_4) * 16 +
                digitalRead(ID_DIP_SWITCH_5) * 32;

    // 가변저항 값 읽고, -200~200 범위로 변환
    int potentiometer_value = analogRead(POTENTIOMETER_PIN);
    potentiometer_value = map(potentiometer_value, 0, 1023, -40, 40);
    bias = BIAS + potentiometer_value;

    // 모터 설정
    unit.setBias(bias);
    unit.setSpeed(SPEED); // 모터 속도 설정 (RPM 단위)
    unit.init();          // 초기화
}

void loop()
{
    deepSleepManager.loop();

    if (Serial.available() > 0)
    {
        uint8_t incomingByte = Serial.read();
        if (index < 2)
        {
            packet[index] = incomingByte;
            index++;
            if (index == 2)
            {
                if (packet[0] == device_id)
                {
                    // 자신의 ID와 일치하면
                    unit.rotateToIndex(packet[1]);
                    //unit.rotateTest(packet[1]);
                }
                else
                {
                    Serial.write(packet[0]);
                    Serial.write(packet[1]);
                }
                index = 0;
            }
        }
    }
}
