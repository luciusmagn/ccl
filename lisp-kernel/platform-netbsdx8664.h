/*
 * Copyright 1994-2010 Clozure Associates
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define WORD_SIZE 64
#define PLATFORM_OS PLATFORM_OS_NETBSD
#define PLATFORM_CPU PLATFORM_CPU_X86
#define PLATFORM_WORD_SIZE PLATFORM_WORD_SIZE_64

#include <stdint.h>
#include <ucontext.h>
#include <machine/trap.h>
#include <x86/cpu_extended_state.h>

typedef ucontext_t ExceptionInformation;

#define MAXIMUM_MAPPABLE_MEMORY (512L<<30L)
#define IMAGE_BASE_ADDRESS 0x300000000000L

#include "lisptypes.h"

#define REG_RDI _REG_RDI
#define REG_RSI _REG_RSI
#define REG_RDX _REG_RDX
#define REG_RCX _REG_RCX
#define REG_R8 _REG_R8
#define REG_R9 _REG_R9
#define REG_R10 _REG_R10
#define REG_R11 _REG_R11
#define REG_R12 _REG_R12
#define REG_R13 _REG_R13
#define REG_R14 _REG_R14
#define REG_R15 _REG_R15
#define REG_RBP _REG_RBP
#define REG_RBX _REG_RBX
#define REG_RAX _REG_RAX
#define REG_GS _REG_GS
#define REG_FS _REG_FS
#define REG_ES _REG_ES
#define REG_DS _REG_DS
#define REG_TRAPNO _REG_TRAPNO
#define REG_ERR _REG_ERR
#define REG_RIP _REG_RIP
#define REG_RFL _REG_RFL
#define REG_RSP _REG_RSP
#define REG_SS _REG_SS

#include "x86-constants64.h"

#define xpGPRvector(x) ((natural *)((x)->uc_mcontext.__gregs))
#define xpGPR(x,gprno) (xpGPRvector(x)[gprno])
#define set_xpGPR(x,gpr,new) xpGPR((x),(gpr)) = (natural)(new)
#define eflags_register(xp) xpGPR(xp,Iflags)
#define xpPC(x) xpGPR(x,Iip)
#define xpFPregs(x) ((struct fxsave *)&((x)->uc_mcontext.__fpregs))
#define xpMMXreg(x,n) *((natural *)(&(xpFPregs(x)->fx_87_ac[n])))
#define xpXMMregs(x) (&(xpFPregs(x)->fx_xmm[0]))
#define xpMXCSR(x) (xpFPregs(x)->fx_mxcsr)

#define SIGNUM_FOR_INTN_TRAP SIGSEGV
#define IS_MAYBE_INT_TRAP(info,xp) \
  ((xpGPR((xp),REG_TRAPNO) == T_PROTFLT) && \
   ((xpGPR((xp),REG_ERR) & 7) == 2))
#define IS_PAGE_FAULT(info,xp) (xpGPR((xp),REG_TRAPNO) == T_PAGEFLT)
#define SIGRETURN(context)

#ifdef _UC_XSAVE
#define AVX_CONTEXT_PRESENT(xp) (((xp)->uc_flags & _UC_XSAVE) != 0)
#define AVX_CONTEXT_PTR(xp) \
  ((xp)->uc_mcontext.__fpregs.__xsave.__xsaveptr)
#define AVX_CONTEXT_SIZE(xp) \
  ((natural)((xp)->uc_mcontext.__fpregs.__xsave.__xsavelen))
#endif

#include "os-netbsd.h"
