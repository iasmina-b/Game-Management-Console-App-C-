#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

#define RESET       "\033[0m"
#define BLUE 	    "\033[38;5;153m"
#define LAVENDER    "\033[38;5;183m"
#define GREEN       "\033[38;5;151m"
#define YELLOW      "\033[38;5;230m"

void header()
{
    printf("\n-------------------------------------------------------------------------------------------------------\n");
    printf("1-Installed || ");
    printf("2-Available || ");
    printf("3-Install || ");
    printf("4-Uninstall || ");
    printf("5-Sort by Name || ");
    printf("6-Sort by Size || ");
    printf("0-Exit\n");
    printf("-------------------------------------------------------------------------------------------------------\n");

}

void clearScreen() {
    system(CLEAR_SCREEN);

}

 typedef struct{
    char name[50];
    int size;
} Game;

Game InstalledGames[99];
Game AvailableGames[99];
int installedCount = 0;
int AvailableCount = 0;
int memoryUsed = 0;


void loadGamesFromFile(const char *filename, Game *games, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file)
        return;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        sscanf(line, "%[^,],%d", games[*count].name, &games[*count].size);
        (*count)++;
    }
    fclose(file);
}

void saveGamesToFile(const char *filename, Game *games, int count) {
    FILE *file = fopen(filename, "w");
    if (!file) return;

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%d\n", games[i].name, games[i].size);
    }
    fclose(file);
}

void installedGames(){
    clearScreen();
    printf(GREEN"--- Installed Games ---\n"RESET);
    if (installedCount == 0) {
        printf("No games installed.\n");
        }
    else {
        for (int i = 0; i < installedCount; i++) {
            printf(BLUE "%s " RESET LAVENDER "%d" RESET "GB \n", InstalledGames[i].name, InstalledGames[i].size);
        }
    }
    printf("------------------------\n");
    printf("Press" YELLOW " Enter" RESET " to go back.");
    getchar();
    getchar();
    clearScreen();
}

void availableGames(){
    clearScreen();
    printf(GREEN"--- Available Games ---\n"RESET);
    for (int i=0; i<AvailableCount; i++) {
        printf(BLUE "%s " RESET LAVENDER "%d" RESET "GB \n", AvailableGames[i].name, AvailableGames[i].size);
    }
    printf("------------------------\n");
    printf("Press" YELLOW " Enter" RESET " to go back.");
    getchar();
    getchar();
    clearScreen();
}

void installGame() {
    clearScreen();
    printf(GREEN"--- Install Game ---\n"RESET);
    char gameName[50];
    printf("Enter the name of the game to install: ");
    scanf("%s", gameName);
    for (int i = 0; i < AvailableCount; i++) {
        if (strcmp(AvailableGames[i].name, gameName) == 0) {
            if (memoryUsed + AvailableGames[i].size > 1028) {
                printf("Not enough memory to install the game \n");

            }
            if (strcmp(InstalledGames[i].name, gameName) == 0) {
            printf("Game '%s' is already installed.\n", gameName);
            }
            else {
                InstalledGames[installedCount++] = AvailableGames[i];
                memoryUsed += AvailableGames[i].size;
                saveGamesToFile("installed_games.txt", InstalledGames, installedCount);
                printf("Game '%s' installed successfully!\n", gameName);
            }
            printf("------------------------\n");
            printf("Press" YELLOW " Enter" RESET " to go back.");
            getchar();
            getchar();
            clearScreen();
            return;
        }
    }
    printf("Game not found. \n");
    printf("------------------------\n");
    printf("Press" YELLOW " Enter" RESET " to go back.");
    getchar();
    getchar();
    clearScreen();
}

void uninstallGame() {
    clearScreen();
    printf(GREEN"--- Uninstall Game ---\n"RESET);
    char gameName[50];
    printf("Enter the name of the game to uninstall: ");
    scanf("%s", gameName);
    for (int i = 0; i < installedCount; i++) {
        if (strcmp(InstalledGames[i].name, gameName) == 0) {
            memoryUsed -= InstalledGames[i].size;
            for (int j = i; j < installedCount - 1; j++) {
                InstalledGames[j] = InstalledGames[j + 1];
            }
            installedCount--;
            saveGamesToFile("installed_games.txt", InstalledGames, installedCount);
            printf("Game '%s' uninstalled successfully!\n", gameName);
            printf("------------------------\n");
            printf("Press" YELLOW " Enter" RESET " to go back.");
            getchar();
            getchar();
            clearScreen();
            return;
        }
    }
    printf("Game is not installed. \n");
    printf("------------------------\n");
    printf("Press" YELLOW " Enter" RESET " to go back.");
    getchar();
    getchar();
    clearScreen();
}

int compareByName(const void *a, const void *b) {
    return strcmp(((Game *)a)->name, ((Game *)b)->name);
}

int compareBySize(const void *a, const void *b) {
    return ((Game *)a)->size - ((Game *)b)->size;
}

void sortByName() {
    clearScreen();
    qsort(InstalledGames, installedCount, sizeof(Game), compareByName);
    printf(GREEN"--- Games sorted by name ---\n"RESET);
    printf("------------------------\n");
    printf("Press" YELLOW " Enter" RESET " to go back.");
    getchar();
    getchar();
    clearScreen();
}

void sortBySize() {
    clearScreen();
    qsort(InstalledGames, installedCount, sizeof(Game), compareBySize);
    printf(GREEN"--- Games sorted by size ---\n"RESET);
    printf("------------------------\n");
    printf("Press" YELLOW " Enter" RESET " to go back.");
    getchar();
    getchar();
    clearScreen();
}

void goodbye(){
    clearScreen();
    printf(GREEN"--- Goodbye! ---\n"RESET);


}

void menu(int option) {
    switch (option) {
        case 1:
            installedGames();
            break;
        case 2:
            availableGames();
            break;
        case 3:
            installGame();
            break;
        case 4:
            uninstallGame();
            break;
        case 5:
            sortByName();
            break;
        case 6:
            sortBySize();
            break;
        case 0:
            goodbye();
            exit(0);
        default:
            printf("Invalid option. Please try again.\n");
    }
}

int main()
{   loadGamesFromFile("installed_games.txt", InstalledGames, &installedCount);
    loadGamesFromFile("available_games.txt", AvailableGames, &AvailableCount);
    int option;
    do{
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        menu(option);}
    while(option != 0);
        return 0;
}


