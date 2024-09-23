# MyServer

고성능 비동기 IOCP 기반의 서버와 클라이언트를 C++로 구현한 프로젝트입니다. 
Windows IOCP, 멀티스레딩, 그리고 Protocol Buffers를 사용하여 효율적인 네트워크 프로그래밍 기술을 선보입니다. 이 프로젝트는 게임 서버와 더미 클라이언트를 포함하며, 커스텀 프로토콜을 통해 통신합니다.

## 목차

- [프로젝트 특징](#프로젝트-특징)
- [기술 스택](#기술-스택)
- [설치 방법](#설치-방법)
- [사용 방법](#사용-방법)
- [프로젝트 구조](#프로젝트-구조)
- [작동 영상](#작동-영상)
- [문의](#문의)

## 프로젝트 특징

- Windows IOCP를 활용한 고성능 비동기 서버 구현
- Google Protocol Buffers를 사용한 커스텀 프로토콜 통신
- 멀티스레딩과 스레드 동기화 처리
- 서버 기능 테스트를 위한 더미 클라이언트 제공
- 기본 채팅 기능이 있는 게임 서버 구현
- 디자인 패턴을 적용한 모듈화되고 확장 가능한 코드베이스

## 기술 스택

- **언어**: C++
- **네트워킹**: Windows Sockets API, IOCP(입출력 완료 포트)
- **직렬화**: Google Protocol Buffers
- **멀티스레딩**: C++11 스레드, 뮤텍스, 조건 변수
- **디자인 패턴**: 싱글톤, 팩토리, 전략 패턴 등
- **개발 도구**: Visual Studio (또는 선호하는 IDE)

## 설치 방법

1. **레포지토리 클론**

   ```bash
   git clone https://github.com/synm4/MyServer.git


2. **의존성 설치**

   - **Google Protocol Buffers**

     Google Protocol Buffers 라이브러리와 프로토콜 컴파일러가 필요합니다. 아래 링크를 참고하여 설치하세요.

     [Protocol Buffers 설치 가이드](https://developers.google.com/protocol-buffers/docs/downloads)

   - **빌드 도구**

     C++11 이상을 지원하는 컴파일러를 사용하세요 (예: Visual Studio 2019 이상).

3. **프로젝트 빌드**

   Visual Studio에서 `MyServer.sln` 솔루션 파일을 열고 빌드하세요.

## 사용 방법

## 게임 서버 실행
 Visual Studio에서 솔루션 우클릭 > 속성 > 시작 프로젝트
 GameServer.cpp 와 DummyClient.cpp 를 동시에 실행하면 됩니다.

## 프로젝트 구조
 MyServer/

 ├── GameServer/                  게임 서버 소스 코드
 │   ├── GameServer.cpp
 │   ├── GameSession.h
 │   ├── GameSession.cpp
 │   ├── GameSessionManager.h
 │   ├── GameSessionManager.cpp
 │   ├── ClientPacketHandler.h
 │   ├── ClientPacketHandler.cpp
 │   └── ... (기타 게임 서버 파일)
 ├── DummyClient/                 더미 클라이언트 소스 코드
 │   ├── DummyClient.cpp
 │   ├── ServerPacketHandler.h
 │   ├── ServerPacketHandler.cpp
 │   └── ... (기타 클라이언트 파일)
 ├── ServerCore/                  서버 코어 라이브러리
 │   ├── CorePch.h
 │   ├── CorePch.cpp
 │   ├── IocpCore.h
 │   ├── IocpCore.cpp
 │   ├── Session.h
 │   ├── Session.cpp
 │   └── ... (서버 코어 구성 요소)
 ├── Protocol/                    Protocol Buffers 정의 파일
 │   ├── Protocol.proto
 │   ├── Struct.proto
 │   ├── Enum.proto
 │   ├── 생성된 프로토콜 버퍼 파일 (.pb.h 및 .pb.cc)
 ├── Utils/                       유틸리티 함수와 도구들
 │   ├── BufferReader.h
 │   ├── BufferReader.cpp
 │   ├── BufferWriter.h
 │   ├── BufferWriter.cpp
 │   └── ... (유틸리티 파일)
 └── README.md                    프로젝트 설명 파일

## 작동 영상

  [프로젝트의 작동 영상을 확인하려면 클릭하세요](https://youtu.be/nMi-CTqovKk)

##  문의

[김영우] - synm4@naver.com

프로젝트 링크: https://github.com/synm4/MyServer


**참고사항:**

- `[Your Name]`과 `your.email@example.com` 부분을 본인의 이름과 이메일로 변경해주세요.
- `VIDEO_ID`를 YouTube에 업로드한 영상의 실제 ID로 대체해주세요.
- 설치 방법이나 사용 방법에 추가로 안내할 내용이 있다면 해당 섹션에 상세히 기재하시면 좋습니다.
- 프로젝트의 주요 기능이나 기술적인 상세 내용을 추가로 설명하고 싶다면 **프로젝트 특징** 섹션을 확장하실 수 있습니다.

**추가로 도움이 필요하시거나 수정할 내용이 있으시면 언제든지 말씀해주세요!**
