### 일자: 2026 10 01

### 관련 파일: main.cpp

### 문제 상황: 종료 시(우측 상단 x버튼 클릭) QT Creator - Applicatioin Output 창에서 terminated abnormally 발생

### 1차 접근(틀림) : 지역 객체의 소멸 순서 변경
오류가 발생한 시점이 프로그램 종료인 점에 근거하여, QQmlApplicationEngine과 QMLImageProvier(QQuickImageProvider을 상속한 커스터 클래스)의 객체 소멸 순서에 문제가 있을 가능성을 의심했습니다.
```
//기존 코드
QQMLApplicatoinEngine engine;
QMLImageProvider imageProvider;
```
cpp의 지역객체는 생성된 역순으로 소멸하기에 engine의 위치를 하단으로 조정하여 imageprovider보다 먼저 소멸되게 유도하였습니다.

->microsoft visualcpp runtime library의 _crtIsValidHeapPointer(block) 오류 발생

#### _crtIsValidHeapPointer
- 지정된 포인터가 CRT(C Runtime Library)가 할당한 heap에 존재하는지 확인합니다.
```
	int _CrtIsValidHeapPointer(
		const void *userData // 할당된 메모리 블록의 시작 주소
	);
```
간단히 말하면, pointer가 allocated heap block에 존재하는지 검사하는 함수
_CrtIsValidHeapPointer(block) 오류는 유효하지 않은 heap pointer가 사용되었을 가능성을 의미합니다.

- 출처 : https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/crtisvalidheappointer?view=msvc-170

### 2차 접근
_CrtIsValidHeapPointer(block) 오류를 통해 heap을 중심으로 문제에 접근하였습니다.
먼저 QQmlEngine의 공식 문서를 검토하면서 'QQmlEngine는 provider의 소유권을 갖습니다.'라는 특징과 new를 통해 동적 할당하는 예시를 확인하였습니다.
이 시점에서 기존 코드에서 imageprovider 객체를 스택 할당한 방식이 문제의 원인일 것이라는 생각을 하였습니다.

이에 provider 객체를 new를 통해 heap에 할당하도록 코드를 수정하였습니다. 그 결과 오류 없이 정상적으로 실행되었습니다.
- 출처 : https://doc.qt.io/qt-6/ko/qqmlengine.html

### 원인 분석

QQmlEngine이 ownership을 갖는 QMLImageProvider을 stack 객체로 생성한 것이 문제의 주요 원인입니다.
QQmlEngine은 provider 객체를 할당 해제하면서 heap pointer 검사를 실행합니다. 이 과정에서 heap 영역에 존재하지 않는 객체는 _CrtIsValidHeapPointer(block) 오류를 발생시킵니다.
또한, stack 할당된 provider는 cpp 코드에서 scope이 끝나면 소멸자가 호출됩니다. scope 탈출에 의한 스택 해제에서는 CRT heap 검사가 발생하지 않습니다.
provider가 engine보다 아래에서 stack으로 선언된 경우, 소유권을 갖고있는 QQMLEngine은 이미 소멸한 provider 주소에 대해 delete를 시도하고 CRT heap 검사 이전에 terminated abnormally로 프로세스가 종료됩니다.

### 문제 해결
new를 통해 provider 객체를 동적으로 할당하면 cpp에서 소멸자가 호출되지 않기에 Qt로 안전하게 소유권이 이전되며, heap pointer 검사도 문제없이 통과합니다.
