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


extern t_kalyna	*kalyna;

static void
xor_key(uint64_t * state, uint64_t *w)
{
	size_t		i;

    for (i = 0; i < kalyna->state; i++)
        state[i] ^= w[i];
}


static void
minus_key(uint64_t *state, uint64_t *w)	// INVERSE VERSION OF ADD KEY()
{
	size_t		i;

    for (i = 0; i < kalyna->state; i++)
        state[i] -= w[i];
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


int
cipher(uint64_t *input, uint64_t *w, uint64_t *output)
{
	size_t 		round;
	size_t 		rows_count;
    uint64_t 	*state;


    if (!(state = init_state(uint64_t *input, size_t rows_count)))
    	return 1;
    add_key(state, w);

  
    for (round = 1; round < kalyna->rounds; round++)
    {
        //sub_bytes(state);
        //shift_rows(state);
        //mix_columns(state);
        xor_key(state, w + round * kalyna->state);
    }
    //sub_bytes(state);
    //shift_rows(state);
    //mix_columns(state);
    
    add_key(state, w + kalyna->rounds * kalyna->state);
    
    memcpy(output, state, rows_count * kalyna->state);
    return 0;
}
