#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "bookings.dat"

typedef struct {
    int booking_id;
    char name[50];
    char phone[20];
    int room_type;   // 1-Deluxe, 2-Super Deluxe, 3-Suite
    int nights;
    float amount;
} Booking;

float calculateAmount(int room_type, int nights) {
    switch(room_type) {
        case 1: return 3500 * nights;
        case 2: return 5000 * nights;
        case 3: return 8000 * nights;
        default: return 0;
    }
}

void addBooking() {
    FILE *fp = fopen(FILENAME, "ab");
    Booking b;
    printf("\n--- Add New Booking ---\n");

    printf("Enter Booking ID: ");
    scanf("%d", &b.booking_id);

    printf("Enter customer name: ");
    scanf(" %[^\n]", b.name);

    printf("Enter phone number: ");
    scanf(" %[^\n]", b.phone);

    printf("Room Type (1-Deluxe, 2-Super Deluxe, 3-Suite): ");
    scanf("%d", &b.room_type);

    printf("Number of nights: ");
    scanf("%d", &b.nights);

    b.amount = calculateAmount(b.room_type, b.nights);

    fwrite(&b, sizeof(Booking), 1, fp);
    fclose(fp);

    printf("\nBooking Added Successfully!\n");
}

void displayBookings() {
    FILE *fp = fopen(FILENAME, "rb");
    Booking b;

    printf("\n--- All Bookings ---\n");
    printf("%-10s %-20s %-15s %-10s %-10s %-10s\n",
           "ID", "Name", "Phone", "Room", "Nights", "Amount");

    while (fread(&b, sizeof(Booking), 1, fp)) {
        printf("%-10d %-20s %-15s %-10d %-10d %-10.2f\n",
               b.booking_id, b.name, b.phone, b.room_type, b.nights, b.amount);
    }

    fclose(fp);
}

void searchBooking() {
    FILE *fp = fopen(FILENAME, "rb");
    Booking b;
    char name[50];

    printf("\nEnter name to search: ");
    scanf(" %[^\n]", name);

    int found = 0;
    while (fread(&b, sizeof(Booking), 1, fp)) {
        if (strcasecmp(b.name, name) == 0) {
            printf("\nBooking Found!\n");
            printf("ID: %d\nName: %s\nPhone: %s\nRoom: %d\nNights: %d\nAmount: %.2f\n",
                   b.booking_id, b.name, b.phone, b.room_type, b.nights, b.amount);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) printf("\nNo booking found.\n");
}

void deleteBooking() {
    FILE *fp = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    Booking b;
    int id, found = 0;

    printf("\nEnter Booking ID to delete: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(Booking), 1, fp)) {
        if (b.booking_id != id) {
            fwrite(&b, sizeof(Booking), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if(found)
        printf("\nBooking Deleted Successfully!\n");
    else
        printf("\nBooking ID not found!\n");
}

void modifyBooking() {
    FILE *fp = fopen(FILENAME, "rb+");
    Booking b;
    int id, found = 0;

    printf("\nEnter Booking ID to modify: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(Booking), 1, fp)) {
        if (b.booking_id == id) {
            printf("Enter new phone number: ");
            scanf(" %[^\n]", b.phone);

            printf("Enter new number of nights: ");
            scanf("%d", &b.nights);

            b.amount = calculateAmount(b.room_type, b.nights);

            fseek(fp, -sizeof(Booking), SEEK_CUR);
            fwrite(&b, sizeof(Booking), 1, fp);

            found = 1;
            printf("\nBooking Updated Successfully!\n");
            break;
        }
    }

    fclose(fp);

    if(!found)
        printf("\nBooking ID not found!\n");
}

int main() {
    int choice;

    while (1) {
        printf("\n========= BLUE WATER RESORT BOOKING SYSTEM =========\n");
        printf("1. Add Booking\n");
        printf("2. View All Bookings\n");
        printf("3. Search Booking\n");
        printf("4. Modify Booking\n");
        printf("5. Delete Booking\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addBooking(); break;
            case 2: displayBookings(); break;
            case 3: searchBooking(); break;
            case 4: modifyBooking(); break;
            case 5: deleteBooking(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
