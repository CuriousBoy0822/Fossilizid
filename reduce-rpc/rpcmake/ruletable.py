# 2014-12-18
# build by qianqians
# ruletable

import deletenonespacelstrip

class state(object):
    STATENONE = 0
    STATENORMALFUNC = 1.1
    STATECLASSFUNC = 1.2
    STATESTATICCLASSFUNC = 1.3
    STATEFUNCARGVPAIRBEGIN = 11
    STATEFUNCARGVPAIREND = 13
    STATEFUNCDEFINE = 14
    STATEFUNCACHIVEBEGIN = 15
    STATEFUNCACHIVEEND = 16
    STATECLASS = 2
    STATECLASSACHIVEBEGIN = 21
    STATECLASSACHIVEEND = 22
    STATECLASSDEFINEEND = 23
    STATENAMESPACE = 6
    STATENAMESPACEACHIVEBEGIN = 61
    STATENAMESPACEACHIVEEND = 62
    STATENAMESPACEDEFINEEND = 63
    STATESTATIC = 3
    STATESECTIONEND = 4
    STATERPCCALL = 5
    STATEPREPROCESS = 7

    def __init__(self, parentstate):
        self.pairstate = 0
        self.attachstate = state.STATENONE
        self.state = state.STATENONE
        self.rpcstate = state.STATENONE
        self.achivestate = state.STATENONE
        self.statechange = False
        self.clearcache = False

        self.parentstate = parentstate

    def pop(self):
        if self.parentstate != None:
            return self.parentstate

        self.pairstate = 0
        self.state = state.STATENONE
        self.attachstate = state.STATENONE
        self.state = state.STATENONE
        self.rpcstate = state.STATENONE
        self.achivestate = state.STATENONE

        return self

    def is_need_clear(self):
        return self.clearcache

    def is_func(self):
        return self.state == state.STATECLASSFUNC or self.state == state.STATESTATICCLASSFUNC or self.state == state.STATENORMALFUNC

    def is_change(self):
        return self.statechange

    def is_pair(self):
        return self.pairstate > 0

    def is_wait_check(self):
        return self.state == state.STATENONE or (self.state == state.STATECLASS and self.achivestate == state.STATECLASSACHIVEBEGIN) or \
            (self.state == state.STATENAMESPACE and self.achivestate == state.STATENAMESPACEACHIVEBEGIN)

preprocessrule = {'key':'#', 'end':'\n', 'keyword':{'include':{'pairbegin':'<', 'pairend':'>'}, 'pragma':{}, 'if':{'endkey':'endif'}, 'endif':{}, 'define':{}}}

pair = {'pairbegin':'<', 'pairend':'>'}

rule = {'namespace':{'key':'namespace', 'achivebegin':'{', 'achiveend':'}', 'defineend':'}'},
        'class':{'key':'class', 'achivebegin':'{', 'achiveend':'}', 'defineend':';'},
        'func':{'key':'(', 'argvbegin':'(', 'argvend':')', 'argvsplit':',', 'defineend':';', 'achivebegin':'{', 'achiveend':'}', 'achivedefineend':'}', 'rpc':'RPCCALL'},
        'template':{'key':'template', 'templateargvbegin':'<', 'templateargvend':'>'},
        'preprocessrule':preprocessrule}

def checkpair(_state, ch):
    if _state.is_wait_check():
        if ch == pair['pairbegin']:
            _state.pairstate += 1
        elif ch == pair['pairend']:
            _state.pairstate -= 1
    return _state.pairstate

def checkachivetype(_state, keyword):
    for k, v in rule.items():
        if keyword == v['key']:
            if _state.state != state.STATENONE:
                _state = state(_state)
            if k == 'namespace':
                _state.state = state.STATENAMESPACE
                _state.achivestate = state.STATENAMESPACE
            elif k == 'class':
                _state.state = state.STATECLASS
                _state.achivestate = state.STATECLASS
            elif k == 'func':
                if _state.state == state.STATENONE:
                    _state.state = state.STATENORMALFUNC
                elif _state.parentstate.state == state.STATECLASS:
                    if _state.attachstate == state.STATESTATIC:
                        _state.state = state.STATESTATICCLASSFUNC
                    else:
                        _state.state = state.STATECLASSFUNC
                _state.achivestate = state.STATEFUNCARGVPAIRBEGIN
            elif k == 'preprocessrule':
                _state.state = state.STATEPREPROCESS

            _state.statechange = True

    return _state

