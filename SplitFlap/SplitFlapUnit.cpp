#include "SplitFlapUnit.h"

// 생성자 정의
SplitFlapUnit::SplitFlapUnit(int spr, int in1, int in2, int in3, int in4, int switch_pin)
    : PULLUP_BUTTON_PIN(switch_pin),
      in1(in1), in2(in2), in3(in3), in4(in4), SPR(spr),
      motor(spr, in1, in3, in2, in4) // Stepper 객체 초기화
{
    pinMode(PULLUP_BUTTON_PIN, INPUT_PULLUP);
    last_switch_state = digitalRead(PULLUP_BUTTON_PIN);
}

// 모터 속도 설정 setter
void SplitFlapUnit::setSpeed(int speed)
{
    motor.setSpeed(speed);
}

// bias 값을 설정 setter
void SplitFlapUnit::setBias(int bias)
{
    this->BIAS = bias;
}

void SplitFlapUnit::init()
{
    CARD_STEP = SPR / CARD_CNT;
    _rotateToFallingEdge();
    _rotateSteps(BIAS);
    // 모터 전원 차단
    delay(10);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void SplitFlapUnit::rotateTest(int index)
{
    motor.step(index);
    motor.step(index);
    delay(10);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}
void SplitFlapUnit::rotateToIndex(int index)
{

    int one_index = index % CARD_CNT;
    // 기준점으로 부터 목표까지 스텝
    int abs_target_steps = BIAS + (CARD_STEP * one_index);
    abs_target_steps = abs_target_steps % SPR;

    // 현재스텝에서 목표스텝까지 스텝
    int steps = abs_target_steps - current_steps;
    if (steps < 0)
        steps += SPR;
    // 기준에서 현재스텝 지나 목표스텝까지 스텝
    int temp_steps = current_steps + steps;

    // 0에서 63이하
    if (index < CARD_CNT)
    {
        // 현재 스텝에서 목표 스텝까지 계산
        _rotateSteps(steps);
    }
    else
    {
        int times = index / CARD_CNT;
        // index를 CARD_CNT로 나누어지는 몫 만큼_rotateToFallingEdge() 반복
        for (int i = 0; i < times; i++)
        {
            _rotateToFallingEdge();
        }

        _rotateSteps(temp_steps);
    }
    delay(10);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    digitalWrite
}
// 강제 스텝이동
void SplitFlapUnit::_rotateSteps(int steps)
{
    motor.step(steps);
    current_steps = (current_steps + steps) % SPR;
}

// 스위치 하강 엣지까지 회전
void SplitFlapUnit::_rotateToFallingEdge()
{
    while (true)
    {
        motor.step(1);
        if (_isSwitchFallingEdge())
        {
            current_steps = 0;
            break;
        }
    }
}

// HIGH -> LOW 하강 엣지 검출
bool SplitFlapUnit::_isSwitchFallingEdge()
{
    // 현재 스위치 상태 읽기
    bool current_switch_state = digitalRead(PULLUP_BUTTON_PIN);
    // 하강 엣지 확인
    if (last_switch_state == HIGH && current_switch_state == LOW)
    {
        last_switch_state = current_switch_state; // 상태 업데이트
        return true;
    }
    // 상태 업데이트
    last_switch_state = current_switch_state;
    return false;
}
