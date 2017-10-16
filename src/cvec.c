/*

	cvec
	growing char buffer
	
	shygoo 2017

*/

#include <stdlib.h>

#include "cvec.h"

int cvec_init(cvec_t* cvec)
{
	cvec->buf_size = 1024;
	cvec->data = (char*)malloc(cvec->buf_size);
	cvec->length = 0;
	return 1;
}

int cvec_push(cvec_t* cvec, char c)
{
	if(cvec->length >= cvec->buf_size)
	{
		cvec->buf_size *= 2;
		cvec->data = realloc(cvec->data, cvec->buf_size);
	}
	
	cvec->data[cvec->length] = c;
	cvec->length++;
	return 1;
}

int cvec_free(cvec_t* cvec)
{
	free(cvec->data);
	return 1;
}
