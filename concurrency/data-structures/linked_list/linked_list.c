#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct node_t
{
    int data;
    struct node_t *next;
} node_t;
typedef struct list_t
{
    node_t *head;
    pthread_mutex_t mutex;
} list_t;

void list_init(list_t *list)
{
    list->head = NULL;
    pthread_mutex_init(&list->mutex, NULL);
}
void list_insert(list_t *list, int data)
{
    // No need to lock the list_t here
    node_t *node = malloc(sizeof(node_t));
    if (node == NULL)
    {
        perror("Error while allocating node_t\n");
        return;
    }
    node->data = data;
    // Lock is needed when accessing shared data
    pthread_mutex_lock(&list->mutex);
    if (list->head == NULL)
    {
        list->head = node;
        pthread_mutex_unlock(&list->mutex);
        return;
    };
    node_t *current = list->head;
    while (current->next)
    {
        current = current->next;
    }
    current->next = node;
    pthread_mutex_unlock(&list->mutex);
}
int list_lookup(list_t *list, int data)
{
    int result = -1;
    pthread_mutex_lock(&list->mutex);
    node_t *current = list->head;
    while (current)
    {
        if (current->data == data)
            result = data;
        current = current->next;
    }
    pthread_mutex_unlock(&list->mutex);
    return result;
};
void print_list(list_t *list)
{
    pthread_mutex_lock(&list->mutex);
    node_t *current = list->head;
    while (current)
    {
        printf("Data:%d\n", current->data);
        current = current->next;
    }
    pthread_mutex_unlock(&list->mutex);
}
void list_destroy(list_t *list)
{
    // Free all nodes
    node_t *current = list->head;
    while (current)
    {
        node_t *temp = current;
        current = current->next;
        free(temp);
    }
    pthread_mutex_destroy(&list->mutex); // Destroy mutex
}

int main()
{
    list_t list;
    list_init(&list);
    list_insert(&list, 1);
    list_insert(&list, 3);
    list_insert(&list, 2);
    list_destroy(&list);
    // printf("%d", list_lookup(&list, 4));
    // print_list(&list);
}