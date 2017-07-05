//
// Created by victor on 6/30/17.
//

#ifndef ED_NEW_CACHE_H
#define ED_NEW_CACHE_H

#include <iostream>
#include <map>
#include <string.h>
#define MAX_CACHE_SIZE 10000
#define CHUNK_SIZE 4096

typedef struct code_chunk{
    uint8_t flag; //1 means the block is allocated status, 0 means the block is in the free status
    uint8_t chunk[4096];
    struct code_chunk *prev;
    struct code_chunk *next; //point to the next node of LRU chain.
}rb_structure;

class new_cache{
private:
    new_cache();
    new_cache(new_cache const&);
    new_cache &operator = (new_cache const&);
    ~new_cache();

    rb_structure *allocate_pool_head;
    rb_structure *delete_pool_head;

    rb_structure *head_cache;
    rb_structure *tail_cache;

    std::string code_tail;

    void ini_pool();
    void destory_pool();
    rb_structure* malloc_rbs();
    int free_rbs(rb_structure *f_rbs);

    int cache_size;
    std::map<std::string, struct code_chunk *> chunk_container; //save(ECC or hash code, Chunk_reference)

public:
    static new_cache *Get_cache();
    static new_cache *cache_instance;
    int cache_find(std::string &code, char chunk_reference[]);//0 represent cache miss, 1 means hit, 2 means ecc crash
    int cache_insert(std::string &code, char chunk_reference[]); //add new member of ecc and resort the cache
    int cache_update(rb_structure *rbs);
};


#endif //ED_NEW_CACHE_H
