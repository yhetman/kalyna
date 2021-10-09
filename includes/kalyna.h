/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   kalyna.h                                                                 */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/10/03 12:13:13 by yhetman                                  */
/*   Updated: 2021/10/05 08:48:15 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#ifndef KALYNA_H
# define KALYNA_H


# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>


# define S_BOX_DIMENSION 4
# define BUFFER_SIZE  (64 * 1024)

typedef int 		(*function)(uint64_t *, uint64_t *, uint64_t *);
	
typedef struct 		s_kalyna
{
	size_t			state;
	size_t			rounds;
	size_t			double_block;
	size_t			key_length;
}					t_kalyna;


void				xor_key(uint64_t * state, uint64_t *w);
void				add_key(uint64_t *state, uint64_t *w);
void				sub_bytes(uint64_t * state);
void				shift_rows(uint64_t *state);
void				mix_columns(uint64_t *state, uint8_t matrix[8][8]);
void 				key_scheduler(uint64_t * key, uint64_t * w);

int					cipher(uint64_t *input, uint64_t *w, uint64_t *output);
int 				get_options(int argc, char ** argv, FILE ** input, FILE ** output,
					uint64_t * key, bool * op, t_kalyna ** kalyna, bool *padding);
#endif
