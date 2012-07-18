/*
 * check-name: find interesting items for symbol
 * check-command: undertaker -j checkexpr -m ../kconfig-dumps/models/x86.model 'CONFIG_SMP'
 * check-output-start
I: loaded rsf model for x86
I: Using x86 as primary model
CONFIG_ANON_INODES=y
CONFIG_ARCH_HAS_CACHE_LINE_SIZE=y
CONFIG_ARCH_HAS_CPU_IDLE_WAIT=y
CONFIG_ARCH_HAS_CPU_RELAX=y
CONFIG_ARCH_HAS_DEFAULT_IDLE=y
CONFIG_ARCH_HIBERNATION_POSSIBLE=y
CONFIG_ARCH_MAY_HAVE_PC_FDC=y
CONFIG_ARCH_POPULATES_NODE_MAP=y
CONFIG_ARCH_SUPPORTS_DEBUG_PAGEALLOC=y
CONFIG_ARCH_SUPPORTS_OPTIMIZED_INLINING=y
CONFIG_ARCH_SUSPEND_POSSIBLE=y
CONFIG_ARCH_WANT_FRAME_POINTERS=y
CONFIG_ARCH_WANT_OPTIONAL_GPIOLIB=y
CONFIG_CLOCKSOURCE_WATCHDOG=y
CONFIG_DEFAULT_SECURITY_APPARMOR=n
CONFIG_DEFAULT_SECURITY_DAC=n
CONFIG_DEFAULT_SECURITY_SELINUX=y
CONFIG_DEFAULT_SECURITY_SMACK=n
CONFIG_DEFAULT_SECURITY_TOMOYO=n
CONFIG_DYNAMIC_FTRACE=y
CONFIG_GENERIC_CALIBRATE_DELAY=y
CONFIG_GENERIC_CLOCKEVENTS=y
CONFIG_GENERIC_CMOS_UPDATE=y
CONFIG_GENERIC_FIND_FIRST_BIT=y
CONFIG_GENERIC_FIND_LAST_BIT=y
CONFIG_GENERIC_FIND_NEXT_BIT=y
CONFIG_GENERIC_HWEIGHT=y
CONFIG_GENERIC_IOMAP=y
CONFIG_GENERIC_IRQ_PROBE=y
CONFIG_GENERIC_ISA_DMA=y
CONFIG_GENERIC_PENDING_IRQ=y
CONFIG_HAVE_AOUT=y
CONFIG_HAVE_ARCH_JUMP_LABEL=y
CONFIG_HAVE_ARCH_KGDB=y
CONFIG_HAVE_ARCH_KMEMCHECK=y
CONFIG_HAVE_ARCH_TRACEHOOK=y
CONFIG_HAVE_C_RECORDMCOUNT=y
CONFIG_HAVE_DMA_API_DEBUG=y
CONFIG_HAVE_DMA_ATTRS=y
CONFIG_HAVE_DYNAMIC_FTRACE=y
CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS=y
CONFIG_HAVE_FTRACE_MCOUNT_RECORD=y
CONFIG_HAVE_FTRACE_NMI_ENTER=y
CONFIG_HAVE_FUNCTION_GRAPH_FP_TEST=y
CONFIG_HAVE_FUNCTION_GRAPH_TRACER=y
CONFIG_HAVE_FUNCTION_TRACER=y
CONFIG_HAVE_FUNCTION_TRACE_MCOUNT_TEST=y
CONFIG_HAVE_GENERIC_DMA_COHERENT=y
CONFIG_HAVE_GENERIC_HARDIRQS=y
CONFIG_HAVE_HW_BREAKPOINT=y
CONFIG_HAVE_IDE=y
CONFIG_HAVE_IOREMAP_PROT=y
CONFIG_HAVE_IRQ_WORK=y
CONFIG_HAVE_KERNEL_BZIP2=y
CONFIG_HAVE_KERNEL_GZIP=y
CONFIG_HAVE_KERNEL_LZMA=y
CONFIG_HAVE_KERNEL_LZO=y
CONFIG_HAVE_KPROBES=y
CONFIG_HAVE_KRETPROBES=y
CONFIG_HAVE_KVM=y
CONFIG_HAVE_LATENCYTOP_SUPPORT=y
CONFIG_HAVE_MEMBLOCK=y
CONFIG_HAVE_MIXED_BREAKPOINTS_REGS=y
CONFIG_HAVE_MMIOTRACE_SUPPORT=y
CONFIG_HAVE_OPROFILE=y
CONFIG_HAVE_OPTPROBES=y
CONFIG_HAVE_PERF_EVENTS=y
CONFIG_HAVE_PERF_EVENTS_NMI=y
CONFIG_HAVE_REGS_AND_STACK_ACCESS_API=y
CONFIG_HAVE_SETUP_PER_CPU_AREA=y
CONFIG_HAVE_SPARSE_IRQ=y
CONFIG_HAVE_SYSCALL_TRACEPOINTS=y
CONFIG_HAVE_TEXT_POKE_SMP=y
CONFIG_HAVE_UNSTABLE_SCHED_CLOCK=y
CONFIG_HAVE_USER_RETURN_NOTIFIER=y
CONFIG_HZ_100=y
CONFIG_HZ_1000=n
CONFIG_HZ_250=n
CONFIG_HZ_300=n
CONFIG_IO_DELAY_0X80=y
CONFIG_IO_DELAY_0XED=n
CONFIG_IO_DELAY_NONE=n
CONFIG_IO_DELAY_UDELAY=n
CONFIG_ISA_DMA_API=y
CONFIG_LOCKDEP_SUPPORT=y
CONFIG_MMU=y
CONFIG_NEED_PER_CPU_EMBED_FIRST_CHUNK=y
CONFIG_NEED_PER_CPU_PAGE_FIRST_CHUNK=y
CONFIG_NEED_SG_DMA_LENGTH=y
CONFIG_NO_BOOTMEM=y
CONFIG_PERF_EVENTS=y
CONFIG_PREEMPT=n
CONFIG_PREEMPT_NONE=y
CONFIG_PREEMPT_VOLUNTARY=n
CONFIG_SLAB=y
CONFIG_SLOB=n
CONFIG_SLUB=n
CONFIG_SMP=y
CONFIG_STACKTRACE_SUPPORT=y
CONFIG_TINY_PREEMPT_RCU=n
CONFIG_TINY_RCU=n
CONFIG_TRACE_IRQFLAGS_SUPPORT=y
CONFIG_TREE_PREEMPT_RCU=n
CONFIG_TREE_RCU=y
CONFIG_USER_STACKTRACE_SUPPORT=y
CONFIG_X86=y
CONFIG_X86_32=y
CONFIG_X86_CPU=y
CONFIG_ZONE_DMA=y
 * check-output-end
 */
