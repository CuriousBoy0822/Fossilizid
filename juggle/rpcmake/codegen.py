# 2014-12-24
# build by qianqians
# codegenclient

import os

build_path = './build/'
achieve_include = ''
achieve_object = 'Fossilizid::juggle::object'

def maketypegetvalue(type):
    if type == 'int':
        return '.asint()'
    if type == 'float':
        return '.asfloat();'
    if type == 'bool':
        return '.asbool();'
    if type == 'string' or type == 'std::string':
        return '.asstring();'

def  maketype(type, name):
    if type == 'int':
        return '	n.' + name + ' = ' + '(*r)[\"ret\"][' + name + ']' + '.asint();'
    if type == 'float':
        return '	n.' + name + ' = ' + '(*r)[\"ret\"][' + name + ']' + '.asfloat();'
    if type == 'bool':
        return '	n.' + name + ' = ' + '(*r)[\"ret\"][' + name + ']' + '.asbool();'
    if type == 'string' or type == 'std::string':
        return '	n.' + name + ' = ' + '(*r)[\"ret\"][' + name + ']' + '.asstring();'
    if type == 'array':
        code = '	n.' + name + ';\n'
        code += '	for(int i = 0; i < (*r)[\"ret\"].size(); i++){\n'
        code += '		n.' + name + '.push_back((*r)[\"ret\"][i]' + maketypegetvalue(type) + ')'
        code += '}\n'
        return code

def unpackstruct(type, struct):
    for k, v in struct:
        for name, define in v:
            if type == name:
                code = '	name n;\n'
                for argv in define:
                    code += maketype(argv[0], argv[1])
                code += '	return n;'
                return code

def makearray(type):
    indexb = type.find('<')
    indexe = type.find('>')
    templatetype = type[indexb + 1 : indexe]
    if templatetype == 'int':
        return 'std::vector<int64_t> v;\n' \
               'for(int i = 0; i < (*r)[\"ret\"].size(); i++){\n' \
               '	v.push_back((*r)[\"ret\"][i].asint());\n' \
               '}\n' \
               'return v;'
    if templatetype == 'float':
        return 'std::vector<double> v;\n' \
               'for(int i = 0; i < (*r)[\"ret\"].size(); i++){\n' \
               '	v.push_back((*r)[\"ret\"][i].asfloat());\n' \
               '}\n' \
               'return v;'
    if templatetype == 'bool':
        return 'std::vector<bool> v;\n' \
               'for(int i = 0; i < (*r)[\"ret\"].size(); i++){\n' \
               '	v.push_back((*r)[\"ret\"][i].asbool());\n' \
               '}\n' \
               'return v;'
    if templatetype == 'string' or type == 'std::string':
        return 'std::vector<std::string> v;\n' \
               'for(int i = 0; i < (*r)[\"ret\"].size(); i++){\n' \
               '	v.push_back((*r)[\"ret\"][i].asstring());\n' \
               '}\n' \
               'return v;'

def makeret(type, struct):
    if type == 'int':
        return '(*r)[\"ret\"].asint();'
    if type == 'float':
        return '(*r)[\"ret\"].asfloat();'
    if type == 'bool':
        return '(*r)[\"ret\"].asbool();'
    if type == 'string' or type == 'std::string':
        return '(*r)[\"ret\"].asstring();'
    if type.find('array') != -1:
        return makearray(type)
    if type == 'map':
        return '(*r)[\"ret\"];'
    if type == 'void':
        return ''
    else:
        return unpackstruct(type, struct)

