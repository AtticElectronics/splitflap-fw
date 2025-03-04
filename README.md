# splitflap-fw
## 목적

판매중인 디스플레이가 비싸서, 최대한 싸게 만들어 보자!!

### 제작방법

1. 이 저장소의 자료를 다운받아 구조물을 출력한다
2. 다음 부품들(아래 사진 참고)을 준비하여 함께 조립한다. 
3. 이 저장소의 자료중 아두이노 라이브러리와 회로 제작파일(거버, BOM, Pick&Place)이 있다. 회로를 제작하여 예제폴더의 main.ino를 ISP업로드해준다.
4. 펌웨어를 넣은 회로에 아래사진의 노랑색 전선부분에 LOW를 주고, uart로 [0x01,0x00]를 보내본다. ID가 1인 장치에 카드가 빙글빙글 촤르르 돌다가 0번째카드에 멈춘다.
   
### 3D파일에 관하여

* 파일은 bambustudio에서 열면 잘보입니다.
* 멀티칼라 프린터로 출력했습니다.
* 카드는 0.48T로 모델링되어있으며, 프린터의 출력옵션 중 적층높이는 0.16mm로 설정하여 출력하였습니다.

![19](https://github.com/user-attachments/assets/042c8f40-31cf-4845-947a-56d1f3eb404b)
![18](https://github.com/user-attachments/assets/e06d69c2-7e17-4adc-8fc9-3cda1ab0eaa7)


### 회로 핀 설명

| 실크 | 설명 | 비고 |  |
| --- | --- | --- | --- |
| GND | GND |  |  |
| 12V | 12V~17V |  |  |
| RX | 데이터 수신 | Baudrates : 115200  | 프로토콜 설명 참고 |
| D2 | LOW: 활성화
HIGH:딥슬립 |  |  |
| TX | 데이터 송신 | Baudrates : 115200  | 프로토콜 설명 참고 |
| D11 | 장치가 활성화될 때 LOW출력 | 장치가 딥슬립 직전에 HIGH로변경됨 |  |
| DIP SWITCH | ID값 | 부팅 시 값 읽음 |  |
| 가변저항 | BIAS값 설정 | 첫번째 카드 미세조정, 부팅시 값 읽음 |  |

### 프로토콜 설명

RX로 2바이트 수신을 기다린다

2바이트는  [ID: 1바이트 , IINDEX: 1바이트]로 구성되어있다.

- ID

ID가 해당 회로의 DIP 스위치와 일치하면, INDEX만큼 모터를 회전시킨다

ID가 해당 회로의 DIP 스위치가 불일치하면 TX로 수신받은 2바이트를 전송

- 인덱스

장치에 전원이 인가되면 가변저항 값을 읽는데 이를 BIAS라 한다

회전 하다가 스위치가 눌린다음 BIAS 이동한 것이 인덱스 0번이다

카드가 총 64개임으로 0~63 이면 한바퀴를 돈다

1과 64는 동일한 인덱스이지만 64는 한바퀴를 더 돌면서 정지마찰력이없는상태에서 스위치로 초기화후 해당 인덱스까지 이동하기 때문에 더 신뢰도가 높다.

1바이트이기 떄문에 인덱스는 255를 넘으면 어떻게 되는지 모르겠다

 

### 문제점

1. 스위치의 결합에 모델링 결함이 있어서, 8mm 유두머리 볼트를 사용하니 짧고 10mm유두머리를 사용하니 길어서 회전에 방해가되었다. 모델링 수정하기귀찮아서  접시 머리 볼트 10mm를 사용했다. 그렇기 떄문에 너무 꽉조으면 스위치가 부서지는경우가있었다.
2. 간혹 알수없는 스위치 인식에 문제가있어 무한 회전한다.
3. 싸게 만든다고 힘도약한 싸구려모터와 물리적 스위치를 쓴게 후회된다. 
4. 1개 출력시간이 너무 오래걸린다
5. 출력 공차를 매우 타이트하기에 상당한 프린팅 실력이 필요하다.

### 생각

마음같아선 270개보다 많은 80*24로 터미널을 만들고싶었다. 그렇기에 I2C를 사용하면 버스용량 문제가 너무뻔했다.

그렇다고 여러개단위로 묶어서 제어하는건  유튜브 콘텐츠로 너무 생각할게 많았다. 비용이 진짜 약간 증가하더라도  1개씩 모듈화시키는게 최선이었다. 이때 생기는 문제는 통신속도인데. 16핀정도사용해서 직접 병렬 통신을 IO로 만들지, 아니면 UART 통신으로  할지 고민했다. 허나 DIP스위치를 둬야해서 핀이 부족해져 오히려 UART보다 느려지는 계산이 나와서 이에 만족했다.

DIP스위치를 없애고 부팅시 자동ID 할당을 하고싶었다. 이것을하면 병렬 통신 라이브러리를 만들어야하는데 작업량이 너무 많더라… 

지금 잠와서 오타덩아리인 똥을 싼느낌이지만! 이걸 구경할정도의 지성인들은 이해해줄것이라 생각한다!! 일단 내용만 안틀리게 쓰려고 최선을 다해봤다!!! 공학은 존잼!! 빠이!





### 사용된 필라맨트
bambu PLA basic 검정
bambu PLA matte 흰색

### 커스텀 회로없이 아두이노 나노로 구동하는 방법

아두이노 나노
ULN2003 드라이버

<img width="1179" alt="스크린샷 2025-02-23 오후 11 15 30" src="https://github.com/user-attachments/assets/c509164d-c40a-47cc-89ec-a6692123039d" />






