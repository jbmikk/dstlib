#include "arrays.h"

void int_to_array(char *array, unsigned int *size, int integer)
{
    int remainder = integer;
    int i;

    for (i = 0; i < sizeof(int); i++) {
        array[i] = remainder & 0xFF;
        remainder >>= 8;
        if(remainder == 0)
        {
            i++;
            break;
        }
    }
    *size = i;
}

int array_to_int(char *array, unsigned int size)
{
	int symbol = 0;
	int i;

	for (i = size-1; i >= 0; i--) {
		symbol <<= 8;
		symbol = symbol | array[i];
	}
	return symbol;
}

void int_to_padded_array(char *array, int integer)
{
	int remainder = integer;
	int i;

	for (i = 0; i < sizeof(int); i++) {
		array[i] = remainder & 0xFF;
		remainder >>= 8;
	}
}

/*
 * Convert int to padded array in little-endian format
 */
void int_to_padded_array_le(char *array, int integer)
{
	int remainder = integer;
	int i;

	for (i = sizeof(int)-1; i >= 0; i--) {
		array[i] = remainder & 0xFF;
		remainder >>= 8;
	}
}
