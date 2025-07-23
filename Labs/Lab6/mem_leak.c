#include<stdio.h>
#include<stdlib.h>

struct node {
    int value;
    struct node* next;
    struct node* previous;
};

struct node* make_node(int val, struct node* next, struct node* previous) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    new_node->value = val;
    new_node->next = next;
    new_node->previous = previous;
    return new_node;
}

void add_back(struct node** list, int value) {
    // Handle case where list is NULL
    if (*list == NULL) {
        *list = make_node(value, NULL, NULL);
        return;
    }
    // Iterate until I find the last node in the linked list.
    struct node* item = *list;
    while (item->next != NULL) {
        item = item->next;
    }
    struct node* new_node = make_node(value, NULL, item);
    item->next = new_node;
}

int remove_node(struct node** list, int value) {
    struct node* item = *list;

    while (item != NULL) {
        if (item->value == value) {
            if (item->previous == NULL) {
                *list = item->next;
                item->next->previous = NULL;
            }
            else if (item->next == NULL) {
                struct node* before = item->previous;
                before->next = NULL;
            }
            else {
                struct node* before = item->previous;
                struct node* after = item->next;
                before->next = after;
                after->previous = before;
            }

            free(item);
            return 0;
        }
        item = item->next;
    }

    return -1; // value not found
}

void print_list(struct node* list, int backwards) {
    if (list == NULL) return;

    if (backwards == 0) {
        while (list != NULL) {
            printf("%d ", list->value);
            list = list->next;
        }
        printf("\n");
    } else {
        // move to the tail
        while (list->next != NULL) {
            list = list->next;
        }
        // now print backwards
        while (list != NULL) {
            printf("%d ", list->value);
            list = list->previous;
        }
        printf("\n");
    }
}

//    int value = ..
//    struct node* next = …
//    struct node* previous = ..
//    struct node* node = make_node(value, next, previous);

int main(int argc, char** argv) {
    struct node* list = NULL;        // initially the tree is NULL

    add_back(&list, 42);
    add_back(&list, 13);
    add_back(&list, 50);
    add_back(&list, 15);
    add_back(&list, 2);
    print_list(list, 0);                // 42, 13, 50, 15, 2
    print_list(list, 1);   

    remove_node(&list, 15);   
    remove_node(&list, 13);   
    remove_node(&list, 42);         
    print_list(list, 0);                // 50, 2

    while (list != NULL) {
        struct node* store = list->next;
        free(list);
        list = store;
    }
}