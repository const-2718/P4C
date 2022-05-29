# stack
stack은 추상 데이터 타입이다. 데이터는 LIFO방식으로 저장되며 push, pop연산이 가능하다. 
프로그램을 실행할때 stack이 할당되는데 왜 그런지 알아보자.

##### stack의 필요성
stack은 지역변수의 참조를 위해 사용된다. stack에는 함수가 실행될때마다 함수가 소유할수있는 하나의 영역이 생성되는데 여기에 rbp에서 오프셋을 계산하는 방식으로 지역변수에 접근한다.

##### 함수의 프롤로그와 에필로그
**프롤로그**
```
push rbp
mov rbp, rsp
sub rsp, 0x10
```

**에필로그**
```
leave
ret
```

leave는 rbp레지스터 값을 rsp에 대입한 후에 스택에서 pop하여 rbp레지스터에 대입합니다.
leave를 다음의 2가지 명령어로 이해할 수 있다.
```
mov rsp, rbp
pop rbp 
```

##### stack base pointer의 역할
- 지역변수의 번지수를 계산하기위한 base pointer
- 현재 함수의 스택을 정리시 사용

##### context
- instruction pointer
- arguments
- stack base pointer
- return adress

main 함수에서 어떤 함수 func()를 호출하면 func()함수는 func()함수만의 context를 가져야 하고, func()함수가 리턴되면 다시 main함수의 context로 돌아와야한다.

프롤로그, 에필로그의 구현은 아키텍쳐마다 다르지만, 다음과정을 크게 벗어나지 않는다.
1. 함수를 호출하기 전, 현재 context에 대해 저장합니다.
2. 함수는 자신의 context를 만듭니다.
3. 함수가 실행됩니다.
4. 함수는 자신의 context를 정리합니다.
5. 함수는 이전 context를 복구시킵니다.


