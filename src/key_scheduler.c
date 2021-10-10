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

extern t_kalyna     *kalyna;
extern const size_t rows_count;
extern uint8_t      mix_columns_matrix[8][8];

static void
cipher_round_with_add_key(uint64_t *state, uint64_t *key)
{
    add_key(state, key);
    sub_bytes(state);
    shift_rows(state);
    mix_columns(state, mix_columns_matrix);

}


static void
cipher_round_with_xor_key(uint64_t *state, uint64_t *key)
{
    xor_key(state, key);
    sub_bytes(state);
    shift_rows(state);
    mix_columns(state, mix_columns_matrix);
}




static void
rotation(uint64_t *values, size_t size)
{
    uint64_t    temp;
    size_t      i;

    temp = values[0];
    for (i = 1; i < size; i++)
        values[i - 1] = values[i];
    values[size - 1] = temp;
}


static void
odd_key(uint64_t *w)
{
    size_t  i,
            to_rotate,
            bytes_amount;
    uint8_t *bytes,
            *buffer;

    for (i = 1; i < kalyna->rounds; i += 2)
    {
        memcpy(w + i * kalyna->state, w + (i - 1) * kalyna->state, kalyna->state * rows_count);

        to_rotate = 2 * kalyna->state + 3;
        bytes_amount = kalyna->state * 8;        
        bytes = (uint8_t*)(w + i * kalyna->state);
        buffer = (uint8_t*)malloc(to_rotate);

        memcpy(buffer, bytes, to_rotate);
        memmove(bytes, bytes + to_rotate, bytes_amount - to_rotate);
        memcpy(bytes + bytes_amount - to_rotate, buffer, to_rotate);

        free(buffer);
    }
}

static void
memcpy_with_adding_key(uint64_t *temp, uint64_t * key_row,
    uint64_t * state, uint64_t *initial, uint64_t *w,
    uint64_t *key_round, size_t round)
{
    memcpy(state, key_row, kalyna->state * rows_count);
    add_key(state, temp);
    memcpy(key_round, state, kalyna->state * rows_count);

    memcpy(state, initial, kalyna->state * rows_count);
    
    cipher_round_with_add_key(state, key_round);
    cipher_round_with_xor_key(state, key_round);
    add_key(state, key_round);
    memcpy(w + round * kalyna->state, state, kalyna->state * rows_count);
}


static void
even_key(uint64_t * key, uint64_t * w, uint64_t * key_row, uint64_t * state)
{
    uint64_t    *key_round,
                *temp,
                *init;
    size_t      i,
                j,
                k,
                round;

    round = 0;
    key_round = malloc(kalyna->state * rows_count);
    temp = malloc(kalyna->state * rows_count);
    init = malloc(kalyna->key_length * rows_count);

    memcpy(init, key, kalyna->key_length * rows_count);
    for (i = 0; i < kalyna->state; i++)
        temp[i] = 0x0001000100010001;
    while (1)
    {
        memcpy_with_adding_key(temp, key_row, state, init, w, key_round, round);
        if (round == kalyna->rounds)
            break;
        if (kalyna->key_length != kalyna->double_block)
        {
            round +=2;
            for (j = 0; j < kalyna->state; j++)
                temp[j] <<=1;
            memcpy_with_adding_key(temp, key_row, state, init + kalyna->state, w, key_round, round);
            
            if (kalyna->rounds == round)
                break;
        }
        round +=2;
        for (k = 0; k < kalyna->state; k++)
            temp[k] <<=1;
        rotation(init, kalyna->key_length);
    }
    free(init);
    free(key_round);
    free(temp);
}


static void
key_expand(uint64_t * key, uint64_t * key_row, uint64_t * state)
{
    uint64_t    *key_0,
                *key_1;

    key_0 = malloc(rows_count * kalyna->state);
    key_1 = malloc(rows_count * kalyna->state);
    
    memset(state, 0, rows_count * kalyna->state);

    ((uint8_t *)state)[0] = kalyna->key_length + kalyna->state + 1; 
    
    if (kalyna->key_length == kalyna->state)
    {
        key_0 = memcpy(key_0, key, rows_count * kalyna->state);
        key_1 = memcpy(key_1, key, rows_count * kalyna->state);
    }
    else
    {
        key_0 = memcpy(key_0, key, rows_count * kalyna->state);
        key_1 = memcpy(key_1, key + kalyna->state, rows_count * kalyna->state);
    }
    cipher_round_with_add_key(state, key_0);
    cipher_round_with_xor_key(state, key_1);
    cipher_round_with_add_key(state, key_0);

    memcpy(key_row, state, rows_count * kalyna->state);

    free(key_0);
    free(key_1);
}



void
key_scheduler(uint64_t * key, uint64_t * w)
{
    uint64_t 	*key_row,
    			*state;


    state  = (uint64_t *) malloc(rows_count * kalyna->state);
    key_row = (uint64_t *) malloc(rows_count * kalyna->state);
    key_expand(key, key_row, state);
    even_key(key, w, key_row, state);
    odd_key(w);
    free(key_row);
    free(state);
}
