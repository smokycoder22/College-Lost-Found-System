/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include "lost_found.h"

// Global variables
int item_counter = 1000; // Starting ID for items

// Initialize hash table
void initializeHashTable(HashTable* ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->table[i] = NULL;
    }
}

// Hash function based on category
int hashFunction(Category category) {
    return category % HASH_SIZE;
}

// Add item to hash table
void addItem(HashTable* ht, Item* item) {
    int index = hashFunction(item->category);
    
    if (ht->table[index] == NULL) {
        ht->table[index] = (HashNode*)malloc(sizeof(HashNode));
        ht->table[index]->items = item;
        ht->table[index]->next = NULL;
    } else {
        // Add to the beginning of the linked list
        item->next = ht->table[index]->items;
        ht->table[index]->items = item;
    }
}

// Display main menu
void displayMenu() {
    printf("\n========== COLLEGE LOST & FOUND SYSTEM ==========\n");
    printf("1. Report Lost Item\n");
    printf("2. Report Found Item\n");
    printf("3. View All Lost Items\n");
    printf("4. View All Found Items\n");
    printf("5. Search by Category\n");
    printf("6. Search by Keyword\n");
    printf("7. Match Lost and Found Items\n");
    printf("8. Save Data to File\n");
    printf("9. Load Data from File\n");
    printf("0. Exit\n");
    printf("================================================\n");
    printf("Enter your choice: ");
}

// Get current date as string
void getCurrentDate(char* dateStr) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateStr, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

// Convert category enum to string
const char* getCategoryString(Category cat) {
    switch (cat) {
        case ELECTRONICS: return "Electronics";
        case BOOKS: return "Books";
        case CLOTHING: return "Clothing";
        case ACCESSORIES: return "Accessories";
        case STATIONERY: return "Stationery";
        case SPORTS: return "Sports";
        case KEYS: return "Keys";
        case BAGS: return "Bags";
        case JEWELRY: return "Jewelry";
        case OTHERS: return "Others";
        default: return "Others";
    }
}

// Convert string to category enum
Category getCategoryFromString(const char* str) {
    if (strcmp(str, "Electronics") == 0) return ELECTRONICS;
    if (strcmp(str, "Books") == 0) return BOOKS;
    if (strcmp(str, "Clothing") == 0) return CLOTHING;
    if (strcmp(str, "Accessories") == 0) return ACCESSORIES;
    if (strcmp(str, "Stationery") == 0) return STATIONERY;
    if (strcmp(str, "Sports") == 0) return SPORTS;
    if (strcmp(str, "Keys") == 0) return KEYS;
    if (strcmp(str, "Bags") == 0) return BAGS;
    if (strcmp(str, "Jewelry") == 0) return JEWELRY;
    return OTHERS;
}

// Display category menu and get selection
Category selectCategory() {
    int choice;
    printf("\nSelect Category:\n");
    printf("0. Electronics\n1. Books\n2. Clothing\n3. Accessories\n4. Stationery\n");
    printf("5. Sports\n6. Keys\n7. Bags\n8. Jewelry\n9. Others\n");
    printf("Enter choice (0-9): ");
    scanf("%d", &choice);
    
    if (choice >= 0 && choice <= 9) {
        return (Category)choice;
    }
    return OTHERS;
}

// Add lost item
void addLostItem(HashTable* ht) {
    Item* newItem = (Item*)malloc(sizeof(Item));
    
    printf("\n--- REPORT LOST ITEM ---\n");
    newItem->id = item_counter++;
    newItem->is_lost = 1;
    
    printf("Item Name: ");
    getchar(); // Clear buffer
    fgets(newItem->name, MAX_NAME_LEN, stdin);
    newItem->name[strcspn(newItem->name, "\n")] = 0; // Remove newline
    
    printf("Description: ");
    fgets(newItem->description, MAX_DESC_LEN, stdin);
    newItem->description[strcspn(newItem->description, "\n")] = 0;
    
    newItem->category = selectCategory();
    
    printf("Last seen location: ");
    fgets(newItem->location, MAX_LOCATION_LEN, stdin);
    newItem->location[strcspn(newItem->location, "\n")] = 0;
    
    printf("Your contact info: ");
    fgets(newItem->contact, MAX_CONTACT_LEN, stdin);
    newItem->contact[strcspn(newItem->contact, "\n")] = 0;
    
    getCurrentDate(newItem->date);
    newItem->next = NULL;
    
    addItem(ht, newItem);
    printf("Lost item reported successfully! ID: %d\n", newItem->id);
}

