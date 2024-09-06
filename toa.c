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
#define STARTING_FRAGS 0
#define DEFLT_GOLD 15
#define DEFLT_LVL 0

typedef struct {
    char name[MAX_NAME_LEN];
    int hp;
    int attack;
    int defense;
    int inventorySize;
    int fragmentsCollected;
    int gold;
    int level;           
    int xp;               
    int xpToNextLevel;
} Player;

int gameLoop(Player *player);
int menuStart();
int menuPause(Player *player);
int loadGame(Player *player);
int saveGame(Player *player);
int whichBoss(Player *player);
int createNewPlayer(Player *player);
int combat(Player *player, const char *guardianName, int guardianHp, int guardianAtk, int guardianDef);
int gaurdianCombat(Player *player, int guardianHp, int guardianAtk, int guardianDef);
void displayStory(const char *story);
void exploreRegion(Player *player);
void upgradeGear(Player *player);
void randomEncounter(Player *player);
void levelUp(Player *player);
void checkStats(Player *player);

int isNewPlyr = 1;
int startMsg = 0;
int wAmount = 1;
int aAmount = 1;

void waitMsg(char *msg) {
    printf("%s", msg);
    while(getchar() != '\n');
    getchar();
}

int autosave(Player *player) {
    printf("Autosaving...\n");
    sleep(2);
    system("clear");
    if (saveGame(player) == SUCCESS) {
        system("clear");
        printf("Game saved successfully.\n");
        waitMsg("Press Enter to Continue!...");
        return SUCCESS;
    } else {
        system("clear");
        printf("Failed to save the game.\n");
        waitMsg("Press Enter to Continue!...");
        return FAILURE;
    }
}

