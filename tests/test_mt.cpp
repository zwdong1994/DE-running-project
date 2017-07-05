//
// Created by victor on 6/5/17.
//
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mt.h"
#include "com_t.h"

void random_string(char str[], int length){
    int i;
    for(i = 0; i < length; i++){
        str[i] = 'a' + (rand() % 26);
    }
    str[length] = '\0';
}

int main(){
    cp_t ti;
    double stat_t = 0.0, end_t = 0.0;
    char *block[1000];
    char *ecc_code[1000];
    char *ecc_crash[100];
//    int flag;
    char read_buf[4097];
    struct addr *head_addr = NULL;
    int exist = 0;
    int not_exist = 0;
//    int crash = 0;
    mt *mt_index = mt::Get_mt("/dev/sdb1");
    int i;
    for(i = 0; i < 1000; i++){
        block[i] = new char[4097];
        random_string(block[i], 4096);
    }
    for(i = 0; i < 1000; i++){
        ecc_code[i] = new char[33];
        random_string(ecc_code[i], 32);
    }
    for(i = 0; i< 100; i++){
        ecc_crash[i] = new char[33];
        random_string(ecc_crash[i], 32);
    }
/////////////////////////////////////////////////////////////////////////////
///                    test insert and get functions                      ///
///                 and test the read and write functions                 ///
/////////////////////////////////////////////////////////////////////////////

    for(i = 0; i < 100; i++){
        stat_t = ti.get_time();
        if( !mt_index -> insert_mt(ecc_code[i], block[i], 32)){
            std::cout << "insert error" << std::endl;
            return 0;
        }
        end_t = ti.get_time();
        ti.cp_all((end_t - stat_t) * 1000, 1);
    }
    ti.cp_aver("test mt");
    for(i = 0; i < 100; i++){
        if((head_addr = mt_index -> Get_addr(ecc_code[i], 32)) == NULL){
            not_exist++;
        }
        else{
            while(head_addr != NULL){
                memset(read_buf, 0, 4097);
                read_block(head_addr, read_buf);
                if(memcmp(read_buf, block[i], 4096) == 0){ //chunk exist
                    exist++;
                    break;
                }
                head_addr = head_addr -> next;
            }
            if(head_addr == NULL)
                not_exist++;
        }

    }
    for(i = 0; i < 1000; i++){
        if((head_addr = mt_index -> Get_addr(ecc_crash[i], 32)) == NULL){
            not_exist++;
        }
        else{
            while(head_addr != NULL){
                memset(read_buf, 0, 4097);
                read_block(head_addr, read_buf);
                if(memcmp(read_buf, block[i%100], 4096) == 0){ //chunk exist
                    exist++;
                    break;
                }
                head_addr = head_addr -> next;
            }
        }

    }
    std::cout<< "total number: 1100"
             << std::endl
             << "exist number: "
             << exist
             << std::endl
             << "not exist number: "
             << not_exist
             << std::endl;

    return 0;
}
