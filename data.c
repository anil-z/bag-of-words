#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

struct list {
    char termName[30];
    int tekrarSayisi;
    struct list *next;
    struct secList *gecis;
};

struct secList {

    int docID;
    int totalInThis;
    int lengthOfDoc;
    char category[8];
    struct secList *next;
};

struct econList {
    char term[30];
    struct econList *next;
};

struct healthList {
    char term[30];
    struct healthList *next;
};

struct magList {
    char term[30];
    struct magList *next;
};

typedef struct list node;
typedef struct secList secNode;
typedef struct econList econNode;
typedef struct healthList healthNode;
typedef struct magList magNode;


node *root;
econNode *econRoot;
healthNode *healthRoot;
magNode *magRoot;

int kontrol(char *st, char *stKategori, int stNum, int ktktrSayisi) {

    // Eğer aradığımız veri daha önceden kayıtlıysa 0 döndürüyor yoksa 1 döndürüyor

    node *kontrolNode;
    kontrolNode = root;

    while (kontrolNode != NULL) {
        if (strcmp(kontrolNode->termName, st) == 0) {
            node *iter;
            iter = kontrolNode;


            int eklenmis = 0;

            secNode *temp;
            temp = iter->gecis;

            while (temp->next != NULL) {
                if (strcmp(temp->category, stKategori) == 0 && temp->docID == stNum)
                    eklenmis = 1;
                temp = temp->next;
            }

            if (strcmp(temp->category, stKategori) == 0 && temp->docID == stNum)
                eklenmis = 1;


            if (!eklenmis) {
                temp->next = malloc(sizeof(secNode));
                temp->next->docID = stNum;
                strcpy(temp->next->category, stKategori);
                temp->next->lengthOfDoc = ktktrSayisi;
                temp->next->totalInThis = 1;
                temp->next->next = NULL;
            } else {
                temp->totalInThis++;
            }


            return 0;
        } else {
            if (kontrolNode->next != NULL) {
                kontrolNode = kontrolNode->next;
            } else {
                break;
            }
        }
    }
    return 1;
}

void bolVeEkle(char *st, char *kategori, int num, int krktrNum) {

    char *ch;
    char *rest = st;

    while ((ch = strtok_r(rest, " ", &rest))) {
        int bool = 1;
        if (root == NULL) {
            // LL bos ise
            root = (node *) malloc(sizeof(node));
            strcpy(root->termName, ch);
            root->next = NULL;

            root->gecis = malloc(sizeof(secNode));
            root->gecis->docID = num;
            strcpy(root->gecis->category, kategori);
            root->gecis->lengthOfDoc = krktrNum;
            root->gecis->totalInThis = 1;
            root->gecis->next = NULL;


        } else if (kontrol(ch, kategori, num, krktrNum)) {

            if (strcmp(root->termName, ch) > 0) {

                // Gelen ch roottan küçükse

                node *temp = malloc(sizeof(node));
                strcpy(temp->termName, ch);
                temp->next = root;

                temp->gecis = malloc(sizeof(secNode));
                temp->gecis->docID = num;
                strcpy(temp->gecis->category, kategori);
                temp->gecis->lengthOfDoc = krktrNum;
                temp->gecis->totalInThis = 1;
                temp->gecis->next = NULL;

                root = temp;
                bool = 0;
            }

            if (bool) {


                node *iter = root;

                while (iter->next != NULL && strcmp(iter->next->termName, ch) < 0) {
                    iter = iter->next;
                }

                node *temp = malloc(sizeof(node));
                strcpy(temp->termName, ch);

                temp->gecis = malloc(sizeof(secNode));
                temp->gecis->docID = num;
                strcpy(temp->gecis->category, kategori);
                temp->gecis->lengthOfDoc = krktrNum;
                temp->gecis->totalInThis = 1;
                temp->gecis->next = NULL;

                temp->next = iter->next;
                iter->next = temp;
            }

        }
    }

}

int karakterSayisiBul(char *fileName) {
    int numberOfCharacters = 0;

    FILE *tempFile;
    tempFile = fopen(fileName, "r");

    while (!feof(tempFile)) {
        getc(tempFile);
        numberOfCharacters++;
    }
    numberOfCharacters--;

    fclose(tempFile);

    return numberOfCharacters;
}

int dosyaVarmi(const char *filename) {
    // duzgun calisiyor
    FILE *file;
    if ((file = fopen(filename, "r"))) {
        fclose(file);
        return 1;
    }
    return 0;
}

