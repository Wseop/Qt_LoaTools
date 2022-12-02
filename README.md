# LoaTools
## 프로젝트 설명
* 로스트아크 플레이시 유저들이 주로 사용하는 보조 기능들을 하나의 프로그램으로 제공
## 주요 기능
### [캐릭터 조회](https://github.com/Wseop/Qt_LoaTools/tree/master/Profile)
* 캐릭터의 스펙을 한 화면으로 볼 수 있게 로스트아크 전투정보실 정보를 재가공하여 제공
> 실시간 정보 반영<br>조회 시 database에 추가됨
### [직업별 세팅 조회](https://github.com/Wseop/Qt_LoaTools/tree/master/setting_adviser)
* 직업 선택 시, 유저들이 가장 많이 채용한 세팅을 직업각인별로 최대 10개까지 제공
* 전체 직업의 Top3 세팅 조회 기능 제공
> database 데이터 기반<br>유효한 세팅 제공을 위해 현재 database에 1600레벨 이상 캐릭터(약 90,000개)만 등록되어 있음
### [캐릭터 랭킹](https://github.com/Wseop/Qt_LoaTools/tree/master/ranking_board)
* 아이템 레벨을 기준으로 캐릭터 랭킹 조회
* 전체 랭킹, 직업별 랭킹 조회 가능
> database 데이터 기반
### [각인 시뮬레이터](https://github.com/Wseop/Qt_LoaTools/tree/master/engrave_simulator)
* 아이템의 각인 수치에 따라 가능한 각인 값을 시각화하여 제공
### [메테오 타이머](https://github.com/Wseop/Qt_LoaTools/tree/master/meteor_timer)
* 아브렐슈드 레이드 6관문의 타일 재생 타이머
### [경매 손익 계산](https://github.com/Wseop/Qt_LoaTools/tree/master/auction_calc)
* 경매 아이템의 적정 분배금 제공
* 4인 레이드는 거래소 가격의 65%, 8인 레이드는 76%로 설정되어있음
## [Database](https://github.com/Wseop/Qt_LoaTools/tree/master/db)
* MongoDB 사용
* `캐릭터 조회`기능 에서 캐릭터 조회시 DB에 캐릭터 정보 저장
* 저장된 캐릭터 정보는 `직업별 세팅 조회` 기능에서 활용
### Document Format (Example)
#### Character (캐릭터)
```json
{
  "Name":"CharacterName",
  "Class":"Berserker"
  "Level":1561.43
}
```
#### Setting (캐릭터 세팅)
```json
{
  "Name":"CharacterName",
  "Class":"Berserker",
  "Abilities":["특화", "치명", "특화", "특화", "특화", "치명"],
  "EngraveNames":["원한", "기습의 대가", "광전사의 비기"],
  "EngraveLevels":[3, 3, 1],
  "SetEffects":["사멸", "사멸", "악몽", "악몽", "악몽", "악몽"]
}
```
