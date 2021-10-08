/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   get_options.c                                                            */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/10/08 11:27:20 by yhetman                                  */
/*   Updated: 2021/10/08 15:12:23 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

# include "kalyna.h"

extern t_kalyna     kalyna_128x128;
extern t_kalyna     kalyna_128x256;
extern t_kalyna     kalyna_256x256;
extern t_kalyna     kalyna_256x512;
extern t_kalyna     kalyna_512x512;

static int
ft_hex2dec(char hexademical, char *decimal) {
    if (decimal == NULL)
        return 0;

    if (hexademical >= '0' && hexademical <= '9') {
        *decimal = hexademical - '0';
    } else if (hexademical >= 'A' && hexademical <= 'F') {
        *decimal = hexademical - 'A' + 10;
    } else if (hexademical >= 'a' && hexademical <= 'f') {
        *decimal = hexademical - 'a' + 10;
    } else {
        return 0;
    }

    return 1;
}


static size_t
ft_hexbin(char *hexstr, unsigned char **binary) {
    size_t	len,
    		i;
    char 	b1,
    		b2;

    if (hexstr == NULL || *hexstr == '\0' || binary == NULL)
        return 0;

    len = strlen(hexstr);
    if (len % 2 != 0)
        return 0;
    len /= 2;

    memset(*binary, 'A', len);
    for (i = 0; i < len; i++)
    {
        if (!(ft_hex2dec(hexstr[i * 2], &b1)) || !(ft_hex2dec(hexstr[i * 2 + 1], &b2)))
            return 0;
        (*binary)[i] = (b1 << 4) | b2;
    }
    return len;
}


int
get_options(int argc, char ** argv, FILE ** input, FILE ** output,
	uint64_t * key, bool * op, t_kalyna ** kalyna, bool *padding)
{
    int 	options,
    		is_all;
    size_t 	key_length;
    
    is_all = 0;
    while ((options = getopt(argc, argv, "k:i:o:des:ph")) != -1)
    {
        switch (options)
        {
        	case 'k':
                key_length = ft_hexbin(optarg, (uint8_t **)&key);
                is_all++;
                break;
            case 'i':
                *input = fopen(optarg, "r");
                is_all++;
                break;
            case 'o':
                *output = fopen(optarg, "w+");
                is_all++;
                break;
            case 'd':
                *op = false;
                break;
            case 'e':
                *op = true;
                break;
            case 's':
                if (strcmp(optarg, "kalyna_128x128") == 0)
                   *kalyna = &kalyna_128x128;
                else if (strcmp(optarg, "kalyna_128x256") == 0)
                   *kalyna = &kalyna_128x256;
                else if (strcmp(optarg, "kalyna_256x256") == 0)
                   *kalyna = &kalyna_256x256;
                else if (strcmp(optarg, "kalyna_256x512") == 0)
                	*kalyna = &kalyna_256x512;
                else if (strcmp(optarg, "kalyna_512x512") == 0)
                    *kalyna = &kalyna_512x512;
                else
                {
                    fprintf(stderr, "%s wrong name, looks -h for supported kalynas.\n", optarg);
                    return -1;
                }
                break;
            case 'p':
                *padding = true;
                break;
            case 'h':
                printf("Usage:  ./kalyna -k key -i ./file_path -o ./file_path [optionss]\n\n\
Kalyna decryption\\encyption.\n\
Flags:\n\
  -k    Given key.\n\
  -i    Input file path.\n\
  -o    Output file path.\n\
  -d    Decrypt function.\n\
  -e    Encrypt function. Encrypt by default.\n\
  -s    Standart name. Allowed options: kalyna_128x128, kalyna_128x256,\n\
                        kalyna_256x256, kalyna_256x512, kalyna_512x512\n\
  -p    Use CBC padding.\n\
  -h    display this help.\n");

                return 1;

            case '?':
                fprintf(stderr, "%s unsuported flag.", optarg);
                return -2;
        }
    }
    if (is_all < 3)
    {
        fprintf(stderr, "Needs to satisfy all parameters, use -h for help.\n");
        return -3;
    }
    if (key_length != (*kalyna)->key_length * sizeof(uint64_t))
    {
        fprintf(stderr, "Key lenght(%lu) is not kalyna compliant(%lu).\n",
                key_length, (*kalyna)->key_length * sizeof(uint64_t));

        return -4;
    }

    return 0;
}
