#ifndef ARRAYS_H
#define ARRAYS_H

void int_to_array(unsigned char *buffer, unsigned int *size, int symbol);
void int_to_padded_array(unsigned char *array, int integer);
int array_to_int(unsigned char *buffer, unsigned int size);

#endif //ARRAYS_H
