#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "penyimpanan.h"
#include "admin.h"
#include "user.h"

int login(User daftar_user[], int jumlah_user,
          char username_login[], char role_login[]) {

    char password[50];

    printf("\n=== LOGIN PERPUSTAKAAN ===\n");

    printf("Username: ");
    scanf("%49s", username_login);

    printf("Password: ");
    scanf("%49s", password);

    for (int i = 0; i < jumlah_user; i++) {

        if (strcmp(username_login, daftar_user[i].username) == 0 &&
            strcmp(password, daftar_user[i].password) == 0) {

            strcpy(role_login, daftar_user[i].role);
            return 1;
        }
    }

    printf("Username atau password salah!\n");
    return 0;
}

int main() {

    User *daftar_user = NULL;
    int jumlah_user = 0;

    char username_login[50];
    char role_login[20];

    if (!baca_user("user.txt", &daftar_user, &jumlah_user)) {
        printf("Gagal membaca data akun!\n");
        return 1;
    }

    if (login(daftar_user, jumlah_user, username_login, role_login)) {

        if (strcmp(role_login, "admin") == 0) {
            printf("\nLogin sebagai Admin berhasil!\n");
            menu_admin();
        }
        else if (strcmp(role_login, "user") == 0) {
            printf("\nLogin sebagai User berhasil!\n");
            menu_user(username_login);
        }
        else {
            printf("Role tidak dikenal!\n");
        }
    }

    free(daftar_user);

    return 0;
}