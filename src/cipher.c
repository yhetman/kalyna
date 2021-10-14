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
extern uint8_t mix_columns_matrix[8][8];

void
xor_key(uint64_t * state, uint64_t *w)
{
	size_t		i;

    for (i = 0; i < kalyna->state; i++)
        state[i] ^= w[i];
}


void
add_key(uint64_t *state, uint64_t *w)
{
	size_t		i;

    for (i = 0; i < kalyna->state; i++)
        state[i] += w[i];
}



void
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


static uint8_t
multiply_galois_fields(uint8_t a, uint8_t b)
{
    uint8_t result,
            hbit;
    size_t  i;

    result = 0;
    hbit = 0;
    for (i = 0; i < 8; i++)
    {
        if ((b & 0x1) == 1)
            result ^= a;
        hbit = a & 0x80;
        a <<= 1;
        if (hbit == 0x80)
            a ^= 0x011d;
        b >>= 1;
    }
    return result;
}



void
shift_rows(uint64_t *state)
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
            for (k = kalyna->state - 1; k > 0; k--)
                ((uint8_t *)&state[k])[i] = ((uint8_t *)&state[k - 1])[i];
            ((uint8_t *)&state[0])[i] = swap;
        }
    }
}


void
mix_columns(uint64_t *state, uint8_t matrix[8][8])
{   uint8_t     product;
    uint64_t    result;
    size_t      columns;
    int         rows,
                i;

    for (columns = 0; columns < kalyna->state; columns++)
    {
        result = 0;

        for (rows = rows_count - 1; rows >= 0; rows--)
        {
            product = 0;
            for (i = rows_count - 1; i >= 0; i--)
                product ^= multiply_galois_fields(((uint8_t *) &state[columns])[i], matrix[rows][i]);
            result |= (uint64_t) product << (rows * rows_count);
        }
        state[columns] = result;
    }

}


uint64_t*
init_state(uint64_t *input)
{
    uint64_t    *state;

    state = (uint64_t *) malloc(rows_count * kalyna->state);
    memcpy(state, input, rows_count * kalyna->state);
    return state;
}


void init(t_kalyna *k)
{
    kalyna = k;
}



int
cipher(uint64_t *input, uint64_t *w, uint64_t *output)
{
	size_t 		round;
    uint64_t 	*state;

    if (!(state = init_state(input)))
        return 1;

    add_key(state, w);
    for (round = 1; round < kalyna->rounds; round++)
    {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state, mix_columns_matrix);
        xor_key(state, w + round * kalyna->state);
    }
    sub_bytes(state);
    shift_rows(state);
    mix_columns(state, mix_columns_matrix);   
    add_key(state, w + kalyna->rounds * kalyna->state);
    
    memcpy(output, state, rows_count * kalyna->state);
    
    free(state);
    return 0;
}
