/*

	plainhext
	commented hex -> bin utility

	shygoo 2017
	
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "plainhext.h"

int parser_load(parser_context_t* ctx, const char* path)
{
	FILE* fp = fopen(path, "rb");
	
	if(fp == NULL)
	{
		return 0;
	}
	
	fseek(fp, 0, SEEK_END);
	ctx->length = ftell(fp);
	rewind(fp);
	
	ctx->text = (char*)malloc(ctx->length + 1);
	fread(ctx->text, ctx->length, 1, fp);
	
	ctx->text[ctx->length] = 0x00;
	ctx->pos = 0;
	ctx->line_number = 1;
	ctx->column_number = 0;
	
	cvec_init(&ctx->result);
	
	return 1;
}

int parser_unload(parser_context_t* ctx)
{
	free(ctx->text);
	cvec_free(&ctx->result);
	return 1;
}

int parser_encode(parser_context_t* ctx)
{
	while(ctx->pos < ctx->length)
	{
		uint8_t upper;
		uint8_t lower;
		
		parser_seek_token(ctx);
		
		if(!parser_fetch_hex_nybble(ctx, &upper))
		{
			break;
		}
		
		if(!parser_fetch_hex_nybble(ctx, &lower))
		{
			break;
		}
		
		uint8_t val = (upper << 4) | lower;
		
		cvec_push(&ctx->result, val);
	}
	
	if(!parser_is_eof(ctx))
	{
		printf("error %d:%d Unexpected '%c'\n", ctx->line_number, ctx->column_number, ctx->text[ctx->pos]);
		return 0;
	}
	
	return 1;
}

int parser_dump_result(parser_context_t* ctx, const char* path)
{
	FILE* fp = fopen(path, "wb");
	fwrite(ctx->result.data, ctx->result.length, 1, fp);
	fclose(fp);
	return 1;
}

void parser_print_result(parser_context_t* ctx)
{
	for(int i = 0; i < ctx->result.length; i++)
	{
		if(i % 16 == 0)
		{
			printf("%04X: ", i);
		}
		printf("%02X ", (uint8_t)ctx->result.data[i]);
		
		if((i+1)%16 == 0)
		{
			printf("\n");
		}
	}
}

int parser_fetch_hex_nybble(parser_context_t* ctx, uint8_t* val)
{
	char cur_char;
	
	if(!parser_curc(ctx, &cur_char))
	{
		return 0;
	}
	
	if(isdigit(cur_char))
	{
		*val = cur_char - 0x30;
	}
	else if(cur_char >= 'A' && cur_char <= 'F')
	{
		*val = cur_char - 0x37;
	}
	else if(cur_char >= 'a' && cur_char <= 'f')
	{
		*val = cur_char - 0x57;
	}
	else
	{
		return 0;
	}
	
	ctx->pos++;
	return 1;
}

int parser_seek_token(parser_context_t* ctx)
{
	// Skip whitespace and comments
	int done = 0;
	char cur_char;
	
	while(parser_curc(ctx, &cur_char) && !done)
	{
		if(isspace(cur_char))
		{
			// Skip all whitespace
			parser_inc_pos(ctx);
			continue;
		}
		
		switch(cur_char)
		{
		case '/':
			parser_inc_pos(ctx);

			if(!parser_curc(ctx, &cur_char))
			{
				done = 1;
				break;
			}
			
			if(cur_char != '/')
			{
				// Unexpected '/'
				parser_dec_pos(ctx);
				done = 1;
				break;
			}
			// '//' Line comment, fall into ';' case
		case ';':
			while(parser_curc(ctx, &cur_char) && cur_char != '\n')
			{
				parser_inc_pos(ctx);
			}
			break;
		default:
			done = 1;
			break;
		}
	}
	
	if(parser_is_eof(ctx))
	{
		return 0;
	}
	
	return 1;
}

int parser_curc(parser_context_t* ctx, char* c)
{
	if(parser_is_eof(ctx))
	{
		return 0;
	}
	*c = ctx->text[ctx->pos];
	return 1;
}

int parser_inc_pos(parser_context_t* ctx)
{
	char cur_char;
	
	if(!parser_curc(ctx, &cur_char))
	{
		return 0;
	}
	
	if(cur_char == '\n')
	{
		ctx->line_number++;
		ctx->column_number = 1;
	}
	else
	{
		ctx->column_number++;
	}
	
	ctx->pos++;
	return 1;
}

int parser_dec_pos(parser_context_t* ctx)
{
	if(ctx->pos == 0)
	{
		return 0;
	}
	
	ctx->pos--;
	
	// TODO handle column number properly
	
	ctx->column_number--;
	
	char cur_char;
	
	if(parser_curc(ctx, &cur_char) && cur_char == '\n')
	{
		ctx->line_number--;
	}
	
	return 1;
}

int parser_is_eof(parser_context_t* ctx)
{
	return ctx->pos >= ctx->length;
}
