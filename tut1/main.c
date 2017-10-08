#include <stdio.h>

int _g = 10; 
extern int _asm_min(int i, int j, int k);
extern int _asm_p(int i, int j, int k, int l);
extern int _asm_gcd(int a, int b);
#define min (_asm_min)
#define p (_asm_p)
#define gcd (_asm_gcd)

int check(int asmAns, int stdAns);
int testMin();
int testP();
int testGcd();

int main (){

    int minFails = testMin();
    int pFails = testP();
    int gcdFails = testGcd();
    printf(" ----- Total Failed ----- \n");
    printf("min: %d\np: %d\ngcd: %d\n", minFails, pFails, gcdFails);

    return 0;
}

void quickTest(){
    int minv = min(10, 11, 13);
    printf("Min: %d\n", minv);
    int pv = p(11, 12 , 13, 14);
    printf("p: %d\n", pv);
    int gcdv = gcd(49, 21);
    printf("gcd: %d\n", gcdv);
}


int check (int asmAns, int stdAns){
    if (asmAns == stdAns ){
        return 1;
    }else{
        return 0; 
    }
}

int testMin(){
    printf(" ----- Testing Min ----- \n");
    int testFailed = 0;
    int intentialFails = 0;
    int numTests = 14;
    int tests[14][4]={
        /*simple*/  { 1,2,3,1 },    { 3,2,1,1 },    { 2,1,3,1 },
        /*adv*/     { 12,13,14,12 },{ 14,13,12,12 },{ 12,12,13,12 },
        /*neg*/     { -1,-2,-3,-3 },{ -3,-2,-1,-3 },{ -2,-1,-3,-3 },
        /*adv neg*/ { -2,3,4,-2 },  { 2,-2,3,-2 },  { 2,3,-2,-2 },
        /*special*/ { 0,0,0,0 },    { 1,2,3,-1}}; // intential fail
    intentialFails++;

    for (int i=0; i<numTests; i++){
        if ( check (min(tests[i][0], tests[i][1], tests[i][2]), tests[i][3] ) == 0 ){
            printf("failed: min(%d, %d, %d)\n", tests[i][0], tests[i][1], tests[i][2]);
            testFailed++;
        }else{
            printf("passed: min(%d, %d, %d)\n", tests[i][0], tests[i][1], tests[i][2]);
        }
    }
    return testFailed - intentialFails;
}

int testP(){
    printf(" ----- Testing P ----- \n");
    int testFailed = 0;
    int intentialFails = 0;
    int numTests = 14;
    int tests[14][5]={
        /*simple*/  { 1,2,3,4,1 },      { 4,3,2,1,1 },      { 2,1,4,3,1 },
        /*adv*/     { 12,15,13,14,10 }, { 14,13,15,12,10 }, { 12,15,9,13,9 },
        /*neg*/     { -1,-2,-3,-4,-4 }, { -3,-2,-1,-4,-4 }, { -2,-1,-3,-4,-4 },
        /*adv neg*/ { -2,3,4,5,-2 },    { 2,-2,3,5,-2 },    { 2,3,-2,5,-2 },
        /*special*/ { 0,0,0,0 },        { 1,2,3,5,-1}}; // intential fail
    intentialFails++;

    for (int i=0; i<numTests; i++){
        if ( check (p(tests[i][0], tests[i][1], tests[i][2], tests[i][3]), tests[i][4] ) == 0 ){
            printf("failed: p(%d, %d, %d, %d)\n", tests[i][0], tests[i][1], tests[i][2], tests[i][3] );
            testFailed++;
        }else{
            printf("passed: p(%d, %d, %d, %d)\n", tests[i][0], tests[i][1], tests[i][2], tests[i][3] );
        }
    }
    return testFailed - intentialFails;
}

int testGcd(){
    printf(" ----- Testing Gcd ----- \n");
    int testFailed = 0;
    int intentialFails = 0;
    int numTests = 8;
    int tests[8][3]={
        /*simple*/      { 21, 14, 7},   {1406700, 164115, 23445},   { 12, 6, 6 },
        /*neg*/         { 21, -14, 7},  { -21,-14, -7 },
        /*special*/     { 0,0,0 },      { 3,5,1},                   {4,2,-1}}; // intential fail
    intentialFails++;

    for (int i=0; i<numTests; i++){
        if ( check (gcd(tests[i][0], tests[i][1]), tests[i][2] ) == 0 ){
            printf("failed: min(%d, %d)\n", tests[i][0], tests[i][1]);
            testFailed++;
        }else{
            printf("passed: min(%d, %d)\n", tests[i][0], tests[i][1]);
        }
    }
    return testFailed - intentialFails;
}

