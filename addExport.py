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

    if len(sys.argv) <= 1:
        print('addExport spacename')
        return
    if len(sys.argv) >= 2:
        CLASSNAME = sys.argv[1]

    sl = CLASSNAME.split('::')
    # CLASSNAME = sl[-1]
    # sl = sl[:-1]

    PATHNAME = ''
    for s in sl:
        PATHNAME = PATHNAME + s + '/'

    DEFNAME = ''
    for s in sl:
        DEFNAME = DEFNAME + s + '_'

    SPACENAME = ''
    for s in sl:
        SPACENAME = SPACENAME + s + '::'

    # include path
    INCLUDE_PATH = os.getcwd()+'/include/'+PATHNAME


    # print info
    print( 'PATHNAME = ' + PATHNAME )
    print( 'CLASSNAME = ' + CLASSNAME )
    print( 'SPACENAME = ' + SPACENAME )
    print( 'INCLUDE_PATH = ' + INCLUDE_PATH )


    # make dir
    if os.path.isdir(INCLUDE_PATH) == False:
        os.makedirs(INCLUDE_PATH)


    # write Export.h file
    include_file_object = open(INCLUDE_PATH+'/'+'Export.h', 'w')
    include_file_object.write('#ifndef _'+DEFNAME+'EXPORT_H\n')
    include_file_object.write('#define _'+DEFNAME+'EXPORT_H_\n')

    include_file_object.write('\n')
        
    include_file_object.write('#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)\n')
    include_file_object.write('#if defined(EGG_LIBRARY_STATIC)\n')
    include_file_object.write('#   define EGG_RUNTIME_EXPORT\n')
    include_file_object.write('#elif defined(EGG_RUNTIME_LIBRARY)\n')
    include_file_object.write('#   define EGG_RUNTIME_EXPORT __declspec(dllexport)\n')
    include_file_object.write('#else\n')
    include_file_object.write('#   define EGG_RUNTIME_EXPORT __declspec(dllimport)\n')
    include_file_object.write('#endif\n')
    include_file_object.write('#else\n')
    include_file_object.write('#define EGG_RUNTIME_EXPORT\n')
    include_file_object.write('#endif\n')

    include_file_object.write('\n')

    include_file_object.write('#endif //_'+DEFNAME+'EXPORT_H_\n')
    include_file_object.write('\n')
    include_file_object.close()




if __name__ == "__main__":
    main()








