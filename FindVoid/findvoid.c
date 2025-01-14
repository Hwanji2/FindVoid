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
    int day;        // 0: ������, 1: ȭ����, ..., 4: �ݿ���
    int time_slot;  // 0: 9��-10��, ..., 8: 5��-6��
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
        printf("������ ã�� �� �����ϴ�. ���α׷��� �����մϴ�.\n");
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
        printf("reservations.txt ������ ������ �� �����ϴ�.\n");
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
    printf("\n�н��ð�ǥ - ���ǽ�: %s\n", room->room_id);
    printf("          ��        ȭ        ��        ��        ��\n");  // ���� ���� �߰�
    for (int t = 0; t < TIME_SLOTS; t++) {
        printf("%-9s", t == 0 ? "9��-10��  " :        // �ð� �� ���� �ʺ� �߰�
            t == 1 ? "10��-11�� " :
            t == 2 ? "11��-12�� " :
            t == 3 ? "12��-13�� " :
            t == 4 ? "13��-14�� " :
            t == 5 ? "14��-15�� " :
            t == 6 ? "15��-16�� " :
            t == 7 ? "16��-17�� " :
            "17��-18�� ");
        for (int d = 0; d < DAYS; d++) {
            if (strlen(room->reservations[d][t].user_id) > 0) {
                printf("%-10s", room->reservations[d][t].purpose);  // �ʺ� 10ĭ ����
            }
            else {
                printf("%-10s", "�������");  // �ʺ� 10ĭ ����
            }
        }
        printf("\n");
    }
}

void find_and_print_empty_rooms(int day, int time_slot) {
    if (day < 0 || day >= DAYS || time_slot < 0 || time_slot >= TIME_SLOTS) {
        printf("�߸��� �Է��Դϴ�. �ٽ� �õ����ּ���.\n");
        return;
    }

    printf("\n���� ����ִ� ���ǽ� - �ش���: %d, �ð� ��ư: %d\n", day, time_slot);

    for (int i = 0; i < building_count; i++) {
        printf("�ǹ�: %s\n", buildings[i].building_name);

        // ���ǽ� ����
        qsort(buildings[i].rooms, buildings[i].room_count, sizeof(struct Room), compare_rooms);

        // ���� Ž������ �� ���ǽ� ã��
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

        // �� ���ǽ��� ���� ���
        if (!found) {
            printf("��� �ִ� ���ǽ��� �����ϴ�.\n");
            continue;
        }

        // �������� Ȯ���Ͽ� �� ���ǽ� ���
        int left = mid;
        while (left >= 0 && strlen(buildings[i].rooms[left].reservations[day][time_slot].user_id) == 0) {
            printf("���ǽ�: %s\n", buildings[i].rooms[left].room_id);
            left--;
        }

        // ���������� Ȯ���Ͽ� �� ���ǽ� ���
        int right = mid + 1;
        while (right < buildings[i].room_count && strlen(buildings[i].rooms[right].reservations[day][time_slot].user_id) == 0) {
            printf("���ǽ�: %s\n", buildings[i].rooms[right].room_id);
            right++;
        }
    }
}


// �Լ� ����
int compare_rooms(const void* a, const void* b) {
    return strcmp(((struct Room*)a)->room_id, ((struct Room*)b)->room_id);
}
void find_and_print_empty_rooms(int day, int time_slot) {
    if (day < 0 || day >= DAYS || time_slot < 0 || time_slot >= TIME_SLOTS) {
        printf("�߸��� �Է��Դϴ�. �ٽ� �õ����ּ���.\n");
        return;
    }

    printf("\n���� ����ִ� ���ǽ� - �ش���: %d, �ð� ��ư: %d\n", day, time_slot);

    for (int i = 0; i < building_count; i++) {
        printf("�ǹ�: %s\n", buildings[i].building_name);

        // ���ǽ� ����
        qsort(buildings[i].rooms, buildings[i].room_count, sizeof(struct Room), compare_rooms);

        // ���� Ž������ �� ���ǽ��� ������ ã��
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

        // �� ���ǽ��� ���� ���
        if (!found) {
            printf("��� �ִ� ���ǽ��� �����ϴ�.\n");
            continue;
        }

        // �������� Ȯ���Ͽ� �� ���ǽ� ���
        int left = mid;
        while (left >= 0 && strlen(buildings[i].rooms[left].reservations[day][time_slot].user_id) == 0) {
            printf("���ǽ�: %s\n", buildings[i].rooms[left].room_id);
            left--;
        }

        // ���������� Ȯ���Ͽ� �� ���ǽ� ���
        int right = mid + 1;
        while (right < buildings[i].room_count && strlen(buildings[i].rooms[right].reservations[day][time_slot].user_id) == 0) {
            printf("���ǽ�: %s\n", buildings[i].rooms[right].room_id);
            right++;
        }
    }
}


