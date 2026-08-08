#
# Copyright 2005-2006 Clozure Associates
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

VC_REVISION != sh -c "git describe --dirty 2>/dev/null || echo unknown"

VPATH = ..
RM = /bin/rm
M4 = m4
M4FLAGS = -DNETBSD -DX86 -DX8664 -DHAVE_TLS
CDEFINES = -DNETBSD -D_REENTRANT -D_NETBSD_SOURCE -DX86 -DX8664 \
	-DHAVE_TLS -DVC_REVISION="$(VC_REVISION)"
CDEBUG = -g
COPT = -O2
# Once in a while, -Wformat says something useful.  The odds are against that,
# however.
WFORMAT = -Wno-format
PLATFORM_H = platform-netbsdx8664.h

SPOBJ = pad.o x86-spjump64.o x86-spentry64.o x86-subprims64.o
ASMOBJ = x86-asmutils64.o imports.o

COBJ = pmcl-kernel.o gc-common.o x86-gc.o bits.o x86-exceptions.o \
	x86-utils.o image.o thread_manager.o lisp-debug.o memory.o unix-calls.o

DEBUGOBJ = lispdcmd.o plprint.o plsym.o xlbt.o x86_print.o
KERNELOBJ = $(COBJ) $(ASMOBJ)

SPINC = lisp.s m4macros.m4 x86-constants.s x86-macros.s errors.s x86-uuo.s \
	x86-constants64.s lisp_globals.s

CHEADERS = area.h bits.h x86-constants.h lisp-errors.h gc.h lisp.h \
	lisp-exceptions.h lisp_globals.h macros.h memprotect.h image.h \
	threads.h x86-constants64.h x86-exceptions.h lisptypes.h x86-utils.h \
	$(PLATFORM_H) constants.h os-netbsd.h

KSPOBJ = $(SPOBJ)
all:	../../nx86cl64

OSLIBS = -lm -lpthread

.s.o:
	$(M4) $(M4FLAGS) -I../ $< | $(CC) -c -g -x assembler -o $@ -
.c.o:
	$(CC) -include ../$(PLATFORM_H) -c $< $(CDEFINES) $(CDEBUG) $(COPT) \
		$(WFORMAT) -m64 -o $@

../../nx86cl64: $(KSPOBJ) $(KERNELOBJ) $(DEBUGOBJ)
	$(CC) -m64 $(CDEBUG) -Wl,--export-dynamic -o $@ $(KSPOBJ) \
		$(KERNELOBJ) $(DEBUGOBJ) $(OSLIBS)
	/usr/sbin/paxctl +m +a $@

$(SPOBJ): $(SPINC)
$(ASMOBJ): $(SPINC)
$(COBJ): $(CHEADERS)
$(DEBUGOBJ): $(CHEADERS) lispdcmd.h

cclean:
	$(RM) -f $(KERNELOBJ) $(DEBUGOBJ) ../../nx86cl64

clean: cclean
	$(RM) -f $(SPOBJ)

strip: ../../nx86cl64
	strip -g ../../nx86cl64
