#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BUILDINGS 20
#define MAX_ROOMS 50
#define DAYS 5
#define TIME_SLOTS 9

struct Reservation {
    char user_id[10];
    char purpose[50];
    int day;        // 0: 월요일, 1: 화요일, ..., 4: 금요일
    int time_slot;  // 0: 9시-10시, ..., 8: 5시-6시
};

struct Room {
    char room_id[10];
    struct Reservation reservations[DAYS][TIME_SLOTS];
};

struct Building {
    char building_name[50];
    struct Room rooms[MAX_ROOMS];
    int room_count;
};

struct Building buildings[MAX_BUILDINGS];
int building_count = 0;

void check_file_existence(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("파일을 찾을 수 없습니다. 프로그램을 종료합니다.\n");
        exit(1);
    }
    fclose(fp);
}

void load_building_data() {
    check_file_existence("buildings.txt");
    FILE* fp = fopen("buildings.txt", "r");
    fscanf(fp, "%d", &building_count);
    for (int i = 0; i < building_count; i++) {
        fscanf(fp, "%s %d", buildings[i].building_name, &buildings[i].room_count);
        for (int j = 0; j < buildings[i].room_count; j++) {
            fscanf(fp, "%s", buildings[i].rooms[j].room_id);
        }
    }
    fclose(fp);
}

void load_reservations() {
    check_file_existence("reservations.txt");
    FILE* fp = fopen("reservations.txt", "r");
    int building_index = 0, room_index = 0, day = 0, time_slot = 0;
    while (fscanf(fp, "%d %d %d %d %s %s", &building_index, &room_index, &day, &time_slot, buildings[building_index].rooms[room_index].reservations[day][time_slot].user_id, buildings[building_index].rooms[room_index].reservations[day][time_slot].purpose) != EOF) {
        // Reservations are loaded directly into the data structure
    }
    fclose(fp);
}

void save_reservations() {
    FILE* fp = fopen("reservations.txt", "w");
    if (fp == NULL) {
        printf("reservations.txt 파일을 저장할 수 없습니다.\n");
        return;
    }
    for (int i = 0; i < building_count; i++) {
        for (int j = 0; j < buildings[i].room_count; j++) {
            for (int d = 0; d < DAYS; d++) {
                for (int t = 0; t < TIME_SLOTS; t++) {
                    if (strlen(buildings[i].rooms[j].reservations[d][t].user_id) > 0) {
                        fprintf(fp, "%d %d %d %d %s %s\n", i, j, d, t, buildings[i].rooms[j].reservations[d][t].user_id, buildings[i].rooms[j].reservations[d][t].purpose);
                    }
                }
            }
        }
    }
    fclose(fp);
}

void print_timetable(struct Room* room) {
    printf("\n학습시간표 - 강의실: %s\n", room->room_id);
    printf("          월        화        수        목        금\n");  // 요일 간격 추가
    for (int t = 0; t < TIME_SLOTS; t++) {
        printf("%-9s", t == 0 ? "9시-10시  " :        // 시간 열 고정 너비 추가
            t == 1 ? "10시-11시 " :
            t == 2 ? "11시-12시 " :
            t == 3 ? "12시-13시 " :
            t == 4 ? "13시-14시 " :
            t == 5 ? "14시-15시 " :
            t == 6 ? "15시-16시 " :
            t == 7 ? "16시-17시 " :
            "17시-18시 ");
        for (int d = 0; d < DAYS; d++) {
            if (strlen(room->reservations[d][t].user_id) > 0) {
                printf("%-10s", room->reservations[d][t].purpose);  // 너비 10칸 고정
            }
            else {
                printf("%-10s", "비어있음");  // 너비 10칸 고정
            }
        }
        printf("\n");
    }
}

