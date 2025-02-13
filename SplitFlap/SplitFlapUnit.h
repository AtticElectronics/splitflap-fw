#ifndef SPLITFLAPUNIT_H
#define SPLITFLAPUNIT_H

#include <Arduino.h>
#include <Stepper.h>

class SplitFlapUnit
{
public:
    // 생성자 선언
    SplitFlapUnit(int spr, int in1, int in2, int in3, int in4, int switch_pin);

    // bias 설정
    void setBias(int bias);
    void setSpeed(int speed);

    // 스위치 엣지까지 회전하고, bias만큼 이동하여 해당 위치를 기준으로 설정
    void init();

    // index 까지 회전, 회전 중 스위치 엣지감지된다면, 기준 갱신
    void rotateToIndex(int index);
    void rotateTest(int index);

private:
    Stepper motor; // Stepper 객체

    const int in1, in2, in3, in4; // 모터 핀 번호

    int PULLUP_BUTTON_PIN; // 스위치 핀 번호
    int SPR;               // 스텝 갯수
    int CARD_CNT = 64;     // 카드 갯수

    int CARD_STEP; // 카드 한장당 스텝
    int BIAS = 0;  // Bias 값 (초기값 0)

    int current_steps = -1; // 현재 스텝
    bool last_switch_state; // 마지막 스위치 상태
    void _rotateToFallingEdge();
    bool _isSwitchFallingEdge(); // HIGH -> LOW 검출 (폴링 검출)
    // 스텝만큼 회전
    void _rotateSteps(int steps);
};

#endif // SPLITFLAPUNIT_H
