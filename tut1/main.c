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

int _g = 4; 
#define g (_g)

extern int _asm_min(int i, int j, int k);
extern int _asm_p(int i, int j, int k, int l);
extern int _asm_gcd(int a, int b);
#define min (_asm_min)
#define p (_asm_p)
#define gcd (_asm_gcd)

// ---- c implementation of functions to test against ----
int cMin(int a, int b, int c);
int cP(int i, int j, int k, int l);
int cGcd(int a, int b);

// ---- testing functiosn which compare c vs asm implementations against testcases and return number of fails ----
int testMin();
int testP();
int testGcd();

// ---- quickly check asm is working ----
void quickTest();

int main (){

    // run test cases
    int minFails = testMin();
    int pFails = testP();
    int gcdFails = testGcd();
    
    // print results
    printf(" ----- Total Failed ----- \n");
    printf("min:  %d\np:    %d\ngcd:  %d\n", minFails, pFails, gcdFails);

//     quickTest();

    return 0;
}

// ---- a quick test to see if the asm functions are working correctly ---- 
void quickTest(){
    printf(" ---- Running Quicktest ---- \n");
    int minv = min(10, 11, 13);
    printf("min(10, 11, 13): %d\n", minv);
    int pv = p(11, 12 , 13, 14);
    printf("p(11, 12, 13, 14) (g=4): %d\n", pv);
    int gcdv = gcd(49, 21);
    printf("gcd(49, 21): %d\n", gcdv);
}

// ---- C implementations for testing ----
int cMin (int a, int b, int c){
    int v = a;
    if (b < v){
        v = b;
    }
    if (c < v){
        v = c;
    }
    return v;
}

int cP (int i, int j, int k, int l){
    return cMin(cMin(g, i, j), k, l);
}

int cGcd (int a, int b){
    if (b == 0){
        return a;
    }else{
        return cGcd(b, a % b);
    }
}

// ---- Testing Functions ----

int testMin(){
    printf(" ----- Testing Min ----- \n");
    int testFailed = 0;
    int numTests = 13;
    int tests[13][3]={
        /*simple*/  { 1,2,3 },    { 3,2,1 },    { 2,1,3 },
        /*adv*/     { 12,13,14 },{ 14,13,12 },{ 12,12,13 },
        /*neg*/     { -1,-2,-3 },{ -3,-2,-1 },{ -2,-1,-3 },
        /*adv neg*/ { -2,3,4 },  { 2,-2,3 },  { 2,3,-2 },
        /*special*/ { 0,0,0 },};

    for (int i=0; i<numTests; i++){
        if (         min(tests[i][0], tests[i][1], tests[i][2]) !=  
                    cMin(tests[i][0], tests[i][1], tests[i][2]) ){
            printf("failed: min(%d, %d, %d)\n", tests[i][0], tests[i][1], tests[i][2]);
            testFailed++;
        }else{
            printf("passed: min(%d, %d, %d)\n", tests[i][0], tests[i][1], tests[i][2]);
        }
    }
    return testFailed; 
}

int testP(){
    printf(" ----- Testing P ( g = %d ) ----- \n", g);
    int testFailed = 0;
    int numTests = 13;
    int tests[13][4]={
        /*simple*/  { 1,2,3,4 },      { 4,3,2,1 },      { 2,1,4,3 },
        /*adv*/     { 12,15,13,14 }, { 14,13,15,12 }, { 12,15,9,13 },
        /*neg*/     { -1,-2,-3,-4 }, { -3,-2,-1,-4 }, { -2,-1,-3,-4 },
        /*adv neg*/ { -2,3,4,5 },    { 2,-2,3,5 },    { 2,3,-2,5 },
        /*special*/ { 0,0,0,0 }};

    for (int i=0; i<numTests; i++){
        if (         p(tests[i][0], tests[i][1], tests[i][2], tests[i][3]) !=
                    cP(tests[i][0], tests[i][1], tests[i][2], tests[i][3]) ){
            printf("failed: p(%d, %d, %d, %d)\n", tests[i][0], tests[i][1], tests[i][2], tests[i][3] );
            testFailed++;
        }else{
            printf("passed: p(%d, %d, %d, %d)\n", tests[i][0], tests[i][1], tests[i][2], tests[i][3] );
        }
    }
    return testFailed;
}

int testGcd(){
    printf(" ----- Testing Gcd ----- \n");
    int testFailed = 0;
    int numTests = 7;
    int tests[7][2]={
        /*simple*/      { 21, 14 },   {1406700, 164115 },   { 12, 6 },
        /*neg*/         { 21, -14 },  { -21,-14 },
        /*special*/     { 0,0 },      { 3,5 } };

    for (int i=0; i<numTests; i++){
        if (         gcd(tests[i][0], tests[i][1]) !=
                    cGcd(tests[i][0], tests[i][1]) ){
            printf("failed: min(%d, %d)\n", tests[i][0], tests[i][1]);
            testFailed++;
        }else{
            printf("passed: min(%d, %d)\n", tests[i][0], tests[i][1]);
        }
    }
    return testFailed;
}

