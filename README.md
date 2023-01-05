# LoaTools
## 프로젝트 설명
* 로스트아크 OpenAPI와 MongoDB를 활용하여 로스트아크 관련 편의 기능을 제공
## 주요 기능
### 캐릭터
#### [캐릭터 조회](https://github.com/Wseop/Qt_LoaTools/tree/master/tools/character_search)
* 캐릭터의 스펙을 한 화면으로 볼 수 있게 로스트아크 전투정보실 정보를 재가공하여 제공
> 실시간 정보 반영<br>조회 시 database에 추가됨
* 캐릭터의 특성, 세트효과, 각인을 기반으로 [SettingCode](https://github.com/Wseop/Qt_LoaTools/blob/master/db/document/settingcode_manager.cpp)를 생성하여 database에 추가
> SettingCode (Example)
```
SettingCode = A121111S222222C044CL3N002004029032034NL13333

특성(A) : A121111
- index : 목걸이1/2, 귀걸이1/2, 반지1/2
- value : 치명(0), 특화(1), 신속(2), 제압(3), 인내(4), 숙련(5)
 
세트효과(S) : S222222 
- index : 무기/머리/상의/하의/장갑/어깨
- value : 악몽(0), 구원(1), 사멸(2), 지배(3), 환각(4), 갈망(5), 배신(6), 매혹(7), 파괴(8)

직업각인(C), 직업각인레벨(CL), 일반각인(N), 일반각인레벨(NL) : C044 CL3 N002004029032034 NL13333
- 각인은 3자리 숫자로 구분, 오름차순 정렬 (순서만 다른 경우에도 동일한 코드가 생성되도록 함)
- 레벨은 해당하는 각인이 적힌 순서와 매칭이 되도록 코드 생성
```
#### [직업별 세팅 조회](https://github.com/Wseop/Qt_LoaTools/tree/master/tools/setting_adviser)
* 직업 선택 시, 유저들이 가장 많이 채용한 세팅을 최대 30개까지 제공
> database 데이터 기반<br>유효한 세팅 제공을 위해 현재 상위권 약90000개 캐릭터의 정보가 등록되어 있음
#### [캐릭터 랭킹](https://github.com/Wseop/Qt_LoaTools/tree/master/tools/ranking_board)
* 아이템 레벨을 기준으로 캐릭터 랭킹 조회
* 전체 랭킹, 직업별 랭킹 조회 가능
> database 데이터 기반
#### [군장 검사](https://github.com/Wseop/Qt_LoaTools/tree/master/tools/spec_checker)
* 캐릭터 조회의 간소화 버전, 정보를 간소화하여 동시에 여러 캐릭터를 등록시켜 비교할 수 있도록 구현
* 캐릭터 정보 중 레이드 관련 정보만 가공하여 제공
* 기준 스펙 미달일 경우 빨간색으로 표기하여 알려주도록 구현
```c++
const int SPEC_CUT_ABILITY = 2100;
const int SPEC_CUT_GEM_AVG = 7;
const int SPEC_CUT_TRIPOD_ENABLED = 10;
const int SPEC_CUT_ENGRAVING = 15;
```
#### [각인 시뮬레이터](https://github.com/Wseop/Qt_LoaTools/tree/master/tools/engrave_simulator)
* 아이템의 각인 수치에 따라 가능한 각인 값을 시각화하여 제공
### 레이드
#### [메테오 타이머](https://github.com/Wseop/Qt_LoaTools/tree/master/tools/meteor_timer)
* 아브렐슈드 레이드 6관문의 타일 재생 타이머
#### [경매 손익 계산](https://github.com/Wseop/Qt_LoaTools/tree/master/tools/auction_calc)
* 경매 아이템의 적정 분배금 제공
* 4인 레이드는 거래소 가격의 65%, 8인 레이드는 76%로 설정되어있음
### 거래소 / 경매장
#### [악세 검색기](https://github.com/Wseop/Qt_LoaTools/tree/master/tools/accessory_searcher)
* 경매장에 존재하는 악세를 간편하게 검색할 수 있는 기능
* 검색필터를 클릭으로 간편하게 설정할 수 있도록 구현
* 선택한 아이템 상단 고정 기능 구현
## [Database](https://github.com/Wseop/Qt_LoaTools/tree/master/db)
* MongoDB 사용
* `캐릭터 조회`기능 에서 캐릭터 조회시 DB에 캐릭터 정보 저장
* 저장된 캐릭터 정보는 `직업별 세팅 조회` 기능에서 활용
### Document Format (Example)
#### Character_v2 (캐릭터)
```json
{
  "Name": "DogDwarf",
  "Class": "버서커"
  "Level": 1571.67
}
```
#### Setting_v2 (캐릭터 세팅)
```json
{
  "Name": "DogDwarf",
  "Class": "버서커",
  "SettingCode": "A121111S222222C044CL3N002004029032034NL13333"
}
```
