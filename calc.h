/*
 * =====================================================================================
 *
 *       Filename:  calc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  6/26/2016 17:19:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea, 
 *   Organization:  
 *
 * =====================================================================================
 */
#pragma once

#include "stack.h"

typedef  int num_t;

enum  {
	FALSE, //4 = 0
	TRUE,  //4 0 + 1 = 1
};

enum  FLAG {
	F_OPR, //0
	F_NUM, //1
};


struct calc_info {
	struct stack_info *num;
	struct stack_info *opr;
	const char *cur;
	enum  FLAG  flag; //用来记住前一个字符的类型(0 运算符 1 数字)
	num_t (*parser)(struct calc_info *info, 
				const char *expr);	
};

int calc_init(struct calc_info *info);
int calc_destroy(struct calc_info *info);

#define   NUM_PUSH(n)	\
	info->num->push(info->num, &(n), sizeof(n))
#define   NUM_TOP(n)	\
	info->num->top(info->num, &(n), sizeof(n))
#define   NUM_POP(n)	\
	info->num->pop(info->num, &(n), sizeof(n))
#define   NUM_IS_EMPTY \
	info->num->is_empty(info->num)

#define   OPR_PUSH(n)   \
        info->opr->push(info->opr, &(n), sizeof(n))
#define   OPR_TOP(n)    \
        info->opr->top(info->opr, &(n), sizeof(n))
#define   OPR_POP(n)    \
        info->opr->pop(info->opr, &(n), sizeof(n))
#define   OPR_IS_EMPTY \
        info->opr->is_empty(info->opr)


