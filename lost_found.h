#ifndef LOST_FOUND_H
#define LOST_FOUND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_NAME_LEN 100
#define MAX_DESC_LEN 200
#define MAX_CONTACT_LEN 50
#define MAX_LOCATION_LEN 100
#define HASH_SIZE 10

// Item categories for hash table
typedef enum {
    ELECTRONICS = 0,
    BOOKS = 1,
    CLOTHING = 2,
    ACCESSORIES = 3,
    STATIONERY = 4,
    SPORTS = 5,
    KEYS = 6,
    BAGS = 7,
    JEWELRY = 8,
    OTHERS = 9
} Category;

// Structure for an item (Lost or Found)
typedef struct Item {
    int id;
    char name[MAX_NAME_LEN];
    char description[MAX_DESC_LEN];
    Category category;
    char location[MAX_LOCATION_LEN];
    char contact[MAX_CONTACT_LEN];
    char date[20];
    int is_lost; // 1 for lost, 0 for found
    struct Item* next;
} Item;

// Hash table node
typedef struct HashNode {
    Item* items;
    struct HashNode* next;
} HashNode;

// Hash table structure
typedef struct {
    HashNode* table[HASH_SIZE];
} HashTable;

// Function declarations
void initializeHashTable(HashTable* ht);
int hashFunction(Category category);
void addItem(HashTable* ht, Item* item);
void displayMenu();
void addLostItem(HashTable* ht);
void addFoundItem(HashTable* ht);
void displayItems(HashTable* ht, int is_lost);
void searchByCategory(HashTable* ht, Category category);
void searchByKeyword(HashTable* ht, const char* keyword);
void matchItems(HashTable* ht);
void saveToFile(HashTable* ht);
void loadFromFile(HashTable* ht);
void sortItemsByDate(Item** head);
Item* mergeSortedLists(Item* a, Item* b);
void splitList(Item* source, Item** front, Item** back);
const char* getCategoryString(Category cat);
Category getCategoryFromString(const char* str);
void freeHashTable(HashTable* ht);

#endif