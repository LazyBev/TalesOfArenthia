#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SUCCESS 1
#define FAILURE 0
#define MAX_NAME_LEN 100
#define MAX_PASS_LEN 20
#define DEF_INV_SIZE 10
#define DEFLT_HP 10
#define DEFLT_ATK 10
#define DEFLT_DEF 10
#define MAX_REGIONS 4

// Player structure to hold player details
typedef struct {
    char name[MAX_NAME_LEN];
    int hp;
    int attack;
    int defense;
    int inventorySize;
    int fragmentsCollected;
    int gold;
} Player;

// Function declarations
int gameLoop(Player *player);
int menuStart();
int menuPause(Player *player);
int loadGame(Player *player);
int saveGame(Player *player);
int whichBoss(Player *player);
int createNewPlayer(Player *player);
int encounterGuardian(Player *player, const char* guardianName, int guardianHp, int guardianAtk, int guardianDef);
void displayStory(const char* story);
void exploreRegion(Player *player);
void upgradeGear(Player *player);
void randomEncounter(Player *player);
void checkStats(Player *player);

// Global vars
int isNewPlyr = 1;

// Start menu function
int menuStart() {
    system("clear");
    printf("---- Welcome to lazyrpg! ----\n\n");

    FILE *new = fopen("playerData/isNew.txt", "r");
    int choice;

    if (new != NULL) {
        printf("It seems you're new here!\n");
        fclose(new);
        while (1) {
            printf("1) Start New Game\n");
            printf("2) Quit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            if (choice == 1) {
                Player player;
                if (createNewPlayer(&player) == SUCCESS) {
                    remove("playerData
            /isNew.txt"); // Remove isNew.txt as player starts a new game
                    gameLoop(&player);
                } else {
                    printf("Failed to create new player.\n");
                }
                break;
            } else if (choice == 2) {
                system("clear");
                exit(0);
            } else {
                printf("Invalid choice. Please try again.\n");
            }
        }
    } else {
        // Player has played before (isNew.txt does not exist)
        printf("Welcome back!\n");
        
        while (1) {
            printf("1) Continue\n");
            printf("2) Start New Game\n");
            printf("3) Quit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            Player player;
            if (choice == 1) {
                if (loadGame(&player) == SUCCESS) {
                    gameLoop(&player);
                } else {
                    printf("Failed to load the game. Starting new game.\n");
                    if (createNewPlayer(&player) == SUCCESS) {
                        gameLoop(&player);
                    } else {
                        printf("Failed to create new player.\n");
                    }
                }
                break;
            } else if (choice == 2) {
                // Mark as new by creating isNew.txt again
                system("clear");
                remove("playerData/savefile.txt");
                FILE *newFile = fopen("playerData
        /isNew.txt", "w");
                if (newFile == NULL) {
                    return FAILURE;
                }
                fclose(newFile);
                menuStart();
                break;
            } else if (choice == 3) {
                system("clear");
                printf("See ya next time! :D\n");
                exit(0);
            } else {
                printf("Invalid choice. Please try again.\n");
            }
        }
    }

    return SUCCESS;
}

int gameLoop(Player *player) {
    system("clear");
    printf("\nStarting game as %s\n", player->name);
    printf("Stats -> HP: %d, Attack: %d, Defense: %d, Gold: %d\n", player->hp, player->attack, player->defense, player->gold);
    displayStory("As the last heir of Arenthia, you embark on a quest to restore your kingdom, shattered by Drüig's dark magic.");

    int choice;
    while (player->fragmentsCollected < 5) { // Assuming 5 fragments to collect
        printf("\n---- Main Menu ----\n");
        printf("1) Explore Region\n");
        printf("2) Upgrade Gear\n");
        printf("3) Check Stats\n");  // New option to check stats
        printf("4) Quit to Pause Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            exploreRegion(player);
        } else if (choice == 2) {
            upgradeGear(player);
        } else if (choice == 3) {
            checkStats(player);
        } else if (choice == 4) {
            system("clear");
            menuPause(player);
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return SUCCESS;
}

void checkStats(Player *player) {
    printf("\n---- Player Stats ----\n");
    printf("Name: %s\n", player->name);
    printf("HP: %d\n", player->hp);
    printf("Attack: %d\n", player->attack);
    printf("Defense: %d\n", player->defense);
    printf("Gold: %d\n", player->gold);
    printf("Fragments Collected: %d\n", player->fragmentsCollected);
}

// Explore Region function
void exploreRegion(Player *player) {
    int regionChoice;
    while (1) {
        printf("\n---- Select a Region to Explore ----\n");
        printf("1) Forest of Whispering Shadows (Easy)\n");
        printf("2) Mount Frostbite (Medium)\n");
        printf("3) Desert of Lost Souls (Hard)\n");
        printf("4) Swamp of Despair (Very Hard)\n");
        printf("Enter your choice: ");
        scanf("%d", &regionChoice);

        if (regionChoice >= 1 && regionChoice <= 4) {
            randomEncounter(player);
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}

// Random encounter function
void randomEncounter(Player *player) {
    // Monster stats can be adjusted based on the region difficulty
    int monsterHp = rand() % 10 + 5; // Randomized HP between 5 and 15
    int monsterAtk = rand() % 5 + 3; // Randomized Attack between 3 and 8
    int monsterDef = rand() % 3 + 2; // Randomized Defense between 2 and 5

    printf("\nA wild monster appears!\n");
    encounterGuardian(player, "Wild Monster", monsterHp, monsterAtk, monsterDef);
    
    int goldReward = rand() % 10 + 5; // Randomized gold reward between 5 and 15
    player->gold += goldReward;
    printf("You found %d gold!\n", goldReward);
}

// Gear upgrade function
void upgradeGear(Player *player) {
    int upgradeChoice;
    int upgradeCost = 20;

    if (player->gold < upgradeCost) {
        printf("You don't have enough gold to upgrade your gear. Earn more by exploring!\n");
        return;
    }

    while (1) {
        printf("\n---- Upgrade Gear ----\n");
        printf("You have %d gold.\n", player->gold);
        printf("Upgrade cost: %d gold\n", upgradeCost);
        printf("1) Upgrade Weapon (Increase Attack)\n");
        printf("2) Upgrade Armor (Increase Defense)\n");
        printf("3) Cancel\n");
        printf("Enter your choice: ");
        scanf("%d", &upgradeChoice);

        if (upgradeChoice == 1) {
            player->attack += 2;
            player->gold -= upgradeCost;
            printf("Your attack increased to %d!\n", player->attack);
            break;
        } else if (upgradeChoice == 2) {
            player->defense += 2;
            player->gold -= upgradeCost;
            printf("Your defense increased to %d!\n", player->defense);
            break;
        } else if (upgradeChoice == 3) {
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}

// Encounter Guardian function
int encounterGuardian(Player *player, const char* guardianName, int guardianHp, int guardianAtk, int guardianDef) {
    printf("\nYou have encountered %s!\n", guardianName);
    printf("%s Stats -> HP: %d, Attack: %d, Defense: %d\n", guardianName, guardianHp, guardianAtk, guardianDef);

    // Simple combat simulation
    while (guardianHp > 0 && player->hp > 0) {
        guardianHp -= player->attack - guardianDef;
        if (guardianHp > 0) {
            player->hp -= guardianAtk - player->defense;
        }
    }

    if (player->hp <= 0) {
        printf("You were defeated by %s...\nGame Over!\n", guardianName);
        exit(0);
    } else {
        printf("You defeated %s!\n", guardianName);
        return SUCCESS;
    }
}

int whichBoss(Player *player) {
    // Storyline progression
    if (player->fragmentsCollected == 0) {
        // First encounter
        if (encounterGuardian(player, "Shadow Wraith", 15, 5, 5) == SUCCESS) {
            player->fragmentsCollected++;
            displayStory("With the Shadow Wraith defeated, you hold the first fragment of the Crystal. Four more remain. The path ahead leads to Mount Frostbite, where the Ice Dragon awaits.");
        }
        return SUCCESS;
    }
    if (player->fragmentsCollected == 1) {
        // Second encounter
        if (encounterGuardian(player, "Ice Dragon", 20, 7, 7) == SUCCESS) {
            player->fragmentsCollected++;
            displayStory("You have conquered the Ice Dragon and claimed the second fragment of the Crystal. Your journey now takes you to the Desert of Lost Souls, where the Sand Serpent lurks.");
        }
        return SUCCESS;
    }
    if (player->fragmentsCollected == 2) {
        // Third encounter
        if (encounterGuardian(player, "Sand Serpent", 18, 6, 6) == SUCCESS) {
            player->fragmentsCollected++;
            displayStory("The Sand Serpent has fallen, and the third fragment is yours. Next, venture into the Swamp of Despair to face the Bog Beast.");
        }
        return SUCCESS;
    }
    if (player->fragmentsCollected == 3) {
        // Fourth encounter
        if (encounterGuardian(player, "The Ruler of the Bog", 22, 8, 8) == SUCCESS) {
            player->fragmentsCollected++;
            displayStory("The Ruler of the Bog is no more, and the fourth fragment is now in your possession. The final fragment awaits in Zoltar's Fortress.");
        }
        return SUCCESS;
    }
    if (player->fragmentsCollected == 4) {
        // Final battle
        if (encounterGuardian(player, "Dark Sorcerer Drüig", 30, 10, 10) == SUCCESS) {
            player->fragmentsCollected++;
            displayStory("You have defeated Drüig and reunited the Crystal of Light. Arenthia is saved, and you have restored peace to the kingdom. Congratulations!");
            printf("You have completed the game!\n");
            exit(0);
        }
        return SUCCESS;
    }
}

// Pause menu function
int menuPause(Player *player) {
    while (1) {
        int choice;
        printf("\n---- Pause Menu ----\n");
        printf("1) Resume\n");
        printf("2) Save Game\n");
        printf("3) Quit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            return SUCCESS;
        } else if (choice == 2) {
            if (saveGame(player) == SUCCESS) {
                printf("Game saved successfully.\n");
            } else {
                printf("Failed to save the game.\n");
            }
            return SUCCESS;
        } else if (choice == 3) {
            system("clear");
            printf("Returning to main menu...\n");
            menuStart();
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return SUCCESS;
}

// Load game function
int loadGame(Player *player) {
    FILE *file = fopen("playerData/savefile.txt", "r");
    if (file == NULL) {
        printf("Couldn't find save file, starting new game instead!\n");
        saveGame(player);
        return FAILURE;
    }

    fscanf(file, "%s %d %d %d %d %d %d", player->name, &player->hp, &player->attack, &player->defense, &player->inventorySize, &player->fragmentsCollected, &player->gold);
    fclose(file);

    return SUCCESS;
}

// Save game function
int saveGame(Player *player) {
    FILE *file = fopen("playerData/savefile.txt", "w");
    if (file == NULL) {
        printf("ERROR: Couldn't save!\n");
        return FAILURE;
    }

    fprintf(file, "%s %d %d %d %d %d %d\n", player->name, player->hp, player->attack, player->defense, player->inventorySize, player->fragmentsCollected, player->gold);
    fclose(file);

    return SUCCESS;
}

// Create new player function
int createNewPlayer(Player *player) {
    printf("Enter your character's name: ");
    scanf("%s", player->name);
    player->hp = DEFLT_HP;
    player->attack = DEFLT_ATK;
    player->defense = DEFLT_DEF;
    player->inventorySize = DEF_INV_SIZE;
    player->fragmentsCollected = 0;
    player->gold = 0;

    return SUCCESS;
}

// Display story function
void displayStory(const char* story) {
    printf("\n%s\n", story);
    sleep(2); // Adding a short delay for effect
}

// Main function
int main(void) {
    system("clear");
    system("mkdir -p player");

    srand(time(NULL)); // Seed the random number generator

    menuStart();
    return SUCCESS;
}
