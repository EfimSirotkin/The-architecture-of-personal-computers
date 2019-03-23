// APC_Lab_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <string>

int main()
{
	float a, b, step, x, f, y;
	double time;
	LARGE_INTEGER frequency, start, end;
	while (true)
	{
		std::cout << " a: ";
		do {
			std::cin >> a;
		} while (a < 0);
		std::cout << " b: ";
		do {
			std::cin >> b;
		} while (b < 0 || b < a);
		std::cout << "[" << a << ";" << b << "]," << " step: ";
		do
		{
			std::cin >> step;
		} while (step > (b - a));


		x = a;
		f = 0;
		QueryPerformanceCounter(&start);
		QueryPerformanceFrequency(&frequency);

		_asm {
			finit
			fld b
			fld x
			loop_start :
				fcom
				fstsw ax
				and ah, 01000101b
				jz loop_end

				fld x
				fmul x

				fld x
				fsqrt
				fsin

				fsub

				fadd f
				fstp f
				fadd step
				fst x
				jmp loop_start
			loop_end :
			fwait
		}
		QueryPerformanceCounter(&end);
		time = (end.QuadPart - start.QuadPart)*1000.0f / frequency.QuadPart;
		printf("asm %lf time: %lf",f, time);
		
		x = a;
		f = 0;
		QueryPerformanceCounter(&start);
		while (x <= b)
		{
			f += x*x - sin(sqrt(x));
			x += step;
		}
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&end);
		time = (end.QuadPart - start.QuadPart)*1000.0f / frequency.QuadPart;
		printf("\n C - %lf  time: %lf",f, time);
		printf("\n test again: 1  ");
		int choice;
		std::cin >> choice;
		if (choice != 1) break;
	}
	return 0;
}
