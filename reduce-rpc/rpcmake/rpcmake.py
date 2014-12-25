# 2014-12-24
# build by qianqians
# rpcmake

import os

import deletenote
import syntaxanalysis
import clientcodegen

def traversalclass(dirpath):
    genfilestrs = {}
    rpcsysmbal = {}

    for filename in os.listdir(dirpath):
        fex = os.path.splitext(filename)[1]
        if fex == ".h" or fex == ".hpp" or fex == ".cpp" or fex == ".cc" or fex == ".c":
            file = open(dirpath+filename, "r")
            genfilestr = deletenote.deletenote(file.readlines())
            genfilestrs[filename] = genfilestr
            tmprpc = syntaxanalysis.syntaxanalysis(genfilestr)
            rpcsysmbal[filename] = tmprpc

    #for filename, genfilestr in genfilestrs.items():
    #    genrealize(rpcsysmbal, genfilestr)

    print rpcsysmbal
    return rpcsysmbal

def make(path):
    clientcodegen.codegenclient(traversalclass(path))

make('./')