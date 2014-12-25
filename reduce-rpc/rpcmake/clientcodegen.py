# 2014-12-24
# build by qianqians
# codegenclient

import os
from deletenonespacelstrip import deleteNoneSpacelstrip

build_path = './build/'

def returntype(type):
    if type == 'int' or type == 'int32_t' or type == 'long':
        return 'asInt()'
    if type == 'unsigned int' or type == 'unsigned long' or type == 'uint32_t':
        return 'asUInt()'
    if type == 'int64_t' or type == 'long long':
        return 'asInt64()'
    if type == 'unsigned long long' or type == 'uint64_t':
        return 'asUInt64()'
    if type == 'bool':
        return 'asBool()'
    if type == 'double':
        return 'asDouble()'
    if type == 'float':
        return 'asFloat()'
    if type == 'string' or type == 'std::string':
        return 'asString()'

def codegenclient(rpcsysmbal):
    if not os.path.isdir(build_path):
        os.mkdir(build_path)

    for k,v in rpcsysmbal.items():
        code = '#include <IRemoteEndpoint.h>\n\n'
        for sysmbal in v['globalfunc']:
            code += sysmbal[0] + ' ' + sysmbal[1] + '(IRemoteEndpoint ep'
            funcsys = sysmbal[1]
            avgr = sysmbal[2:]
            for i in xrange(len(avgr)):
                 code += ', ' + avgr[i]
            code += '){\n'
            code += '	boost::shared_ptr<session> s = GetSession(ep);\n\n'
            code += '	Json::Value value;\n'
            code += '	value[\'epuuid\'] = ' + 's.enppui();\n'
            code += '	value[\'suuid\'] = UUID();\n'
            code += '	value[\'eventtype\'] = \'rpc_event\';\n'
            code += '	value[\'rpc_event_type\'] = \'call_rpc_mothed\';\n'
            code += '	value[\'fnargv\'] = Json::Value(Json::objectValue) ;\n'
            for sys in xrange(len(avgr)):
                syss = avgr[sys].split(' ')
                funcsys += '_' + syss[0]
                code += '	value[\'fnargv\'][\'' + syss[1] + '\'] = ' + syss[1] + ';'
            code += '	value[\'fnname\'] = \'' + funcsys + '\';\n'
            code += '	s->do_push(s, value);\n\n'
            code += '	Json::Value ret = _service_handle->wait(value[\'suuid\'].asString(), 1);\n'
            code += '	if (ret[\'suuid\'] != value[\'suuid\']){\n		throw std::exception(\"error suuid\")\n	}\n'
            if sysmbal[0] != 'void':
                code += '\n	return '
                if sysmbal[0].find('std::pair') != -1 or sysmbal[0].find('pair') != -1:
                    index = sysmbal[0].find('std::pair')
                    if index == -1:
                        index = sysmbal[0].find('pair')
                    temavgr = sysmbal[0][index + 6: -1].split(',')
                    code += 'std::make_pair('
                    for i in xrange(len(temavgr)):
                        code += 'ret[\'rpcret\'][ret' + str(i) + '].' + returntype(temavgr[i])
                    code += ');\n'
                elif sysmbal[0] is 'std::tuple' or sysmbal[0] is 'tuple':
                    index = sysmbal[0].find('std::tuple')
                    if index == -1:
                        index = sysmbal[0].find('tuple')
                    temavgr = sysmbal[0][index + 6: -1].split(',')
                    code += 'std::make_tuple('
                    for i in xrange(len(temavgr)):
                        code += 'ret[\'rpcret\'][ret' + str(i) + '].' + returntype(temavgr[i])
                    code += ');\n'
                else:
                    code += ' ret[\'rpcret\'].' + returntype(sysmbal[0]) + ';\n'
            code += '}\n\n'

        for classname, sysmbal in v['classfunc'].items():
            code += 'class ' + classname + '{\n' + 'private:\n' + '	IRemoteEndpoint ep;\n\n'
            code += '	' + classname + '(IRemoteEndpoint _ep){\n		ep = _ep;\n	}\n\n'
            code += 'public:\n'
            for func in sysmbal:
                code += '	' + func[0] + ' ' + func[1] + '('
                funcsys = func[1]
                avgr = func[3:]
                code += func[2]
                for i in xrange(len(avgr)):
                    code += ', ' + avgr[i]
                code += '){\n'
                code += '		boost::shared_ptr<session> s = GetSession(ep);\n\n'
                code += '		Json::Value value;\n'
                code += '		value[\'epuuid\'] = ' + 's.enppui();\n'
                code += '		value[\'suuid\'] = UUID();\n'
                code += '		value[\'eventtype\'] = \'rpc_event\';\n'
                code += '		value[\'rpc_event_type\'] = \'call_rpc_mothed\';\n'
                code += '		value[\'fnargv\'] = Json::Value(Json::objectValue) ;\n'
                avgr = func[2:]
                for sys in xrange(len(avgr)):
                    syss = avgr[sys].split(' ')
                    funcsys += '_' + syss[0]
                    code += '		value[\'fnargv\'][\'' + syss[1] + '\'] = ' + syss[1] + ';\n'
                code += '		value[\'fnname\'] = \'' + funcsys + '\';\n'
                code += '		s->do_push(s, value);\n\n'
                code += '		Json::Value ret = _service_handle->wait(value[\'suuid\'].asString(), 1);\n'
                code += '		if (ret[\'suuid\'] != value[\'suuid\']){\n			throw std::exception(\"error suuid\")\n		}\n'
                if func[0] != 'void':
                    code += '\n		return '
                    if func[0].find('std::pair') != -1 or func[0].find('pair') != -1:
                        temavgr = func[0][func[0].find('<') + 1: -1].split(',')
                        code += 'std::make_pair('
                        for i in xrange(len(temavgr)):
                            if i != len(temavgr) - 1:
                                 code += 'ret[\'rpcret\'][ret' + str(i) + '].' + returntype(deleteNoneSpacelstrip(temavgr[i])) + ', '
                            else:
                                 code += 'ret[\'rpcret\'][ret' + str(i) + '].' + returntype(deleteNoneSpacelstrip(temavgr[i]))
                        code += ');\n'
                    elif func[0].find('std::tuple') != -1 or func[0].find('tuple') != -1:
                        temavgr = func[0][func[0].find('<') + 1: -1].split(',')
                        code += 'std::make_tuple('
                        for i in xrange(len(temavgr)):
                            if i != len(temavgr) - 1:
                                code += 'ret[\'rpcret\'][' + str(i) + '].' + returntype(deleteNoneSpacelstrip(temavgr[i])) + ', '
                            else:
                                code += 'ret[\'rpcret\'][' + str(i) + '].' + returntype(deleteNoneSpacelstrip(temavgr[i]))
                        code += ');\n'
                    else:
                        code += ' ret[\'rpcret\'];\n'
                code += '	}\n\n'
            code += '};\n\n'

        if code != '#include <IRemoteEndpoint.h>\n\n':
            file = open(build_path + k, 'w')
            file.write(code)
