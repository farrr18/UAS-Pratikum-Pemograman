#include "penyimpanan.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024

static void trim_spasi(char *str){
    int awal = 0, akhir = (int)strlen(str)-1;

    while (str[awal] != '\0' && isspace((unsigned char)str[awal])) {awal++;}
    while (akhir >= awal && isspace((unsigned char)str[akhir])) {akhir--;}
    
    int j = 0;
    for (int i = awal; i <= akhir; i++) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}