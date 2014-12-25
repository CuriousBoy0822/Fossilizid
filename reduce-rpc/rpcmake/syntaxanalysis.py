# 2014-12-17
# build by qianqians
# syntaxanalysis

#ast template
#filename
#namespace
#--nonenamespace/namenamespace
#----globalfunc
#------template
#--------templateinfo
#--------funcinfo
#------normal
#--------funcinfo
#----------funcsysmbal
#----------funcname
#----------argvlist
#----------return
#----------funcachieve
#----class
#------template
#--------templateinfo
#------normal
#--------classname
#----------rpcfunc
#----------constructor
#----------destruct
#----------otherclassinfo

import ruletable
import statemachine
import deletenonespacelstrip

stateprocess = {
    ruletable.state.STATENONE:lambda tlst, keyword : tlst.append(keyword),
    ruletable.state.STATENORMALFUNC:None ,
    ruletable.state.STATECLASSFUNC:None,
    ruletable.state.STATESTATICCLASSFUNC:None,
    ruletable.state.STATEFUNCARGVPAIRBEGIN:None,
    ruletable.state.STATEFUNCARGVPAIREND:None,
    ruletable.state.STATEFUNCDEFINE:None,
    ruletable.state.STATEFUNCACHIVEBEGIN:None,
    ruletable.state.STATEFUNCACHIVEEND:None,
    ruletable.state.STATECLASS:None,
    ruletable.state.STATECLASSACHIVEBEGIN:lambda tlst, keyword : tlst.append(keyword),
    ruletable.state.STATECLASSACHIVEEND:None,
    ruletable.state.STATECLASSDEFINEEND:None,
    ruletable.state.STATESTATIC:None,
    ruletable.state.STATESECTIONEND:None,
    ruletable.state.STATERPCCALL:None,
}

def syntaxanalysis(genfilestr):
    tempkeywork = []
    funcdef = []

    templateglobalfunc = []
    globalfunc = []
    templateclassfunc = {}

    classdef = False
    classfunc = {}
    classfunclist = None

    _statemachine = statemachine.statemachine()

    for i in xrange(len(genfilestr)):
        str = deletenonespacelstrip.deleteNoneSpacelstrip(genfilestr[i])
        for ch in str:
            s = _statemachine.push(ch)

            if s.is_change():
                if s.state == ruletable.state.STATECLASS:
                    if  classdef == False:
                        classdef = True
                    else:
                        if classfunclist is None:
                            classfunc[_statemachine.keyword] = []
                            classfunclist = classfunc[_statemachine.keyword]
                        if s.achivestate == ruletable.state.STATESECTIONEND:
                            classdef = False
                            classfunclist = None


                if s.is_wait_check():
                    tempkeywork.append(_statemachine.keyword)

                if s.is_func():
                    funcdef += tempkeywork
                    tempkeywork = []

                if s.is_func() and s.achivestate != ruletable.state.STATESECTIONEND:
                    if _statemachine.keyword != '':
                        funcdef.append(_statemachine.keyword)

                if s.is_func() and s.achivestate == ruletable.state.STATESECTIONEND:
                    if 'RPCCALL' in funcdef:
                        funcdef.remove('RPCCALL')
                        if s.parentstate is None or s.parentstate.state != ruletable.state.STATECLASS:
                            globalfunc.append(funcdef)
                        elif s.parentstate is not None and s.parentstate.state == ruletable.state.STATECLASS:
                            classfunclist.append(funcdef)
                    funcdef = []

                _statemachine.keyword = ''

                if s.achivestate == ruletable.state.STATESECTIONEND:
                    _statemachine.state = _statemachine.state.pop()
                    tempkeywork = []

                if s.is_need_clear():
                    s.clearcache = False
                    tempkeywork = []

    return {'globalfunc':globalfunc, 'templateglobalfunc':templateglobalfunc, 'classfunc':classfunc, 'templateclassfunc':templateclassfunc}



