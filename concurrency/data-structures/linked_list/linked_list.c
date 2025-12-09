#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct node_t
{
    int data;
    struct node_t *next;
} node_t;
typedef struct List
{
    node_t *head;
    pthread_mutex_t mutex;
} List;

void list_init(List *list)
{
    list->head = NULL;
    pthread_mutex_lock(&list->mutex);
}
void list_insert(List *list, int data)
{
    // No need to lock the List here
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
void print_list(List *list)
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
int main()
{
    List list;
    list_init(&list);
    list_insert(&list, 1);
    list_insert(&list, 3);
    list_insert(&list, 2);
    print_list(&list);
}