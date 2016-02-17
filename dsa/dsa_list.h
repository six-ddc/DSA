#ifndef _DSA_LIST_H_
#define _DSA_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#ifndef DSA_MALLOC
#define DSA_MALLOC malloc
#endif

#ifndef DSA_FREE
#define DSA_FREE free
#endif

// node, list

typedef struct dsa_list_node_
{
    struct dsa_list_node_*  prev;
    struct dsa_list_node_*  next;
    void*  data;
} dsa_list_node;

typedef struct
{
    dsa_list_node* head;
    dsa_list_node* tail;
    size_t length;
    void (*free_node)(void*);
    int (*comp_node)(void*, void*);
} dsa_list;

typedef enum
{
    DSA_LIST_HEAD,
    DSA_LIST_TAIL
} dsa_list_direction;

typedef struct
{
    dsa_list_node* next;
    dsa_list_direction direction;
} dsa_list_iterator;

// macros

#define dsa_list_foreach(lst, direction, block) {                       \
        dsa_list_node* node = direction == DSA_LIST_HEAD ? lst->head : lst->tail;   \
        while(node) {                                                   \
            void* data = node->data;                                    \
            block                                                       \
            node = direction == DSA_LIST_HEAD ? node->next : node->prev;\
        }                                                               \
    }

#define dsa_list_foreach2(lst, direction, block) {                      \
        dsa_list_iterator* it = dsa_list_iterator_new(lst, direction);  \
        dsa_list_node* node = NULL;                                     \
        while((node = dsa_list_iterator_next(it))) {                    \
            void* data = node->data;                                    \
            block                                                       \
        }                                                               \
        dsa_list_iterator_destory(it);                                  \
    }

// prototypes
dsa_list_node* dsa_list_node_new(void* v_) {
    dsa_list_node* n = DSA_MALLOC(sizeof(dsa_list_node));
    if(n) {
        n->prev = NULL;
        n->next = NULL;
        n->data = v_;
    }
    return n;
}

dsa_list* dsa_list_new() {
    dsa_list* l = DSA_MALLOC(sizeof(dsa_list));
    if(l) {
        l->head = NULL;
        l->tail = NULL;
        l->length = 0;
        l->free_node = NULL;
        l->comp_node = NULL;
    }
    return l;
}

void dsa_list_destory(dsa_list* l_) {
    dsa_list_node *next, *curr = l_->head;
    while(curr) {
        next = curr->next;
        if(l_->free_node) l_->free_node(curr->data);
        DSA_FREE(curr);
        curr = next;
    }
    DSA_FREE(l_);
}

void dsa_list_set_free_node(dsa_list* l_, void (*free_)(void* node_)) {
    if(l_) l_->free_node = free_;
}

void dsa_list_set_comp_node(dsa_list* l_, int (*comp_)(void*, void*)) {
    if(l_) l_->comp_node = comp_;
}

dsa_list_node* dsa_list_rpush(dsa_list* l_, dsa_list_node* n_) {
    if(!l_ || !n_) return NULL;
    if(l_->length) {
        n_->prev = l_->tail;
        n_->next = NULL;
        l_->tail->next = n_;
        l_->tail = n_;
    } else {
        l_->head = l_->tail = n_;
        n_->prev = n_->next = NULL;
    }
    ++l_->length;
    return n_;
}

dsa_list_node* dsa_list_rpop(dsa_list* l_) {
    if(!l_ || !l_->length) return NULL;
    dsa_list_node* ret = l_->tail;
    if(--l_->length) {
        l_->tail = ret->prev;
        l_->tail->next = NULL;
    } else {
        l_->tail = l_->head = NULL;
    }
    return ret;
}

dsa_list_node* dsa_list_lpush(dsa_list* l_, dsa_list_node* n_) {
    if(!l_ || !n_) return NULL;
    if(l_->length) {
        n_->next = l_->head;
        n_->prev = NULL;
        l_->head->prev = n_;
        l_->head = n_;
    } else {
        l_->head = l_->tail = n_;
        n_->prev = n_->next = NULL;
    }
    ++l_->length;
    return n_;
}

dsa_list_node* dsa_list_lpop(dsa_list* l_) {
    if(!l_ || !l_->length) return NULL;
    dsa_list_node* ret = l_->head;
    if(--l_->length) {
        l_->head = ret->next;
        l_->head->prev = NULL;
    } else {
        l_->head = l_->tail = NULL;
    }
    return ret;
}

dsa_list_node* dsa_list_find(dsa_list* l_, void* v_) {
    dsa_list_foreach(l_, DSA_LIST_HEAD, {
        if(l_->comp_node) {
            if(l_->comp_node(data, v_) == 0) {
                return data;
            }
        } else {
            if(data == v_) {
                return data;
            }
        }
    })
    return NULL;
}

// iterator
dsa_list_iterator* dsa_list_iterator_new_from_node(dsa_list_node* n_, dsa_list_direction d_) {
    dsa_list_iterator* i = DSA_MALLOC(sizeof(dsa_list_iterator));
    if(i) {
        i->next = n_;
        i->direction = d_;
    }
    return i;
}

dsa_list_iterator* dsa_list_iterator_new(dsa_list* l_, dsa_list_direction d_) {
    if(!l_) return NULL;
    return dsa_list_iterator_new_from_node(d_ == DSA_LIST_HEAD ? l_->head : l_->tail, d_);
}

dsa_list_node* dsa_list_iterator_next(dsa_list_iterator* i_) {
    dsa_list_node* ret = i_->next;
    if(ret) {
        i_->next = i_->direction == DSA_LIST_HEAD ? ret->next : ret->prev;
    }
    return ret;
}

void dsa_list_iterator_destory(dsa_list_iterator* i_) {
    DSA_FREE(i_);
}

#ifdef __cplusplus
}
#endif

#endif  /* _DSA_LIST_H_ */