void tekrarBulucu() {
    //econ-5 sorunu bunda değil
    node *temp = root;
    secNode *sayiciNode;

    while (temp != NULL) {

        int count = 0;

        sayiciNode = temp->gecis;

        while (sayiciNode != NULL) {
            sayiciNode = sayiciNode->next;
            count++;
        }

        temp->tekrarSayisi = count;
        temp = temp->next;

    }
}

void bastir() {
    printf("Butun kelimeler\n");
    node *basici;
    basici = root;
    while (basici != NULL) {
        printf("%s\tx%d\n", basici->termName, basici->tekrarSayisi);
        basici = basici->next;
    }
}

void econWords(char onlyEcon[5][30]) {
    node *iter = root;
    int i = 0, control;

    econRoot = NULL;

    while (iter != NULL) {
        control = 1;
        if (strcmp(iter->gecis->category, "econ") == 0) {

            secNode *temp = iter->gecis;

            while (temp != NULL) {
                if (strcmp(temp->category, "econ") != 0) {
                    control = 0;
                    break;
                }
                temp = temp->next;
            }

            if (control) {
                if (i < 5) {
                    strcpy(onlyEcon[i], iter->termName);
                    i++;
                }
            }

            if (econRoot == NULL) {
                econRoot = malloc(sizeof(econNode));
                strcpy(econRoot->term, iter->termName);
                econRoot->next = NULL;

            } else {
                econNode *econIter = econRoot;
                while (econIter->next != NULL) {
                    econIter = econIter->next;
                }
                econIter->next = malloc(sizeof(econNode));
                strcpy(econIter->next->term, iter->termName);
                econIter->next->next = NULL;
            }

        }
        iter = iter->next;
    }
}

void healthWords(char onlyHealth[5][30]) {
    node *iter = root;
    healthRoot = NULL;

    int control, i = 0;

    while (iter != NULL) {
        control = 1;

        secNode *temp = iter->gecis;

        secNode *temp2 = iter->gecis;
        while (temp2 != NULL) {
            if (strcmp(temp2->category, "health") != 0) {
                control = 0;
                break;
            }
            temp2 = temp2->next;
        }

        if (control) {
            if (i < 5) {
                strcpy(onlyHealth[i], iter->termName);
                i++;
            }
        }


        while (strcmp(temp->category, "health") != 0) {
            temp = temp->next;
            if (temp == NULL)
                break;
        }

        if (temp != NULL) {
            if (healthRoot == NULL) {
                healthRoot = malloc(sizeof(healthNode));
                strcpy(healthRoot->term, iter->termName);
                healthRoot->next = NULL;
            } else {
                healthNode *healthIter;
                healthIter = healthRoot;
                while (healthIter->next != NULL) {
                    healthIter = healthIter->next;
                }
                healthIter->next = malloc(sizeof(healthNode));
                strcpy(healthIter->next->term, iter->termName);
                healthIter->next->next = NULL;
            }
        }

        iter = iter->next;
    }
}

void magWords(char onlyMag[5][30]) {
    node *iter = root;
    magRoot = NULL;

    int i = 0;

    while (iter != NULL) {

        secNode *temp = iter->gecis;

        if (strcmp(temp->category, "magazin") == 0) {
            if (i < 5) {
                strcpy(onlyMag[i], iter->termName);
                i++;
            }
        }

        while (strcmp(temp->category, "magazin") != 0) {
            temp = temp->next;
            if (temp == NULL)
                break;
        }
        if (temp != NULL) {
            if (magRoot == NULL) {
                magRoot = malloc(sizeof(magNode));
                strcpy(magRoot->term, iter->termName);
                magRoot->next = NULL;
            } else {
                magNode *magIter = magRoot;
                while (magIter->next != NULL) {
                    magIter = magIter->next;
                }
                magIter->next = malloc(sizeof(magNode));
                strcpy(magIter->next->term, iter->termName);
                magIter->next->next = NULL;
            }
        }

        iter = iter->next;
    }
}

void generalWords(char words[5][30]) {

    int i = 0, control;
    econNode *econIter = econRoot;
    while (econIter != NULL) {

        healthNode *healthIter = healthRoot;

        while (healthIter != NULL) {
            control = 1;

            if (strcmp(econIter->term, healthIter->term) == 0) {
                magNode *magIter = magRoot;
                while (magIter != NULL) {

                    if (strcmp(econIter->term, magIter->term) == 0) {
                        control = 0;
                        if (i < 5) {
                            strcpy(words[i], econIter->term);
                            i++;
                        }
                        break;
                    }
                    magIter = magIter->next;
                }

            }
            if (control)
                healthIter = healthIter->next;
            else
                break;
        }

        econIter = econIter->next;
    }

}

