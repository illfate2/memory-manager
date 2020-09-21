#include "memory.h"
#include <stdlib.h>
#include "assert.h"

int main(int argc, char *argv[]) {
    int res = _init_(3, 10);
    assert(res == 0);
    VA ptr = NULL;
    VA *ptr2 = &ptr;
    _malloc(ptr2, 10);
    char *data = "masha";
    _write(*ptr2, data, 6);

    char *resData = malloc(6);
    _read(*ptr2, resData, 6);
    printf("%s", resData);
    _free(*ptr2);
    _clean();
    return 0;
}