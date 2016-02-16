#include "dsa/dsa_list.h"

#include <stdio.h>

void free_node(void* node_) {
    printf("%d-", *(int*)node_);
    free((int*)node_);
}

void test_list() {
    dsa_list* lst = dsa_list_new();
    dsa_list_set_free_node(lst, free_node);
    for(int i = 0; i < 100; ++i) {
        int *a = malloc(sizeof(int)); *a = i;
        dsa_list_rpush(lst, dsa_list_node_new(a));
    }
    dsa_list_node* mx = dsa_list_lpop(lst);
    dsa_list_node* my = dsa_list_rpop(lst);
    printf("%d, %d\n", *(int*)mx->data, *(int*)my->data);
    dsa_list_destory(lst);
}

int main()
{
    test_list();
    return 0;
}
