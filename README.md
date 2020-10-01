# Segmented Paging
## Author
 Ilya Hontarau, Maria Zhirko
## Description
Is an implementation of segmented paging schema memory allocation(5 variant). Main memory is diveded into 
segment table, each row is a segment that point to page table, each page points to fixed size memory space, 
in which memory allocations of blocks are done.
NOTE: we are using old interface of this laba.

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
