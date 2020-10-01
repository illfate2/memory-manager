#include "memory/memory.h"
#include <stdlib.h>
#include "assert.h"
#include <string.h>

void test_str_alloc(char *data) {
    VA ptr = NULL;
    VA *ptr2 = &ptr;
    _malloc(ptr2, strlen(data));
    _write(*ptr2, data, strlen(data));

    char *resData = malloc(strlen(data));
    _read(*ptr2, resData, strlen(data));
    assert(strcmp(resData, data) == 0);

    _free(*ptr2);

    char *emptyData = malloc(strlen(data));
    int res_code = _read(*ptr2, emptyData, strlen(data));
    assert(res_code == ERR_BAD_PARAMS);
}

int main(int argc, char *argv[]) {
    int res = _init_(3, 10);
    assert(res == 0);
    test_str_alloc("masha");
    test_str_alloc("ilya");
    test_str_alloc("lol");
    test_str_alloc("kek");
    test_str_alloc("chebyrek");

    _clean();
    printf("success!");
    return 0;
}