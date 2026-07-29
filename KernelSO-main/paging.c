#include "paging.h"

extern void paging_load_directory(unsigned int page_directory_phys);
extern void paging_enable(void);

/* Static paging structures for a minimal user-mode demo. */
static unsigned int page_directory[1024] __attribute__((aligned(PAGE_SIZE)));
static unsigned int low_page_table[1024] __attribute__((aligned(PAGE_SIZE)));
static unsigned int user_code_page_table[1024] __attribute__((aligned(PAGE_SIZE)));
static unsigned int user_stack_page_table[1024] __attribute__((aligned(PAGE_SIZE)));

static void paging_clear_table(unsigned int *table)
{
    unsigned int i;
    for (i = 0; i < 1024; i++) {
        table[i] = 0;
    }
}

void paging_build_user_demo(struct user_paging_context *ctx,
                            unsigned int user_code_phys,
                            unsigned int user_code_virt,
                            unsigned int user_stack_phys,
                            unsigned int user_stack_top_virt)
{
    unsigned int i;
    unsigned int pd_index_code;
    unsigned int pt_index_code;
    unsigned int pd_index_stack;
    unsigned int pt_index_stack;
    unsigned int stack_page_virt;
    unsigned int vga_pt_index;

    if (ctx == 0) {
        return;
    }

    paging_clear_table(page_directory);
    paging_clear_table(low_page_table);
    paging_clear_table(user_code_page_table);
    paging_clear_table(user_stack_page_table);

    /* Identity-map the first 4MB for kernel/runtime continuity. */
    for (i = 0; i < 1024; i++) {
        low_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    /* Allow user-mode writes to VGA text memory page only. */
    vga_pt_index = (0xB8000u >> 12) & 0x3FFu;
    low_page_table[vga_pt_index] = (vga_pt_index * PAGE_SIZE)
        | PAGE_PRESENT | PAGE_RW | PAGE_USER;

    page_directory[0] = ((unsigned int)low_page_table & PAGE_ALIGN_MASK)
        | PAGE_PRESENT | PAGE_RW | PAGE_USER;

    /* Map one user code page at USER_CODE_VIRT_BASE (or provided virtual). */
    pd_index_code = (user_code_virt >> 22) & 0x3FFu;
    pt_index_code = (user_code_virt >> 12) & 0x3FFu;

    user_code_page_table[pt_index_code] = (user_code_phys & PAGE_ALIGN_MASK)
        | PAGE_PRESENT | PAGE_RW | PAGE_USER;

    page_directory[pd_index_code] = ((unsigned int)user_code_page_table & PAGE_ALIGN_MASK)
        | PAGE_PRESENT | PAGE_RW | PAGE_USER;

    /* Map one user stack page right below USER_VIRT_STACK_TOP. */
    stack_page_virt = (user_stack_top_virt - 1u) & PAGE_ALIGN_MASK;
    pd_index_stack = (stack_page_virt >> 22) & 0x3FFu;
    pt_index_stack = (stack_page_virt >> 12) & 0x3FFu;

    user_stack_page_table[pt_index_stack] = (user_stack_phys & PAGE_ALIGN_MASK)
        | PAGE_PRESENT | PAGE_RW | PAGE_USER;

    page_directory[pd_index_stack] = ((unsigned int)user_stack_page_table & PAGE_ALIGN_MASK)
        | PAGE_PRESENT | PAGE_RW | PAGE_USER;

    ctx->directory_phys = (unsigned int)page_directory;
    ctx->user_code_virt = user_code_virt;
    ctx->user_stack_top = user_stack_top_virt;
}

void paging_activate(unsigned int page_directory_phys)
{
    paging_load_directory(page_directory_phys);
    paging_enable();
}
