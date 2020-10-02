# Segmented Paging
## Author
 Ilya Hontarau, Maria Zhirko
## Description
Is an implementation of segmented paging memory allocation schema(5-th variant). Main memory is diveded into 
segment table, each row is a segment that points to the page table, each page points to fixed size memory space, 
in which memory allocations of blocks are done.\
**_NOTE: we are using the old interface of this lab._**

### Generate Makefile:
```bash
cmake .
```
### Build project:
```bash
make
```

### Run project:
```bash
./bin
```
