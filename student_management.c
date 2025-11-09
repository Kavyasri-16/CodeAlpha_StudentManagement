#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"

struct Student {
    int id;
    char name[50];
    int age;
    float marks;
};

// Function declarations
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    int choice;
    while (1) {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                printf("Exiting... Thank you!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// ---------------- Add Student ----------------
void addStudent() {
    FILE *fp;
    struct Student s;

    fp = fopen(FILENAME, "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &s.id);

    printf("Enter Name: ");
    getchar(); // clear buffer before fgets
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0; // remove newline

    printf("Enter Age: ");
    scanf("%d", &s.age);

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);

    printf("Record added successfully!\n");
}

// ---------------- Display All Students ----------------
void displayStudents() {
    FILE *fp;
    struct Student s;

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("\n%-10s %-20s %-10s %-10s\n", "ID", "Name", "Age", "Marks");
    printf("-------------------------------------------------\n");

    while (fread(&s, sizeof(struct Student), 1, fp)) {
        printf("%-10d %-20s %-10d %-10.2f\n", s.id, s.name, s.age, s.marks);
    }

    fclose(fp);
}

// ---------------- Search Student ----------------
void searchStudent() {
    FILE *fp;
    struct Student s;
    int id, found = 0;

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Student ID to search: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(struct Student), 1, fp)) {
        if (s.id == id) {
            printf("\nRecord Found:\n");
            printf("ID: %d\nName: %s\nAge: %d\nMarks: %.2f\n", s.id, s.name, s.age, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("No record found with ID %d\n", id);

    fclose(fp);
}

// ---------------- Update Student ----------------
void updateStudent() {
    FILE *fp, *temp;
    struct Student s;
    int id, found = 0;

    fp = fopen(FILENAME, "rb");
    temp = fopen("temp.dat", "wb");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Student ID to update: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(struct Student), 1, fp)) {
        if (s.id == id) {
            printf("Enter new details:\n");
            printf("Enter Name: ");
            getchar(); // clear buffer before fgets
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0;

            printf("Enter Age: ");
            scanf("%d", &s.age);
            printf("Enter Marks: ");
            scanf("%f", &s.marks);
            found = 1;
        }
        fwrite(&s, sizeof(struct Student), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found)
        printf("Record updated successfully!\n");
    else
        printf("No record found with ID %d\n", id);
}

// ---------------- Delete Student ----------------
void deleteStudent() {
    FILE *fp, *temp;
    struct Student s;
    int id, found = 0;

    fp = fopen(FILENAME, "rb");
    temp = fopen("temp.dat", "wb");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(struct Student), 1, fp)) {
        if (s.id != id) {
            fwrite(&s, sizeof(struct Student), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found)
        printf("Record deleted successfully!\n");
    else
        printf("No record found with ID %d\n", id);
}
