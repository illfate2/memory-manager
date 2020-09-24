#include "memory.h"
#include <stdlib.h>
#include "assert.h"
#include <string.h>

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
    assert(strcmp(resData, data) == 0);

    _free(*ptr2);

    char *emptyData = malloc(6);
    int res_code = _read(*ptr2, emptyData, 6);
    assert(res_code == ERR_BAD_PARAMS);

    char *expEmpty = "";
    assert(strcmp(emptyData, expEmpty) == 0);

    _clean();
    printf("success!!!");
    return 0;
}