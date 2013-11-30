#pragma once
#include <setjmp.h>
#include <string.h>
#include <stdio.h>

extern char ex_buf[];
extern jmp_buf ex_jbuf;
#define throw_jump() {strcat(ex_buf, " in" __FILE__ " on"); sprintf(ex_buf+strlen(ex_buf), " %d", __LINE__); longjmp(ex_jbuf, __LINE__);}
