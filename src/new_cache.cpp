//
// Created by victor on 6/30/17.
//

#include "new_cache.h"

new_cache::new_cache() {
    cache_size = 0;
    head_cache = NULL;
    tail_cache = NULL;
    ini_pool();

}

new_cache::~new_cache() {
    destory_pool();
}

new_cache* new_cache::cache_instance = NULL;

new_cache *new_cache::Get_cache() {
    static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
    if(cache_instance == NULL){
        pthread_mutex_lock(&mu);
        if(cache_instance == NULL)
            cache_instance = new new_cache();
        pthread_mutex_unlock(&mu);
    }
    return cache_instance;
}

int new_cache::cache_find(std::string &code, char chunk_reference[]) {
    rb_structure *get_container = chunk_container[code];
    if(get_container != NULL && get_container -> flag == 1){ //ecc exist
        if(memcmp(chunk_reference, get_container -> chunk, CHUNK_SIZE) == 0 ){//chunk exist
            if(!cache_update(get_container)) { //update cache error
                std::cout << "cache update error!" << std::endl;
                exit(-1);
            }
            return 1;
        }
        else{
            memcpy(get_container -> chunk, chunk_reference, CHUNK_SIZE);
            if(!cache_update(get_container)) { //update cache error
                std::cout << "cache update error!" << std::endl;
                exit(-1);
            }
            return 0;
        }
    }
    else{ //the ecc is not in the cache, so insert it into the cache
        if(!cache_insert(code, chunk_reference)){//insert error
            std::cout<<"cache insert error!"<<std::endl;
            exit(-1);
        }
        return 0;
    }
}

int new_cache::cache_insert(std::string &code, char chunk_reference[]) {
    rb_structure *add_container = malloc_rbs();
    memcpy(add_container -> chunk, chunk_reference, CHUNK_SIZE);
    chunk_container[code] = add_container;
    if(head_cache == NULL){ //cache is empty

        add_container -> next = NULL;
        add_container -> prev = NULL;
        head_cache = add_container;
//        code_tail = code;
        cache_size++;
        return 1;
    }
    else{
        if(cache_size == 1){ //cache contained only one member


            add_container -> next = head_cache;
            head_cache -> next = add_container;
            add_container -> prev = head_cache;
            head_cache -> prev = add_container;
            tail_cache = head_cache;
            head_cache = add_container;
            ++cache_size;
            return 1;
        }
        else if( cache_size == MAX_CACHE_SIZE) {
            /*struct Code_chunk *chunk_info = NULL, *hp_chunk = NULL;
            chunk_info = chunk_container[tail_cache -> code];
            hp_chunk = chunk_info;
            while(hp_chunk != NULL) {
                hp_chunk = hp_chunk -> next;
                delete chunk_info;
                chunk_info = hp_chunk;
            }
            chunk_container.erase(tail_cache -> code); //delete the tail member in the R-BTree*/
            //add_container -> code = mid_code;
            add_container -> next = head_cache;
            head_cache -> prev = add_container;
            head_cache = add_container;

            tail_cache -> prev -> next = head_cache;
            head_cache -> prev = tail_cache -> prev;
            free_rbs(tail_cache);
            tail_cache = head_cache -> prev;

            return 1;
        }
        else{ //cache contained more than one member

            //add_member -> code = mid_code;
            add_container -> next = head_cache;
            tail_cache -> next = add_container;
            head_cache -> prev = add_container;
            add_container -> prev = tail_cache;
            head_cache = add_container;
            ++cache_size;
            return 1;
        }
    }
}

int new_cache::cache_update(rb_structure *rbs) {
    rb_structure *mid_rbs = NULL;
    mid_rbs = rbs;
    if(cache_size == 1)
        return 1;
    else if(cache_size == 2){
        if(rbs == head_cache)
            return 1;
        else{
            mid_rbs = head_cache;
            head_cache = rbs;
            tail_cache = mid_rbs;
            return 1;
        }
    }
    else{
        rbs -> prev -> next = mid_rbs -> next;
        rbs -> next -> prev = mid_rbs -> prev;
        head_cache -> prev = rbs;
        rbs -> next = head_cache;
        tail_cache -> next = rbs;
        rbs -> prev = tail_cache;
        head_cache = rbs;
        return 1;
    }
}

int new_cache::free_rbs(rb_structure *f_rbs) {
    f_rbs -> next = delete_pool_head;
    f_rbs -> flag = 0;
    delete_pool_head = f_rbs;
    return 0;
}

rb_structure* new_cache::malloc_rbs() {
    rb_structure *alloc_node = NULL;
    if(allocate_pool_head != NULL) {
        alloc_node = allocate_pool_head;
        allocate_pool_head = allocate_pool_head -> next;
        alloc_node -> flag = 1;
        alloc_node->next = NULL;
    }
    else{
        allocate_pool_head = delete_pool_head;
        delete_pool_head = NULL;
        alloc_node = allocate_pool_head;
        allocate_pool_head = allocate_pool_head -> next;
        alloc_node -> next = NULL;
        alloc_node -> flag = 1;
    }
    return alloc_node;
}

void new_cache::ini_pool() {
    int i = 0;
    rb_structure *new_alloc = NULL;
    allocate_pool_head = NULL;
    delete_pool_head = NULL;

    for(; i < MAX_CACHE_SIZE + 100; ++i){
        new_alloc = new rb_structure;
        new_alloc -> next = allocate_pool_head;
        new_alloc -> flag = 0;
        allocate_pool_head = new_alloc;
    }

}

void new_cache::destory_pool() {
    rb_structure *ap = allocate_pool_head, *hp = allocate_pool_head -> next;
    rb_structure *dp = delete_pool_head, *dhp = delete_pool_head -> next;
    rb_structure *LRUp = head_cache, *dLRUp = head_cache -> next;

    while(ap != NULL){
        delete ap;
        ap = hp;
        if(hp != NULL)
            hp = hp ->next;
    }

    while(dp != NULL){
        delete dp;
        dp = dhp;
        if(dhp != NULL)
            dhp = dhp -> next;
    }

    while(LRUp != tail_cache){
        delete LRUp;
        LRUp = dLRUp;
        if(dLRUp != tail_cache)
            dLRUp = dLRUp -> next;
    }
    delete tail_cache;

}



