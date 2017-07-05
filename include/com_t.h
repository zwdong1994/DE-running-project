//
// Created by victor on 4/5/17.
//

#ifndef PRE_ECC_TEST_COM_T_H
#define PRE_ECC_TEST_COM_T_H



#include "tpdef.h"

class cp_t{


public:
    double get_time(void);
    uint8_t cp_all(double elps_time, int i);
    uint8_t cp_aver(char *fn);

    cp_t();
    ~cp_t();
public:
    uint64_t chunk_num;
    double time_total;
    double time_aver;
    char filename[256];

};
#endif //PRE_ECC_TEST_COM_T_H