void find_and_print_empty_rooms(int day, int time_slot) {
    if (day < 0 || day >= DAYS || time_slot < 0 || time_slot >= TIME_SLOTS) {
        printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        return;
    }

    printf("\n현재 비어있는 강의실 - 해당일: %d, 시간 버튼: %d\n", day, time_slot);

    for (int i = 0; i < building_count; i++) {
        printf("건물: %s\n", buildings[i].building_name);

        // 강의실 정렬
        qsort(buildings[i].rooms, buildings[i].room_count, sizeof(struct Room), compare_rooms);

        // 이진 탐색으로 빈 강의실 찾기
        int low = 0, high = buildings[i].room_count - 1;
        int mid = -1, found = 0;

        while (low <= high) {
            mid = (low + high) / 2;
            if (strlen(buildings[i].rooms[mid].reservations[day][time_slot].user_id) == 0) {
                found = 1;
                break;
            }
            if (strlen(buildings[i].rooms[mid].reservations[day][time_slot].user_id) > 0) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }

        // 빈 강의실이 없을 경우
        if (!found) {
            printf("비어 있는 강의실이 없습니다.\n");
            continue;
        }

        // 왼쪽으로 확장하여 빈 강의실 출력
        int left = mid;
        while (left >= 0 && strlen(buildings[i].rooms[left].reservations[day][time_slot].user_id) == 0) {
            printf("강의실: %s\n", buildings[i].rooms[left].room_id);
            left--;
        }

        // 오른쪽으로 확장하여 빈 강의실 출력
        int right = mid + 1;
        while (right < buildings[i].room_count && strlen(buildings[i].rooms[right].reservations[day][time_slot].user_id) == 0) {
            printf("강의실: %s\n", buildings[i].rooms[right].room_id);
            right++;
        }
    }
}


// 함수 정의
int compare_rooms(const void* a, const void* b) {
    return strcmp(((struct Room*)a)->room_id, ((struct Room*)b)->room_id);
}
void find_and_print_empty_rooms(int day, int time_slot) {
    if (day < 0 || day >= DAYS || time_slot < 0 || time_slot >= TIME_SLOTS) {
        printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        return;
    }

    printf("\n현재 비어있는 강의실 - 해당일: %d, 시간 버튼: %d\n", day, time_slot);

    for (int i = 0; i < building_count; i++) {
        printf("건물: %s\n", buildings[i].building_name);

        // 강의실 정렬
        qsort(buildings[i].rooms, buildings[i].room_count, sizeof(struct Room), compare_rooms);

        // 이진 탐색으로 빈 강의실의 시작점 찾기
        int low = 0, high = buildings[i].room_count - 1;
        int mid = -1, found = 0;

        while (low <= high) {
            mid = (low + high) / 2;
            if (strlen(buildings[i].rooms[mid].reservations[day][time_slot].user_id) == 0) {
                found = 1;
                break;
            }
            if (strlen(buildings[i].rooms[mid].reservations[day][time_slot].user_id) > 0) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }

        // 빈 강의실이 없을 경우
        if (!found) {
            printf("비어 있는 강의실이 없습니다.\n");
            continue;
        }

        // 왼쪽으로 확장하여 빈 강의실 출력
        int left = mid;
        while (left >= 0 && strlen(buildings[i].rooms[left].reservations[day][time_slot].user_id) == 0) {
            printf("강의실: %s\n", buildings[i].rooms[left].room_id);
            left--;
        }

        // 오른쪽으로 확장하여 빈 강의실 출력
        int right = mid + 1;
        while (right < buildings[i].room_count && strlen(buildings[i].rooms[right].reservations[day][time_slot].user_id) == 0) {
            printf("강의실: %s\n", buildings[i].rooms[right].room_id);
            right++;
        }
    }
}


void compare_performance(int day, int time_slot) {
    clock_t start, end;

    // 순차 탐색 성능 측정
    start = clock();
    find_and_print_empty_rooms_sequential(day, time_slot);  // 순차 탐색 호출
    end = clock();
    printf("\n순차 탐색 시간: %lf초\n", (double)(end - start) / CLOCKS_PER_SEC);

    // 이진 탐색 성능 측정
    start = clock();
    find_and_print_empty_rooms(day, time_slot);  // 기존 이진 탐색 호출
    end = clock();
    printf("이진 탐색 시간: %lf초\n", (double)(end - start) / CLOCKS_PER_SEC);
}



