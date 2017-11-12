#include <stdio.h>
int calls;
int numused;
int numwins;
int overflow;
int underflow;
int max;
int cur;

void init (int wins){
    calls = 0;
    numused = 0;
    numwins = wins;
    overflow = 0;
    underflow = 0;
    cur = 0;
    max = 0;
}

void underFlow(){
    if (numused == 2){
        underflow++;
        cur--;
    }else{
        numused--;
    }
}
void overFlow(){
    if (numused >= numwins){
        overflow++;
        cur++;
        if (cur > max){
            max = cur;
        }
    }else{
        numused++;
    }
}

int ackermann(int x, int y){
    calls++;
    int result;
    overFlow();
    if (x == 0) {
        underFlow();
        return y+1; 
    } else if (y == 0) {
        result = ackermann (x-1, 1);
        underFlow();
        return result;
    } else {
        result = ackermann(x, y-1);
        result = ackermann(x-1, result);
        underFlow();
        return result;
    }
}

void test(int wins){
    init(wins);
    printf("WINDOWS: %d\n", wins);
    printf("\tvalues %d\n", ackermann(3,6));
    printf("\tcalls %d\n", calls);
    printf("\tover %d\n", overflow);
    printf("\tunder %d\n", underflow);
    printf("\tmax %d\n", max);
}

int main(){
    test(6);
    test(8);
    test(16);
    return 0;
}
