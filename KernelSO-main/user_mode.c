#include "user_mode.h"
#include "gdt.h"
#include "kernel_debug.h"
#include "paging.h"

/*
 * Minimal static user stack for the chapter 11 transition demo.
 * This is not a full process model yet (no per-process paging/CR3 switch).
 */
extern unsigned char _binary_user_user_bin_start[];
extern unsigned char _binary_user_user_bin_end[];

static unsigned char user_code_page[USER_CODE_SIZE] __attribute__((aligned(PAGE_SIZE)));
static unsigned char user_stack[USER_STACK_SIZE] __attribute__((aligned(PAGE_SIZE)));

static void user_mode_copy_payload(void)
{
    unsigned int i;
    unsigned int payload_size =
        (unsigned int)(_binary_user_user_bin_end - _binary_user_user_bin_start);

    for (i = 0; i < USER_CODE_SIZE; i++) {
        user_code_page[i] = 0x90; /* NOP padding */
    }

    if (payload_size > USER_CODE_SIZE) {
        payload_size = USER_CODE_SIZE;
    }

    for (i = 0; i < payload_size; i++) {
        user_code_page[i] = _binary_user_user_bin_start[i];
    }
}

void user_mode_prepare_process(struct user_process *proc,
                               unsigned int entry_point,
                               unsigned int user_stack_top)
{
    if (proc == 0) {
        return;
    }

    proc->entry_point = entry_point;
    proc->user_stack_top = user_stack_top;
}

void user_mode_start_demo(void)
{
    struct user_process proc;
    struct user_paging_context paging_ctx;

    KDEBUG_LOG("debug: user payload copy\n");
    user_mode_copy_payload();

    user_mode_prepare_process(&proc, USER_CODE_VIRT_BASE, USER_VIRT_STACK_TOP);

    paging_build_user_demo(&paging_ctx,
                           (unsigned int)user_code_page,
                           proc.entry_point,
                           (unsigned int)user_stack,
                           proc.user_stack_top);

    proc.page_directory_phys = paging_ctx.directory_phys;
    proc.entry_point = paging_ctx.user_code_virt;
    proc.user_stack_top = paging_ctx.user_stack_top;

    KDEBUG_LOG("debug: paging on\n");
    paging_activate(proc.page_directory_phys);

    KDEBUG_LOG("debug: iret to ring3\n");
    enter_user_mode(proc.entry_point, proc.user_stack_top);

    while (1) {
        /* Should never return from enter_user_mode. */
    }
}