int main() {
    setlocale(LC_ALL, "Turkish");

    int numberOfCharacters = 0;

    int i = 1, j = 1, k = 1;
    char dosyaNumarasi[3];

    root = NULL;


    while (1) {
        char filePath[70] = "C:\\Users\\sasuk\\Desktop\\dataset\\econ\\";
        itoa(i, dosyaNumarasi, 10);
        strcat(filePath, dosyaNumarasi);
        strcat(filePath, ".txt");

        if (dosyaVarmi(filePath)) {
            numberOfCharacters = karakterSayisiBul(filePath);
            FILE *file;
            file = fopen(filePath, "r");
            char str[numberOfCharacters + 1], mainStr[numberOfCharacters + 1];
            strcpy(mainStr, " ");
            while (fgets(str, numberOfCharacters + 1, file) != NULL) {
                strcat(mainStr, str);
            }
            // Burası \n leri bulup çıkarması için
            int index = 0;
            for (index = 0; mainStr[index] != '\0'; index++) {
                if (mainStr[index] == '\n')
                    mainStr[index] = ' ';
            }
            bolVeEkle(mainStr, "econ", i, numberOfCharacters);
            i++;
        } else {
            break;

        }

    }


    while (1) {
        char filePath[70] = "C:\\Users\\sasuk\\Desktop\\dataset\\health\\";
        itoa(j, dosyaNumarasi, 10);
        strcat(filePath, dosyaNumarasi);
        strcat(filePath, ".txt");

        if (dosyaVarmi(filePath)) {
            numberOfCharacters = karakterSayisiBul(filePath);
            FILE *file;
            file = fopen(filePath, "r");
            char str[numberOfCharacters + 1], mainStr[numberOfCharacters + 1];
            strcpy(mainStr, " ");
            while (fgets(str, numberOfCharacters + 1, file) != NULL) {
                strcat(mainStr, str);
            }
            // Burası \n leri bulup çıkarması için
            int index = 0;
            for (index = 0; mainStr[index] != '\0'; index++) {
                if (mainStr[index] == '\n')
                    mainStr[index] = ' ';
            }
            bolVeEkle(mainStr, "health", j, numberOfCharacters);
            j++;
        } else {
            break;

        }

    }


    while (1) {
        char filePath[70] = "C:\\Users\\sasuk\\Desktop\\dataset\\magazin\\";
        itoa(k, dosyaNumarasi, 10);
        strcat(filePath, dosyaNumarasi);
        strcat(filePath, ".txt");

        if (dosyaVarmi(filePath)) {
            numberOfCharacters = karakterSayisiBul(filePath);
            FILE *file;
            file = fopen(filePath, "r");
            char str[numberOfCharacters + 1], mainStr[numberOfCharacters + 1];
            strcpy(mainStr, " ");
            while (fgets(str, numberOfCharacters + 1, file) != NULL) {
                strcat(mainStr, str);
            }
            // Burası \n leri bulup çıkarması için
            int index = 0;
            for (index = 0; mainStr[index] != '\0'; index++) {
                if (mainStr[index] == '\n')
                    mainStr[index] = ' ';
            }
            bolVeEkle(mainStr, "magazin", k, numberOfCharacters);
            k++;
        } else {
            break;

        }

    }

    node *iter = root;

    for (i = 0; i < 401; i++) {
        iter = iter->next;
    }

    tekrarBulucu();

    bastir();

    char onlyEcon[5][30];
    char onlyHealth[5][30];
    char onlyMag[5][30];

    econWords(onlyEcon);
    healthWords(onlyHealth);
    magWords(onlyMag);

    char commonWords[5][30];

    generalWords(commonWords);

    printf("\nGeneral words\n");

    for (i = 0; i < 5; i++) {
        printf("%s\n", commonWords[i]);
    }

    printf("\nDiscriminating for econ\n");
    for (i = 0; i < 5; i++) {
        printf("%s\n", onlyEcon[i]);
    }

    printf("\nDiscriminating for health\n");
    for (i = 0; i < 5; i++) {
        printf("%s\n", onlyHealth[i]);
    }

    printf("\nDiscriminating for magazin\n");
    for (i = 0; i < 5; i++) {
        printf("%s\n", onlyMag[i]);
    }


    return 0;
}