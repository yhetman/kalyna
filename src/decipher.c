/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   decipher.c                                                               */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/10/08 16:03:38 by yhetman                                  */
/*   Updated: 2021/10/08 16:03:39 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "kalyna.h"

extern t_kalyna	        *kalyna;
extern uint8_t          inverse_mix_columns_matrix[8][8];
extern const uint8_t    inverse_s_box[4][16][16];

static void
minus_key(uint64_t *state, uint64_t *w)	// INVERSE VERSION OF ADD KEY()
{
	size_t		i;

    for (i = 0; i < kalyna->state; i++)
        state[i] -= w[i];
}