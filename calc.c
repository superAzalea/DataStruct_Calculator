/*
 * =====================================================================================
 *
 *       Filename:  calc.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created: 6/26/2016 17:23:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "calc.h"

static  int num_handler(struct calc_info *info)
{
	assert(info != NULL);
	
	num_t nu = *info->cur - '0'; 

	if (info->flag == F_OPR) {
		info->flag = F_NUM;
	} else if (info->flag == F_NUM) {
		num_t top_n;
		if (NUM_POP(top_n) < 0) {
			fprintf(stderr, "Error:pop failed.\n");
			goto err_num_pop;
		}

		nu += 10 * top_n;
	}
	
	NUM_PUSH(nu);

	return 0;
err_num_pop:
	return -1;
}

static  int opr2level(char opr)
{
	int level = 1;
	
	switch (opr) {
		case '+':
		case '-':
			level += 1;
			break;
		case '*':
		case '/':
		case '%':
			level += 2;
			break;
		case '(':
			break;
		default:
			fprintf(stderr,"Error: Invlid opr \'\%c'\n", opr);
			break;
	}	

	return level;
}

static  int level_cmp(char a, char b)
{
	return  opr2level(a) - opr2level(b);
}

static int  do_pop_stack(struct calc_info *info)
{
	//1.数字栈取两次
	num_t a,b,ret;
	char c;
	
	if (! NUM_IS_EMPTY) {
		NUM_POP(b);
	} else {
		goto err;
	}
	
	if (! NUM_IS_EMPTY) {
		NUM_POP(a);
	} else {
		goto err;
	}

	//2.运算符栈取一次
	if (! OPR_IS_EMPTY) {
		OPR_POP(c);
	} else {
		goto err;
	}

	//3.根据运算符计算，得到结果
	switch (c) {
		case '+':
			ret = a + b;
			break;
		case '-':
			ret = a - b;
			break;
		case '*':
			ret = a * b;
			break;
		case '/':
			ret = a / b;
			break;
		case '%':
			ret = a % b;
			break;
		default:
			break;
	}
	//4.把结果再压入数字栈
	
	NUM_PUSH(ret);
#ifdef _DEBUG
	printf("%d %c %d = %d\n", a, c, b, ret);
#endif

	return 0;
err:
	return -1;
}

static  int opr_handler(struct calc_info *info)
{
	assert(info != NULL);

	char c = *info->cur;

	if (info->flag == F_NUM) {
		info->flag = F_OPR;
	} else if (info->flag == F_OPR) {
		//TODO 出现多个连续的运算符
		if (c == '-') {
			//负号
			num_t  zero = 0;
			NUM_PUSH(zero);
			OPR_PUSH(c);
			return 0;
		} else if (c == '+') {
			//正号
			return 0;
		} else {
			//其他运算符
			fprintf(stderr, "Error: 不支持多位运算符\n");
			return -1;
		}
	}
	

	//1. 入栈(空栈，优先级高于栈顶运算符)，否则出栈（数字栈出两次，运算符栈出一次）

	char top_c;
	
	while (1) {
		//1. 运算符栈为空，无条件压栈
		if (OPR_IS_EMPTY) {
			OPR_PUSH(c);
			return 0;
		}
		
		OPR_TOP(top_c);
		//2. 运算符优先级高于栈顶，压栈
		if (level_cmp(c, top_c) > 0) {
			OPR_PUSH(c);
			return 0;
		} else { //3.否则就出栈
			do_pop_stack(info);
		}
	}

}

static int bracket_handler(struct calc_info *info)
{
	char c = *info->cur;
	
	if (c == '(') {
		OPR_PUSH(c);
	} else if (c == ')') {
		while (OPR_TOP(c) == 0) {
			if (c == '(') {
				OPR_POP(c);
				return 0;
			} else {
				do_pop_stack(info);
			}
		}
		
		fprintf(stderr, "Error: Miss a left bracket\n");
		return -1;	
	}

	return 0;
}


static  num_t calc_parser(struct  calc_info *info,
				const char *expr)
{
	assert(info != NULL);

	if (expr == NULL) {
		return 0;
	}

	num_t ret = 0;
	info->cur = expr;

	for (; *info->cur != '\0'; ++info->cur) {
		switch (*info->cur) {
			case '0' ... '9':
				if (num_handler(info) < 0) {
					fprintf(stderr, "Error: num handler failed.\n");
					goto err_num_handler;
				}
				break;
			case '+':
			case '-':
			case '*':
			case '/':
			case '%':
				if (opr_handler(info) < 0) {
					fprintf(stderr, "Error: opr handler failed.\n");
					goto err_opr_handler;
				}
				break;
			case ' ':
			case '\t':
			case '\n':
				//ignore
				break;
			case '(':
			case ')':
				if (bracket_handler(info) < 0) {
				fprintf(stderr, "Error: bracket_handler failed.\n");
				goto err_bracket_handler;
	}
				break;
			default:
					fprintf(stderr, "Error: \'%c\' is invalid char.\n", *info->cur);
				goto err_invalid_char;
				break;
		}
	}

//字符串表达式解析完成，但是运算符栈中，依然有数据
	char top_c ;
	while (! OPR_IS_EMPTY) {
		OPR_TOP(top_c);
		if (top_c == '(') {
			fprintf(stderr, "Error: Miss Right brakcet\n");
			goto err_miss_right_bracket;
		}
		do_pop_stack(info);
	}
	
	NUM_POP(ret);

	return ret;
err_miss_right_bracket:
err_num_handler:
err_opr_handler:
err_bracket_handler:
err_invalid_char:
	return -1;
}


int calc_init(struct calc_info *info)
{
	assert(info != NULL);
	
	info->num = (struct stack_info *)
		malloc(sizeof(struct stack_info));
	info->opr = (struct stack_info *)
		malloc(sizeof(struct stack_info));
	
	if (info->num == NULL) {
		perror("malloc");
		goto  err_malloc;
	}

	if (info->opr == NULL) {
		perror("malloc");
		goto  err_malloc2;
	}

	int iret = 0;

	iret = stack_init(info->num);
	if (iret < 0) {
		fprintf(stderr, "num_stack_init failed.\n");
		goto err_init;
	}
	
	iret = stack_init(info->opr);
	if (iret < 0) {
		fprintf(stderr, "opr_stack_init failed.\n");
		goto err_init;
	}
	//默认 flag是运算符
	info->flag = F_OPR;

	info->cur = NULL;
	info->parser = calc_parser;

	return 0;

err_init:
	free(info->opr);
err_malloc2:
	free(info->num);
err_malloc:
	return -1;
}

int calc_destroy(struct calc_info *info)
{
	stack_destroy(info->num);
	stack_destroy(info->opr);
	
	free(info->num);
	free(info->opr);

	return 0;
}