def  maketypevalue(type, name):
    if type == 'int':
        return '	n.' + name + ' = ' + '(*v)[' + name + ']' + '.asint();'
    if type == 'float':
        return '	n.' + name + ' = ' + '(*v)[' + name + ']' + '.asfloat();'
    if type == 'bool':
        return '	n.' + name + ' = ' + '(*v)[' + name + ']' + '.asbool();'
    if type == 'string' or type == 'std::string':
        return '	n.' + name + ' = ' + '(*v)[' + name + ']' + '.asstring();'
    if type.find('array') != -1:
        indexb = type.find('<')
        indexe = type.find('>')
        templatetype = type[indexb + 1 : indexe]
        code = '	n.' + name + ';\n'
        code += '	for(int i = 0; i < (*v).size(); i++){\n'
        code += '		n.' + name + '.push_back((*v)[i]' + maketypegetvalue(templatetype) + ')'
        code += '}\n'
        return code

def unpackstructvalue(type, name, struct):
    for k, v in struct:
        for typename, define in v:
            if type == typename:
                code = '	typename ' + name + ';\n'
                for argv in define:
                    code += maketypevalue(argv[0], argv[1])
                return code

def makearrayvalue(type, name):
    indexb = type.find('<')
    indexe = type.find('>')
    templatetype = type[indexb + 1 : indexe]
    if templatetype == 'int':
        return '		std::vector<int64_t> ' + name + ';\n' \
               '		for(int i = 0; i < (*v)[\"' + name + '\"].size(); i++){\n' \
               '			v.push_back((*v)[\"' + name + '\"][i].asint());\n' \
               '		}\n'
    if templatetype == 'float':
        return '		std::vector<double> ' + name + ';\n' \
               '		for(int i = 0; i < (*v)[\"' + name + '\"].size(); i++){\n' \
               '			v.push_back((*v)[\"' + name + '\"][i].asfloat());\n' \
               '		}\n'
    if templatetype == 'bool':
        return '		std::vector<bool> ' + name + ';\n' \
               '		for(int i = 0; i < (*v)[\"' + name + '\"].size(); i++){\n' \
               '			v.push_back((*v)[\"' + name + '\"].asbool());\n' \
               '		}\n'
    if templatetype == 'string' or templatetype == 'std::string':
        return '		std::vector<std::string> ' + name + ';\n' \
               '		for(int i = 0; i < (*v)[\"' + name + '\"].size(); i++){\n' \
               '			v.push_back((*v)[\"' + name + '\"][i].asstring());\n' \
               '		}\n'

def makevalue(type, name, struct):
    if type == 'int':
        return '		auto ' + name + ' = (*v)[\"' + name + '\"].asint();\n'
    if type == 'float':
        return '		auto ' + name + ' = (*v)[\"' + name + '\"].asfloat();\n'
    if type == 'bool':
        return '		auto ' + name + ' = (*v)[\"' + name + '\"].asbool();\n'
    if type == 'string' or type == 'std::string':
        return '		auto ' + name + ' = (*v)[\"' + name + '\"].asstring();\n'
    if type.find('array') != -1:
        return makearrayvalue(type, name)
    if type == 'map':
        return '		auto ' + name + ' = (*v)[\"' + name + '\"];\n'
    else:
        return unpackstructvalue(type, name, struct)

def typetocpptype(type):
    if type == 'int':
        return 'int64_t'
    if type == 'float':
        return 'double'
    if type == 'string':
        return 'std::string'
    return type

def makearraytocpp(type, name):
    indexb = type.find('<')
    indexe = type.find('>')
    templatetype = type[indexb + 1 : indexe]
    return '	' + 'std::vector<' + typetocpptype(templatetype) + '> ' + name + ';'

def maketypetocpptype(type):
    if type.find('array') != -1:
        indexb = type.find('<')
        indexe = type.find('>')
        templatetype = type[indexb + 1 : indexe]
        return 'std::vector<' + typetocpptype(templatetype) + '> '
    if type == 'int':
        return 'int64_t'
    if type == 'float':
        return 'double'
    if type == 'string':
        return 'std::string'
    return type

