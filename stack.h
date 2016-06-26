/*
 * =====================================================================================
 *
 *       Filename:  stack.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/26/2016 15:31:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: Azalea, 
 *   Organization:  
 *
 * =====================================================================================
 */
#pragma once
#include "dlist.h"

struct  stack_info {
	struct dlist_info *pdlist;
	int (*push)(struct stack_info *info, const void *pdata, size_t size);
	int (*top)(struct stack_info *info, void *pbuf, size_t size);
	int (*pop)(struct stack_info *info, void *pbuf, size_t size);
	int (*is_empty)(const struct stack_info *info);
};

int stack_init(struct stack_info *info);
int stack_destroy(struct stack_info *info);



