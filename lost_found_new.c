// COLLEGE LOST & FOUND SYSTEM - SIMPLIFIED VERSION
// Using: Linked List, Stack, Queue concepts

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 100

// ==================== STRUCTURES ====================

// Item Node (Linked List Node)
typedef struct Item {
    int id;
    char name[MAX_LEN];
    char description[MAX_LEN];
    char category[MAX_LEN];
    char location[MAX_LEN];
    char contact[MAX_LEN];
    char date[20];
    int is_lost; // 1 = lost, 0 = found
    struct Item* next;
} Item;

// Stack Node for Recent Items (shows last 5 items)
typedef struct StackNode {
    Item* item;
    struct StackNode* next;
} StackNode;

// Stack structure
typedef struct {
    StackNode* top;
    int count;
} Stack;

// Queue Node for Processing Items
typedef struct QueueNode {
    Item* item;
    struct QueueNode* next;
} QueueNode;

// Queue structure
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// ==================== GLOBAL VARIABLES ====================

Item* lost_list = NULL;     // Linked list of lost items
Item* found_list = NULL;    // Linked list of found items
Stack recent_stack;         // Stack to track recent 5 items
Queue match_queue;          // Queue for processing matches
int item_counter = 1000;

// ==================== STACK OPERATIONS ====================

void initStack(Stack* s) {
    s->top = NULL;
    s->count = 0;
}

void push(Stack* s, Item* item) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->item = item;
    newNode->next = s->top;
    s->top = newNode;
    s->count++;
    
    // Keep only last 5 items
    if (s->count > 5) {
        StackNode* temp = s->top;
        for (int i = 0; i < 4; i++) {
            temp = temp->next;
        }
        StackNode* toDelete = temp->next;
        temp->next = NULL;
        free(toDelete);
        s->count = 5;
    }
}

void displayStack(Stack* s) {
    printf("\n=== RECENTLY ADDED ITEMS (Last 5) ===\n");
    StackNode* temp = s->top;
    int count = 1;
    
    while (temp != NULL) {
        printf("%d. %s (%s) - %s\n", count++, temp->item->name, 
               temp->item->is_lost ? "LOST" : "FOUND", temp->item->date);
        temp = temp->next;
    }
    if (s->count == 0) printf("No recent items.\n");
}

// ==================== QUEUE OPERATIONS ====================

void initQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

void enqueue(Queue* q, Item* item) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->item = item;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

Item* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    
    QueueNode* temp = q->front;
    Item* item = temp->item;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    return item;
}

int isQueueEmpty(Queue* q) {
    return (q->front == NULL);
}

// ==================== LINKED LIST OPERATIONS ====================

void insertAtEnd(Item** head, Item* newItem) {
    if (*head == NULL) {
        *head = newItem;
    } else {
        Item* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newItem;
    }
}

void displayList(Item* head, const char* title) {
    printf("\n========== %s ==========\n", title);
    printf("%-5s %-20s %-15s %-20s %-10s\n", "ID", "Name", "Category", "Location", "Date");
    printf("==================================================================\n");
    
    Item* temp = head;
    int count = 0;
    
    while (temp != NULL) {
        printf("%-5d %-20s %-15s %-20s %-10s\n", 
               temp->id, temp->name, temp->category, temp->location, temp->date);
        temp = temp->next;
        count++;
    }
    
    if (count == 0) {
        printf("No items found.\n");
    } else {
        printf("\nTotal items: %d\n", count);
    }
}

