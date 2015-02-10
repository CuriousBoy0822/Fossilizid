# 2014-12-24
# build by qianqians
# rpcmake

import os

import deletenote
import statemachine
import codegen

def traversalclass(dirpath):
    filelist = {}

    for filename in os.listdir(dirpath):
        fname = os.path.splitext(filename)[0]
        fex = os.path.splitext(filename)[1]
        if fex == '.juggle':
            file = open(dirpath+filename, 'r')
            genfilestr = deletenote.deletenote(file.readlines())

            smc = statemachine.statemachine()
            smc.syntaxanalysis(genfilestr)
            module = smc.getmodule()
            struct = smc.getstruct()

            filelist[fname] = {}
            filelist[fname]['module'] = module
            filelist[fname]['struct'] = struct

    codegen.codegenclient(filelist)
    codegen.codegenserver(filelist)
    codegen.codegenstruct(filelist)

if __name__ == '__main__':
    import sys
    codegen.build_path = sys.argv[2]
    codegen.achieve_object = sys.argv[3]
    codegen.achieve_include = sys.argv[4]
    traversalclass(sys.argv[1])