void compare_performance(int day, int time_slot) {
    clock_t start, end;

    // ���� Ž�� ���� ����
    start = clock();
    find_and_print_empty_rooms_sequential(day, time_slot);  // ���� Ž�� ȣ��
    end = clock();
    printf("\n���� Ž�� �ð�: %lf��\n", (double)(end - start) / CLOCKS_PER_SEC);

    // ���� Ž�� ���� ����
    start = clock();
    find_and_print_empty_rooms(day, time_slot);  // ���� ���� Ž�� ȣ��
    end = clock();
    printf("���� Ž�� �ð�: %lf��\n", (double)(end - start) / CLOCKS_PER_SEC);
}



void make_reservation() {
    char building_name[50], room_id[10], user_id[10], purpose[50];
    int day, time_slot;
    printf("�ǹ� �̸��� �Է����ּ���: ");
    scanf("%s", building_name);
    printf("���ǽ� ��ȣ�� �Է����ּ���: ");
    scanf("%s", room_id);
    printf("�ش��� (0: ��, 1: ȭ, ..., 4: ��): ");
    if (scanf("%d", &day) != 1 || day < 0 || day >= DAYS) {
        printf("�߸��� �Է��Դϴ�. �ٽ� �õ����ּ���.\n");
        return;
    }
    printf("�ð� ��ư (0: 9��-10��, ..., 8: 5��-6��): ");
    if (scanf("%d", &time_slot) != 1 || time_slot < 0 || time_slot >= TIME_SLOTS) {
        printf("�߸��� �Է��Դϴ�. �ٽ� �õ����ּ���.\n");
        return;
    }
    printf("����� ID�� �Է����ּ���: ");
    scanf("%s", user_id);
    printf("��� ����: ");
    scanf("%s", purpose);

    for (int i = 0; i < building_count; i++) {
        if (strcmp(buildings[i].building_name, building_name) == 0) {
            for (int j = 0; j < buildings[i].room_count; j++) {
                if (strcmp(buildings[i].rooms[j].room_id, room_id) == 0) {
                    if (strlen(buildings[i].rooms[j].reservations[day][time_slot].user_id) == 0) {
                        strcpy(buildings[i].rooms[j].reservations[day][time_slot].user_id, user_id);
                        strcpy(buildings[i].rooms[j].reservations[day][time_slot].purpose, purpose);
                        printf("�������� ������ ���������� �Ϸ�Ǿ����ϴ�.\n");
                        save_reservations();
                        return;
                    }
                    else {
                        printf("�ش� ���ǽ��� �̹� ����Ǿ� �ֽ��ϴ�.\n");
                        return;
                    }
                }
            }
        }
    }
    printf("���ǽ��� �����ϴ�.\n");
}

int main() {
    printf("���ε� ���̵� - ���ǽ� ���� �ý���\n");
    load_building_data();
    load_reservations();
    int choice;

    while (1) {
        printf("\n1. �ð�ǥ ����\n");
        printf("2. ����ִ� ���ǽ� �˻�\n");
        printf("3. ���ǽ� ���� ��û\n");
        printf("4. ����\n");
        printf("5. ���� ��\n");
        printf("�������ּ���: ");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("�߸��� �Է��Դϴ�. �ٽ� �õ����ּ���.\n");
            while (getchar() != '\n');  // �Է� ���� ����
            continue;
        }

        if (choice == 1) {
            char building_name[50], room_id[10];
            printf("�ǹ� �̸��� �Է����ּ���: ");
            scanf("%s", building_name);
            printf("���ǽ� ��ȣ�� �Է����ּ���: ");
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
            printf("�ش��� (0: ��, 1: ȭ, ..., 4: ��): ");
            if (scanf("%d", &day) != 1 || day < 0 || day >= DAYS) {
                printf("�߸��� �Է��Դϴ�. �ٽ� �õ����ּ���.\n");
                while (getchar() != '\n');  // �Է� ���� ����
                continue;
            }
            printf("�ð� ��ư (0: 9��-10��, ..., 8: 5��-6��): ");
            if (scanf("%d", &time_slot) != 1 || time_slot < 0 || time_slot >= TIME_SLOTS) {
                printf("�߸��� �Է��Դϴ�. �ٽ� �õ����ּ���.\n");
                while (getchar() != '\n');  // �Է� ���� ����
                continue;
            }
            find_and_print_empty_rooms(day, time_slot);
        }
        else if (choice == 3) {
            make_reservation();
        }
        else if (choice == 4) {
            printf("�̿����ּż� �����մϴ�.\n");
            break;
        }
        else if (choice == 5) {
            int day, time_slot;
            printf("�ش��� (0: ��, 1: ȭ, ..., 4: ��): ");
            scanf("%d", &day);
            printf("�ð� ��ư (0: 9��-10��, ..., 8: 5��-6��): ");
            scanf("%d", &time_slot);
            compare_performance(day, time_slot);
        }

    }
    return 0;
}