Item* searchByName(Item* head, const char* name) {
    Item* temp = head;
    while (temp != NULL) {
        if (strstr(temp->name, name) != NULL) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// ==================== UTILITY FUNCTIONS ====================

void getCurrentDate(char* dateStr) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateStr, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

// ==================== MAIN FUNCTIONS ====================

void addLostItem() {
    Item* newItem = (Item*)malloc(sizeof(Item));
    
    printf("\n--- REPORT LOST ITEM ---\n");
    newItem->id = item_counter++;
    newItem->is_lost = 1;
    newItem->next = NULL;
    
    printf("Item Name: ");
    getchar();
    fgets(newItem->name, MAX_LEN, stdin);
    newItem->name[strcspn(newItem->name, "\n")] = 0;
    
    printf("Description: ");
    fgets(newItem->description, MAX_LEN, stdin);
    newItem->description[strcspn(newItem->description, "\n")] = 0;
    
    printf("Category (e.g., Electronics, Books, Keys): ");
    fgets(newItem->category, MAX_LEN, stdin);
    newItem->category[strcspn(newItem->category, "\n")] = 0;
    
    printf("Last seen location: ");
    fgets(newItem->location, MAX_LEN, stdin);
    newItem->location[strcspn(newItem->location, "\n")] = 0;
    
    printf("Your contact: ");
    fgets(newItem->contact, MAX_LEN, stdin);
    newItem->contact[strcspn(newItem->contact, "\n")] = 0;
    
    getCurrentDate(newItem->date);
    
    // Add to linked list
    insertAtEnd(&lost_list, newItem);
    
    // Push to stack (recent items)
    push(&recent_stack, newItem);
    
    printf("\n✓ Lost item reported! ID: %d\n", newItem->id);
}

void addFoundItem() {
    Item* newItem = (Item*)malloc(sizeof(Item));
    
    printf("\n--- REPORT FOUND ITEM ---\n");
    newItem->id = item_counter++;
    newItem->is_lost = 0;
    newItem->next = NULL;
    
    printf("Item Name: ");
    getchar();
    fgets(newItem->name, MAX_LEN, stdin);
    newItem->name[strcspn(newItem->name, "\n")] = 0;
    
    printf("Description: ");
    fgets(newItem->description, MAX_LEN, stdin);
    newItem->description[strcspn(newItem->description, "\n")] = 0;
    
    printf("Category (e.g., Electronics, Books, Keys): ");
    fgets(newItem->category, MAX_LEN, stdin);
    newItem->category[strcspn(newItem->category, "\n")] = 0;
    
    printf("Found location: ");
    fgets(newItem->location, MAX_LEN, stdin);
    newItem->location[strcspn(newItem->location, "\n")] = 0;
    
    printf("Your contact: ");
    fgets(newItem->contact, MAX_LEN, stdin);
    newItem->contact[strcspn(newItem->contact, "\n")] = 0;
    
    getCurrentDate(newItem->date);
    
    // Add to linked list
    insertAtEnd(&found_list, newItem);
    
    // Push to stack (recent items)
    push(&recent_stack, newItem);
    
    printf("\n✓ Found item reported! ID: %d\n", newItem->id);
}

void searchItems() {
    char name[MAX_LEN];
    printf("\nEnter item name to search: ");
    getchar();
    fgets(name, MAX_LEN, stdin);
    name[strcspn(name, "\n")] = 0;
    
    printf("\n=== SEARCH RESULTS ===\n");
    
    printf("\n-- Lost Items --\n");
    Item* result = searchByName(lost_list, name);
    if (result) {
        printf("Found! ID: %d, Name: %s, Location: %s, Contact: %s\n", 
               result->id, result->name, result->location, result->contact);
    } else {
        printf("No lost items found.\n");
    }
    
    printf("\n-- Found Items --\n");
    result = searchByName(found_list, name);
    if (result) {
        printf("Found! ID: %d, Name: %s, Location: %s, Contact: %s\n", 
               result->id, result->name, result->location, result->contact);
    } else {
        printf("No found items found.\n");
    }
}

void matchItems() {
    printf("\n=== MATCHING LOST & FOUND ITEMS ===\n");
    printf("Processing matches using Queue...\n\n");
    
    // Clear queue
    initQueue(&match_queue);
    
    // Add all lost items to queue for processing
    Item* temp = lost_list;
    while (temp != NULL) {
        enqueue(&match_queue, temp);
        temp = temp->next;
    }
    
    int matches = 0;
    
    // Process queue and match with found items
    while (!isQueueEmpty(&match_queue)) {
        Item* lost = dequeue(&match_queue);
        
        // Check against all found items
        Item* found = found_list;
        while (found != NULL) {
            if (strstr(lost->name, found->name) || strstr(found->name, lost->name)) {
                printf("MATCH FOUND!\n");
                printf("Lost:  %s (Contact: %s)\n", lost->name, lost->contact);
                printf("Found: %s (Contact: %s)\n", found->name, found->contact);
                printf("Category: %s\n", lost->category);
                printf("-----------------------------------\n");
                matches++;
            }
            found = found->next;
        }
    }
    
    if (matches == 0) {
        printf("No matches found.\n");
    } else {
        printf("\nTotal matches: %d\n", matches);
    }
}

void displayMenu() {
    printf("\n========== LOST & FOUND SYSTEM ==========\n");
    printf("1. Report Lost Item \n");
    printf("2. Report Found Item \n");
    printf("3. View All Lost Items \n");
    printf("4. View All Found Items \n");
    printf("5. Search Item by Name\n");
    printf("6. View Recent Items \n");
    printf("7. Match Lost & Found \n");
    printf("0. Exit\n");
    printf("=========================================\n");
    printf("Choice: ");
}

// ==================== MAIN ====================

int main() {
    initStack(&recent_stack);
    initQueue(&match_queue);
    
    printf("╔════════════════════════════════════════╗\n");
    printf("║  COLLEGE LOST & FOUND SYSTEM          ║\n");
    printf("║  Using: Linked List, Stack, Queue     ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    int choice;
    
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addLostItem();
                break;
            case 2:
                addFoundItem();
                break;
            case 3:
                displayList(lost_list, "ALL LOST ITEMS");
                break;
            case 4:
                displayList(found_list, "ALL FOUND ITEMS");
                break;
            case 5:
                searchItems();
                break;
            case 6:
                displayStack(&recent_stack);
                break;
            case 7:
                matchItems();
                break;
            case 0:
                printf("\nThank you for using Lost & Found System!\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}