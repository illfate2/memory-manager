//
// Created by illfate on 9/15/20.
//
// странично-сегментное разбиение

#include "memory.h"
#include <stdlib.h>
#include <string.h>

typedef struct block {
    void *data;
    size_t data_size; // todo
    size_t size; // todo
    struct block *next;
    size_t n; // todo
} block;

typedef struct page {
    size_t size;
    size_t reserved_size; // TODO
    block *block;
    struct page *next;
} page;


typedef struct segment {
    page *page;
    size_t n;
    struct segment *next;
} segment;

typedef struct {
    segment *segment;
    size_t n; //todo
} segment_table;

typedef struct {
    int num_of_pages;
    size_t page_size;
    segment_table *segment_table;
} memory;

static memory *mem;

int _malloc(VA *ptr, size_t szBlock) {
    // случай когда блок в первой странице и есть место
    // случай когда блоку не хватает место в первой странице и переходим на новый
    // случай когда не хватает места в сегменте и создаем новый
    segment *temp_segment = mem->segment_table->segment;
    for (size_t i = 0; i < mem->segment_table->n; ++i) {
        page *temp_page = temp_segment->page;
        for (size_t j = 0; j < mem->num_of_pages; j++) {
            if (temp_page->size - temp_page->reserved_size < szBlock) {
                temp_page = temp_page->next;
                continue;
            }
            // случай когда block первый
            if (temp_page->block == NULL) {
                temp_page->block = calloc(1, sizeof(block));
                temp_page->block->size = szBlock;
                *ptr = (VA) temp_page->block;
                return SUCCESS;
            }

            block *temp_block = temp_page->block;
            while (temp_block->next != NULL) { // идем до последнего
                temp_block = temp_block->next;
            }
            temp_page->block = calloc(1, sizeof(block));
            temp_page->block->size = szBlock;
            *ptr = (VA) temp_page->block;
            return SUCCESS;
        }
        temp_segment = temp_segment->next;
    }

    segment *new_segment = calloc(1, sizeof(segment));
    temp_segment->next = new_segment;
    mem->segment_table->n++;

    new_segment->page = calloc(1, sizeof(page));
    new_segment->n++;

    page *temp = mem->segment_table->segment->page;
    temp->size = mem->page_size;
    for (int i = 1; i < mem->num_of_pages; ++i) {
        page *next = calloc(1, sizeof(page));
        next->size = mem->page_size;
        temp->next = next;
        temp = temp->next;
        mem->segment_table->segment->n++;
    }
    return SUCCESS;
}


page *find_page_with_block(VA ptr) {
    segment *temp_segment = mem->segment_table->segment;
    for (size_t i = 0; i < mem->segment_table->n; ++i) {
        page *temp_page = temp_segment->page;
        for (size_t j = 0; j < mem->num_of_pages; j++) {
            block *temp_block = temp_page->block;
            if (temp_block == NULL) {
                temp_page = temp_page->next;
                continue;
            }
            while (strcmp((char *) temp_block, (char *) ptr) != 0) {
                temp_block = temp_block->next;
                if (temp_block == NULL) {
                    temp_page = temp_page->next;
                    continue;
                }
            }
            return temp_page;
        }
        temp_segment = temp_segment->next;
    }
    return NULL;
}

block *find_block(VA ptr) {
    page *page = find_page_with_block(ptr);
    if (page == NULL) {
        return NULL;
    }
    block *temp_block = page->block;
    while (strcmp((char *) temp_block, (char *) ptr) != 0) {
        temp_block = temp_block->next;
    }
    return temp_block;
}

int _free(VA ptr) {
    page *page = find_page_with_block(ptr);
    if (page == NULL) {
        return ERR_BAD_PARAMS;
    }
    block *cur_block = page->block;
    if (strcmp((char *) cur_block, (char *) ptr) == 0) {
        block *next_block = cur_block->next;
        page->block = next_block;
        free(cur_block);
        return SUCCESS;
    }
    block *prev_block = cur_block;
    cur_block = cur_block->next;
    while (strcmp((char *) cur_block, (char *) ptr) != 0) {
        cur_block = cur_block->next;
        prev_block = prev_block->next;
    }
    prev_block->next = cur_block->next;
    free(cur_block);
    return SUCCESS;
}

int _read(VA ptr, void *pBuffer, size_t szBuffer) {
    block *block = find_block(ptr);
    if (block == NULL) {
        return ERR_BAD_PARAMS;
    }
    if (block->size < szBuffer) {
        return ERR_BAD_PARAMS;
    }
    memcpy(pBuffer, block->data, szBuffer);
    return SUCCESS;
}


int _write(VA ptr, void *pBuffer, size_t szBuffer) {
    block *block = find_block(ptr);
    if (block == NULL) {
        return ERR_BAD_PARAMS;
    }
    if (block->size < szBuffer) {
        return ERR_BAD_PARAMS;
    }
    void *data = malloc(szBuffer);
    memcpy(data, pBuffer, szBuffer);
    block->data = data;
    block->data_size = szBuffer;
    return SUCCESS;
}


int _init_(int n, int szPage) {
    mem = calloc(1, sizeof(memory));
    if (mem == NULL) {
        return ERR_UNKNOWN;
    }
    mem->segment_table = calloc(1, sizeof(segment_table));
    if (mem->segment_table == NULL) {
        return ERR_UNKNOWN;
    }
    mem->segment_table->segment = calloc(1, sizeof(segment));
    if (mem->segment_table->segment == NULL) {
        return ERR_UNKNOWN;
    }
    mem->segment_table->n++;
    mem->page_size = szPage;

    mem->num_of_pages = n;

    mem->segment_table->segment->page = calloc(1, sizeof(page));
    mem->segment_table->segment->n++;

    page *temp = mem->segment_table->segment->page;
    temp->size = szPage;
    for (int i = 1; i < mem->num_of_pages; ++i) {
        page *next = calloc(1, sizeof(page));
        next->size = szPage;
        temp->next = next;
        temp = temp->next;
        mem->segment_table->segment->n++;
    }
    return SUCCESS;
}

// нужно ли освобождать выделенные блоки? клиент выделил - должен и удалить.
int _clean() {
    memory *m = mem; // TODO: remove

    segment *cur_segment = mem->segment_table->segment;
    segment *next_segment = mem->segment_table->segment;
    for (size_t i = 0; i < mem->segment_table->n; ++i) {
        page *cur_page = cur_segment->page;
        page *next_page = NULL;
        if (cur_page->next != NULL) {
            next_page = cur_segment->page->next;
        }
        for (int j = 0; j < mem->num_of_pages; ++j) {
            free(cur_page);
            cur_page = next_page;
            if (cur_page != NULL) {
                next_page = cur_page->next;
            }
        }
        free(cur_segment);
        cur_segment = next_segment;
        if (cur_segment != NULL) {
            next_segment = cur_segment->next;
        }
    }

    free(mem->segment_table);
    free(mem);
    return SUCCESS;
}

