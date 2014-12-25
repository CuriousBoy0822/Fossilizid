# 2014-12-18
# build by qianqians
# statemachine

import ruletable

class statemachine(object):
    def __init__(self):
        self.strlist = ['']
        self.keyword = ''
        self.state = ruletable.state(None)

        self.submachine = None

    def push(self, ch):
        if ch != '\\':
            self.strlist[-1].join(ch)
            self.state, self.keyword = ruletable.ruletable(self.state, self.keyword, ch)

        return self.state