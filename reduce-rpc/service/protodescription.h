/*
 * protodescription.h
 *
 *  Created on: 2014-11-9
 *      Author: qianqians
 */
#ifndef _protodescription_h
#define _protodescription_h

#define RPCCALL

namespace Fossilizid{
namespace reduce_rpc{

/*******************************/
/*          event key          */
/*******************************/

/*
 *  suuid
 *  ������ϢΨһID
 */

/*
 *  epuuid 
 *  ��������ΨһID
 */

/*
 *  objuuid
 *  ��ǰobjΨһID
 */

/*
 *  eventtype
 *  ������Ϣ����Ϣ����
 */


/*******************************/
/*          eventtype          */
/*******************************/

/*
 *  rpc_event
 *  rpc�¼�
 */

/*
 *  create_obj
 *  ����obj�¼�
 */

/*
 *  connect_server
 *  ���ӷ�����
 */


/*******************************/
/*        rpc_event_type       */
/*******************************/

/*
 *  call_rpc_mothed
 *  ����rpc���� 
 */

/*
 *  call_rpc_mothed_ret
 *  rpc���÷���
 */


/*******************************/
/*          rpccallinfo        */
/*******************************/

/*
 *  fnname
 *  ����rpc��������
 */
	
/*
 *  fnargv
 *  ����rpc��������
 */

/*
 *  rpcret
 *  rpc���÷���ֵ
 */


} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_protodescription_h