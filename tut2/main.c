/*
 * created by Richard Lynch 5/oct/2017
 * 
 * Tutorial 1 - Computer Architecture 2 - CS4321
 * 
 * Build;
 * nasm -f elf -l "example.lst" "example.asm" //for all asm files ; outputs .o .lst files
 * gcc -m32 -o main main.c exampleASM.o // for all .c and .o files to be linked ; requires gcc-multilib(-m32)
 * 
 * Run;
 * ./main
 * 
 */

#include <stdio.h>
#include <stdint.h>
// #define INT64 int64_t c
int64_t _g = 4; 
extern int64_t _asm_min(int64_t i, int64_t j, int64_t k) __attribute__ ((ms_abi));
extern int64_t _asm_p(int64_t i, int64_t j, int64_t k, int64_t l) __attribute__ ((ms_abi));
extern int64_t _asm_gcd(int64_t a, int64_t b) __attribute__ ((ms_abi));
#define g (_g)
#define min (_asm_min)
#define p (_asm_p)
#define gcd (_asm_gcd)

// ---- c implementation of functions to test against ----
int64_t cMin(int64_t a, int64_t b, int64_t c);
int64_t cP(int64_t i, int64_t j, int64_t k, int64_t l);
int64_t cGcd(int64_t a, int64_t b);

// ---- testing functiosn which compare c vs asm implementations against testcases and return number of fails ----
int64_t testMin();
int64_t testP();
int64_t testGcd();

// ---- quickly check asm is working ----
void quickTest();

// ---- main body ---- 
int main (){

    // run test cases
    int64_t minFails = testMin();
    int64_t pFails = testP();
    int64_t gcdFails = testGcd();
    
    // print results
    printf(" ----- Total Failed ----- \n");
    printf("min:  %lld\np:    %lld\ngcd:  %lld\n", minFails, pFails, gcdFails);

//     quickTest();

    return 0;
}

// ---- a quick test to see if the asm functions are working correctly ---- 
void quickTest(){
    printf(" ---- Running Quicktest ---- \n");
    int64_t minv = min(10, 11, 13);
    printf("min(10, 11, 13): %lld\n", minv);
    int64_t pv = p(11, 12 , 13, 14);
    printf("p(11, 12, 13, 14) (g=4): %lld\n", pv);
    int64_t gcdv = gcd(49, 21);
    printf("gcd(49, 21): %lld\n", gcdv);
}

// ---- C implementations for testing ----
// ---- min ----
int64_t cMin (int64_t a, int64_t b, int64_t c){
    int64_t v = a;
    if (b < v){
        v = b;
    }
    if (c < v){
        v = c;
    }
    return v;
}
// ---- p ----
int64_t cP (int64_t i, int64_t j, int64_t k, int64_t l){
    return cMin(cMin(g, i, j), k, l);
}
// ---- gcd ----
int64_t cGcd (int64_t a, int64_t b){
    if (b == 0){
        return a;
    }else{
        return cGcd(b, a % b);
    }
}
int64_t cq(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e){
    int64_t sum = a + b + c + d + e;
    printf("a = %lld b = %lld c = %lld d = %lld e = %lld sum = %lld\n", a, b, c, d, e, sum);
    return sum;
}

// ---- Testing min ----
int64_t testMin(){
    printf(" ----- Testing Min ----- \n");
    int64_t testFailed = 0;
    int64_t numTests = 13;
    int64_t tests[13][3]={
    /*simple*/  {  1,  2,  3 }, {  3,  2,  1 }, {  2,  1,  3 },
    /*adv*/     { 12, 13, 14 }, { 14, 13, 12 }, { 12, 12, 13 },
    /*neg*/     { -1, -2, -3 }, { -3, -2, -1 }, { -2, -1, -3 },
    /*adv neg*/ { -2,  3,  4 }, {  2, -2,  3 }, {  2,  3, -2 },
    /*special*/ {  0,  0,  0 }};

    for (int64_t i=0; i<numTests; i++){
        int64_t a =  min(tests[i][0], tests[i][1], tests[i][2]);
        int64_t c = cMin(tests[i][0], tests[i][1], tests[i][2]); 
        if ( a != c ){
            printf("failed: min(%lld, %lld, %lld)\n", 
                    tests[i][0], tests[i][1], tests[i][2]);
            printf("a: (%lld) c:(%lld)\n", a, c);
            testFailed++;
        }else{
            printf("passed: min(%lld, %lld, %lld)\n", 
                    tests[i][0], tests[i][1], tests[i][2]);
        }
    }
    return testFailed; 
}

// ---- Testing p ----
int64_t testP(){
    printf(" ----- Testing P ( g = %lld ) ----- \n", g);
    int64_t testFailed = 0;
    int64_t numTests = 13;
    int64_t tests[13][4]={
    /*simple*/  {  1,  2,  3,  4 }, {  4,  3,  2,  1 }, {  2,  1,  4,  3 },
    /*adv*/     { 12, 15, 13, 14 }, { 14, 13, 15, 12 }, { 12, 15,  9, 13 },
    /*neg*/     { -1, -2, -3, -4 }, { -3, -2, -1, -4 }, { -2, -1, -3, -4 },
    /*adv neg*/ { -2,  3,  4,  5 }, {  2, -2,  3,  5 }, {  2,  3, -2,  5 },
    /*special*/ {  0,  0,  0,  0 }};
    
    for (int64_t i=0; i<numTests; i++){
        int64_t a =  p(tests[i][0], tests[i][1], tests[i][2], tests[i][3]); 
        int64_t c = cP(tests[i][0], tests[i][1], tests[i][2], tests[i][3]);
        if ( a != c){
            printf("failed: p(%lld, %lld, %lld, %lld)\n", 
                    tests[i][0], tests[i][1], tests[i][2], tests[i][3] );
            printf("a: (%lld) c:(%lld)\n", a, c);
            testFailed++;
        }else{
            printf("passed: p(%lld, %lld, %lld, %lld)\n", 
                    tests[i][0], tests[i][1], tests[i][2], tests[i][3] );
        }
    }
    return testFailed;
}

// ---- Testing gcd ----
int64_t testGcd(){
    printf(" ----- Testing Gcd ----- \n");
    int64_t testFailed = 0;
    int64_t numTests = 6;
    int64_t tests[6][2]={
    /*simple*/      { 21,  14 },   { 1406700, 164115 }, { 12, 6 },
    /*neg/special*/ { 21, -14 },   {     -21,    -14 }, {  0, 0 }};

    for (int64_t i=0; i<numTests; i++){
        int64_t a = gcd(tests[i][0], tests[i][1]);
        int64_t c = cGcd(tests[i][0], tests[i][1]); 
        if ( a != c ){
            printf("failed: min(%lld, %lld)\n", 
                    tests[i][0], tests[i][1]);
            printf("a: (%lld) c:(%lld)\n", a, c);
            testFailed++;
        }else{
            printf("passed: min(%lld, %lld)\n", 
                    tests[i][0], tests[i][1]);
        }
    }
    return testFailed;
}

