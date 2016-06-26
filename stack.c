/*
 * =====================================================================================
 *
 *       Filename:  stack.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/14/2014 10:35:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

static int stack_push(struct stack_info *info, 
		const void *pdata, size_t size)
{
	assert(info != NULL);
	assert(pdata != NULL);

	return info->pdlist->add(pdata, size, info->pdlist);
}

static int stack_is_empty(const struct stack_info *info)
{
	return info->pdlist->phead->next == info->pdlist->phead;
}

static int stack_top(struct stack_info *info,
		void *pbuf, size_t size)
{
	assert(info != NULL);
	assert(pbuf != NULL);
	
	if (stack_is_empty(info)) {
		fprintf(stderr, "stack top failed (stack is empty).\n");
		return -1;
	}

	memcpy(pbuf, info->pdlist->phead->next->buf, size);
	return 0;	
}


static int stack_pop(struct stack_info *info,
		void *pbuf, size_t size)
{
	if (stack_top(info, pbuf, size) < 0) {
		fprintf(stderr, "stack pop failed.\n");
		return -1;
	}
	
	return	info->pdlist->del(info->pdlist->phead->next);

}

int stack_init(struct stack_info *info)
{
	assert(info != NULL);
	
	info->pdlist = (struct dlist_info *)
		malloc(sizeof(struct dlist_info));
	
	if (info->pdlist == NULL) {
		perror("malloc");
		goto err;
	}

	dlist_init(info->pdlist);

	info->push = stack_push;
	info->top = stack_top;
	info->pop = stack_pop;
	info->is_empty = stack_is_empty;

	return 0;

err:
	return -1;
}

int stack_destroy(struct stack_info *info)
{
	dlist_destroy(info->pdlist);
	free(info->pdlist);

	return 0;
}
