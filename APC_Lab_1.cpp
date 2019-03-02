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
				/*and ah, 01000101b
				jz loop_end*/

				fld x
				fmul x

				fld x
				fsqrt
				fsin

				//fsub st(1), st(0)
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

//_asm {
//	finit	//	команда инициализации сопроцессора
//	fld b	//	загружает вещественное число в стек ST(0) = b
//	fld x	//	загружает вещественное число в стек 
//			//	ST(0) = x, ST(1) = b
//
//			loop_start :
//	fcom	//	команда сравнения двух операндов в стеке
//		fstsw ax	//	копирует содержимое регистра состояния в AX
//		and ah, 01000101b	//	проверка регистра состояния (биты 8, 10, 14 - ответственны за сравнения)
//		jz loop_end		//	переход на метку конца цикла при условии равенства x == b
//
//		fld1	//	загружаем константу 1
//				//	ST(0) = 1, ST(1) = x, ST(2) = b
//		fadd x	//	сложение ST(0)= 1 + x (1 выталкивается и замещается)
//		fsin	//	вычисление синуса вершины стека ST(0) = sin(ST(0))= sin(1+x)	
//		fld x	//	загрузка на вершину х со смещением стека вниз
//		fcos	//	вычисление cos(x) с занесением на вершину вместо х
//		fiadd two	// целочисленное сложение: ST(0) = ST(0)+2
//		fdiv	//делим два операнда с вершины стека(ST(0)=ST(0)/ST(1)
//		fadd f	// ST(0) = ((cos(x) + 2)/ sin(1+x))) + f
//		fstp f	//	копируем ST(0) в f, после чего выталкиваем из ST(0)
//				//	ST(0) = x, ST(1)=b
//		fadd step	// ST(0) = x +step
//		fst x	//	ST(0) = x + step
//		jmp loop_start	// безусловный переход
//		loop_end :
//	fwait	//	команда синхронизации
//}