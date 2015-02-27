#include "jugglemodule.h"

static juggle * _handle_juggle = 0;

namespace Fossilizid{
namespace juggle{

void create_module(){
	_handle_juggle= create_juggle();
}

}
}

