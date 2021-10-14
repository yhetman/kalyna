/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   decipher.c                                                               */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/10/06 12:42:38 by yhetman                                  */
/*   Updated: 2021/10/08 16:03:07 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "kalyna.h"

extern t_kalyna	        *kalyna;
extern uint8_t          inverse_mix_columns_matrix[8][8];
extern const uint8_t    inverse_s_box[4][16][16];
extern const size_t     rows_count;

static void
minus_key(uint64_t *state, uint64_t *w)	// INVERSE VERSION OF ADD KEY()
{
	size_t		i;

    for (i = 0; i < kalyna->state; i++)
        state[i] -= w[i];
}


static void
inverse_sub_bytes(uint64_t * state)
{
    size_t  column_num,
            row_num;
    uint8_t *columns_state; 

    for (column_num = 0; column_num < kalyna->state; column_num++)
    {
        columns_state = (uint8_t *)&state[column_num];
        for (row_num = 0; row_num < rows_count; row_num++)
            columns_state[row_num] = inverse_s_box[row_num % S_BOX_DIMENSION] \
            [((columns_state[row_num]&0xF0) >> 4)][(columns_state[row_num]&0x0F)];
    }
}


static void
inverse_shift_rows(uint64_t *state)
{
    size_t  i,
            j,
            k,
            shifts;
    uint8_t swap;

    for (i = 0; i < rows_count; i++)
    {
        shifts = (i * kalyna->double_block) / 8; 
        for (j = 0; j < shifts; j++)
        {
            swap = ((uint8_t *)&state[kalyna->state - 1])[i];
            for (k = 0; k < kalyna->state - 1; k++)
                ((uint8_t *)&state[k])[i] = ((uint8_t *)&state[k + 1])[i];
            ((uint8_t *)&state[kalyna->state - 1])[i] = swap;
        }
    }
}



int
decipher(uint64_t *input, uint64_t *w, uint64_t *output)
{
    uint64_t    *state;
    int         round;

    state = init_state(input);
    minus_key(state, w + kalyna->rounds * kalyna->state);

    round = kalyna->rounds - 1;
    for (; round > 0; round--)
    {
        mix_columns(state, inverse_mix_columns_matrix);
        inverse_shift_rows(state);
        inverse_sub_bytes(state);
        xor_key(state, w + round * kalyna->state);
    }
    mix_columns(state, inverse_mix_columns_matrix);
    inverse_shift_rows(state);
    inverse_sub_bytes(state);

    minus_key(state, w);

    memcpy(output, state, rows_count * kalyna->state);

    free(state);
    return 0;
}