def codegenstruct(filelist):
    if not os.path.isdir(build_path):
        os.mkdir(build_path)

    defstructlist = []

    for filename, list in filelist.items():
        code = '#include <juggle.h>\n\n'

        struct = list['struct']

        for k, v in struct.items():
            if k in defstructlist:
                raise 'redefined struct %s'% k
            code += 'struct ' + k + '{\n'
            for argv in v:
                code += '	' + maketypetocpptype(argv[0]) + ' ' + argv[1] + ';'
            code += '};\n\n'
            defstructlist.append(k)

        if code != '#include <juggle.h>\n\n':
            file = open(build_path + filename + 'struct.h', 'w')
            file.write(code)

def codegenclient(filelist):
    if not os.path.isdir(build_path):
        os.mkdir(build_path)
    if not os.path.isdir(build_path + 'client'):
        os.mkdir(build_path + 'client')

    defmodulelist = []

    codecreatemodule = "#include <juggle.h>\n\nnamespace Fossilizid{\nnamespace juggle{\n\nvoid create_module(){\n"

    for filename, list in filelist.items():
        code = '#include <' + achieve_include + '>\n#include <juggle.h>\n#include <boost/make_shared.hpp>\n\n'

        struct = list['struct']
        module = list['module']

        if len(struct) > 0:
            code += '#include \"' + filename +  'struct.h' + '\"'

        for k, v in module.items():
            if k in defmodulelist:
                raise 'redefined module %s' % k
            code += 'namespace sync{\n\n'
            code += 'class ' + k + ': public ' + 'Fossilizid::juggle::caller' + '{\n' + 'public:\n'
            code += '	' + k + '(Fossilizid::juggle::channel * ch) : caller(ch, \"' + k + '\"){\n' + '	}\n\n'
            code += '	~' + k + '(){\n' + '	}\n\n'
            for func in v:
                code += '	' + maketypetocpptype(func[0]) + ' ' + func[1] + '(' + maketypetocpptype(func[2][0]) + ' ' + func[2][1]
                for argv in func[3:]:
                    code += ',' + maketypetocpptype(argv[0]) + ' ' + argv[1]
                code += '){\n'
                code += '		boost::shared_ptr<Fossilizid::juggle::object> v = boost::make_shared<' + achieve_object + '>();\n'
                for argv in func[2:]:
                    code += '		(*v)[\"' + argv[1] + '\"] = ' + argv[1] + ';\n'
                code += '		boost::shared_ptr<Fossilizid::juggle::object> r = call_module_method_sync(\"' + k + '_' + func[1] + '\", v);\n'
                code += '		return ' + makeret(func[0], struct) + ';\n'
                code += '	}\n\n'
            code += '};\n\n'
            code += '}\n\n'

            code += 'namespace async{\n\n'
            code += 'class ' + k + ': public ' + 'Fossilizid::juggle::caller' + '{\n' + 'public:\n'
            code += '	' + k + '(Fossilizid::juggle::channel * ch) : caller(ch, \"' + k + '\"' + '){\n	}\n\n'
            code += '	~' + k + '(){\n	}\n\n'
            for func in v:
                code += '	' + maketypetocpptype(func[0]) + ' ' + func[1] + '('
                for argv in func[2:]:
                    code += maketypetocpptype(argv[0]) + ' ' + argv[1] + ', '
                code += 'boost::function<void(' + maketypetocpptype(func[0]) + ')> callback){\n'
                code += '		boost::shared_ptr<Fossilizid::juggle::object> v = boost::make_shared<' + achieve_object + '>();\n'
                for argv in func[2:]:
                    code += '		(*v)[\"' + argv[1] + '\"] = ' + argv[1] + ';\n'
                code += '		auto cb = [this, callback](boost::shared_ptr<Fossilizid::juggle::object> r){\n'
                if func[0] != 'void':
                    code += '			' + maketypetocpptype(func[0]) + ' ret = ' + makeret(func[0], struct) + '\n'
                code += '			callback(ret);\n' \
                        '		};\n'
                code += '		call_module_method_async(\"' + k + '_' + func[1] + '\", v, cb' + ');\n'
                code += '	}\n\n'
            code += '};\n\n'
            code += '}\n\n'
            defmodulelist.append(k)

        if code != '#include <' + achieve_include + '>\n#include <juggle.h>\n#include <boost/make_shared.hpp>\n\n':
            file = open(build_path + 'client/' + filename + 'caller.h', 'w')
            file.write(code)

    codecreatemodule += '}\n\n}\n}\n\n'

    file = open(build_path + 'client/' + 'callerservicedefine.cpp', 'w')
    file.write(codecreatemodule)

