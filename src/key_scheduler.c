/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   key_scheduler.c                                                          */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/10/07 10:29:20 by yhetman                                  */
/*   Updated: 2021/10/08 08:41:22 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */


#include "kalyna.h"



extern const size_t rows_count;



static void
key_expand(uint64_t * key, uint64_t * w, uint64_t * key_row, uint64_t * state)
{
	return ;
}


void
key_scheduler(uint64_t * key, uint64_t * w)
{
    uint64_t 	*key_row,
    			*state;

    state  = (uint64_t *) malloc(rows_count * kalyna->state);
    key_row = (uint64_t *) malloc(rows_count * kalyna->state);
    key_expand(key, w, key_row, state);

    free(key_row);
    free(state);
}
