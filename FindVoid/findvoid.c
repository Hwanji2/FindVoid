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

void load_building_data() {
    FILE* fp = fopen("buildings.txt", "r");
    if (fp == NULL) {
        printf("buildings.txt ������ �ε��� �� �����ϴ�.\n");
        exit(1);
    }
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
    FILE* fp = fopen("reservations.txt", "r");
    if (fp == NULL) {
        printf("reservations.txt ������ �ε��� �� �����ϴ�.\n");
        return;
    }
    int building_index, room_index, day, time_slot;
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
    printf("\t��\tȭ\t��\t��\t��\n");
    for (int t = 0; t < TIME_SLOTS; t++) {
        printf("%d-%d��\t", 9 + t, 10 + t);
        for (int d = 0; d < DAYS; d++) {
            if (strlen(room->reservations[d][t].user_id) > 0) {
                printf("%s\t", room->reservations[d][t].purpose);
            }
            else {
                printf("�������\t");
            }
        }
        printf("\n");
    }
}

void find_and_print_empty_rooms(int day, int time_slot) {
    printf("\n���� ����ִ� ���ǽ� - �ش���: %d, �ð� ��ư: %d\n", day, time_slot);
    for (int i = 0; i < building_count; i++) {
        printf("�ǹ�: %s\n", buildings[i].building_name);
        for (int j = 0; j < buildings[i].room_count; j++) {
            if (strlen(buildings[i].rooms[j].reservations[day][time_slot].user_id) == 0) {
                printf("���ǽ�: %s\n", buildings[i].rooms[j].room_id);
            }
        }
    }
}

void make_reservation() {
    char building_name[50], room_id[10], user_id[10], purpose[50];
    int day, time_slot;
    printf("�ǹ� �̸��� �Է����ּ���: ");
    scanf("%s", building_name);
    printf("���ǽ� ID�� �Է����ּ���: ");
    scanf("%s", room_id);
    printf("�ش��� (0: ��, 1: ȭ, ..., 4: ��): ");
    scanf("%d", &day);
    printf("�ð� ��ư (0: 9��-10��, ..., 8: 5��-6��): ");
    scanf("%d", &time_slot);
    printf("����� ID�� �Է����ּ���: ");
    scanf("%s", user_id);
    printf("��� ���: ");
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
        printf("�������ּ���: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char building_name[50], room_id[10];
            printf("�ǹ� �̸��� �Է����ּ���: ");
            scanf("%s", building_name);
            printf("���ǽ� ID�� �Է����ּ���: ");
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
            scanf("%d", &day);
            printf("�ð� ��ư (0: 9��-10��, ..., 8: 5��-6��): ");
            scanf("%d", &time_slot);
            find_and_print_empty_rooms(day, time_slot);
        }
        else if (choice == 3) {
            make_reservation();
        }
        else if (choice == 4) {
            printf("�̿����ּż� �����մϴ�.\n");
            break;
        }
        else {
            printf("�߸��� �Է��Դϴ�. �ٽ� �õ����ּ���.\n");
        }
    }
    return 0;
}
