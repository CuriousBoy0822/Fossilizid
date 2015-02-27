/*
 *  qianqians
 *  2015-1-1
 */

#include "../../context/context.h"

void context1();

Fossilizid::context::context mainct = 0;
Fossilizid::context::context ct1 = Fossilizid::context::getcontext(context1);

void context3(){
	printf("context3\n");

	Fossilizid::context::yield(ct1);
}

void context2(){
	printf("context2\n");

	Fossilizid::context::context ct3 = Fossilizid::context::getcontext(context3);
	Fossilizid::context::yield(ct3);
}

void context1(){
	printf("context1\n");

	Fossilizid::context::context ct2 = Fossilizid::context::getcontext(context2);
	Fossilizid::context::yield(ct2);

	printf("context1,1\n");

	Fossilizid::context::yield(mainct);
}

void init(){
	mainct = Fossilizid::context::makecontext();
}

int main(){
	printf("main");

	init();

	Fossilizid::context::yield(ct1);

	return 0;
}
