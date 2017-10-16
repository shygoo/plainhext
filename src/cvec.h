/*

	cvec
	growing char buffer
	
	shygoo 2017

*/

#ifndef CVEC_H
#define CVEC_H

typedef struct
{
	char* data;
	size_t buf_size;
	size_t length;
} cvec_t;

int cvec_init(cvec_t* cvec);
int cvec_push(cvec_t* cvec, char c);
int cvec_free(cvec_t* cvec);

#endif // CVEC_H
