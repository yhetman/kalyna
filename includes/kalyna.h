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

#define BUFFER_SIZE 64 * 1024;

typedef void 		(*function)(uint64_t *, uint64_t *, uint64_t *);
	
typedef struct 		s_kalyna
{
	size_t			state;
	size_t			rounds;
	size_t			double_block;
	size_t			key_length;
}					t_kalyna;



int				cipher(uint64_t *input, uint64_t *w, uint64_t *output);
void 			key_scheduler(uint64_t * key, uint64_t * w);

int 			get_options(int argc, char ** argv, FILE ** input, FILE ** output,
					uint64_t * key, bool * op, t_kalyna ** kalyna, bool *padding)
#endif