int menuStart() {
    system("clear");
    printf("\n#       Welcome to lazyrpg!       #\n\n");

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
                    remove("playerData/isNew.txt ");
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
        printf("Welcome back!\n");

        while (1) {
            printf("1) Continue\n");
            printf("2) Start New Game\n");
            printf("3) Quit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            Player player;
            if (choice == 1) {
                if (loadGame( &player) == SUCCESS) {
                    gameLoop( &player);
                } else {
                    printf("Failed to load the game. Starting new game.\n");
                    if (createNewPlayer( &player) == SUCCESS) {
                        gameLoop( &player);
                    } else {
                        printf("Failed to create new player.\n");
                    }
                }
                break;
            } else if (choice == 2) {
                system("clear");
                remove("playerData/savefile.txt");
                FILE *sf = fopen("playerData/savefile.txt", "w");
                fclose(sf);

                FILE *newFile = fopen("playerData/isNew.txt", "w");
                if (newFile == NULL) {
                    return FAILURE;
                }
                fclose(newFile);

                menuStart();
                break;
            } else if (choice == 3) {
                system("clear");
                sleep(2);
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
    int choice;
    system("clear");

    if (startMsg == 0) {
        printf("Starting game as %s...\n", player->name);
        sleep(2);
        system("clear");
        printf("\n#       Story       #\n");
        displayStory("As the last heir of Arenthia, you embark on a quest to restore your once glorious kingdom, shattered by the earth shattering dark magic\nof Drüig, the almighty dark sorcerer. The kingdom of Arenthia used to be a fluorishing kingdom full of life and amazing people. Now its\nyour job to go and save your people");
        waitMsg("Press Enter to continue!...");
        system("clear");
        startMsg = 1;
    }

    autosave(player);
    while (1) {
        system("clear");
        printf("\n#       Main Menu       #\n\n");
        printf("1) Explore Region\n");
        printf("2) Upgrade Gear\n");
        printf("3) Check Stats\n");
        printf("4) Save Game\n");
        printf("5) Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            exploreRegion(player);
        } else if (choice == 2) {
            upgradeGear(player);
        } else if (choice == 3) {
            checkStats(player);
        } else if (choice == 4) {
            if (saveGame(player) == SUCCESS) {
                system("clear");
                printf("Game saved successfully.\n");
                waitMsg("Press Enter to Continue!...\n");
            } else {
                system("clear");
                printf("Failed to save the game.\n");
                waitMsg("Press Enter to Continue!...\n");
            }
        } else if (choice == 5) {
            system("clear");
            exit(0);
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    
    return SUCCESS;
}

void checkStats(Player *player) {
    system("clear");
    printf("\n#       Player Stats       #\n\n");
    printf("Name: %s\n", player->name);
    printf("Level: %d\n", player->level);
    printf("XP: %d / %d\n", player->xp, player->xpToNextLevel);
    printf("HP: %d\n", player->hp);
    printf("Attack: %d\n", player->attack);
    printf("Defense: %d\n", player->defense);
    printf("Gold: %d\n", player->gold);
    printf("Fragments Collected: %d\n", player->fragmentsCollected);
    waitMsg("Press Enter to Continue!...\n");

}

void exploreRegion(Player *player) {
    int regionChoice;
    int monsterHp = rand() % 10 + 5;
    int monsterAtk = rand() % 5 + 3;
    int monsterDef = rand() % 3 + 2;

    system("clear");
    printf("Loading map...\n");
    sleep(2);
    system("clear");

    while (1) {
        printf("\n#       Select a Region to Explore       #\n\n");
        printf("1) Forest of Whispering Shadows (Easy)\n");
        printf("2) Mount Frostbite (Medium)\n");
        printf("3) Desert of Lost Souls (Hard)\n");
        printf("4) Swamp of Despair (Very Hard)\n");
        printf("5) Go Back\n");
        printf("Enter your choice: ");
        scanf("%d", &regionChoice);
        system("clear");
        if (regionChoice != 5 && regionChoice >= 1 && regionChoice <= 4) {
            int encounterChoice;
            while (1) {
                system("clear");
                printf("\n#       Choose Your Encounter       #\n\n");
                printf("1) Fight the gaurdian that rules this land\n");
                printf("2) Fight a Regular Enemy\n");
                printf("3) Go Back\n");
                printf("Enter your choice: ");
                scanf("%d", &encounterChoice);
                system("clear");
                if (encounterChoice == 1) {
                    printf("\n#       Choose Your boss       #\n\n");
                    printf("Current ammount of fragments collected: %d\n", player ->fragmentsCollected);
                    printf("1) Shadow Wraith\n");
                    printf("2) Ice Dragon (requires 1 fragment collected)\n");
                    printf("3) Sand Serpent (requires 2 fragments collected)\n");
                    printf("4) The Ruler Of The Bog (requires 3 fragments collected)\n");
                    printf("5) Dark Sorcerer Drüig (requires 4 fragments collected)\n");
                    scanf("%d", &encounterChoice);
                    system("clear");
                    if (encounterChoice == 1) {
                        if (player->fragmentsCollected <= 0) {
                            if (gaurdianCombat(player, 15, 5, 5) == SUCCESS) {
                                player->fragmentsCollected++;
                                displayStory("With the Shadow Wraith defeated, you hold the first fragment of the Crystal. Four more remain. The path ahead leads to Mount Frostbite, where the Ice Dragon awaits.");
                            }
                        } else {
                            break;
                        }
                    } else if (encounterChoice == 2) {
                        if (player->fragmentsCollected == 1) {
                            if (gaurdianCombat(player, 20, 7, 7) == SUCCESS) {
                                player->fragmentsCollected++;
                                displayStory("You have conquered the Ice Dragon and claimed the second fragment of the Crystal. Your journey now takes you to the Desert of Lost Souls, where the Sand Serpent lurks.");
                            } else {
                                break;
                            }
                        }
                    } else if (encounterChoice == 3) {
                        if (player->fragmentsCollected == 2) {
                            if (gaurdianCombat(player, 18, 6, 6) == SUCCESS) {
                                player->fragmentsCollected++;
                                displayStory("The Sand Serpent has fallen, and the third fragment is yours. Next, venture into the Swamp of Despair to face the Bog Beast.");
                            } else {
                                break;
                            }
                        }
                    } else if (encounterChoice == 4) {
                        if (player->fragmentsCollected == 3) {
                            if (gaurdianCombat(player, 22, 8, 8) == SUCCESS) {
                                player->fragmentsCollected++;
                                displayStory("The Ruler of the Bog is no more, and the fourth fragment is now in your possession. The final fragment awaits in Zoltar's Fortress.");
                            } else {
                                break;
                            }
                        } else if (encounterChoice == 5) {
                            if (player->fragmentsCollected == 4) {
                                if (gaurdianCombat(player, 30, 10, 10) == SUCCESS) {
                                    player->fragmentsCollected++;
                                    displayStory("You have defeated Drüig and reunited the Crystal of Light. Arenthia is saved, and you have restored peace to the kingdom. Congratulations!");
                                    printf("You have completed the game!\n");
                                    exit(0);
                                } else {
                                    break;
                                }
                            }
                        } else if (encounterChoice == 2) {
                            combat(player, "Wild Monster", monsterHp, monsterAtk, monsterDef);
                            break;
                        } else {
                            printf("Invalid choice. Please try again.\n");
                            waitMsg("Press Enter to Continue!...\n");
                            system("clear");
                        }
                    }
                    break;
                } else if (encounterChoice == 2) {
                    combat(player, "Wild Monster", monsterHp, monsterAtk, monsterDef);
                } else if (encounterChoice == 3) {
                    gameLoop(player);
                } else {
                    printf("Invalid try again!");
                    waitMsg("Press Enter to Continue!...\n");
                    system("clear");
                }
            }
        } else if (regionChoice == 5) {
            gameLoop(player);
        } else {
            printf("Invalid try again!");
            waitMsg("Press Enter to Continue!...\n");
            system("clear");
        }
    }
}

void upgradeGear(Player *player) {
    int upgradeChoice;
    int weaponUpgradeCost = 20 * wAmount;
    int armorUpgradeCost = 20 * aAmount;
    
    system("clear");
    printf("Checking funds...\n");
    sleep(2);
    system("clear");
    printf("Loading shop...\n");
    sleep(2);
    system("clear");

    while (1) {
        system("clear");
        printf("\n#       Upgrade Gear       #\n\n");
        printf("You have %d gold.\n\n", player->gold);
        printf("Upgrade cost:\nWeapon: %d gold\nArmor: %d gold\n", weaponUpgradeCost, armorUpgradeCost);
        printf("1) Upgrade Weapon (Increase Attack)\n");
        printf("2) Upgrade Armor (Increase Defense)\n");
        printf("3) Cancel\n");
        printf("Enter your choice: ");
        scanf("%d", &upgradeChoice);
        system("clear");

        if (upgradeChoice == 1 && player->gold >= weaponUpgradeCost) {
            player->attack += 2;
            player->gold -= weaponUpgradeCost;
            ++wAmount;
            printf("Your attack increased to %d!\n", player->attack);
            waitMsg("Press Enter to Continue!...\n");
        } else if (upgradeChoice == 1 && player->gold < weaponUpgradeCost) {
            system("clear");
            printf("You don't have enough gold to upgrade your weapon. Earn more by exploring!\n");
            waitMsg("Press Enter to Continue!...\n");
        } else if (upgradeChoice == 2 && player->gold >= armorUpgradeCost) {
            player->defense += 2;
            player->gold -= armorUpgradeCost;
            ++aAmount;
            printf("Your defense increased to %d!\n", player->defense);
            waitMsg("Press Enter to Continue!...\n");
        } else if (upgradeChoice == 2 && player->gold < armorUpgradeCost) {
            system("clear");
            printf("You don't have enough gold to upgrade your armor. Earn more by exploring!\n");
            waitMsg("Press Enter to Continue!...\n");
        } else if (upgradeChoice == 3) {
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
            waitMsg("Press Enter to Continue!...\n");
        }

    }
}

void randomEncounter(Player *player) {
    int goldReward = rand() % 10 + 5;
    
    player->gold += goldReward;
    printf("You found %d gold!\n", goldReward);
    waitMsg("Press Enter to Continue!...\n");
}

int combat(Player *player, const char *enemyName, int enemyHp, int enemyAtk, int enemyDef) {
    printf("\nYou have encountered %s!\n", enemyName);
    printf("%s's Stats -> HP: %d, Attack: %d, Defense: %d\n", enemyName, enemyHp, enemyAtk, enemyDef);

    while (enemyHp > 0 && player->hp > 0) {
        int damage = player->attack - enemyDef;
        if (damage < 0) damage = 0;
        enemyHp -= damage;
        printf("You dealt %d damage to %s. %s HP: %d\n", damage, enemyName, enemyName, enemyHp);

        if (enemyHp > 0) {
            damage = enemyAtk - player->defense;
            if (damage < 0) damage = 0;
            player->hp -= damage;
            printf("%s attacks and deals %d damage to you. Your HP: %d\n", enemyName, damage, player->hp);
        }
    }

    if (player->hp <= 0) {
        printf("You were defeated by %s...\n", enemyName);
        waitMsg("Press Enter to Continue!...\n");
        return FAILURE;
    } else {
        printf("You defeated %s!\n", enemyName);
        int xpGain = rand() % 50 + 50;
        player->xp += xpGain;
        printf("You gained %d XP!\n", xpGain);
        int goldReward = rand() % 10 + 5;
        player->gold += goldReward;
        printf("You found %d gold!\n", goldReward);
        waitMsg("Press Enter to Continue!...\n");

        levelUp(player);
        return SUCCESS;
    }
}

int gaurdianCombat(Player *player, int guardianHp, int guardianAtk, int guardianDef) {
    int choice;
    int damage;

    while (guardianHp > 0 && player->hp > 0) {
        printf("\n#       Boss Fight Menu      #\n\n");
        printf("1) Attack\n");
        printf("2) Defend\n");
        printf("3) Use Item (not implemented)\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            damage = player->attack - guardianDef;
            if (damage < 0) damage = 0;
            guardianHp -= damage;
            printf("You dealt %d damage to the guardian. Guardian HP: %d\n", damage, guardianHp);

            damage = guardianAtk - player->defense;
            if (damage < 0) damage = 0;
            player->hp -= damage;
            printf("The guardian retaliates and deals %d damage to you. Your HP: %d\n", damage, player->hp);
            waitMsg("Press Enter to Continue!...\n");
        } else if (choice == 2) {
            damage = (guardianAtk - player->defense) / 2;
            if (damage < 0) damage = 0;
            player->hp -= damage;
            printf("You defend against the guardian's attack, reducing damage to %d. Your HP: %d\n", damage, player->hp);
            waitMsg("Press Enter to Continue!...\n");
        } else if (choice == 3) {
            printf("This feature is not yet implemented.\n");
            waitMsg("Press Enter to Continue!...\n");
        } else {
            printf("Invalid choice. Please try again.\n");
            waitMsg("Press Enter to Continue!...\n");
        }

        if (guardianHp <= 0) {
            printf("You defeated the guardian!\n");
            int xpGain = 100;
            player->xp += xpGain;
            printf("You gained %d XP!\n", xpGain);

            levelUp(player);
            waitMsg("Press Enter to Continue!...\n");
            return SUCCESS;
        } else if (player->hp <= 0) {
            printf("You were defeated by the guardian...\n");
            waitMsg("Press Enter to Continue!...\n");
            return FAILURE;
        }
    }

    return SUCCESS;
}

int menuPause(Player *player) {
    while (1) {
        int choice;
        printf("\n#       Pause Menu       #\n\n");
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
                waitMsg("Press Enter to Continue!...\n");
            } else {
                printf("Failed to save the game.\n");
                waitMsg("Press Enter to Continue!...\n");
            }
            return SUCCESS;
        } else if (choice == 3) {
            system("clear");
            printf("Returning to main menu...\n");
            menuStart();
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
            waitMsg("Press Enter to Continue!...\n");
        }
    }

    return SUCCESS;
}

int loadGame(Player *player) {
    FILE *file = fopen("playerData/savefile.txt", "r");

    int result = fscanf(file, "%s %d %d %d %d %d %d %d %d %d", 
                        player->name, &player->hp, &player->attack, 
                        &player->defense, &player->inventorySize, 
                        &player->fragmentsCollected, &player->gold, 
                        &player->level, &player->xp, &player->xpToNextLevel);

    fclose(file);
    return SUCCESS;
}

int saveGame(Player *player) {
    FILE *file = fopen("playerData/savefile.txt", "w");
    if (file == NULL) {
        printf("ERROR: Couldn't save!\n");
        waitMsg("Press Enter to Continue!...\n");
        return FAILURE;
    }
    remove("playerData/isNew.txt");

    fprintf(file, "%s %d %d %d %d %d %d %d %d %d\n", 
            player->name, player->hp, player->attack, player->defense, 
            player->inventorySize, player->fragmentsCollected, player->gold, 
            player->level, player->xp, player->xpToNextLevel);

    fclose(file);
    return SUCCESS;
}

void levelUp(Player *player) {
    system("clear");
    if (player->xp >= player->xpToNextLevel) {
        player->level++;
        player->xp -= player->xpToNextLevel;
        player->xpToNextLevel += 100 + (10 + (rand() % player->level));

        player->hp += 5;
        player->attack += 2;
        player->defense += 2;

        printf("\nCongratulations! You've leveled up to level %d!\n", player->level);
        printf("Your HP increased to %d, Attack to %d, and Defense to %d.\n", player->hp, player->attack, player->defense);
        waitMsg("Press Enter to continue!...");
    }
}

int createNewPlayer(Player *player) {
    system("clear");
    printf("\n#       Character Creation       #\n\n");
    printf("Enter your character's name: ");
    scanf("%s", player->name);
    player->hp = DEFLT_HP;
    player->attack = DEFLT_ATK;
    player->defense = DEFLT_DEF;
    player->inventorySize = DEF_INV_SIZE;
    player->fragmentsCollected = STARTING_FRAGS;
    player->gold = DEFLT_GOLD;
    player->level = DEFLT_LVL;
    player->xp = 0;
    player->xpToNextLevel = 100;
    system("clear");
    printf("Creating new character...\n");
    sleep(2);
    system("clear");
    return SUCCESS;
}

void displayStory(const char *story) {
    printf("\n%s\n", story);
    sleep(2);
}

int main(void) {
    system("clear");
    system("mkdir -p playerData");

    srand(time(NULL));
    Player player;

    if (loadGame(&player) == FAILURE) {
        if (createNewPlayer(&player) != SUCCESS) {
            printf("Failed to create new player.\n");
            return FAILURE;
        }
    }
    
    menuStart();
    return SUCCESS;
}
