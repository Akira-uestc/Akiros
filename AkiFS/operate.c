/*
 * Filename: operate.c
 * Author: Akira
 * Description: Support operation of my filesystem
 * Copyright (c) 2024 Akira. All Rights Reserved.
 */

#include <stdio.h>
#include <stddef.h>

const char* operator[] = {
    "create",
    NULL
};

void fs_create();


typedef void (*fs_func)() ;

struct 
{
    char* name;
    fs_func fs_func;
} func_table= 
{
    {"create",fs_create}
};

void fs_create()
{
    printf("Succeded step into fs_create\n");
    return;
}
