#include <stdio.h>
#include "../tempo.h"

int main(int argc, char* argv[])
{
    tempo_t<int> tmp(64); /* 64 object */

    tempo::set(tmp, 0, 10);
    printf("tmp.count: %d\n", tmp.count);

    tempo::push(tmp, 11);
    printf("tmp.count: %d\n", tmp.count);

    printf("tmp[last]: %d\n", tempo::pop(tmp));
    printf("tmp.count: %d\n", tmp.count);
    
    printf("tmp[last]: %d\n", tempo::pop(tmp));
    printf("tmp.count: %d\n", tmp.count);

    return 0;
}