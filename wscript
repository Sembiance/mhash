import Options
from os import symlink
from os.path import exists, abspath
import os

srcdir = '.'
blddir = 'build'
mhashdir = abspath(srcdir) + "/deps/mhash"
VERSION = '0.0.1'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

  # configure mhash
  print "Configuring mhash library ..."
  cmd = "cd deps/mhash && sh configure"
  if os.system(cmd) != 0:
    conf.fatal("Configuring mhash failed.")

def build(bld):
  print "Building mhash library ..."
  cmd = "cd deps/mhash && make"
  if os.system(cmd) != 0:
    conf.fatal("Building mhash failed.")
  else:
    obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
    obj.target = 'mhash'
    obj.source = 'mhash.cc'
    obj.includes = [mhashdir + '/include']
    obj.cxxflags = ['-O2']
    obj.linkflags = [mhashdir + '/lib/.libs/libmhash.a']

def shutdown():
  # HACK to get mhash.node out of build directory.
  # better way to do this?
  if Options.commands['clean'] or Options.commands['distclean']:
    os.system("rm mhash.node")
    if Options.commands['distclean']: os.system("cd deps/mhash && make distclean")
    if Options.commands['clean']: os.system("cd deps/mhash && make clean")
  else:
    if exists('build/default/mhash.node') and not exists('mhash.node'):
      symlink('build/default/mhash.node', 'mhash.node')