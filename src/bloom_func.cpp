//
// Created by victor on 5/22/17.
//

#include "bloom_func.h"
#include <iostream>
#include <string>
#include <iomanip>

bloom::bloom() {
    init_bloom(10000000, 0.000001);
}

bloom::~bloom() {

}

bloom* bloom::bloom_instance = NULL;

bloom* bloom::Get_bloom() {
    static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
    if(bloom_instance == NULL){
        pthread_mutex_lock(&mu);
        if(bloom_instance == NULL){
            bloom_instance = new bloom();
        }
        pthread_mutex_unlock(&mu);
    }
    return bloom_instance;
}

int bloom::init_bloom(unsigned long long element_count, double false_positive_prob) {
    para.projected_element_count = element_count;
    para.false_positive_probability = false_positive_prob;
    para.random_seed = 0xA5A5A5A5;

    if(!para){
        std::cout<<"Invalid bloom filter parameters!"<<std::endl;
        return -1;
    }
    para.compute_optimal_parameters();  //parameter computation
    bloom_filter filter(para);
    bloom_fil_instance = filter;
    return 1;
}

int bloom::bloom_insert(std::string add_str){
   bloom_fil_instance.insert(add_str);
   return 1;
}

bool bloom::bloom_exist(std::string com_str){
   if (bloom_fil_instance.contains(com_str)) {
      return 1;
   }
   else{
       bloom_insert(com_str);
       return 0;
   }
}
