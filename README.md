# Baseball - 숫자 야구 게임

Unreal Engine 5.5 기반의 멀티플레이어 숫자 야구 게임입니다.
두 명의 플레이어가 서로의 비밀 숫자를 맞추는 방식으로 진행됩니다.

---

## 게임 규칙

- 서버가 **1~9 사이의 중복 없는 3자리 숫자**를 생성합니다.
- 두 플레이어가 번갈아 가며 3자리 숫자를 입력합니다.
- 각 추측 결과는 다음과 같이 판정됩니다:

| 판정 | 설명 |
|------|------|
| **Strike** | 숫자와 위치가 모두 일치 |
| **Ball** | 숫자는 맞지만 위치가 다름 |
| **Out** | 해당 숫자가 정답에 없음 |

- **3 Strike** 달성 시 승리
- 한 턴당 **3번의 기회**가 주어지며, 기회를 모두 소진하면 상대방에게 턴이 넘어갑니다.
- 턴마다 **30초 제한시간**이 있습니다.

---

## 프로젝트 구조

```
Baseball/
├── Source/Baseball/
│   ├── Game/
│   │   ├── BaseballGameMode       # 게임 진행 로직, 정답 생성, 판정
│   │   └── BaseballGameState      # 현재 턴/타이머 상태 (복제)
│   ├── Player/
│   │   ├── BaseballPlayerController  # 플레이어 입력 및 서버 RPC
│   │   └── BaseballPlayerState       # 플레이어 이름, 남은 기회 (복제)
│   └── UI/
│       ├── BaseballHUD            # HUD 관리자
│       ├── BaseballMainWidget     # 메인 UI 컨테이너
│       └── ChatWidget             # 채팅 입력/출력 위젯
├── Content/
│   ├── Level/L_Level              # 게임 레벨
│   ├── UI/                        # UMG 위젯 블루프린트
│   └── UI/Font/Jalnan2/           # 한국어 폰트
└── Baseball.uproject
```

---

## 개발 환경

| 항목 | 버전 |
|------|------|
| Unreal Engine | 5.5 |
| 언어 | C++ |
| UI 프레임워크 | UMG (Unreal Motion Graphics) |
| 네트워크 | Unreal Replication (RPC) |

---

## 빌드 및 실행 방법

### 빌드

1. `Baseball.uproject` 우클릭 → **Generate Visual Studio project files** 선택
2. `Baseball.sln`을 Visual Studio에서 열기
3. 빌드 구성을 `Development Editor`로 설정 후 빌드

### 실행

1. Unreal Editor에서 `Baseball.uproject` 실행
2. Play 버튼 옆 드롭다운에서 **멀티플레이어 플레이어 수를 2로 설정**
3. 플레이 시작 — 2명이 접속되면 자동으로 게임 시작

> 플레이어가 2명 미만이면 게임이 시작되지 않습니다.

---

## 네트워크 아키텍처

```
[Server]
  BaseballGameMode       ← 정답 생성, 판정, 턴 관리 (서버 전용)
  BaseballGameState      → 타이머/턴 정보를 모든 클라이언트에 복제
  BaseballPlayerState    → 플레이어 이름, 남은 기회 복제

[Client]
  BaseballPlayerController
    Server_SendMessage() → 서버로 추측 전송 (Server RPC)
    Client_ReceiveMessage() ← 서버에서 결과 수신 (Client RPC)
  BaseballHUD / UI       ← 채팅 및 타이머 화면 갱신
```

---

## 주요 게임플로우

```
1. 두 플레이어 접속
        ↓
2. 서버가 3자리 비밀 숫자 생성
        ↓
3. 현재 턴 플레이어가 30초 내에 숫자 입력
        ↓
4. 서버가 Strike/Ball/Out 판정 후 전체에 브로드캐스트
        ↓
5. 3 Strike → 승리 / 3번 기회 소진 → 상대방 턴
        ↓
6. 승패 결정 → 게임 리셋
```
