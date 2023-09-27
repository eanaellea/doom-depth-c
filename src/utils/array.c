#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Adds an element to the beginning of the linked list.
 *
 * This function adds a new node at the beginning of the linked list and copies the value into it.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @param new_data Pointer to the value to be added.
 * @param data_size Size of the value type.
 * @sideeffects Modifies the linked list by adding a new node at the beginning.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void push(ArrayNode **head_ref, void *new_data, size_t data_size) {
    ArrayNode *new_node = (ArrayNode *)malloc(sizeof(ArrayNode));
    new_node->value = malloc(data_size);
    new_node->next = *head_ref;

    memcpy(new_node->value, new_data, data_size);
    *head_ref = new_node;
}

/**
 * @brief Adds an element to the end of the linked list.
 *
 * This function adds a new node at the end of the linked list and copies the value into it.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @param new_data Pointer to the value to be added.
 * @param data_size Size of the value type.
 * @sideeffects Modifies the linked list by adding a new node at the end.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void append(ArrayNode **head_ref, void *new_data, size_t data_size) {
    ArrayNode *new_node = (ArrayNode *)malloc(sizeof(ArrayNode));
    ArrayNode *last = *head_ref;
    new_node->value = malloc(data_size);
    new_node->next = NULL;

    memcpy(new_node->value, new_data, data_size);

    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    while (last->next != NULL) {
        last = last->next;
    }

    last->next = new_node;
}

/**
 * @brief Deletes an element at a given index.
 *
 * This function deletes a node at a given index in the linked list.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @param index The index of the node to be deleted.
 * @sideeffects Modifies the linked list by removing a node.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void delete_node(ArrayNode **head_ref, int index) {
    if (*head_ref == NULL || index < 0) {
        return;
    }

    ArrayNode *temp = *head_ref;

    if (index == 0) {
        *head_ref = temp->next;
        free(temp->value);
        free(temp);
        return;
    }

    for (int i = 0; temp != NULL && i < index - 1; i++) {
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL) {
        return;
    }

    ArrayNode *next = temp->next->next;
    free(temp->next->value);
    free(temp->next);
    temp->next = next;
}

/**
 * @brief Inserts an element at a given index in the linked list.
 *
 * This function inserts a new node at a specified index in the linked list and copies the value into it.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @param new_data Pointer to the value to be added.
 * @param data_size Size of the value type.
 * @param index The index at which the new node should be inserted.
 * @return 0 if the operation is successful, -1 otherwise.
 * @sideeffects Modifies the linked list by adding a new node at the specified index.
 * @dependencies No dependencies.
 * @errors Returns -1 if the index is out of bounds.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
int insertAtIndex(ArrayNode **head_ref, void *new_data, size_t data_size, int index) {
    if (index < 0) {
        return -1;  // Index out of bounds
    }

    ArrayNode *new_node = (ArrayNode *)malloc(sizeof(ArrayNode));
    new_node->value = malloc(data_size);
    memcpy(new_node->value, new_data, data_size);

    if (index == 0) {
        new_node->next = *head_ref;
        *head_ref = new_node;
        return 0;
    }

    ArrayNode *prev = *head_ref;
    for (int i = 0; i < index - 1; i++) {
        if (prev == NULL) {
            free(new_node->value);
            free(new_node);
            return -1;  // Index out of bounds
        }
        prev = prev->next;
    }

    new_node->next = prev->next;
    prev->next = new_node;

    return 0;
}

/**
 * @brief Finds an element in the linked list.
 *
 * This function searches for an element in the linked list and returns its index.
 *
 * @param head Pointer to the head of the linked list.
 * @param data_to_find Pointer to the value to find.
 * @param data_size Size of the value type.
 * @return The index of the found node, or -1 if not found.
 * @sideeffects No side effects.
 * @dependencies No dependencies.
 * @errors Returns -1 if the element is not found.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
int find_node(ArrayNode *head, void *data_to_find, size_t data_size) {
    ArrayNode *current = head;
    int index = 0;

    while (current != NULL) {
        // Compare memory block
        if (memcmp(current->value, data_to_find, data_size) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

/**
 * @brief Retrieves the node at a given index in the linked list.
 *
 * This function traverses the linked list to find the node at the specified index.
 * The index is 0-based, meaning that passing 0 will return the first node, 1 will return the second, and so on.
 *
 * @param head Pointer to the head of the linked list.
 * @param index The 0-based index of the node to be retrieved.
 * @return Pointer to the node at the specified index, or NULL if the index is out of bounds.
 * @sideeffects No side effects.
 * @dependencies Assumes that the ArrayNode structure is defined with a 'next' field.
 * @errors Returns NULL if the index is out of bounds.
 * @author Noam DE MASURE
 * @date 27/09/2023
 */
void * get_value_at_index(ArrayNode *head, int index) {
    ArrayNode *current = head;

    while (index != 0){
        if(current->next!=NULL){
            current = current->next;
        } else {
            return NULL;
        }
        index--;
    }

    return current->value;
}

/**
 * @brief Frees the linked list.
 *
 * This function frees all the nodes in the linked list and sets the head to NULL.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @sideeffects Frees the memory allocated for the linked list.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void free_list(ArrayNode **head_ref) {
    ArrayNode *current = *head_ref;
    ArrayNode *next;

    while (current != NULL) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }

    *head_ref = NULL;
}


/**
 * @brief Prints the linked list.
 *
 * This function prints the elements of the linked list using a provided print function.
 *
 * @param node Pointer to the head of the linked list.
 * @param fptr Pointer to the function to print the value.
 * @sideeffects Prints the elements of the linked list to stdout.
 * @dependencies Depends on the provided print function.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void print_list(ArrayNode *node, void (*fptr)(void *)) {
    while (node != NULL) {
        (*fptr)(node->value);
        node = node->next;
    }
    printf("\n");
}

/**
 * @brief Prints a string.
 *
 * This function prints a string (char *) to the standard output.
 *
 * @param str Pointer to the string to be printed.
 * @sideeffects Prints the string to the standard output.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void print_string(void *str) {
    printf(" %s", (char *)str);
}

/**
 * @brief Prints an integer.
 *
 * This function prints an integer (int) to the standard output.
 *
 * @param n Pointer to the integer to be printed.
 * @sideeffects Prints the integer to the standard output.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void print_int(void *n) {
    printf(" %d", *(int *)n);
}

/**
 * @brief Prints a floating-point number.
 *
 * This function prints a floating-point number (float) to the standard output.
 *
 * @param f Pointer to the float to be printed.
 * @sideeffects Prints the float to the standard output.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void print_float(void *f) {
    printf(" %f", *(float *)f);
}

