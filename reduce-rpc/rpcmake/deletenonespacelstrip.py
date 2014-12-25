# 2014-12-18
# build by qianqians
# deletenonespacelstrip

def deleteNoneSpacelstrip(str):
    while(str.lstrip('\t') is not str):str = str.lstrip('\t')
    while(str.lstrip('\0') is not str):str = str.lstrip('\0')
    while(str.lstrip('\t') is not str):str = str.lstrip('\t')
    while(str.lstrip('	') is not str):str = str.lstrip('	')
    while(str.lstrip(' ') is not str):str = str.lstrip(' ')
    while(str.lstrip('	') is not str):str = str.lstrip('	')
    return str