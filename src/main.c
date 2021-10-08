/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.c                                                                   */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/10/03 10:47:14 by yhetman                                  */
/*   Updated: 2021/10/08 12:12:55 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "kalyna.h"


extern t_kalyna kalyna_128x128;
extern t_kalyna kalyna_128x256;
extern t_kalyna kalyna_256x256;
extern t_kalyna kalyna_256x512;
extern t_kalyna kalyna_512x512;


static size_t
remove_paddings(uint64_t * block, size_t double_block)
{
    uint64_t    pad_size;

    pad_size = block[double_block - 1];
    if (pad_size >= double_block)
        return 0;
    for (int i = double_block - 2; i >= (int)double_block - pad_size; i--)
        if (block[i] != pad_size)
            return 0;
    return pad_size;
}

static void
add_paddings(uint64_t * block, size_t size, size_t double_block)
{
    uint64_t    pad_size;
    size_t      i;

    if (size >= double_block)
        return;
    pad_size = double_block - size;
    for (i = size; i < double_block; i++)
        block[i] = pad_size;
}



int
main(int argc, char ** argv)
{
    FILE        *input,
                *output;
    uint64_t    key[8],
                *w,
                *reader,
                *writer;
    size_t      kalyna_chunk_size,
                reads,
                before_r,
                read_chunks,
                i;
    long        long;
    bool        op_ = true;
    bool        pads = false;
    function    op;
    t_kalyna    *kalyna = &kalyna_512x512;
    
    //if (get_args(argc, argv, &input, &output, key, &op_, &kalyna, &pads) != 0)
    //   return 1;
    // if (op_)
    op = &cipher;
    // else
    //     op = &decipher;

    setup(kalyna);
    w = malloc(sizeof(uint64_t) * kalyna->state_rows * (kalyna->rounds + 1));
    key_scheduler(key, w);
    reader = malloc(BUFFER_SIZE);
    writer = malloc(BUFFER_SIZE);
    kalyna_chunk_size = sizeof(uint64_t) * kalyna->double_block;
    before_r = 0;
    while ((reads = fread(reader, sizeof(byte), BUFFER_SIZE, input)) != 0)
    {
        read_chunks = reads / kalyna_chunk_size;
        for (i = 0; i < read_chunks; i++)
            op(reader + kalyna->double_block * i, writer + kalyna->double_block * i, w);
        if (reads % kalyna_chunk_size != 0)
        {
            if (!op_)
            {
                fprintf(stderr, "Invalid file.");
                return -1;
            }
            if (pads)
                add_paddings(reader + kalyna->double_block * read_chunks,
                          (size_t)((double)(reads % kalyna_chunk_size) / sizeof(dw) + 1),
                          kalyna->double_block);
                //reads = (read_chunks + 1) * kalyna_chunk_size;
            else
                memset(reader + kalyna->double_block * read_chunks + reads % kalyna_chunk_size,
                        0x0, kalyna_chunk_size - reads % kalyna_chunk_size);
            reads = (read_chunks + 1) * kalyna_chunk_size;
            op(reader + read_chunks * kalyna->double_block, writer + read_chunks * kalyna->double_block, w);
        }
        if (reads < BUFFER_SIZE && !op_ && pads)
            reads -= remove_paddings(writer + read_chunks - kalyna->double_block,
                                     kalyna->double_block);
        if (fwrite(writer, sizeof(byte), reads, output) == 0)
        {
            fprintf(stderr, "Error occured while writting to the output file.");
            return -1;
        }
        before_r = reads;
    }
    if (!op_ && before_r == BUFFER_SIZE && reads == 0 && pads)
    {
        fseek(output, 0L, SEEK_END);
        sz = ftell(output);
        sz -= remove_paddings(writer + before_r / kalyna_chunk_size - kalyna->double_block, kalyna->double_block);
        if (ftruncate(fileno(output), sz) != 0)
        {
            fprintf(stderr, "File truncate error.");
            return -3;
        }
    }

    free(reader);
    free(writer);
    free(w);
    fclose(input);
    fclose(output);

    return 0;
}


