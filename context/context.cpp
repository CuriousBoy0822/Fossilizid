/*
 * context.cpp
 *
 *  Created on: 2014-8-18
 *      Author: qianqians
 */
#include "context.h"

namespace Fossilizid{
namespace context {

context::context(boost::function<void()> fn){
#ifdef _WINDOWS
	pFiber = CreateFiber(1024*16, contextFiberProc, this);
#endif
}

context::~context(){
#ifdef _WINDOWS
	DeleteFiber(pFiber);
#endif 
}

VOID CALLBACK context::contextFiberProc(_In_ PVOID lpParameter){
	context * _context = (context*)lpParameter;
	if (!_context->contextfn.empty()){
		_context->contextfn();
	}
}

void context::operator()(){
#ifdef _WINDOWS
	SwitchToFiber(pFiber);
#endif 
}

context make_context(){
	context ct(0);
#ifdef _WINDOWS
	ct.pFiber = ConvertThreadToFiber(0);
#endif
	return ct;
}

void yield(context * ct){
#ifdef _WINDOWS
	SwitchToFiber(ct->pFiber);
#endif 
}

} /* namespace context */
} /* namespace Fossilizid */