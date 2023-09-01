#include <stdint.h>
#include "math.h"

uint32_t max(uint32_t a , uint32_t b){
    if(a > b){
        return a;
    }else{
        return b;
    }
}
uint32_t min(uint32_t a , uint32_t b){
    if(a < b){
        return a;
    }else{
        return b;
    }
}