#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "arrays.h"
#include "test.h"

void t_setup(){
}

void t_teardown(){
}

void int_to_array__one_byte(){

	int symbol = 40;
	unsigned char buffer[sizeof(int)];
	unsigned int size;

	int_to_array(buffer, &size, symbol);

	t_assert(size == 1);
	t_assert(buffer[0] == 40);
}

void int_to_array__two_bytes(){

	int symbol = 258;
	unsigned char buffer[sizeof(int)];
	unsigned int size;

	int_to_array(buffer, &size, symbol);

	t_assert(size == 2);
	t_assert(buffer[0] == 2);
	t_assert(buffer[1] == 1);
}

void int_to_array__negative(){

	int symbol = -1;
	unsigned char buffer[sizeof(int)];
	unsigned int size;

	int_to_array(buffer, &size, symbol);

	t_assert(size == sizeof(int));
	t_assert(buffer[0] == 0xFF);
}

void array_to_int__one_byte(){

	int symbol = 0;
	unsigned char buffer[sizeof(int)];
	unsigned int size;

	size = 1;
	buffer[0] = 40;

	symbol = array_to_int(buffer, size);

	t_assert(symbol == 40);
}

void array_to_int__two_bytes(){

	int symbol = 0;
	unsigned char buffer[sizeof(int)];
	unsigned int size;

	size = 2;
	buffer[0] = 2;
	buffer[1] = 1;

	symbol = array_to_int(buffer, size);

	t_assert(symbol == 258);
}

void array_to_int__negative(){

	int symbol = 0;
	unsigned char buffer[sizeof(int)];
	unsigned int size;

	size = sizeof(int);
	int i;
	for(i = 0; i < size; i++) {
		buffer[i] = 0xFF;
	}

	symbol = array_to_int(buffer, size);

	t_assert(symbol == -1);
}

void int_to_padded_array__one_byte(){

	int symbol = 40;
	unsigned char buffer[sizeof(int)];

	int i;

	// Fill with crap
	for (i = 0; i < sizeof(int); i++) {
		buffer[i] = 0xFF;
	}

	int_to_padded_array(buffer, symbol);

	t_assert(buffer[0] == 40);

	for (i = 1; i < sizeof(int); i++) {
		t_assert(buffer[i] == 0);
	}
}

void int_to_padded_array_le__one_byte(){

	int symbol = 40;
	unsigned char buffer[sizeof(int)];

	int i;

	// Fill with crap
	for (i = 0; i < sizeof(int); i++) {
		buffer[i] = 0xFF;
	}

	int_to_padded_array_le(buffer, symbol);

	t_assert(buffer[sizeof(int)-1] == 40);

	for (i = sizeof(int)-2; i >= 0; i--) {
		t_assert(buffer[i] == 0);
	}
}

int main(int argc, char** argv){

	t_init();
	t_test(int_to_array__one_byte);
	t_test(int_to_array__two_bytes);
	t_test(int_to_array__negative);
	t_test(int_to_padded_array__one_byte);
	t_test(int_to_padded_array_le__one_byte);
	t_test(array_to_int__one_byte);
	t_test(array_to_int__two_bytes);
	t_test(array_to_int__negative);

	return t_done();
}