// Add found item
void addFoundItem(HashTable* ht) {
    Item* newItem = (Item*)malloc(sizeof(Item));
    
    printf("\n--- REPORT FOUND ITEM ---\n");
    newItem->id = item_counter++;
    newItem->is_lost = 0;
    
    printf("Item Name: ");
    getchar(); // Clear buffer
    fgets(newItem->name, MAX_NAME_LEN, stdin);
    newItem->name[strcspn(newItem->name, "\n")] = 0;
    
    printf("Description: ");
    fgets(newItem->description, MAX_DESC_LEN, stdin);
    newItem->description[strcspn(newItem->description, "\n")] = 0;
    
    newItem->category = selectCategory();
    
    printf("Found location: ");
    fgets(newItem->location, MAX_LOCATION_LEN, stdin);
    newItem->location[strcspn(newItem->location, "\n")] = 0;
    
    printf("Your contact info: ");
    fgets(newItem->contact, MAX_CONTACT_LEN, stdin);
    newItem->contact[strcspn(newItem->contact, "\n")] = 0;
    
    getCurrentDate(newItem->date);
    newItem->next = NULL;
    
    addItem(ht, newItem);
    printf("Found item reported successfully! ID: %d\n", newItem->id);
}

// Display items of specific type (lost or found)
void displayItems(HashTable* ht, int is_lost) {
    printf("\n--- %s ITEMS ---\n", is_lost ? "LOST" : "FOUND");
    printf("%-5s %-20s %-30s %-12s %-20s %-10s\n", 
           "ID", "Name", "Description", "Category", "Location", "Date");
    printf("================================================================================\n");
    
    int count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht->table[i] != NULL) {
            Item* current = ht->table[i]->items;
            while (current != NULL) {
                if (current->is_lost == is_lost) {
                    printf("%-5d %-20s %-30s %-12s %-20s %-10s\n",
                           current->id, current->name, current->description,
                           getCategoryString(current->category), current->location, current->date);
                    count++;
                }
                current = current->next;
            }
        }
    }
    
    if (count == 0) {
        printf("No %s items found.\n", is_lost ? "lost" : "found");
    } else {
        printf("\nTotal %s items: %d\n", is_lost ? "lost" : "found", count);
    }
}

// Search items by category
void searchByCategory(HashTable* ht, Category category) {
    int index = hashFunction(category);
    printf("\n--- ITEMS IN CATEGORY: %s ---\n", getCategoryString(category));
    printf("%-5s %-15s %-20s %-30s %-20s %-10s\n", 
           "ID", "Type", "Name", "Description", "Location", "Date");
    printf("===================================================================================\n");
    
    int count = 0;
    if (ht->table[index] != NULL) {
        Item* current = ht->table[index]->items;
        while (current != NULL) {
            if (current->category == category) {
                printf("%-5d %-15s %-20s %-30s %-20s %-10s\n",
                       current->id, current->is_lost ? "LOST" : "FOUND",
                       current->name, current->description, current->location, current->date);
                count++;
            }
            current = current->next;
        }
    }
    
    if (count == 0) {
        printf("No items found in this category.\n");
    } else {
        printf("\nTotal items found: %d\n", count);
    }
}

// Search items by keyword
void searchByKeyword(HashTable* ht, const char* keyword) {
    printf("\n--- SEARCH RESULTS FOR: '%s' ---\n", keyword);
    printf("%-5s %-15s %-20s %-30s %-20s %-10s\n", 
           "ID", "Type", "Name", "Description", "Location", "Date");
    printf("===================================================================================\n");
    
    int count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht->table[i] != NULL) {
            Item* current = ht->table[i]->items;
            while (current != NULL) {
                // Search in name and description (case insensitive)
                char name_lower[MAX_NAME_LEN], desc_lower[MAX_DESC_LEN], key_lower[MAX_NAME_LEN];
                strcpy(name_lower, current->name);
                strcpy(desc_lower, current->description);
                strcpy(key_lower, keyword);
                
                // Convert to lowercase for comparison
                for (int j = 0; name_lower[j]; j++) name_lower[j] = tolower(name_lower[j]);
                for (int j = 0; desc_lower[j]; j++) desc_lower[j] = tolower(desc_lower[j]);
                for (int j = 0; key_lower[j]; j++) key_lower[j] = tolower(key_lower[j]);
                
                if (strstr(name_lower, key_lower) || strstr(desc_lower, key_lower)) {
                    printf("%-5d %-15s %-20s %-30s %-20s %-10s\n",
                           current->id, current->is_lost ? "LOST" : "FOUND",
                           current->name, current->description, current->location, current->date);
                    count++;
                }
                current = current->next;
            }
        }
    }
    
    if (count == 0) {
        printf("No items found matching '%s'.\n", keyword);
    } else {
        printf("\nTotal matches found: %d\n", count);
    }
}

