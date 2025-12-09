#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define BUCKETS 51
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
typedef struct Hashmap
{
    list_t buckets[BUCKETS];
} Hashmap;
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

// Hashmap
void hashmap_init(Hashmap *hashmap)
{
    for (size_t i = 0; i < BUCKETS; i++)
    {
        list_init(&hashmap->buckets[i]);
    }
}
void hash_insert(Hashmap *hashmap, int key)
{
    int bucket = key % BUCKETS;
    list_insert(&hashmap->buckets[bucket], key);
}
int hash_lockup(Hashmap *hashmap, int key)
{
    int bucket = key % BUCKETS;
    return list_lookup(&hashmap->buckets[bucket], key);
}
int main()
{
    Hashmap hashmap;
    hashmap_init(&hashmap);
    hash_insert(&hashmap, 2);
    hash_insert(&hashmap, 3);
    hash_insert(&hashmap, 4);
    printf("%d", hash_lockup(&hashmap, 3));
    // print_list(&list);
}