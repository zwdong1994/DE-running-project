//
// Created by victor on 4/5/17.
//

#include <iostream>
#include <iomanip>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include "com_t.h"


cp_t::cp_t() {
    chunk_num = 0;
    time_aver = 0.0;
    time_total = 0.0;
}

cp_t::~cp_t() {
    if(time_total > 0) {
        std::cout.setf(std::ios::fixed);
        std::cout<<std::left<<std::setw(100)<<filename
                 <<std::left<<std::setw(30)<<time_total
                 <<std::left<<std::setw(30)<<chunk_num
                 <<std::left<<std::setw(8)<<std::setprecision(6)<<time_aver<<std::endl;
    }
}

double cp_t::get_time(void) {
    struct	timeval	mytime;
    gettimeofday(&mytime,NULL);
    return (mytime.tv_sec*1.0+mytime.tv_usec/1000000.0);
}

uint8_t cp_t::cp_all(double elps_time, int i){
    time_total += elps_time;
    chunk_num += i;
    return 1;
}

uint8_t cp_t::cp_aver(char *fn){
    strcpy(filename, fn);
    time_aver = time_total / chunk_num;
    return 1;
}

