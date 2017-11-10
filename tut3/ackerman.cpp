#include <stdio.h>

int ackermann(int x, int y) {
    if (x == 0) {
        return y+1; 
    } else if (y == 0) {
        return ackermann(x-1, 1);
    } else {
        return ackermann(x-1, ackermann(x, y-1));
}

int main () {
    prift("values %d", ackerman(3,6))
    return 0
}
