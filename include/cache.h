//
// Created by victor on 5/24/17.
//

#ifndef ED_CACHE_H
#define ED_CACHE_H

#include <iostream>
#include <map>
#define MAX_CACHE_SIZE 10000
#define CHUNK_SIZE 4096

struct cache_list{
    std::string code;
    struct cache_list *next;
    struct cache_list *prev;
};
struct Code_chunk{
    uint8_t chunk[4096];
    struct Code_chunk *next; //point to the next same ecc code or hash code, but the chunk reference are different
};

class cache{
private:
    cache();
    cache(cache const&);
    cache &operator = (cache const&);
    ~cache();

//    int re;
    std::map<std::string, struct Code_chunk *> chunk_container; //save(ECC or hash code, Chunk_reference)
    int cache_size;

    int cache_update(char code[], int code_length);
    int comp_chunk(char Chunk_reference[], struct Code_chunk *chunk_info);


public:
    static cache *Get_cache();
    static cache *cache_instance;
    struct cache_list *head_cache;
    struct cache_list *tail_cache;
    int cache_find(char code[], char chunk_reference[], int code_length);//0 represent cache miss, 1 means hit, 2 means ecc crash
    int cache_insert(char code[], char chunk_reference[], int code_length); //add new member of ecc and resort the cache
};


#endif //ED_CACHE_H

