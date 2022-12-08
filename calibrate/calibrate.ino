#include <Stepper.h>
 
#define BUADRATE 9600
 
#define IN1 10 // IN1
#define IN2 9 // IN2
#define IN3 8 // IN3
#define IN4 7 // IN4
#define STEPS 2048 // 1봐퀴 스텝수
#define RPM 10 // 10 이하로 설정을 권장 
 
// Stepper 클래스 객체 생성
Stepper myStepper(STEPS, IN1, IN3, IN2, IN4);
// 시리얼 포트로 angle 값을 저장할 문자열
String myString;
int angle = 0; // 현재 각도를 저장할 전역 변수
int flag = 0; // 시리얼 통신으로 각도 입력시 활성 flag
 
void setup(){
    myStepper.setSpeed(RPM);
    Serial.begin(BUADRATE);
    while(!Serial){
        ;
    }
    delay(1000);
    Serial.println("Program Start..");
    Serial.print("Input Angle >> ");
}
 
void loop(){
    int temp;
    if(flag == 1){   
        temp = myString.toInt();
        // angle 은 현재 추가된 각도를 계속 저장 0 ~ 360도 단위
        angle = (angle + temp) % 360; 
        // temp 의 각도를 step 으로 변환
        temp = toStep(temp);
        myString = "";
        myStepper.step(temp);
        Serial.print("Present Angle : ");
        Serial.println(angle);
        delay(20);
        Serial.print("Input Angle >> ");
        flag = false;
    }
    delay(100);
}
 
void serialEvent()
{
    char cTemp = (char)Serial.read();
    // 시리얼 포트에서 입력 받을 수 있는 문자는 숫자, Backspace, - 기호만 허용
    if (isDigit(cTemp) || cTemp == '\b'|| cTemp == '-'){
        myString += cTemp;
    }
    // 엔터 입력시 각도를 다 입력 한 것으로 침
    else if(cTemp == '\n'){
        flag = true;
    }
    // 아무 값 입력없이 Enter 를 치면 경고 메세지 출력
    else if(cTemp == '\r'){
        if (myString == "") ErrorHadler();
    }
    else /* if (cTemp < '0' || cTemp > '9') */{
        ErrorHadler();
    }
}
// 시리얼 통신으로 부터 입력 받은 각도를 step 으로 변환
// CW, CCW 각도 모두 사용 가능
// 360도를 초과할 때도 처리 가능
int toStep(int angle){
    int quotient = angle / 360;
    int remainder = angle % 360;
    remainder = map(remainder, -360, 360, -2048, 2048);
    if(abs(quotient) > 0)
        return (2048 * quotient) + remainder;
    else
        return remainder;
}
 
void ErrorHadler()
{
    Serial.print('\r');
    Serial.print('\n');
    Serial.println("Error! Input Angle Number..");
    myString = "";
    Serial.print("Input Angle >> ");
}
