#!/usr/bin/env python

import os.path
import sys

def makeindent(i):
    s = ""
    for i in range(i):
        s = s + '    '
    return s

def main():
    # print "start"

    CLASSNAME = ''
    OUTPUTPATH = ''

    if len(sys.argv) <= 1:
        print('addClass spacename::classname [output_path]')
        return
    if len(sys.argv) >= 2:
        CLASSNAME = sys.argv[1]
    if len(sys.argv) >= 3:
        OUTPUTPATH = sys.argv[2]

    sl = CLASSNAME.split('::')

    CLASSNAME = sl[-1]
    sl = sl[:-1]

    PATHNAME = ''
    for s in sl:
        PATHNAME = PATHNAME + s + '/'
    if PATHNAME[-1] == '/':
        PATHNAME = PATHNAME[:-1]

    DEFNAME = ''
    for s in sl:
        DEFNAME = DEFNAME + s + '_'
    if DEFNAME[-1] == '/':
        DEFNAME = DEFNAME[:-1]

    SPACENAME = ''
    for s in sl:
        SPACENAME = SPACENAME + s + '::'
    while SPACENAME[-1] == ':':
        SPACENAME = SPACENAME[:-1]

    # include and src path
    INCLUDE_PATH = os.getcwd()+'/include/'+PATHNAME
    SRC_PATH = os.getcwd()+'/src/'+PATHNAME
    if OUTPUTPATH != '':
        INCLUDE_PATH = OUTPUTPATH
        SRC_PATH = OUTPUTPATH
        #PATHNAME = '.'


    # print info
    print( PATHNAME + CLASSNAME )
    print( SPACENAME )
    print( INCLUDE_PATH )
    print( SRC_PATH )


    # make dir
    if os.path.isdir(INCLUDE_PATH) == False:
        os.makedirs(INCLUDE_PATH)
    if os.path.isdir(SRC_PATH) == False:
        os.makedirs(SRC_PATH)


    # write *.h file
    include_file_object = open(INCLUDE_PATH+'/'+CLASSNAME+'.h', 'w')
    include_file_object.write('#ifndef _'+DEFNAME+CLASSNAME+'_H\n')
    include_file_object.write('#define _'+DEFNAME+CLASSNAME+'_H_\n')

    indent = 0
    for s in sl:
        include_file_object.write(makeindent(indent)+'namespace '+s+'\n')
        include_file_object.write(makeindent(indent)+'{\n')
        indent = indent + 1

    include_file_object.write(makeindent(indent)+'class '+ CLASSNAME +'\n')      
    include_file_object.write(makeindent(indent)+'{\n')
    include_file_object.write(makeindent(indent)+'public:\n')
    indent = indent + 1
    include_file_object.write(makeindent(indent)+CLASSNAME+'(){}\n')
    include_file_object.write(makeindent(indent)+'~'+CLASSNAME+'(){}\n')
    indent = indent - 1
    include_file_object.write(makeindent(indent)+'};\n')

    for s in sl:
        indent = indent - 1    
        include_file_object.write(makeindent(indent)+'}\n')
        
    include_file_object.write('#endif //_'+DEFNAME+CLASSNAME+'_H_\n')
    include_file_object.write('\n')
    include_file_object.close()


    # write *.cpp file
    src_file_object = open(SRC_PATH+'/'+CLASSNAME+'.cpp', 'w')
    if OUTPUTPATH != '':
        src_file_object.write('#include "'+CLASSNAME+'.h"\n')
    else:
        src_file_object.write('#include <'+PATHNAME+'/'+CLASSNAME+'.h>\n')
    src_file_object.write('using namespace '+SPACENAME+';\n')
    src_file_object.write('\n')
    src_file_object.close()



if __name__ == "__main__":
    main()








