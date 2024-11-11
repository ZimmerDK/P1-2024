#include "UserData.h"
#include "exercises.h"

#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

typedef struct account {
    char username[20];
    exercise_data_t exercise_data;
} account;

static struct account accounts[10];

void read_file(struct account accounts[]) {
    FILE *file;
    int found = 0;
    int i=0;   // count how many lines are in the file
    int c;
    file=fopen("UserData.txt", "r");
    while(!feof(file)) {
        c=fgetc(file);
        if(c=='\n')
            ++i;
    }

    int j=0;
    rewind(file);  // Line I added
    // read each line and put into accounts
    while(j!=i-1) {
        fscanf(file, "%s %s", accounts[j].id, accounts[j].password);
        ++j;
    }

    i = 0;
    while(!feof(file) && !found)
    {
        ++i;
        fscanf(file, "%s", username);
        if (strcmp(username, "") == 0)
        {
            printf("Record found (#%d): %s\n", i, username);
            found = 1;
        }
    }
    if (!found)
        printf("Record could not be found");
}
int main()
{
    read_file(accounts);
    // check if it works or not
    printf("%s, %s,\n%s, %s,\n%s, %s\n",
        accounts[0].id, accounts[0].password,
        accounts[1].id, accounts[1].password,
        accounts[2].id, accounts[2].password);
    return 0;
}

const char* USER_DATA_HEADER_PATH = "./UserData.dat";