void make_reservation() {
    char building_name[50], room_id[10], user_id[10], purpose[50];
    int day, time_slot;
    printf("건물 이름을 입력해주세요: ");
    scanf("%s", building_name);
    printf("강의실 번호를 입력해주세요: ");
    scanf("%s", room_id);
    printf("해당일 (0: 월, 1: 화, ..., 4: 금): ");
    if (scanf("%d", &day) != 1 || day < 0 || day >= DAYS) {
        printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        return;
    }
    printf("시간 버튼 (0: 9시-10시, ..., 8: 5시-6시): ");
    if (scanf("%d", &time_slot) != 1 || time_slot < 0 || time_slot >= TIME_SLOTS) {
        printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        return;
    }
    printf("사용자 ID를 입력해주세요: ");
    scanf("%s", user_id);
    printf("사용 목적: ");
    scanf("%s", purpose);

    for (int i = 0; i < building_count; i++) {
        if (strcmp(buildings[i].building_name, building_name) == 0) {
            for (int j = 0; j < buildings[i].room_count; j++) {
                if (strcmp(buildings[i].rooms[j].room_id, room_id) == 0) {
                    if (strlen(buildings[i].rooms[j].reservations[day][time_slot].user_id) == 0) {
                        strcpy(buildings[i].rooms[j].reservations[day][time_slot].user_id, user_id);
                        strcpy(buildings[i].rooms[j].reservations[day][time_slot].purpose, purpose);
                        printf("통합적인 예약이 성공적으로 완료되었습니다.\n");
                        save_reservations();
                        return;
                    }
                    else {
                        printf("해당 강의실은 이미 예약되어 있습니다.\n");
                        return;
                    }
                }
            }
        }
    }
    printf("강의실이 없습니다.\n");
}

int main() {
    printf("파인드 보이드 - 강의실 예약 시스템\n");
    load_building_data();
    load_reservations();
    int choice;

    while (1) {
        printf("\n1. 시간표 보기\n");
        printf("2. 비어있는 강의실 검색\n");
        printf("3. 강의실 예약 신청\n");
        printf("4. 종료\n");
        printf("5. 성능 비교\n");
        printf("선택해주세요: ");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("잘못된 입력입니다. 다시 시도해주세요.\n");
            while (getchar() != '\n');  // 입력 버퍼 비우기
            continue;
        }

        if (choice == 1) {
            char building_name[50], room_id[10];
            printf("건물 이름을 입력해주세요: ");
            scanf("%s", building_name);
            printf("강의실 번호를 입력해주세요: ");
            scanf("%s", room_id);
            for (int i = 0; i < building_count; i++) {
                if (strcmp(buildings[i].building_name, building_name) == 0) {
                    for (int j = 0; j < buildings[i].room_count; j++) {
                        if (strcmp(buildings[i].rooms[j].room_id, room_id) == 0) {
                            print_timetable(&buildings[i].rooms[j]);
                        }
                    }
                }
            }
        }
        else if (choice == 2) {
            int day, time_slot;
            printf("해당일 (0: 월, 1: 화, ..., 4: 금): ");
            if (scanf("%d", &day) != 1 || day < 0 || day >= DAYS) {
                printf("잘못된 입력입니다. 다시 시도해주세요.\n");
                while (getchar() != '\n');  // 입력 버퍼 비우기
                continue;
            }
            printf("시간 버튼 (0: 9시-10시, ..., 8: 5시-6시): ");
            if (scanf("%d", &time_slot) != 1 || time_slot < 0 || time_slot >= TIME_SLOTS) {
                printf("잘못된 입력입니다. 다시 시도해주세요.\n");
                while (getchar() != '\n');  // 입력 버퍼 비우기
                continue;
            }
            find_and_print_empty_rooms(day, time_slot);
        }
        else if (choice == 3) {
            make_reservation();
        }
        else if (choice == 4) {
            printf("이용해주셔서 감사합니다.\n");
            break;
        }
        else if (choice == 5) {
            int day, time_slot;
            printf("해당일 (0: 월, 1: 화, ..., 4: 금): ");
            scanf("%d", &day);
            printf("시간 버튼 (0: 9시-10시, ..., 8: 5시-6시): ");
            scanf("%d", &time_slot);
            compare_performance(day, time_slot);
        }

    }
    return 0;
}
