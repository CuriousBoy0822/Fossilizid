/*
 * vector.h
 *
 *  Created on: 2014-9-13
 *      Author: qianqians
 */
#ifndef _vector_h
#define _vector_h

#include <vector>
#include "elementinfo.h"

namespace Fossilizid{
namespace comm_protocols {

class vector : public std::vector<elementinfo>
{};

} /* namespace comm_protocols */
} /* namespace Fossilizid */

#endif //_mempool_h