// Match lost and found items
void matchItems(HashTable* ht) {
    printf("\n--- POTENTIAL MATCHES ---\n");
    printf("Checking for potential matches between lost and found items...\n\n");
    
    int matches = 0;
    
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht->table[i] != NULL) {
            Item* lost = ht->table[i]->items;
            while (lost != NULL) {
                if (lost->is_lost == 1) {
                    // Look for found items in the same category
                    Item* found = ht->table[i]->items;
                    while (found != NULL) {
                        if (found->is_lost == 0 && found->category == lost->category) {
                            // Check for keyword matches
                            char lost_name[MAX_NAME_LEN], found_name[MAX_NAME_LEN];
                            strcpy(lost_name, lost->name);
                            strcpy(found_name, found->name);
                            
                            // Simple matching based on similar names
                            for (int j = 0; lost_name[j]; j++) lost_name[j] = tolower(lost_name[j]);
                            for (int j = 0; found_name[j]; j++) found_name[j] = tolower(found_name[j]);
                            
                            if (strstr(lost_name, found_name) || strstr(found_name, lost_name) ||
                                strcmp(lost_name, found_name) == 0) {
                                printf("POTENTIAL MATCH FOUND!\n");
                                printf("Lost Item  - ID: %d, Name: %s, Contact: %s\n", 
                                       lost->id, lost->name, lost->contact);
                                printf("Found Item - ID: %d, Name: %s, Contact: %s\n", 
                                       found->id, found->name, found->contact);
                                printf("Category: %s\n", getCategoryString(lost->category));
                                printf("-------------------------------------------\n");
                                matches++;
                            }
                        }
                        found = found->next;
                    }
                }
                lost = lost->next;
            }
        }
    }
    
    if (matches == 0) {
        printf("No potential matches found at this time.\n");
    } else {
        printf("Total potential matches: %d\n", matches);
    }
}

// Save data to file
void saveToFile(HashTable* ht) {
    FILE *file = fopen("lost_found_data.txt", "w");
    if (!file) {
        printf("Error: Unable to create file!\n");
        return;
    }
    
    fprintf(file, "ID,Name,Description,Category,Location,Contact,Date,Type\n");
    
    int count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht->table[i] != NULL) {
            Item* current = ht->table[i]->items;
            while (current != NULL) {
                fprintf(file, "%d,%s,%s,%s,%s,%s,%s,%s\n",
                        current->id, current->name, current->description,
                        getCategoryString(current->category), current->location,
                        current->contact, current->date,
                        current->is_lost ? "LOST" : "FOUND");
                count++;
                current = current->next;
            }
        }
    }
    
    fclose(file);
    printf("Data saved successfully! %d items saved to 'lost_found_data.txt'\n", count);
}

// Load data from file (basic implementation)
void loadFromFile(HashTable* ht) {
    FILE *file = fopen("lost_found_data.txt", "r");
    if (!file) {
        printf("No existing data file found.\n");
        return;
    }
    
    char line[500];
    int count = 0;
    
    // Skip header line
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file)) {
        Item* newItem = (Item*)malloc(sizeof(Item));
        char category_str[50], type_str[10];
        
        sscanf(line, "%d,%99[^,],%199[^,],%49[^,],%99[^,],%49[^,],%19[^,],%9[^\n]",
               &newItem->id, newItem->name, newItem->description,
               category_str, newItem->location, newItem->contact,
               newItem->date, type_str);
        
        newItem->category = getCategoryFromString(category_str);
        newItem->is_lost = (strcmp(type_str, "LOST") == 0) ? 1 : 0;
        newItem->next = NULL;
        
        if (newItem->id >= item_counter) {
            item_counter = newItem->id + 1;
        }
        
        addItem(ht, newItem);
        count++;
    }
    
    fclose(file);
    printf("Data loaded successfully! %d items loaded from file.\n", count);
}

// Free hash table memory
void freeHashTable(HashTable* ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht->table[i] != NULL) {
            Item* current = ht->table[i]->items;
            while (current != NULL) {
                Item* temp = current;
                current = current->next;
                free(temp);
            }
            free(ht->table[i]);
        }
    }
}

// Main function
int main() {
    HashTable ht;
    initializeHashTable(&ht);
    
    int choice;
    char keyword[100];
    Category cat;
    
    printf("Welcome to College Lost & Found Management System!\n");
    printf("Developed using Data Structures and Algorithms in C\n");
    
    // Load existing data
    loadFromFile(&ht);
    
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addLostItem(&ht);
                break;
            case 2:
                addFoundItem(&ht);
                break;
            case 3:
                displayItems(&ht, 1); // Display lost items
                break;
            case 4:
                displayItems(&ht, 0); // Display found items
                break;
            case 5:
                cat = selectCategory();
                searchByCategory(&ht, cat);
                break;
            case 6:
                printf("Enter keyword to search: ");
                getchar(); // Clear buffer
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = 0; // Remove newline
                searchByKeyword(&ht, keyword);
                break;
            case 7:
                matchItems(&ht);
                break;
            case 8:
                saveToFile(&ht);
                break;
            case 9:
                loadFromFile(&ht);
                break;
            case 0:
                printf("Thank you for using Lost & Found System!\n");
                saveToFile(&ht); // Auto-save before exit
                freeHashTable(&ht);
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}

// Include ctype.h for tolower function
#include <ctype.h>