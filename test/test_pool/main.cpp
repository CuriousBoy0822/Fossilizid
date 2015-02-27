/*
 *  qianqians
 *  2014-10-5
 */

#include "../../pool/factory.h"
#include "../../pool/mempool.h"
#include "../../pool/objpool.h"

int main(){
	for (int i = 16; i < 65536; i++){
		char * buf = (char*)Fossilizid::pool::mempool::allocator(i);
		for (int n = 0; n < i; n++){
			buf[n] = 'a';
		}
		buf[i - 1] = 0;
		printf(buf);
		printf("\n");
		Fossilizid::pool::mempool::deallocator(buf, i);
	}

	for (int i = 16; i < 65536; i++){
		struct tmp{
			int n;
			char a;
			float f;
		};

		tmp * t = (tmp*)Fossilizid::pool::objpool<tmp>::allocator(i);
		for (int n = 0; n < i; n++){
			t[n].n = i;
			t[n].a = 'a';
			t[n].f = 0.111;
		}
		for (int n = 0; n < i; n++){
			printf("tmp %d %c %f", t[n].n, t[n].a, t[n].f);
		}
		printf("\n");
		Fossilizid::pool::objpool<tmp>::deallocator(t, i);
	}

	for (int i = 16; i < 65536; i++){
		struct tmp{
			tmp(int i, char c, float ff){
				n = i;
				a = c;
				f = ff;
			}

			int n;
			char a;
			float f;
		};

		tmp * t = (tmp*)Fossilizid::pool::factory::create<tmp>(i, i, i % 256, *((float*)&i));
		for (int n = 0; n < i; n++){
			printf("tmp %d %c %f", t[n].n, t[n].a, t[n].f);
		}
		printf("\n");
		Fossilizid::pool::factory::release(t, i);
	}

	return 0;
}
