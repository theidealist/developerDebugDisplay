###############################################################################
## @file      SConscript
## @author    Chris L Baker (clb) <chris@chimail.net>
## @date      2014.05.20
## @brief     SConscript for Developer Debug Display lib
##
## @attention Copyright (C) 2014
## @attention All rights reserved
###############################################################################

# to get "HOME"
import os
import SCons.Util

# where we should install stuff
libDir = os.environ['HOME'] + '/Installed/lib/'
binDir = os.environ['HOME'] + '/Installed/bin/'
includeDir = os.environ['HOME'] + '/Installed/include/'

# init our construction env
env = Environment(

    # crap to get qt to work
    tools = ['default', 'qt'],
    QTDIR = '/usr',
    QT_MOC = '/usr/bin/moc-qt4',
    QT_UIC = '/usr/bin/uic-qt4',
    QT_UICCOM = [
        SCons.Util.CLVar('$QT_UIC $QT_UICDECLFLAGS -o ${TARGETS[0]} $SOURCE'),
        SCons.Util.CLVar('touch ${TARGETS[1]}'),
        SCons.Util.CLVar('$QT_MOC $QT_MOCFROMHFLAGS -o ${TARGETS[2]} ${TARGETS[0]}')
        ],
    QT_MOCFROMHFLAGS = '-I/usr/include/qt4 -DQT_NO_KEYWORDS',

    # regular c++ stuff
    CCFLAGS = [ '-Wempty-body', '-Wall', '-Wshadow', '-Winit-self', '-O2' ],
    CPPFLAGS = [ '-Wno-deprecated' ],
    CXXFLAGS = [
        '-lstdc++',
        '-std=c++0x',
        '-isystem/usr/include/qwt',
        '-isystem/usr/include/qt4',
        '-isystem/usr/include/qt4/Qt',
        '-isystem/usr/include/qwt',
        '-DQT_NO_KEYWORDS',
        ],
    LINKFLAGS = [ '-Wl,-E', '-Wl,--no-as-needed,--copy-dt-needed', '-pthread' ],
    CPPPATH = [ includeDir ],
    LIBPATH = [ libDir ],
    )

# store our install locations
env.libDir = libDir
env.binDir = binDir
env.includeDir = includeDir

# speed things up a little
env.Decider('MD5-timestamp')       # If the timestamp on the file matches, don't compute the MD5 (like make)
env.SetOption('max_drift', 10)     # cache the checksum after just 10s instead of 2 days
env.SetOption('implicit_cache', 1) # cache implicit dependencies

# standard way to install libs
def InstallLib(lib):
    global env
    env.Alias('libs', env.Install(env.libDir, lib))
    env.Alias('all',  env.Install(env.libDir, lib))
    env.Alias(str(lib[0]), env.Install(env.binDir, lib))
env.InstallLib = InstallLib

# standard way to install headers
def InstallHeaders(libName, headers):
    global env
    env.Alias('libs', env.Install(env.includeDir + '/' + libName, headers))
    env.Alias('all', env.Install(env.includeDir + '/' + libName, headers))
env.InstallHeaders = InstallHeaders

# standard way to install apps
def InstallApp(app):
    global env
    env.Alias('apps', env.Install(env.binDir, app))
    env.Alias('all',  env.Install(env.binDir, app))
    env.Alias(str(app[0]), env.Install(env.binDir, app))
env.InstallApp = InstallApp

# standard way to install tests
def InstallTest(test):
    global env
    env.Alias('tests', env.Install(env.binDir, test))
    env.Alias('all',   env.Install(env.binDir, test))
    env.Alias(str(test[0]), env.Install(env.binDir, test))
env.InstallTest = InstallTest

# build subdirs
def BuildIt(dir):
    if os.path.exists( 'SConstruct' ):
        env.SConscript( dir + '/SConscript', variant_dir=dir + '/.obj', duplicate=False)
    else:
        env.SConscript(dir + '/SConscript', variant_dir='../' + dir + '/.obj', duplicate=False)
env.BuildIt = BuildIt

# export this for other SConscripts
Export('env')

# build this dir
BuildIt('.')
