#include "combinatorics.h"

uint64_t factorial(uint64_t val) {

    uint64_t fact = 1;

    for(uint64_t count{2}; count<= val; count++){
        fact *= count;
    }

    return fact;
}


uint64_t permutation(uint64_t val, uint64_t val2) {
    if(val<val2)
        return 0;
    return factorial(val)/factorial(val-val2);
}


uint64_t combination(uint64_t val, uint64_t val2) {

    if(val<val2)
        return 0;

    uint64_t comb = factorial(val)/(factorial(val2)* factorial(val-val2));
    return comb;
}
