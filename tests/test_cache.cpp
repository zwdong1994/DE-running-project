//
// Created by victor on 6/2/17.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "cache.h"

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

int main(){
    char *block[20000];
    char *ecc_code[10001];
    char *ecc_crash[100];
    int flag;
    int exist = 0;
    int not_exist = 0;
    int crash = 0;
    double time_total = 0.0, stat_time = 0.0, end_time = 0.0;
    cache *cac = cache::Get_cache();
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

/////////////////////////////////////////////////////////////////////////////
///                              test insert                              ///
/////////////////////////////////////////////////////////////////////////////
    char chunk[4097];
    for(i = 0; i < 10001; i++){
        random_string(chunk, 4096);
        chunk[4096] = '\0';
        cac -> cache_insert(ecc_code[i], chunk, 32);
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

/////////////////////////////////////////////////////////////////////////////
///                              test find                                ///
/////////////////////////////////////////////////////////////////////////////

    for(i = 0; i < 20000; i++){
        stat_time = get_time();
        flag = cac -> cache_find(ecc_crash[i%100], block[i], 32);
        end_time = get_time();
        std::cout<< (end_time - stat_time) * 1000 << "ms" << std::endl;
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

/*    for(i = 0; i < 1000; i++){
        flag = cac -> cache_find(ecc_crash[i%100], block[i], 32);
        if( flag == 1){
            exist++;
        }
        else if( flag == 2){
            crash++;
        }
        else {
            not_exist++;

        }
    }*/
    std::cout<< "exist: " << exist << std::endl
             << "crash: " << crash << std::endl
             << "not exist: " << not_exist << std::endl;
    return 0;
}