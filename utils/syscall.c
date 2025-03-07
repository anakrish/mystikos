#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>

#include <myst/defs.h>
#include <myst/syscall.h>
#include <myst/syscallext.h>

#define PAIR(SYSCALL)     \
    {                     \
        SYSCALL, #SYSCALL \
    }

static const myst_syscall_pair_t _pairs[] = {
    PAIR(SYS_read),
    PAIR(SYS_write),
    PAIR(SYS_open),
    PAIR(SYS_close),
    PAIR(SYS_stat),
    PAIR(SYS_fstat),
    PAIR(SYS_lstat),
    PAIR(SYS_poll),
    PAIR(SYS_lseek),
    PAIR(SYS_mmap),
    PAIR(SYS_mprotect),
    PAIR(SYS_munmap),
    PAIR(SYS_brk),
    PAIR(SYS_rt_sigaction),
    PAIR(SYS_rt_sigprocmask),
    PAIR(SYS_rt_sigreturn),
    PAIR(SYS_ioctl),
    PAIR(SYS_pread64),
    PAIR(SYS_pwrite64),
    PAIR(SYS_readv),
    PAIR(SYS_writev),
    PAIR(SYS_access),
    PAIR(SYS_pipe),
    PAIR(SYS_select),
    PAIR(SYS_sched_yield),
    PAIR(SYS_mremap),
    PAIR(SYS_msync),
    PAIR(SYS_mincore),
    PAIR(SYS_madvise),
    PAIR(SYS_shmget),
    PAIR(SYS_shmat),
    PAIR(SYS_shmctl),
    PAIR(SYS_dup),
    PAIR(SYS_dup2),
    PAIR(SYS_pause),
    PAIR(SYS_nanosleep),
    PAIR(SYS_getitimer),
    PAIR(SYS_alarm),
    PAIR(SYS_setitimer),
    PAIR(SYS_getpid),
    PAIR(SYS_sendfile),
    PAIR(SYS_socket),
    PAIR(SYS_connect),
    PAIR(SYS_accept),
    PAIR(SYS_sendto),
    PAIR(SYS_recvfrom),
    PAIR(SYS_sendmsg),
    PAIR(SYS_recvmsg),
    PAIR(SYS_shutdown),
    PAIR(SYS_bind),
    PAIR(SYS_listen),
    PAIR(SYS_getsockname),
    PAIR(SYS_getpeername),
    PAIR(SYS_socketpair),
    PAIR(SYS_setsockopt),
    PAIR(SYS_getsockopt),
    PAIR(SYS_clone),
    PAIR(SYS_fork),
    PAIR(SYS_vfork),
    PAIR(SYS_execve),
    PAIR(SYS_exit),
    PAIR(SYS_wait4),
    PAIR(SYS_kill),
    PAIR(SYS_uname),
    PAIR(SYS_semget),
    PAIR(SYS_semop),
    PAIR(SYS_semctl),
    PAIR(SYS_shmdt),
    PAIR(SYS_msgget),
    PAIR(SYS_msgsnd),
    PAIR(SYS_msgrcv),
    PAIR(SYS_msgctl),
    PAIR(SYS_fcntl),
    PAIR(SYS_flock),
    PAIR(SYS_fsync),
    PAIR(SYS_fdatasync),
    PAIR(SYS_truncate),
    PAIR(SYS_ftruncate),
    PAIR(SYS_getdents),
    PAIR(SYS_getcwd),
    PAIR(SYS_chdir),
    PAIR(SYS_fchdir),
    PAIR(SYS_rename),
    PAIR(SYS_mkdir),
    PAIR(SYS_rmdir),
    PAIR(SYS_creat),
    PAIR(SYS_link),
    PAIR(SYS_unlink),
    PAIR(SYS_symlink),
    PAIR(SYS_readlink),
    PAIR(SYS_chmod),
    PAIR(SYS_fchmod),
    PAIR(SYS_chown),
    PAIR(SYS_fchown),
    PAIR(SYS_lchown),
    PAIR(SYS_umask),
    PAIR(SYS_gettimeofday),
    PAIR(SYS_getrlimit),
    PAIR(SYS_getrusage),
    PAIR(SYS_sysinfo),
    PAIR(SYS_times),
    PAIR(SYS_ptrace),
    PAIR(SYS_getuid),
    PAIR(SYS_syslog),
    PAIR(SYS_getgid),
    PAIR(SYS_setuid),
    PAIR(SYS_setgid),
    PAIR(SYS_geteuid),
    PAIR(SYS_getegid),
    PAIR(SYS_setpgid),
    PAIR(SYS_getppid),
    PAIR(SYS_getpgrp),
    PAIR(SYS_setsid),
    PAIR(SYS_setreuid),
    PAIR(SYS_setregid),
    PAIR(SYS_getgroups),
    PAIR(SYS_setgroups),
    PAIR(SYS_setresuid),
    PAIR(SYS_getresuid),
    PAIR(SYS_setresgid),
    PAIR(SYS_getresgid),
    PAIR(SYS_getpgid),
    PAIR(SYS_setfsuid),
    PAIR(SYS_setfsgid),
    PAIR(SYS_getsid),
    PAIR(SYS_capget),
    PAIR(SYS_capset),
    PAIR(SYS_rt_sigpending),
    PAIR(SYS_rt_sigtimedwait),
    PAIR(SYS_rt_sigqueueinfo),
    PAIR(SYS_rt_sigsuspend),
    PAIR(SYS_sigaltstack),
    PAIR(SYS_utime),
    PAIR(SYS_mknod),
    PAIR(SYS_uselib),
    PAIR(SYS_personality),
    PAIR(SYS_ustat),
    PAIR(SYS_statfs),
    PAIR(SYS_fstatfs),
    PAIR(SYS_sysfs),
    PAIR(SYS_getpriority),
    PAIR(SYS_setpriority),
    PAIR(SYS_sched_setparam),
    PAIR(SYS_sched_getparam),
    PAIR(SYS_sched_setscheduler),
    PAIR(SYS_sched_getscheduler),
    PAIR(SYS_sched_get_priority_max),
    PAIR(SYS_sched_get_priority_min),
    PAIR(SYS_sched_rr_get_interval),
    PAIR(SYS_mlock),
    PAIR(SYS_munlock),
    PAIR(SYS_mlockall),
    PAIR(SYS_munlockall),
    PAIR(SYS_vhangup),
    PAIR(SYS_modify_ldt),
    PAIR(SYS_pivot_root),
    PAIR(SYS__sysctl),
    PAIR(SYS_prctl),
    PAIR(SYS_arch_prctl),
    PAIR(SYS_adjtimex),
    PAIR(SYS_setrlimit),
    PAIR(SYS_chroot),
    PAIR(SYS_sync),
    PAIR(SYS_acct),
    PAIR(SYS_settimeofday),
    PAIR(SYS_mount),
    PAIR(SYS_umount2),
    PAIR(SYS_swapon),
    PAIR(SYS_swapoff),
    PAIR(SYS_reboot),
    PAIR(SYS_sethostname),
    PAIR(SYS_setdomainname),
    PAIR(SYS_iopl),
    PAIR(SYS_ioperm),
    PAIR(SYS_create_module),
    PAIR(SYS_init_module),
    PAIR(SYS_delete_module),
    PAIR(SYS_get_kernel_syms),
    PAIR(SYS_query_module),
    PAIR(SYS_quotactl),
    PAIR(SYS_nfsservctl),
    PAIR(SYS_getpmsg),
    PAIR(SYS_putpmsg),
    PAIR(SYS_afs_syscall),
    PAIR(SYS_tuxcall),
    PAIR(SYS_security),
    PAIR(SYS_gettid),
    PAIR(SYS_readahead),
    PAIR(SYS_setxattr),
    PAIR(SYS_lsetxattr),
    PAIR(SYS_fsetxattr),
    PAIR(SYS_getxattr),
    PAIR(SYS_lgetxattr),
    PAIR(SYS_fgetxattr),
    PAIR(SYS_listxattr),
    PAIR(SYS_llistxattr),
    PAIR(SYS_flistxattr),
    PAIR(SYS_removexattr),
    PAIR(SYS_lremovexattr),
    PAIR(SYS_fremovexattr),
    PAIR(SYS_tkill),
    PAIR(SYS_time),
    PAIR(SYS_futex),
    PAIR(SYS_sched_setaffinity),
    PAIR(SYS_sched_getaffinity),
    PAIR(SYS_set_thread_area),
    PAIR(SYS_io_setup),
    PAIR(SYS_io_destroy),
    PAIR(SYS_io_getevents),
    PAIR(SYS_io_submit),
    PAIR(SYS_io_cancel),
    PAIR(SYS_get_thread_area),
    PAIR(SYS_lookup_dcookie),
    PAIR(SYS_epoll_create),
    PAIR(SYS_epoll_ctl_old),
    PAIR(SYS_epoll_wait_old),
    PAIR(SYS_remap_file_pages),
    PAIR(SYS_getdents64),
    PAIR(SYS_set_tid_address),
    PAIR(SYS_restart_syscall),
    PAIR(SYS_semtimedop),
    PAIR(SYS_fadvise64),
    PAIR(SYS_timer_create),
    PAIR(SYS_timer_settime),
    PAIR(SYS_timer_gettime),
    PAIR(SYS_timer_getoverrun),
    PAIR(SYS_timer_delete),
    PAIR(SYS_clock_settime),
    PAIR(SYS_clock_gettime),
    PAIR(SYS_clock_getres),
    PAIR(SYS_clock_nanosleep),
    PAIR(SYS_exit_group),
    PAIR(SYS_epoll_wait),
    PAIR(SYS_epoll_ctl),
    PAIR(SYS_tgkill),
    PAIR(SYS_utimes),
    PAIR(SYS_vserver),
    PAIR(SYS_mbind),
    PAIR(SYS_set_mempolicy),
    PAIR(SYS_get_mempolicy),
    PAIR(SYS_mq_open),
    PAIR(SYS_mq_unlink),
    PAIR(SYS_mq_timedsend),
    PAIR(SYS_mq_timedreceive),
    PAIR(SYS_mq_notify),
    PAIR(SYS_mq_getsetattr),
    PAIR(SYS_kexec_load),
    PAIR(SYS_waitid),
    PAIR(SYS_add_key),
    PAIR(SYS_request_key),
    PAIR(SYS_keyctl),
    PAIR(SYS_ioprio_set),
    PAIR(SYS_ioprio_get),
    PAIR(SYS_inotify_init),
    PAIR(SYS_inotify_add_watch),
    PAIR(SYS_inotify_rm_watch),
    PAIR(SYS_migrate_pages),
    PAIR(SYS_openat),
    PAIR(SYS_mkdirat),
    PAIR(SYS_mknodat),
    PAIR(SYS_fchownat),
    PAIR(SYS_futimesat),
    PAIR(SYS_newfstatat),
    PAIR(SYS_unlinkat),
    PAIR(SYS_renameat),
    PAIR(SYS_linkat),
    PAIR(SYS_symlinkat),
    PAIR(SYS_readlinkat),
    PAIR(SYS_fchmodat),
    PAIR(SYS_faccessat),
    PAIR(SYS_pselect6),
    PAIR(SYS_ppoll),
    PAIR(SYS_unshare),
    PAIR(SYS_set_robust_list),
    PAIR(SYS_get_robust_list),
    PAIR(SYS_splice),
    PAIR(SYS_tee),
    PAIR(SYS_sync_file_range),
    PAIR(SYS_vmsplice),
    PAIR(SYS_move_pages),
    PAIR(SYS_utimensat),
    PAIR(SYS_epoll_pwait),
    PAIR(SYS_signalfd),
    PAIR(SYS_timerfd_create),
    PAIR(SYS_eventfd),
    PAIR(SYS_fallocate),
    PAIR(SYS_timerfd_settime),
    PAIR(SYS_timerfd_gettime),
    PAIR(SYS_accept4),
    PAIR(SYS_signalfd4),
    PAIR(SYS_eventfd2),
    PAIR(SYS_epoll_create1),
    PAIR(SYS_dup3),
    PAIR(SYS_pipe2),
    PAIR(SYS_inotify_init1),
    PAIR(SYS_preadv),
    PAIR(SYS_pwritev),
    PAIR(SYS_rt_tgsigqueueinfo),
    PAIR(SYS_perf_event_open),
    PAIR(SYS_recvmmsg),
    PAIR(SYS_fanotify_init),
    PAIR(SYS_fanotify_mark),
    PAIR(SYS_prlimit64),
    PAIR(SYS_name_to_handle_at),
    PAIR(SYS_open_by_handle_at),
    PAIR(SYS_clock_adjtime),
    PAIR(SYS_syncfs),
    PAIR(SYS_sendmmsg),
    PAIR(SYS_setns),
    PAIR(SYS_getcpu),
    PAIR(SYS_process_vm_readv),
    PAIR(SYS_process_vm_writev),
    PAIR(SYS_kcmp),
    PAIR(SYS_finit_module),
    PAIR(SYS_sched_setattr),
    PAIR(SYS_sched_getattr),
    PAIR(SYS_renameat2),
    PAIR(SYS_seccomp),
    PAIR(SYS_getrandom),
    PAIR(SYS_memfd_create),
    PAIR(SYS_kexec_file_load),
    PAIR(SYS_bpf),
    PAIR(SYS_execveat),
    PAIR(SYS_userfaultfd),
    PAIR(SYS_membarrier),
    PAIR(SYS_mlock2),
    PAIR(SYS_copy_file_range),
    PAIR(SYS_preadv2),
    PAIR(SYS_pwritev2),
    PAIR(SYS_pkey_mprotect),
    PAIR(SYS_pkey_alloc),
    PAIR(SYS_pkey_free),
    PAIR(SYS_statx),
    PAIR(SYS_io_pgetevents),
    PAIR(SYS_rseq),
    PAIR(SYS_pidfd_send_signal),
    PAIR(SYS_io_uring_setup),
    PAIR(SYS_io_uring_enter),
    PAIR(SYS_io_uring_register),
    PAIR(SYS_open_tree),
    PAIR(SYS_move_mount),
    PAIR(SYS_fsopen),
    PAIR(SYS_fsconfig),
    PAIR(SYS_fsmount),
    PAIR(SYS_fspick),
    PAIR(SYS_pidfd_open),
    PAIR(SYS_clone3),
    PAIR(SYS_myst_trace),
    PAIR(SYS_myst_trace_ptr),
    PAIR(SYS_myst_dump_stack),
    PAIR(SYS_myst_dump_ehdr),
    PAIR(SYS_myst_dump_argv),
    PAIR(SYS_myst_add_symbol_file),
    PAIR(SYS_myst_load_symbols),
    PAIR(SYS_myst_unload_symbols),
    PAIR(SYS_myst_clone),
    PAIR(SYS_myst_poll_wake),
    PAIR(SYS_myst_run_itimer),
    PAIR(SYS_myst_start_shell),
    PAIR(SYS_myst_gcov),
    PAIR(SYS_myst_unmap_on_exit),
    PAIR(SYS_myst_get_fork_info),
    PAIR(SYS_myst_kill_wait_child_forks),
    PAIR(SYS_myst_get_process_thread_stack),
    PAIR(SYS_myst_fork_wait_exec_exit),
    PAIR(SYS_myst_get_exec_stack_option),
    PAIR(SYS_myst_interrupt_thread),
    PAIR(SYS_myst_pre_launch_hook),
    /* add new entries here! */
    {0, NULL},
};

