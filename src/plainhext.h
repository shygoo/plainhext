/*

	plainhext
	commented hex -> bin utility

	shygoo 2017
	
*/

#ifndef PLAINHEXT_H
#define PLAINHEXT_H

#include <stdint.h>

#include "cvec.h"

typedef struct
{
	char* text;
	size_t length;
	int pos;
	int line_number;
	int column_number;
	cvec_t result;
} parser_context_t;

int parser_load(parser_context_t* ctx, const char* path);
int parser_unload(parser_context_t* ctx);
int parser_encode(parser_context_t* ctx);
int parser_dump_result(parser_context_t* ctx, const char* path);
void parser_print_result(parser_context_t* ctx);

int parser_fetch_hex_nybble(parser_context_t* ctx, uint8_t* val);
int parser_seek_token(parser_context_t* ctx);
int parser_curc(parser_context_t* ctx, char* c);
int parser_inc_pos(parser_context_t* ctx);
int parser_dec_pos(parser_context_t* ctx);
int parser_is_eof(parser_context_t* ctx);

#endif // PLAINHEXT_H
