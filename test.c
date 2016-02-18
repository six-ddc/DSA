#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static size_t malloc_size = 0;

void* new_malloc(size_t size) {
    malloc_size += size;
    return malloc(size);
}

void *new_calloc(size_t n, size_t size) {
    malloc_size += (n * size);
    return calloc(n, size);
}

#define DSA_MALLOC new_malloc
#define DSA_CALLOC new_calloc

#include "dsa/dsa.h"

void free_node(void* node_) {
    printf("%d ", *(int*)node_);
    free((int*)node_);
}

int comp_node(void* a, void* b) {
    return *(int*)a - *(int*)b;
}

void test_list() {
    // prototype
    dsa_list* lst = dsa_list_new();
    dsa_list_set_free_node(lst, free_node);
    for(int i = 0; i < 100; ++i) {
        int *a = malloc(sizeof(int)); *a = i;
        dsa_list_rpush(lst, dsa_list_node_new(a));
        int *b = malloc(sizeof(int)); *b = 2 * i;
        dsa_list_lpush(lst, dsa_list_node_new(b));
    }
    dsa_list_node* mx = dsa_list_lpop(lst);
    dsa_list_node* my = dsa_list_rpop(lst);
    printf("%d, %d\n", *(int*)mx->data, *(int*)my->data);

    // iterator
    dsa_list_iterator* it1 = dsa_list_iterator_new(lst, DSA_LIST_HEAD);
    dsa_list_iterator* it2 = dsa_list_iterator_new(lst, DSA_LIST_TAIL);
    dsa_list_node* next = NULL;
    int i = 0;
    while(i++ < 5) {
        next = dsa_list_iterator_next(it1);
        printf("%d-", *(int*)next->data);
        next = dsa_list_iterator_next(it2);
        printf("%d ", *(int*)next->data);
    }
    printf("\n");
    dsa_list_iterator_destory(it1);
    dsa_list_iterator_destory(it2);

    dsa_list_set_comp_node(lst, comp_node);
    int *a = malloc(sizeof(int)); *a = 20;
    next = dsa_list_find(lst, a);
    if(next) printf("find: %d\n", *a);

    // macro
    dsa_list_foreach(lst, DSA_LIST_TAIL, {
            printf("%d^", *(int*)data);
            if(*(int*)data == 180) {
                break;
            }
        })
    printf("\n");

    dsa_list_destory(lst);
}

void test_xxhash()
{
    srand(time(NULL));
    for(int i = 0; i < 100; ++i) {
        char a[100] = {0};
        for(int j = 0; j < sizeof(a) - 1; ++j) {
            a[j] = rand() % 26 + 'a';  
        }
        printf("%s - %u\n", a, XXH32(a, sizeof(a), 0));
    }
}

void test_hashmap()
{
    size_t begin_size = malloc_size;
    dsa_hashmap* hm = dsa_hashmap_new();
    for(int i = 0; i < 100000; ++i) {
        char a[100] = {0};
        char b[100] = {0};
        for(int j = 0; j < sizeof(a) - 1; ++j) {
            a[j] = rand() % 26 + 'a';  
            b[j] = rand() % 26 + 'a';  
        }
        if(dsa_hashmap_put(hm, a, b) != DSA_HASHMAP_OK || dsa_hashmap_put(hm, b, a) != DSA_HASHMAP_OK) {
            printf("fail,  hashmap size:%u, buckets:%u\n", hm->size, hm->buckets);
        }
    }
    printf("alloc size: %lu, hashmap buckets: %u, n_rehash: %u, n_nothit: %u\n", malloc_size - begin_size, hm->buckets, hm->n_rehash, hm->n_nothit);
}

int main()
{
    test_hashmap();
    return 0;
}
