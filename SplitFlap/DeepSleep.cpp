#include "DeepSleep.h"

// 정적 멤버 초기화
DeepSleep *DeepSleep::instance = nullptr;

// 생성자
DeepSleep::DeepSleep(uint8_t wakePin,
                     void (*wakeUpFunc)(),
                     void (*sleepFunc)(),
                     InterruptMode mode) // 변경됨: normalInterruptFunc 제거
    : wakePin(wakePin),
      wakeUpCallback(wakeUpFunc),
      sleepCallback(sleepFunc),
      interruptMode(mode)
{ // 변경됨: normalInterruptFlag 제거
    // 슬립에서 깨어날 핀을 입력 풀업으로 설정
    pinMode(wakePin, INPUT_PULLUP);

    // ISR을 위한 인스턴스 포인터 설정
    instance = this;

    // 인터럽트 허용 (전역 인터럽트 활성화)
    sei();
    configureInterrupt(interruptMode);
}

// 메인 루프에서 호출하여 슬립 상태 관리
void DeepSleep::loop()
{
    if (digitalRead(wakePin) == HIGH)
    {
        // 슬립모드 진입
        enterSleep();
    }
}

// 슬립 모드 진입
void DeepSleep::enterSleep()
{
    if (sleepCallback)
    {
        sleepCallback();
    }
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    // 자는중

    // 깨어남 처리
    sleep_disable();
    if (wakeUpCallback)
    {
        wakeUpCallback();
    }
}

// ISR에서 호출되는 정적 함수
void DeepSleep::wakeUpISR()
{
}

// 인터럽트 설정
void DeepSleep::configureInterrupt(InterruptMode mode)
{
    switch (mode)
    {
    case INTERRUPT_LOW:
        attachInterrupt(digitalPinToInterrupt(wakePin), DeepSleep::wakeUpISR, LOW);
        break;
    case INTERRUPT_CHANGE:
        attachInterrupt(digitalPinToInterrupt(wakePin), DeepSleep::wakeUpISR, CHANGE);
        break;
    case INTERRUPT_RISING:
        attachInterrupt(digitalPinToInterrupt(wakePin), DeepSleep::wakeUpISR, RISING);
        break;
    case INTERRUPT_FALLING:
        attachInterrupt(digitalPinToInterrupt(wakePin), DeepSleep::wakeUpISR, FALLING);
        break;
    }
}

// 인터럽트 해제
void DeepSleep::detachWakeUpInterrupt()
{
    detachInterrupt(digitalPinToInterrupt(wakePin));
}
