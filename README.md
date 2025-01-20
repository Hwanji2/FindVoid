[![파인드보이드](https://img.youtube.com/vi/pMRmSrVplwA/0.jpg)](https://youtu.be/pMRmSrVplwA)

[🌐 FindVoid_Prototype 데모 바로가기](https://hwanji2.github.io/FindVoid_JS_prototype/)

[📂 GitHub Repository](https://github.com/Hwanji2/FindVoid_JS_prototype)

# 🏫 FindVoid 빈 강의실 대여 시스템

## 🌟 주요 기능
1. **강의실 시간표 보기 📅**
   - 특정 건물과 강의실을 선택하여 요일별/시간대별 예약 상태를 확인합니다.
   
2. **비어있는 강의실 검색 🔍**
   - 선택한 요일과 시간대에 비어있는 강의실을 순차 탐색 또는 이진 탐색을 통해 찾을 수 있습니다.
   
3. **강의실 예약 신청 📝**
   - 원하는 요일과 시간대에 강의실을 예약할 수 있습니다.
  
![image](https://github.com/user-attachments/assets/224ee0bb-383c-4dd8-a79b-f5822fbcf9db)

---

## 🛠️ 데이터 구조
### 🗂️ 구조체 정의
- **`struct Reservation`**: 예약 정보를 저장하는 구조체로 사용자 ID, 예약 목적, 요일, 시간대 등을 포함합니다.
- **`struct Room`**: 강의실 정보를 저장하는 구조체로 강의실 ID와 예약 정보를 포함합니다.
- **`struct Building`**: 건물 정보를 저장하는 구조체로 건물 이름과 강의실 목록을 포함합니다.

### 🌐 전역 변수
- **`struct Building buildings[MAX_BUILDINGS]`**: 최대 20개의 건물을 저장하는 배열.
- **`int building_count`**: 현재 저장된 건물 수.

---

## 🗃️ 파일 형식
### 📄 `reservations.txt`
- 강의실 예약 정보를 저장하는 파일입니다.

![image](https://github.com/user-attachments/assets/5bef23c1-b4cf-4bb7-8a19-c02de38d1ae2)

---

## 🚀 사용 방법
1. **프로그램 실행**
   - 프로그램을 실행하면 다음과 같은 메뉴가 표시됩니다:
     ```
     1. 시간표 보기
     2. 비어있는 강의실 검색
     3. 강의실 예약 신청
     4. 종료
     ```

2. **시간표 보기 📋**
   - 건물 이름과 강의실 ID를 입력하면 해당 강의실의 시간표가 출력됩니다.

3. **비어있는 강의실 검색 🔎**
   - 요일과 시간대를 입력하면 해당 시간대에 비어있는 강의실이 출력됩니다.

4. **강의실 예약 신청 ✅**
   - 사용자 ID, 예약 목적, 요일, 시간대를 입력하여 강의실 예약을 추가할 수 있습니다.

---

## 🛠️ 빌드 및 실행
1. **C 컴파일러 설치 (예: GCC)**  
   아래 명령어로 프로그램을 컴파일합니다:
   ```bash
   gcc -o findvoid findvoid.c
   ./findvoid
