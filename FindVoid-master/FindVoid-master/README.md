[![파인드보이드](https://img.youtube.com/vi/pMRmSrVplwA/0.jpg)](https://youtu.be/pMRmSrVplwA)

[FindVoid_Prototype](https://hwanji2.github.io/FindVoid_JS_prototype/)

https://github.com/Hwanji2/FindVoid_JS_prototype

# FindVoid 빈 강의실 대여 시스템

## 중요
FindVoid는 사용자가 강의실 예약을 조회하고, 특정 시간대에 비어있는 강의실을 검색할 수 있는 강의실 예약 관리 프로그램입니다. 순차 탐색과 이진 탐색을 활용하여 빈 강의실을 효율적으로 찾을 수 있으며, 탐색 방식에 따른 성능 비교 기능도 제공합니다.
[파인드 보이드.pptx](https://github.com/user-attachments/files/18395362/default.pptx)




![image](https://github.com/user-attachments/assets/e8ad293a-5d69-418a-8ca3-4d7dc42d2419)


## 주요 기능
1. **강의실 시간표 보기**
   - 특정 건물과 강의실을 선택하여 요일별/시간대별 예약 상태를 확인합니다.
   
2. **비어있는 강의실 검색**
   - 선택한 요일과 시간대에 비어있는 강의실을 순차 탐색 또는 이진 탐색을 통해 찾을 수 있습니다.
   
3. **강의실 예약 신청**
   - 원하는 요일과 시간대에 강의실을 예약할 수 있습니다.
   
4. **성능 비교**
   - 동일한 조건에서 순차 탐색과 이진 탐색의 성능을 측정하고 비교할 수 있습니다.
  
![image](https://github.com/user-attachments/assets/224ee0bb-383c-4dd8-a79b-f5822fbcf9db)

## 데이터 구조
### 구조체 정의
- **`struct Reservation`**: 예약 정보를 저장하는 구조체로 사용자 ID, 예약 목적, 요일, 시간대 등을 포함합니다.
- **`struct Room`**: 강의실 정보를 저장하는 구조체로 강의실 ID와 예약 정보를 포함합니다.
- **`struct Building`**: 건물 정보를 저장하는 구조체로 건물 이름과 강의실 목록을 포함합니다.

### 전역 변수
- **`struct Building buildings[MAX_BUILDINGS]`**: 최대 20개의 건물을 저장하는 배열.
- **`int building_count`**: 현재 저장된 건물 수.

## 파일 형식
### `reservations.txt`
- 강의실 예약 정보를 저장하는 파일입니다.
- 각 줄은 다음 형식으로 저장됩니다:
<building_index> <room_index> <day> <time_slot> <user_id> <purpose>

![image](https://github.com/user-attachments/assets/5bef23c1-b4cf-4bb7-8a19-c02de38d1ae2)

## 사용 방법
1. **프로그램 실행**
 - 프로그램을 실행하면 다음과 같은 메뉴가 표시됩니다:
   ```
   1. 시간표 보기
   2. 비어있는 강의실 검색
   3. 강의실 예약 신청
   4. 종료
   5. 성능 비교
   ```
2. **시간표 보기**
 - 건물 이름과 강의실 ID를 입력하면 해당 강의실의 시간표가 출력됩니다.
 
3. **비어있는 강의실 검색**
 - 요일과 시간대를 입력하면 해당 시간대에 비어있는 강의실이 출력됩니다.
 
4. **강의실 예약 신청**
 - 사용자 ID, 예약 목적, 요일, 시간대를 입력하여 강의실 예약을 추가할 수 있습니다.
 
5. **성능 비교**
 - 요일과 시간대를 입력하여 순차 탐색과 이진 탐색의 성능을 비교할 수 있습니다.

## 빌드 및 실행
1. C 컴파일러(예: GCC)를 사용하여 프로그램을 컴파일합니다:

## 참고 사항
- 예약 데이터는 `reservations.txt` 파일에 저장되며, 프로그램 실행 시 자동으로 로드됩니다.
- 프로그램 종료 시 변경된 예약 내용이 자동으로 `reservations.txt` 파일에 저장됩니다.
