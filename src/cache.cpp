//
// Created by victor on 5/24/17.
//

#include <string.h>

#include <string>
#include "cache.h"
#include <iomanip>

cache::cache(){
    head_cache = NULL;
    tail_cache = NULL;
    cache_size = 0;
//    re = 0;
}

cache::~cache(){

}

cache* cache::cache_instance = NULL;

cache* cache::Get_cache(){
    static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
    if(cache_instance == NULL){
        pthread_mutex_lock(&mu);
        if(cache_instance == NULL)
            cache_instance = new cache();
        pthread_mutex_unlock(&mu);
    }
    return cache_instance;
}

int cache::cache_insert(char *code, char chunk_reference[], int code_length) {
    struct cache_list *add_member = new cache_list;
    std::string mid_code;
    struct Code_chunk *new_code_chunk = NULL;
    code[code_length] = '\0';

    mid_code = code;

    new_code_chunk = new Code_chunk;
    new_code_chunk -> next = NULL;
    memcpy(new_code_chunk -> chunk, chunk_reference, CHUNK_SIZE);
    chunk_container[mid_code] = new_code_chunk;


    if(head_cache == NULL){ //cache is empty


        add_member -> code = mid_code;
        add_member -> next = NULL;
        add_member -> prev = NULL;
        head_cache = add_member;
        cache_size++;
        return 1;
    }
    else{
        if(cache_size == 1){ //cache contained only one member

            add_member -> code = mid_code;
            add_member -> next = head_cache;
            head_cache -> next = add_member;
            add_member -> prev = head_cache;
            head_cache -> prev = add_member;
            tail_cache = head_cache;
            head_cache = add_member;
            cache_size++;
            return 1;
        }
        else if( cache_size == MAX_CACHE_SIZE) {
            struct Code_chunk *chunk_info = NULL, *hp_chunk = NULL;
            chunk_info = chunk_container[tail_cache -> code];
            hp_chunk = chunk_info;
            while(hp_chunk != NULL) {
                hp_chunk = hp_chunk -> next;
                delete chunk_info;
                chunk_info = hp_chunk;
            }
            chunk_container.erase(tail_cache -> code); //delete the tail member in the R-BTree
            add_member -> code = mid_code;
            add_member -> next = head_cache;
            head_cache -> prev = add_member;
            head_cache = add_member;

            tail_cache -> prev -> next = head_cache;
            head_cache -> prev = tail_cache -> prev;
            delete tail_cache;
            tail_cache = head_cache -> prev;

            return 1;
        }
        else{ //cache contained more than one member

            add_member -> code = mid_code;
            add_member -> next = head_cache;
            tail_cache -> next = add_member;
            head_cache -> prev = add_member;
            add_member -> prev = tail_cache;
            head_cache = add_member;
            cache_size++;
            return 1;
        }
    }

}

int cache::cache_find(char *code, char *chunk_reference, int code_length) {
    std::string mid_string;
    code[code_length] = '\0';
    mid_string = code;
    struct Code_chunk *chunk_info = NULL;
    chunk_info = chunk_container[mid_string];
    if(chunk_info != NULL){ //ecc exist

        if(comp_chunk(chunk_reference, chunk_info)){//chunk exist
            if(!cache_update(code, code_length)) { //update cache error
                std::cout << "cache update error!" << std::endl;
                exit(-1);
            }
            return 1;
        }/*
        else{ //Notice: if we want to achieve crash cache, we need to synthetic the data in the ssd to the cache.
            struct Code_chunk *new_code_chunk = NULL;
            new_code_chunk = new Code_chunk;
            memcpy(new_code_chunk -> chunk, chunk_reference, CHUNK_SIZE);
            new_code_chunk -> next = chunk_info;
            chunk_container[mid_string] = new_code_chunk;
            if(!cache_update(code, code_length)) { //update cache error

                std::cout << "cache update error!" << std::endl;
                exit(-1);
            }
            return 2; // ecc crash happened
        }*/
        else{
            memcpy(chunk_info -> chunk, chunk_reference, CHUNK_SIZE);
            if(!cache_update(code, code_length)) { //update cache error
                std::cout << "cache update error!" << std::endl;
                exit(-1);
            }
            return 0;
        }
    }
    else{ //the ecc is not in the cache, so insert it into the cache
        if(!cache_insert(code, chunk_reference, code_length)){//insert error
            std::cout<<"cache insert error!"<<std::endl;
            exit(-1);
        }
        return 0;
    }
}

int cache::comp_chunk(char *Chunk_reference, struct Code_chunk *chunk_info) {
    struct Code_chunk *p = chunk_info;
    while(p!=NULL){
        if(memcmp(Chunk_reference, p -> chunk, CHUNK_SIZE) == 0){ //chunk exist
            return 1;
        }
        p = p->next;
    }
    return 0;
}

int cache::cache_update(char *code, int code_length) {
    struct cache_list *mid_member = head_cache;
    std::string mid_str;

    code[code_length] = '\0';
    mid_str = code;
/*    std::cout<<"4444444"<<std::endl;
    std::cout<< head_cache -> code <<std::endl;
    std::cout<< mid_str <<std::endl;*/
    if(cache_size == 1)
        return 1;
    else if(cache_size == 2){

        if(mid_str == (head_cache -> code))
            return 1;
        else if(mid_str == (tail_cache -> code)){
            tail_cache -> code = head_cache -> code;
            head_cache -> code = mid_str;
            return 1;
        }
        else
            return 0;
    }
    else {
        while (mid_member->next != head_cache || mid_member == tail_cache) { //the member is not tail member
            if(mid_str == (mid_member -> code)){

                if(mid_member == head_cache)
                    return 1;
                else if(mid_member == tail_cache){
                    head_cache = tail_cache;
                    tail_cache = tail_cache -> prev;
                    return 1;
                }
                else {
                    mid_member->prev->next = mid_member->next;
                    mid_member->next->prev = mid_member->prev;

                    mid_member->next = head_cache;
                    tail_cache->next = mid_member;
                    head_cache->prev = mid_member;
                    mid_member->prev = tail_cache;
                    head_cache = mid_member;
                    return 1;
                }
            }
            else
                mid_member = mid_member -> next;

        }

        return 0;
    }
}




