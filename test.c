#include "dsa/dsa_list.h"

#include <stdio.h>

static size_t malloc_size = 0;

void* new_malloc(size_t size) {
    malloc_size += size;
    return malloc(size);
}

#define DAS_MALLOC new_malloc

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
    int *a = malloc(sizeof(int)); *a = 21;
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

int main()
{
    test_list();
    return 0;
}
