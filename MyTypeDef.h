#pragma once

#define TRUE	1
#define FALSE	0

#define ALL_SET_UI8			0xFF				
#define ALL_CLR_UI8			0x00

#define ALL_SET_UI16		0xFFFF				
#define ALL_CLR_UI16		0x0000				

#define ALL_SET_UI32		0xFFFFFFFF				
#define ALL_CLR_UI32		0x00000000	

#define SUCCESS		0
#define FAIL		(-1)
#define ABORT		1

#define V_NULL 0						// Used as value to initialzed not used variables and pointers
#define P_NULL ((void *)0)				// Used as value to initialzed not used pointers to point to NULL


typedef unsigned char		UC8;
typedef signed char 		SC8;

typedef unsigned char 		UI8;
typedef signed char 		SI8;

typedef unsigned int 		UI16;
typedef signed int 			SI16;

typedef unsigned long int	UI32;
typedef signed long int		SI32;

