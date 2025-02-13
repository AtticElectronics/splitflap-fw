#ifndef DEEPSLEEP_H
#define DEEPSLEEP_H

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/**
 * @brief DeepSleep 클래스는 ATmega328P 기반 Arduino 보드에서 딥 슬립 모드를 관리합니다.
 *
 * 이 클래스는 지정된 시간 동안 `alive()`가 호출되지 않으면 MCU를 딥 슬립 모드로 전환하고,
 * 지정된 핀의 외부 인터럽트 신호로 MCU를 깨우는 기능을 제공합니다. 슬립 전과
 * 깨어난 후에 호출할 수 있는 콜백 함수도 지원합니다.
 */
class DeepSleep
{
public:
    /**
     * @brief 인터럽트 트리거 모드를 정의하는 열거형
     */
    enum InterruptMode
    {
        INTERRUPT_LOW,    /**< 핀의 전압이 LOW일 때 인터럽트 트리거 */
        INTERRUPT_CHANGE, /**< 핀의 전압이 변경될 때 인터럽트 트리거 */
        INTERRUPT_RISING, /**< 핀의 전압이 LOW에서 HIGH로 상승할 때 인터럽트 트리거 */
        INTERRUPT_FALLING /**< 핀의 전압이 HIGH에서 LOW로 하강할 때 인터럽트 트리거 */
    };

    /**
     * @brief 생성자
     *
     * @param wakePin 슬립에서 깨어나는 핀 번호 (예: D2는 2)
     * @param wakeUpFunc 깨어났을 때 호출할 콜백 함수 (선택적, 기본값은 nullptr)
     * @param sleepFunc 잠들기 전에 호출할 콜백 함수 (선택적, 기본값은 nullptr)
     * @param mode 인터럽트 트리거 모드 (기본값은 INTERRUPT_LOW)
     */
    DeepSleep(uint8_t wakePin, void (*wakeUpFunc)() = nullptr, void (*sleepFunc)() = nullptr, InterruptMode mode = INTERRUPT_LOW);

    /**
     * @brief 메인 루프에서 호출하여 슬립 상태 관리
     *
     * 이 메서드는 `loop()` 함수 내에서 주기적으로 호출되어 슬립 상태를 관리합니다.
     */
    void loop();

    /**
     * @brief ISR에서 호출되도록 설정된 정적 함수
     *
     * 외부 인터럽트가 발생하면 이 함수가 호출됩니다.
     */
    static void wakeUpISR();

private:
    uint8_t wakePin;             /**< 슬립에서 깨어나는 핀 번호 */
    void (*wakeUpCallback)();    /**< 깨어났을 때 호출할 콜백 함수 */
    void (*sleepCallback)();     /**< 잠들기 전에 호출할 콜백 함수 */
    InterruptMode interruptMode; /**< 인터럽트 트리거 모드 */

    static DeepSleep *instance; /**< ISR에서 접근하기 위한 인스턴스 포인터 */

    /**
     * @brief 슬립 모드 진입
     */
    void enterSleep();

    /**
     * @brief 깨어남 처리
     */
    void handleWakeUp();

    /**
     * @brief 인터럽트 설정
     *
     * @param mode 인터럽트 트리거 모드
     */
    void configureInterrupt(InterruptMode mode);

    /**
     * @brief 인터럽트 해제
     */
    void detachWakeUpInterrupt();
};

#endif // DEEPSLEEP_H
