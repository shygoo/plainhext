#include <stdlib.h>
#include <stdio.h>

#include "plainhext.h"

int main(int argc, const char* argv[])
{
	parser_context_t ctx;
	
	if(argc < 3)
	{
		printf("plainhext -- plaintext commented hex to binary converter\n\n  Usage: plainhext <input_path> <output_path>\n");
		return EXIT_SUCCESS;
	}
	
	if(!parser_load(&ctx, argv[1]))
	{
		printf("error: couldn't open %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	
	if(!parser_encode(&ctx))
	{
		return EXIT_FAILURE;
	}
	
	parser_dump_result(&ctx, argv[2]);
	parser_unload(&ctx);
	
	return EXIT_SUCCESS;
}
