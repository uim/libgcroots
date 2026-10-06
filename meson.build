project('libgcroots',
        'c',
        version: '0.3.5',
        meson_version: '>=0.61.0')

fs = import('fs')
pkgconfig = import('pkgconfig')

cc = meson.get_compiler('c')
host_cpu = host_machine.cpu()
host_cpu_family = host_machine.cpu_family()
host_system = host_machine.system()

# libtool's -version-info CURRENT:REVISION:AGE is shared with
# Makefile.am via libtool-version.mk.
libtool_version = ''
foreach line : fs.read('libtool-version.mk').split('\n')
  if line.startswith('libgcroots_version')
    libtool_version = line.split('=')[1].strip()
  endif
endforeach
if libtool_version == ''
  error('libgcroots_version is missing in libtool-version.mk')
endif
libtool_version_components = libtool_version.split(':')
lt_current = libtool_version_components[0].to_int()
lt_revision = libtool_version_components[1].to_int()
lt_age = libtool_version_components[2].to_int()
so_version = lt_current - lt_age
library_version = '@0@.@1@.@2@'.format(so_version, lt_age, lt_revision)
darwin_versions = [
  lt_current + 1,
  '@0@.@1@'.format(lt_current + 1, lt_revision),
]

# Definitions that are passed as -D by configure.
c_args = [
  '-DSIZEOF_VOID_P=@0@'.format(cc.sizeof('void *')),
  '-DALIGNOF_VOID_P=@0@'.format(cc.alignment('void *')),
  '-DNO_EXECUTE_PERMISSION=1',
  '-DALL_INTERIOR_POINTERS=1',
  # libgcroots always define SMALL_CONFIG to define GC_abort() as abort()
  '-DSMALL_CONFIG=1',
]

# Since glibc has a stub implementation of getcontext(3), we just
# check whether getcontext(3) exists or not like AC_CHECK_FUNCS().
# See configure.ac for details.
if cc.has_function('getcontext')
  c_args += ['-DHAVE_GETCONTEXT=1']
endif

if get_option('gc_assertions')
  c_args += ['-DGC_ASSERTIONS=1']
endif

# configure.host
if cc.get_id() in ['gcc', 'clang']
  c_args += ['-fexceptions']
endif
if host_cpu == 'mipstx39'
  c_args += ['-G', '0']
endif

# -Wall and -pedantic are controlled by Meson's warning_level option.
c_args += cc.get_supported_arguments([
  '-Wchar-subscripts',
  '-Wmissing-declarations',
  '-Wredundant-decls',
  '-Wmissing-prototypes',
  '-Wnested-externs',
  '-Wpointer-arith',
  '-Wcast-align',
  '-Wsign-compare',
  # Disable the problematic preprocessing on Mac OS X
  '-no-cpp-precomp',
])

# Darwin needs a few extra special tests to deal with variation in the
# system headers.
if host_system == 'darwin'
  if host_cpu_family in ['ppc', 'ppc64']
    thread_state_checks = [
      ['ppc_thread_state_t', 'r0', 'HAS_PPC_THREAD_STATE_R0'],
      ['ppc_thread_state_t', '__r0', 'HAS_PPC_THREAD_STATE___R0'],
      ['ppc_thread_state64_t', 'r0', 'HAS_PPC_THREAD_STATE64_R0'],
      ['ppc_thread_state64_t', '__r0', 'HAS_PPC_THREAD_STATE64___R0'],
    ]
    thread_state_prefix = '#include <mach/thread_status.h>'
  elif host_cpu_family == 'x86'
    thread_state_checks = [
      ['x86_thread_state32_t', 'eax', 'HAS_X86_THREAD_STATE32_EAX'],
      ['x86_thread_state32_t', '__eax', 'HAS_X86_THREAD_STATE32___EAX'],
    ]
    thread_state_prefix = '''
#include <sys/cdefs.h>
#include <mach/thread_status.h>
'''
  elif host_cpu_family == 'x86_64'
    thread_state_checks = [
      ['x86_thread_state64_t', 'rax', 'HAS_X86_THREAD_STATE64_RAX'],
      ['x86_thread_state64_t', '__rax', 'HAS_X86_THREAD_STATE64___RAX'],
    ]
    thread_state_prefix = '''
#include <sys/cdefs.h>
#include <mach/thread_status.h>
'''
  else
    thread_state_checks = []
    thread_state_prefix = ''
  endif
  foreach check : thread_state_checks
    if cc.has_member(check[0], check[1], prefix: thread_state_prefix)
      c_args += ['-D@0@=1'.format(check[2])]
    endif
  endforeach
endif

# We never want libdl on darwin. It is a fake libdl that just ends up making
# dyld calls anyway
dependencies = []
if host_system != 'darwin'
  dl = cc.find_library('dl', required: false)
  if dl.found() and cc.has_function('dlopen', dependencies: [dl])
    dependencies += [dl]
  endif
endif

# Configuration of machine-dependent code
sources = files(
  'gcroots.c',
  'mark.c',
  'mach_dep.c',
)
asm_sources = []
if host_cpu_family in ['sparc', 'sparc64']
  # Only 32-bit SPARC NetBSD uses sparc_netbsd_mach_dep.s like configure.ac.
  if host_cpu_family == 'sparc' and host_system == 'netbsd'
    asm_sources += files('sparc_netbsd_mach_dep.s')
  else
    asm_sources += files('sparc_mach_dep.S')
  endif
elif host_cpu_family == 'ia64'
  asm_sources += files('ia64_save_regs_in_stack.s')
elif host_cpu_family == 'x86' and host_system == 'sunos'
  c_args += ['-DSOLARIS25_PROC_VDB_BUG_FIXED=1']
endif

include_dirs = include_directories('include')

link_whole = []
if asm_sources.length() > 0
  # Assembly sources are always preprocessed like Makefile.am does.
  asm_args = []
  if cc.get_id() != 'xlc'
    # We use -Wp,-P to strip #line directives.  Irix `as' chokes on
    # these.
    asm_args += ['-Wp,-P', '-x', 'assembler-with-cpp']
  endif
  libgcroots_asm = static_library('gcroots-asm',
                                  asm_sources,
                                  c_args: c_args + asm_args,
                                  include_directories: include_dirs,
                                  pic: true)
  link_whole += [libgcroots_asm]
endif

libgcroots = library('gcroots',
                     sources,
                     c_args: c_args,
                     darwin_versions: darwin_versions,
                     dependencies: dependencies,
                     include_directories: include_dirs,
                     install: true,
                     link_whole: link_whole,
                     soversion: so_version,
                     version: library_version)
libgcroots_dep = declare_dependency(include_directories: include_dirs,
                                    link_with: libgcroots)
meson.override_dependency('gcroots', libgcroots_dep)

install_headers('include/gcroots.h')

pkgconfig.generate(libgcroots,
                   description: 'Architecture-dependent part of GC roots acquisition separated from Boehm GC',
                   filebase: 'gcroots',
                   name: 'libgcroots')
