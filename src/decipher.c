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

extern t_kalyna	*kalyna;

static void
minus_key(uint64_t *state, uint64_t *w)	// INVERSE VERSION OF ADD KEY()
{
	size_t		i;

    for (i = 0; i < kalyna->state; i++)
        state[i] -= w[i];
}