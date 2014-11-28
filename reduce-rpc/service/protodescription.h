/*
 * protodescription.h
 *
 *  Created on: 2014-11-9
 *      Author: qianqians
 */
#ifndef _protodescription_h
#define _protodescription_h

namespace Fossilizid{
namespace reduce_rpc{

/*******************************/
/*          event key          */
/*******************************/

/*
 *  suuid
 *  本次消息唯一ID
 */

/*
 *  epuuid 
 *  本次连接唯一ID
 */

/*
 *  objuuid
 *  当前obj唯一ID
 */

/*
 *  eventtype
 *  本次消息，消息类型
 */


/*******************************/
/*          eventtype          */
/*******************************/

/*
 *  rpc_event
 *  rpc事件
 */

/*
 *  create_obj
 *  创建obj事件
 */

/*
 *  connect_server
 *  连接服务器
 */


/*******************************/
/*          rpc_event          */
/*******************************/

/*
 *  call_rpc_mothed
 *  调用rpc函数 
 */

/*
 *  call_rpc_mothed_ret
 *  rpc调用返回
 */



} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_protodescription_h