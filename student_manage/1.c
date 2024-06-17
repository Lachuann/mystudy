#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    char id[20];
    char name[50];
    char gender[10];
    char dob[20];
    int englishScore;
    int dataStructureScore;
    struct Student *next;
} Student;

Student *head = NULL;

void InsertList() {
    Student *student = (Student*)malloc(sizeof(Student));
    printf("请输入学生信息（学号 姓名 性别 出生年月 大学英语成绩 数据结构成绩）:\n");
    scanf("%s %s %s %s %d %d", student->id, student->name, student->gender, student->dob, &(student->englishScore), &(student->dataStructureScore));
    student->next = head;
    head = student;
}

void DeleteList() {
    char id[20];
    printf("请输入要删除的学生学号:\n");
    scanf("%s", id);
    Student *temp = head, *prev = NULL;
    while(temp != NULL && strcmp(temp->id, id) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if(temp == NULL) {
        printf("没有找到学号为 %s 的学生\n", id);
        return;
    }
    if(prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
    printf("学号为 %s 的学生已被删除\n", id);
}

void TraverseList() {
    Student *temp = head;
    while(temp != NULL) {
        printf("%s %s %s %s %d %d\n", temp->id, temp->name, temp->gender, temp->dob, temp->englishScore, temp->dataStructureScore);
        temp = temp->next;
    }
}

void SearchList() {
    char id[20];
    printf("请输入要查找的学生学号:\n");
    scanf("%s", id);
    Student *temp = head;
    while(temp != NULL && strcmp(temp->id, id) != 0) {
        temp = temp->next;
    }
    if(temp == NULL) {
        printf("没有找到学号为 %s 的学生\n", id);
    } else {
        printf("%s %s %s %s %d %d\n", temp->id, temp->name, temp->gender, temp->dob, temp->englishScore, temp->dataStructureScore);
    }
}

void SortList() {
    Student *i, *j;
    for(i = head; i != NULL; i = i->next) {
        for(j = i->next; j != NULL; j = j->next) {
            if((i->englishScore + i->dataStructureScore) < (j->englishScore + j->dataStructureScore)) {
                Student temp = *i;
                *i = *j;
                *j = temp;
                Student *tempNext = i->next;
                i->next = j->next;
                j->next = tempNext;
            }
        }
    }
}

int main() {
    int choice;
    while(1) {
        printf("1. 添加学生信息\n2. 删除学生信息\n3. 显示所有学生信息\n4. 查找学生信息\n5. 根据总分排序学生信息\n6. 退出\n请输入你的选择:\n");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                InsertList();
                break;
            case 2:
                DeleteList();
                break;
            case 3:
                TraverseList();
                break;
            case 4:
                SearchList();
                break;
            case 5:
                SortList();
                break;
            case 6:
                exit(0);
            default:
                printf("无效的选择\n");
        }
    }
    return 0;
}