def codegenserver(filelist):
    if not os.path.isdir(build_path):
        os.mkdir(build_path)
    if not os.path.isdir(build_path + 'server'):
        os.mkdir(build_path + 'server')

    defmodulelist = []

    codeglobalhandle = ""
    codecreatemodule = "namespace Fossilizid{\nnamespace juggle{\n\nvoid create_module(){\n"

    for filename, list in filelist.items():
        code = '#include <' + achieve_include + '>\n#include <juggle.h>\n#include <boost/make_shared.hpp>\n\n'

        struct = list['struct']
        module = list['module']

        if len(struct) > 0:
            code += '#include \"' + filename +  'struct.h' + '\"'

        for k, v in module.items():
            if k in defmodulelist:
                raise 'redefined module %s' % k
            code += 'class ' + k + ': public ' + 'Fossilizid::juggle::module' + '{\n' + 'public:\n'
            code += '	' + k + '() : module(\"' + k + '\", Fossilizid::uuid::UUID()' + '){\n'
            for func in v:
                code += '		Fossilizid::juggle::_service_handle->register_module_method(\"' + k + '_' + func[1] + '\",' + ' boost::bind(' + '&' + k + '::' + 'call_' + func[1] + ', this, _1, _2)' + ');\n'
            code += '	}\n\n'
            code += '	' + '~' + k + '(){\n	}\n\n'
            for func in v:
                code += '	virtual ' + maketypetocpptype(func[0]) + ' ' + func[1] + '(' + maketypetocpptype(func[2][0]) + ' ' + func[2][1]
                for argv in func[3:]:
                    code += ',' + maketypetocpptype(argv[0]) + ' ' + argv[1]
                code += ') = 0;\n\n'
                code += '	void call_' + func[1] + '(Fossilizid::juggle::channel * ch, boost::shared_ptr<Fossilizid::juggle::object> v){\n'
                for argv in func[2:]:
                    code += makevalue(argv[0], argv[1], struct)
                code += '		auto ret = ' + func[1] + '(' + func[2][1]
                for argv in func[3:]:
                    code += ', ' + argv[1]
                code += ');\n'
                code += '		boost::shared_ptr<Fossilizid::juggle::object> r = boost::make_shared<' + achieve_object + '>();\n'
                code += '		(*r)[\"suuid\"] = (*v)[\"suuid\"].asstring();\n'
                code += '		(*r)[\"method\"] = (*v)[\"method\"].asstring();\n'
                code += '		(*r)[\"rpcevent\"] = \"reply_rpc_method\";\n\n'
                if func[0].find('std::vector') != -1:
                    code += '		for(auto v : ret){\n'
                    code += '			(*r)[\"ret\"].append(v);\n'
                    code += '		}\n'
                else:
                    code += '		(*r)[\"ret\"] = ret;\n'
                code += '		ch->push(r);\n'
                code += '	}\n\n'
            code += '};\n\n'

        if code != '#include <' + achieve_include + '>\n#include <juggle.h>\n#include <boost/make_shared.hpp>\n\n':
            file = open(build_path + 'server/' + filename + 'module.h', 'w')
            file.write(code)

        codeglobalhandle = '#include \"' + filename + 'module.h\"\n\nstatic ' + k + ' * _handle_' + k + ' = 0;\n\n'
        codecreatemodule += '	_handle_' + k + '= new ' + k + '();\n'

    codecreatemodule += '}\n\n}\n}\n\n'

    file = open(build_path + 'server/' + 'globalhandledefine.cpp', 'w')
    file.write(codeglobalhandle + codecreatemodule)