def checkfuncrule(state, keyword):
    for k, v in rule['func'].items():
        if keyword == v:
            if k == 'argvbegin':
                state.achivestate = state.STATEFUNCARGVPAIRBEGIN
            elif k == 'argvend' and state.achivestate == state.STATEFUNCARGVPAIRBEGIN:
                state.achivestate = state.STATEFUNCARGVPAIREND
            elif k == 'argvsplit' and state.achivestate == state.STATEFUNCARGVPAIRBEGIN:
                state.statechange = True
            if k == 'defineend' and state.achivestate == state.STATEFUNCARGVPAIREND:
                state.achivestate = state.STATESECTIONEND

            if k == 'achivebegin':
                state.achivestate = state.STATECLASSACHIVEBEGIN
            elif k == 'achiveend' and state.achivestate == state.STATECLASSACHIVEBEGIN:
                state.achivestate = state.STATEFUNCACHIVEEND
            if k == 'achivedefineend' and state.achivestate == state.STATEFUNCACHIVEEND:
                state.achivestate = state.STATESECTIONEND

            if k == 'argvsplit' and state.achivestate == state.STATEFUNCARGVPAIRBEGIN:
                break

            state.statechange = True
            break

def checknamespacerule(state, keyword):
    for k, v in rule['namespace'].items():
        if keyword == v:
            if k == 'achivebegin':
                state.achivestate = state.STATENAMESPACEACHIVEBEGIN
                state.clearcache = True
            elif k == 'achiveend' and state.achivestate == state.STATENAMESPACEACHIVEBEGIN:
                state.achivestate = state.STATENAMESPACEACHIVEEND

            if k == 'defineend' and state.achivestate == state.STATENAMESPACEACHIVEEND:
                state.achivestate = state.STATENAMESPACEDEFINEEND

            state.statechange = True
            break

def checkclassrule(state, keyword):
    for k, v in rule['class'].items():
        if keyword == v:
            if k == 'achivebegin':
                state.achivestate = state.STATECLASSACHIVEBEGIN
                state.clearcache = True
            elif k == 'achiveend' and state.achivestate == state.STATECLASSACHIVEBEGIN:
                state.achivestate = state.STATECLASSACHIVEEND
            elif k == 'defineend' and state.achivestate == state.STATECLASSACHIVEEND:
                state.achivestate = state.STATESECTIONEND

            state.statechange = True
            break

def ruletable(state, keyword, ch):
    if state.is_change():
        state.statechange = False

    while True:
        if ch == '#':
            state = checkachivetype(state, ch)
            break

        if state.state == state.STATEPREPROCESS:
            if ch == '\n':
                state.achivestate = state.STATESECTIONEND
                state.statechange = True
                break

        if ch == '\n':
            break


        checkpair(state, ch)

        if ch == ' ' or ch == '\0':
            if not state.is_pair():
                if not state.is_func():
                    keyword = deletenonespacelstrip.deleteNoneSpacelstrip(keyword)
                    state = checkachivetype(state, keyword)
                    if state.state == state.STATECLASS:
                        checkclassrule(state, keyword)
                    elif state.state == state.STATENAMESPACE:
                        checknamespacerule(state, keyword)

                    state.statechange = True
                    break

        if ch == '{':
            if state.state == state.STATECLASS:
                checkclassrule(state, ch)
            elif state.state == state.STATENAMESPACE:
                checknamespacerule(state, ch)
            break

        if ch == '}':
            if state.state == state.STATECLASS:
                checkclassrule(state, ch)
            elif state.state == state.STATENAMESPACE:
                checknamespacerule(state, ch)
            break

        if ch == ';':
            if state.state == state.STATECLASS:
                checkclassrule(state, ch)
            if state.is_func():
                keyword = deletenonespacelstrip.deleteNoneSpacelstrip(keyword)
                checkfuncrule(state, ch)
            break

        if not state.is_pair() and ch == '(':
            state = checkachivetype(state, ch)
            checkfuncrule(state, ch)
            break

        if not state.is_pair() and ch == ')':
            if state.is_func():
                state = checkachivetype(state, ch)
                checkfuncrule(state, ch)
                break

        if not state.is_pair() and ch == ',':
            if state.is_func() and state.achivestate == state.STATEFUNCARGVPAIRBEGIN:
                checkfuncrule(state, ch)
                break

        if state.state == state.STATECLASS and state.achivestate == state.STATECLASSACHIVEBEGIN:
            if (keyword == 'public' or keyword == 'private' or keyword == 'protected') and ch == ':':
                state.clearcache = True
                state.statechange = True

        if state.is_func():
            keyword = deletenonespacelstrip.deleteNoneSpacelstrip(keyword)
            checkfuncrule(state, keyword)

        keyword += ch
        break

    return state, keyword