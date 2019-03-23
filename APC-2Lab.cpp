/*
2019, 03
author : EFIM SIROTKIN
BSUIR, 4th semester
APC, lab 2 "MMX"
*/


#include <iostream>
#include <Windows.h>
#include <time.h>

using namespace std;

#define ARRAY_SIZE 64
#define REPEAT_COUNTER 1000000


void set_random_values(short matrix[][ARRAY_SIZE], int size) {
	for(int i = 0; i < size;++i)
		for (int j = 0; j < size; ++j)
			matrix[i][j] = rand() % 100;
}

void print_values(short matrix[][ARRAY_SIZE], int size) {
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < ARRAY_SIZE; ++j)
			printf("  %hd", matrix[i][j]);
		printf("\n");
	}
}

void set_default_values(short matrix[][ARRAY_SIZE], int size) {
	for(int i = 0;i < size; ++i)
	for (int j = 0; j < ARRAY_SIZE; ++j)
		matrix[i][j] = 0;
}


int main()
{
	srand(time(NULL));
	short array_size = ARRAY_SIZE * ARRAY_SIZE;

	short a_matrix[ARRAY_SIZE][ARRAY_SIZE];
	short b_matrix[ARRAY_SIZE][ARRAY_SIZE];
	short results[ARRAY_SIZE][ARRAY_SIZE];

	LARGE_INTEGER starting_point;
	LARGE_INTEGER ending_point;
	LARGE_INTEGER current_frequency;

	double c_time = 0;
	double pure_asm_time = 0;
	double asm_mmx_time = 0;

	set_random_values(a_matrix, ARRAY_SIZE);
	set_random_values(b_matrix, ARRAY_SIZE);
	set_default_values(results, ARRAY_SIZE);

	// C
	QueryPerformanceCounter(&starting_point);
	QueryPerformanceFrequency(&current_frequency);
	for (int rep = 0; rep < REPEAT_COUNTER; rep++)
	{
		for (int i = 0; i < ARRAY_SIZE; ++i)
			for (int j = 0; j < ARRAY_SIZE; ++j)
				results[i][j] = a_matrix[i][j] & b_matrix[i][j];
	}
	
	QueryPerformanceCounter(&ending_point);
	print_values(results, ARRAY_SIZE);
	set_default_values(results, ARRAY_SIZE);

	c_time = (ending_point.QuadPart - starting_point.QuadPart)*1000.0f / current_frequency.QuadPart;
	printf("----C: %lf\n", c_time);
	
	//pure asm
	QueryPerformanceCounter(&starting_point);
	for (int rep = 0; rep < REPEAT_COUNTER; ++rep) {
		_asm {
			pusha

			lea esi, a_matrix
			lea edi, b_matrix
			lea ebp, results
			mov ecx, ARRAY_SIZE*ARRAY_SIZE
			loop_ASM :
			mov eax, [esi]
				and eax, [edi]
				mov[ebp], eax
				add esi, 2
				add edi, 2
				add ebp, 2
				loop loop_ASM

				popa
		}
	}
	QueryPerformanceCounter(&ending_point);
	printf("\n");
	print_values(results, ARRAY_SIZE);
	set_default_values(results, ARRAY_SIZE);
	pure_asm_time = (ending_point.QuadPart - starting_point.QuadPart)*1000.0f / current_frequency.QuadPart;
	printf("----pure asm: %lf\n", pure_asm_time);

	
	//asm MMX
	QueryPerformanceCounter(&starting_point);
	for (int rep = 0; rep < REPEAT_COUNTER; ++rep) {
		array_size = ARRAY_SIZE * ARRAY_SIZE;
		_asm {
			pusha
			xor esi, esi

			loop2 :
			movq MM0, a_matrix[esi]
			movq MM1, b_matrix[esi]
			pand MM0, MM1
			movq results[esi], MM0
			add esi, 8
			sub array_size, 4
			jnz loop2

			emms
			popa
		}
	}
	QueryPerformanceCounter(&ending_point);

	asm_mmx_time = (ending_point.QuadPart - starting_point.QuadPart)*1000.0f / current_frequency.QuadPart;
	printf("\n\n");

	print_values(results, ARRAY_SIZE);
	printf("----asm MMX: %lf", asm_mmx_time);

	system("pause>nul");
	return 1;
}

/*
Microsoft Visual Studio Community 2017
Версия 15.9.7
VisualStudio.15.Release / 15.9.7 + 28307.423
Microsoft.NET Framework
Версия 4.7.02046
*/