__attribute__((__unused__)) static void _check_myst_syscalls(void)
{
    myst_syscall_t msyscall = SYS_myst_trace;

    /* Please add new entries to the _pairs[] array above as well */
    switch (msyscall)
    {
        case SYS_myst_trace:
        case SYS_myst_trace_ptr:
        case SYS_myst_dump_stack:
        case SYS_myst_dump_ehdr:
        case SYS_myst_dump_argv:
        case SYS_myst_add_symbol_file:
        case SYS_myst_load_symbols:
        case SYS_myst_unload_symbols:
        case SYS_myst_clone:
        case SYS_myst_poll_wake:
        case SYS_myst_run_itimer:
        case SYS_myst_start_shell:
        case SYS_myst_gcov:
        case SYS_myst_unmap_on_exit:
        case SYS_myst_get_fork_info:
        case SYS_myst_kill_wait_child_forks:
        case SYS_myst_get_process_thread_stack:
        case SYS_myst_fork_wait_exec_exit:
        case SYS_myst_get_exec_stack_option:
        case SYS_myst_interrupt_thread:
        case SYS_myst_pre_launch_hook:
            break;
    }
}

const char* myst_syscall_name(long num)
{
    for (size_t i = 0; _pairs[i].name; i++)
    {
        if (_pairs[i].num == num)
            return _pairs[i].name;
    }

    /* not found */
    return NULL;
}

long myst_syscall_num(const char* name)
{
    for (size_t i = 0; _pairs[i].name; i++)
    {
        if (strcmp(_pairs[i].name, name) == 0)
            return _pairs[i].num;
    }

    /* not found */
    return -ENOENT;
}

const myst_syscall_pair_t* myst_syscall_pairs(void)
{
    return _pairs;
}
