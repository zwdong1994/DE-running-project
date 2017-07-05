//
// Created by victor on 7/2/17.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "cache.h"
#include "new_cache.h"

void random_string(char str[], int length){
    int i;
    for(i = 0; i < length; i++){
        str[i] = 'a' + (rand() % 26);
    }
    str[length] = '\0';
}

double get_time(void) {
    struct	timeval	mytime;
    gettimeofday(&mytime,NULL);
    return (mytime.tv_sec*1.0+mytime.tv_usec/1000000.0);
}

int main(void){
    char *block[20000];
    char *ecc_code[10001];
    char *ecc_crash[100];
    char *ecc_find[20000];
    int flag;
    int exist = 0;
    int not_exist = 0;
    int crash = 0;
    double time_total = 0.0, stat_time = 0.0, end_time = 0.0;
    std::string mid_str;
    cache *cac = cache::Get_cache();
    new_cache *new_cac = new_cache::Get_cache();
    int i;
    for(i = 0; i < 20000; i++){
        block[i] = new char[4097];
        random_string(block[i], 4096);
    }
    for(i = 0; i < 10001; i++){
        ecc_code[i] = new char[33];
        random_string(ecc_code[i], 32);
    }
    for(i = 0; i< 100; i++){
        ecc_crash[i] = new char[33];
        random_string(ecc_crash[i], 32);
    }
    for(i = 0; i < 20000; i++){
        ecc_find[i] = new char[33];
        random_string(ecc_find[i], 32);
    }
/*
/////////////////////////////////////////////////////////////////////////////
///                      new cache test insert                            ///
/////////////////////////////////////////////////////////////////////////////
    char chunk[4097];
    for(i = 0; i < 10001; i++){
        random_string(chunk, 4096);
        chunk[4096] = '\0';
        mid_str = ecc_code[i];
        stat_time = get_time();
        new_cac -> cache_insert(mid_str, chunk);
        end_time = get_time();
        time_total += (end_time - stat_time) * 1000;
        if(i == 9999) {
            std::cout << "head nodes"
                      << std::endl
                      << new_cac -> head_cache ->code
                      << std::endl
                      << new_cac -> head_cache -> next -> code
                      << std::endl;
            std::cout << "tail nodes"
                      << std::endl
                      << new_cac->tail_cache->prev->code
                      << std::endl
                      << new_cac->tail_cache->code
                      << std::endl
                      << "head:"
                      << new_cac->tail_cache->next->code
                      << std::endl;
            std::cout << "/////////////////////////////////////////////"<< std::endl;
        }
        if(i == 10000) {
            std::cout << "head nodes"
                      << std::endl
                      << new_cac -> head_cache ->code
                      << std:: endl
                      << new_cac -> head_cache -> next -> code
                      << std::endl
                      << new_cac -> head_cache -> next -> next -> code
                      << std::endl;
            std::cout << "tail nodes"
                      << std::endl
                      << new_cac->tail_cache->prev->code
                      << std::endl
                      << new_cac->tail_cache->code
                      << std::endl
                      << "head:"
                      << new_cac->tail_cache->next->code
                      << std::endl;
            std::cout << "/////////////////////////////////////////////"<< std::endl;
        }
    }
    std::cout << "New cache insert average time is : " << time_total / 10001 << std::endl;
    time_total = 0.0;

/////////////////////////////////////////////////////////////////////////////
///                        cache test insert                              ///
/////////////////////////////////////////////////////////////////////////////
    char chunk[4097];
    for(i = 0; i < 10001; i++){
        random_string(chunk, 4096);
        chunk[4096] = '\0';
        mid_str = ecc_code[i];
        stat_time = get_time();
        cac -> cache_insert(mid_str, chunk);
        end_time = get_time();
        time_total += (end_time - stat_time) * 1000;
        if(i == 9999) {
            std::cout << "head nodes"
                      << std::endl
                      << cac -> head_cache ->code
                      << std::endl
                      << cac -> head_cache -> next -> code
                      << std::endl;
            std::cout << "tail nodes"
                      << std::endl
                      << cac->tail_cache->prev->code
                      << std::endl
                      << cac->tail_cache->code
                      << std::endl
                      << "head:"
                      << cac->tail_cache->next->code
                      << std::endl;
            std::cout << "/////////////////////////////////////////////"<< std::endl;
        }
        if(i == 10000) {
            std::cout << "head nodes"
                      << std::endl
                      << cac -> head_cache ->code
                      << std:: endl
                      << cac -> head_cache -> next -> code
                      << std::endl
                      << cac -> head_cache -> next -> next -> code
                      << std::endl;
            std::cout << "tail nodes"
                      << std::endl
                      << cac->tail_cache->prev->code
                      << std::endl
                      << cac->tail_cache->code
                      << std::endl
                      << "head:"
                      << cac->tail_cache->next->code
                      << std::endl;
            std::cout << "/////////////////////////////////////////////"<< std::endl;
        }
    }
    std::cout << "Old cache insert average time is : " << time_total / 10001 << std::endl;*/

/////////////////////////////////////////////////////////////////////////////
///                              test new cache find                      ///
/////////////////////////////////////////////////////////////////////////////
    time_total = 0.0;
    for(i = 0; i < 20000; i++){
        mid_str = ecc_find[i];
        stat_time = get_time();
        flag = new_cac -> cache_find(mid_str, block[i]);
        end_time = get_time();
        time_total += (end_time - stat_time) * 1000;
        if( flag == 1){
            exist++;
        }
        else if( flag == 2){
            crash++;
        }
        else {
            not_exist++;

        }
    }
    std::cout<< "exist: " << exist << std::endl
             << "crash: " << crash << std::endl
             << "not exist: " << not_exist << std::endl;
    std::cout << "New cache find average time is : " << time_total / 20000 << std::endl;

    not_exist = 0;
    exist = 0;
    crash = 0;
/////////////////////////////////////////////////////////////////////////////
///                              test old cache find                      ///
/////////////////////////////////////////////////////////////////////////////
    time_total = 0.0;
    for(i = 0; i < 20000; i++){
        mid_str = ecc_find[i];
        stat_time = get_time();
        flag = cac -> cache_find(ecc_find[i], block[i], 32);
        end_time = get_time();
        time_total += (end_time - stat_time) * 1000;
        if( flag == 1){
            exist++;
        }
        else if( flag == 2){
            crash++;
        }
        else {
            not_exist++;

        }
    }
    std::cout<< "exist: " << exist << std::endl
             << "crash: " << crash << std::endl
             << "not exist: " << not_exist << std::endl;
    std::cout << "Old cache find average time is : " << time_total / 20000 << std::endl;

    return 0;
}