/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   kalyna.h                                                                 */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/10/03 12:13:13 by yhetman                                  */
/*   Updated: 2021/10/05 12:13:15 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#ifndef KALYNA_H
# define KALYNA_H


# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct 		s_kalyna
{
	size_t	state;
	size_t	rows;
	size_t	doublesize_block;
	size_t	doublesize_key;
}					t_kalyna;



int				cipher(uint64_t *input, uint64_t *w, uint64_t *output);


#endif
