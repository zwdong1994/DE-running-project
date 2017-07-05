//
// Created by victor on 6/2/17.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "bloom_func.h"
#include "com_t.h"

void random_string(char str[], int length){
    int i;
    for(i = 0; i < length; i++){
        str[i] = 'a' + (rand() % 26);
    }
    str[length] = '\0';
}

int main(){
    bloom *bl = bloom::Get_bloom();
    char str1[1000][33];
    char str2[1000][33];
    double stat_t = 0.0;
    double end_t = 0.0;
    double result;
    cp_t cp_time;
    std::string str;
    int i;
    int exist_time = 0;
    srand((unsigned)time(NULL));
    for( i = 0; i < 1000; i++){
        random_string(str1[i], 32);
    }
    for( i = 0; i < 1000; i++){
        random_string(str2[i], 32);
    }


    std::cout.setf(std::ios::fixed);
/////////////////////////////////////////////////////////////////////////////
    stat_t = cp_time.get_time();
    for( i = 0; i < 1000; i++){
        str = str1[i];
        bl -> bloom_insert(str);
    }
    end_t = cp_time.get_time();
    result = 1000 * (end_t - stat_t) / 1000;
    std::cout<<"Insert average time: "<<result<<std::endl;

    stat_t = cp_time.get_time();
    for( i = 0; i < 1000; i++){
        str = str1[i];
        if(bl -> bloom_exist(str)) {
            std::cout << str << " exist!" << std::endl;
            exist_time++;
        }
        else
            std::cout<< str << " not exist!" << std::endl;
    }
    std::cout<<exist_time<<std::endl;
    end_t = cp_time.get_time();
    result = 1000 * (end_t - stat_t) / 1000;
    std::cout<<"Find average time: "<<result<<std::endl;
    exist_time = 0;
////////////////////////////////////////////////////////////////////////////
    stat_t = cp_time.get_time();
    for( i = 0; i < 1000; i++){
        str = str2[i];
        if(bl -> bloom_exist(str)) {
            std::cout << str << " exist!" << std::endl;
            exist_time++;
        }
        else
            std::cout<< str << " not exist!" << std::endl;
    }
    end_t = cp_time.get_time();
    result = 1000 * (end_t - stat_t) / 1000;
    std::cout<<"Find average time: "<<result<<std::endl;
    std::cout<<exist_time<<std::endl;
    return 0;
}
