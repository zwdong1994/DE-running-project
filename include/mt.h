//
// Created by victor on 6/2/17.
//

#ifndef ED_MT_H
#define ED_MT_H

#include <cstdint>
#include <iostream>
#include <map>
#include <aio.h>

#define BLOCK_SIZE 4096
struct addr{
    unsigned long offset; //block address
    struct addr *next;
};

int write_block(unsigned long offset, char chunk_reference[]);
int read_block(struct addr *write_addr, char chunk_reference[]);

class mt{
public:
    static mt *Get_mt(char *devname);
    static mt *mt_instance;
    struct addr* Get_addr(char ecc_code[], int length_ecc);  //get the chunk addr from the mt list
    int insert_mt(char ecc_code[], char chunk_reference[], int length_ecc); //insert a new ecc-addr pair to the mt list and in the mean time write the block
    unsigned long alloc_addr_point;
    double time_total;
    uint64_t write_time;


private:
    mt(char *devname);
    mt(mt const&);
    mt &operator = (mt const&);
    ~mt();



    unsigned int ssd_capacity;
    unsigned long max_size_addr; //the maximum block address

    std::map<std::string, struct addr*> mt_container; //save(ECC or hash code, block address)


};

#endif //ED_MT_H
