# 2014-10-19
# build by qianqians
# rpcclassinfo

import os

def deletenote(filestr):
    genfilestr = []
    count = 0
    errornote = ""

    for i in xrange(len(filestr)):
        str = filestr[i]

        while(1):
            if count == 1:
                indexafter = str.find("*/")
                if indexafter is not -1:
                    str = str[indexafter+2:]
                    count = 0
                else:
                    break;

            index = str.find('//')
            if index is not -1:
                str = str[0:index]
            else:
                indexbegin = str.find("/*")
                if indexbegin is not -1:
                    errornote = str
                    indexafter = str.find("*/")
                    if indexafter is not -1:
                        str = str[0:indexbegin] + str[indexafter+2:]
                    else:
                        count = 1
                        break

            if str is not "":
                genfilestr.append(str)

            break

    if count is 1:
        raise Exception("c/c++ coding error unpaired /* ", errornote)

    return genfilestr

def deleteNoneSpacelstrip(str):
    while(str.lstrip(' ') is not str):str = str.lstrip(' ')
    return str

"""
gen state
def by qianqians
"""
nonestate = 0
findtemplate = 1
findclass = 2
findrpc = 4

def genrpcinfo(genfilestr):
    errornote = ""

    templateglobalfunc = []
    globalfunc = []
    templateclassfunc = {}
    classfunc = {}

    genstate = nonestate

    templateinfo = None
    classtemplateinfo = None
    strtemplate = ""

    classinfo = None

    rpcfuncdef = ""

    for i in xrange(len(genfilestr)):
        str = genfilestr[i]

        while(1):
            indextemplate = str.find("template")
            if indextemplate is not -1:
                strtemplate = str
                errornote = str
                genstate = findtemplate
            if genstate is findtemplate:
                indexbegin = strtemplate.find("<")
                indexend = strtemplate.find(">")
                if indexbegin is not -1:
                    if indexend is not -1:
                        strtemplate = strtemplate[indexbegin+1 : indexend]
                        templateinfo = strtemplate.split(',')
                        for i in xrange(len(templateinfo)):
                            templateinfo[i] = templateinfo[i].replace(' ', '_')
                            genstate = nonestate
                        str = str[indexend+1:]
                    else:
                        strtemplate += str
                        break
                else:
                    break

            indexclass = str.find("class")
            if indexclass is not -1:
                errornote = str
                str = str[indexclass+5:]
                genstate = findclass
            if genstate is findclass:
                classname = str
                classname = deleteNoneSpacelstrip(classname)
                if classname is "":
                    break
                classtemplateinfo = templateinfo
                templateinfo = None
                classinfo = classname[0:classname.find(' ')]
                genstate = nonestate

            index = str.find("RPCCALL")
            if index is not -1:
                errornote = str
                rpcfuncdef = str[0:index] + str[index+7:]
                genstate = findrpc
            if genstate is findrpc:
                indexlstrip = str.find(');')
                if indexlstrip is -1:
                    rpcfuncdef += str
                    break
                rpcfuncdef = deleteNoneSpacelstrip(rpcfuncdef)
                rpcfuncdeflist = rpcfuncdef.split(' ', 1)
                returntype = rpcfuncdeflist[0]
                funcdef = rpcfuncdeflist[1]
                indexbegin = funcdef.find('(')
                if indexbegin is -1:
                    raise Exception("c/c++ coding error /* ", errornote)
                funcname = funcdef[0:indexbegin]
                indexend = funcdef.find(')')
                argvstr = funcdef[indexbegin+1:indexend]
                argvstr = deleteNoneSpacelstrip(argvstr)
                argvlist = []
                if argvstr is not "":
                    argvstrlist = argvstr.split(',')
                    for i in xrange(len(argvstrlist)):
                        argvtype = deleteNoneSpacelstrip(argvstrlist[i])
                        argvlist.append(argvtype[:argvtype.find(' ')])
                rpcdef = [returntype, funcname]
                if len(argvlist) is not 0:
                    rpcdef += argvlist

                if templateinfo is None and classinfo is None:
                    globalfunc.append(rpcdef)

                if templateinfo is not None and classinfo is None:
                    templateglobalfunc.append([templateinfo,rpcdef])

                if classtemplateinfo is None and classinfo is not None:
                    if not classfunc.has_key(classinfo):
                        classfunc[classinfo] = []
                    classfunc[classinfo].append(rpcdef)

                if classtemplateinfo is not None and classinfo is not None:
                    if not templateclassfunc.has_key(classinfo):
                        templateclassfunc[classinfo] = [classtemplateinfo, []]
                    templateclassfunc[classinfo][1].append([templateinfo,rpcdef])

                genstate = nonestate

            index = str.find(");")
            if index is not -1:
                if templateinfo is not None:
                    templateinfo = None
                str = str[index+2:]
                if str is not "":
                    continue

            if str.find("};") is not -1 and classinfo is not None:
                classinfo = None
                classtemplateinfo = None

            break

    if genstate is not nonestate or templateinfo is not None or classtemplateinfo is not None:
        raise Exception("c/c++ coding error /* ", errornote)

    return {'globalfunc':globalfunc, 'templateglobalfunc':templateglobalfunc, 'classfunc':classfunc, 'templateclassfunc':templateclassfunc}

def traversalclass(dirpath):
    for dirpath, dirnames, filenames in os.walk(dirpath):
        fex = os.path.splitext(filenames)[1]
        if fex == ".h" or fex == ".hpp" or fex == ".cpp" or fex == ".cc" or fex == ".c":
            file = open(dirpath+filenames, "r")
            genfilestr = deletenote(file.readlines())

