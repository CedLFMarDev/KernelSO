#ifndef INCLUDE_PAGING_H
#define INCLUDE_PAGING_H

#define PAGE_SIZE               4096u
#define PAGE_PRESENT            0x001u
#define PAGE_RW                 0x002u
#define PAGE_USER               0x004u
#define PAGE_ALIGN_MASK         0xFFFFF000u

struct user_paging_context {
    unsigned int directory_phys;
    unsigned int user_code_virt;
    unsigned int user_stack_top;
};

void paging_build_user_demo(struct user_paging_context *ctx,
                            unsigned int user_code_phys,
                            unsigned int user_code_virt,
                            unsigned int user_stack_phys,
                            unsigned int user_stack_top_virt);
void paging_activate(unsigned int page_directory_phys);

#endif /* INCLUDE_PAGING_H */
