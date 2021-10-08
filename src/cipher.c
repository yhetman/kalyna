/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cipher.c                                                                 */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/10/04 07:51:03 by yhetman                                  */
/*   Updated: 2021/10/08 08:00:05 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "kalyna.h"

extern t_kalyna	*kalyna;
extern const size_t rows_count;
extern const uint8_t s_box[4][16][16];
// extern uint8_t s_box[4][16][16];
extern uint8_t mix_columns_matrix[8][8];
extern uint8_t inverse_mix_columns_matrix[8][8];


static void
xor_key(uint64_t * state, uint64_t *w)
{
	size_t		i;

    for (i = 0; i < kalyna->state; i++)
        state[i] ^= w[i];
}


static void
add_key(uint64_t *state, uint64_t *w)
{
	size_t		i;

    for (i = 0; i < kalyna->state; i++)
        state[i] += w[i];
}


static uint64_t*
init_state(uint64_t *input, size_t n)
{
	uint64_t	*state;

	state = (uint64_t *) malloc(n * kalyna->state);
    memcpy(state, input, n * kalyna->state);
    return state;
}

static void
sub_bytes(uint64_t * state)
{
    size_t  column_num,
            row_num;
    uint8_t *columns_state; 

    for (column_num = 0; column_num < kalyna->state; column_num++)
    {
        columns_state = (uint8_t *)&state[column_num];
        for (row_num = 0; row_num < rows_count; row_num++)
            columns_state[row_num] = s_box[row_num % S_BOX_DIMENSION] \
            [((columns_state[row_num]&0xF0) >> 4)][(columns_state[row_num]&0x0F)];
    }
}


int
cipher(uint64_t *input, uint64_t *w, uint64_t *output)
{
	size_t 		round;
	// size_t 		rows_count;
    uint64_t 	*state;


    if (!(state = init_state(input, rows_count)))
    	return 1;
    add_key(state, w);
  
    for (round = 1; round < kalyna->rounds; round++)
    {
        sub_bytes(state);
        //shift_rows(state);
        //mix_columns(state, );
        xor_key(state, w + round * kalyna->state);
    }
    sub_bytes(state);
    //shift_rows(state);
    //mix_columns(state);
    
    add_key(state, w + kalyna->rounds * kalyna->state);
    
    memcpy(output, state, rows_count * kalyna->state);
    return 0;
}
