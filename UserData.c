#include "UserData.h"
#include "exercises.h"

/*
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
        fscanf(file, "%s %s", accounts[j].username, accounts[j].password);
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
*/

HashMap* createHashMap() {
    HashMap* map = malloc(sizeof(HashMap));
    map->entries = calloc(HASHMAP_SIZE, sizeof(HashMapEntry));
    map->size = 0;
    return map;
}

unsigned int hash(char* key) {
    unsigned int hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % HASHMAP_SIZE;
}

void set(HashMap* map, char* key, int value) {
    unsigned int index = hash(key);

    if (map->entries[index].key == NULL) {
        map->entries[index].key = strdup(key);
        map->entries[index].value = value;
        map->size++;
    }

    else if (strcmp(map->entries[index].key, key) == 0) {
        map->entries[index].value = value;
    }

    else {
        for (int i = (index + 1) % HASHMAP_SIZE; i != index; i = (i + 1) % HASHMAP_SIZE) {
            if (map->entries[i].key == NULL) {
                map->entries[i].key = strdup(key);
                map->entries[i].value = value;
                map->size++;
                return;
            }
            else if (strcmp(map->entries[i].key, key) == 0) {
                map->entries[i].value = value;
                return;
            }
        }
        // If no available slot is found, the hashmap is full
        printf("Error: Hashmap is full\n");
    }
}

int get(HashMap* map, char* key) {
    unsigned int index = hash(key);

    if (map->entries[index].key == NULL) {
        return -1;
    }
    else if (strcmp(map->entries[index].key, key) == 0) {
        return map->entries[index].value;
    }
    else {
        for (int i = (index + 1) % HASHMAP_SIZE; i != index; i = (i + 1) % HASHMAP_SIZE) {
            if (map->entries[i].key == NULL) {
                return -1;
            }
            else if (strcmp(map->entries[i].key, key) == 0) {
                return map->entries[i].value;
            }
        }
        return -1;
    }
}

int userVerify(char* usernameInput, HashMap* map) {
    int index = get(map, usernameInput);
    if (index == -1) {
        printf("Invalid word.\n");
    } else {
        return 1;
    }
    return 0;
}

void create_new_user(FILE* accountsFILE, char username[MAX_LENGTH], HashMap* map) {
    int user_count = 0;


    //for(int i = 0; i <)

    fwrite(username, 16, 1, accountsFILE);


}

int main() {
    char input[MAX_LENGTH];

    // Create username hashmap
    HashMap* map = createHashMap();
    char tempUsername[128];
    int m = 0;
    while (fscanf(accountsFILE, "%s", tempUsername) != EOF) {
        set(map, tempUsername, m);
        m++;
    }
    printf("\nLoaded %d usernames into the hashmap\n", m);

    printf("\nLogin or signup?");
    printf("\nType 'login' if you already have an account.");
    printf("\nType 'signup' if you want to create a new account.");

    userVerify(input, map);
    create_new_user(accountsFILE, input, map);
    /*// check if it works or not
    printf("%s, %s,\n%s, %s,\n%s, %s\n",
        accounts[0].id, accounts[0].password,
        accounts[1].id, accounts[1].password,
        accounts[2].id, accounts[2].password);
        */
    return 0;
}
