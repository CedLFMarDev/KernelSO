#ifndef INCLUDE_USER_MODE_H
#define INCLUDE_USER_MODE_H

#define USER_STACK_SIZE       4096u
#define USER_CODE_SIZE        4096u
#define USER_CODE_VIRT_BASE   0x00400000u
#define USER_VIRT_STACK_TOP   0xBFFFFFFCu

struct user_process {
	unsigned int entry_point;
	unsigned int user_stack_top;
	unsigned int page_directory_phys;
};

/*
 * User-mode chapter 11 demo toggle.
 * Can be overridden from compiler flags with -DUSER_MODE_DEMO_ENABLED=0/1.
 */
#ifndef USER_MODE_DEMO_ENABLED
#define USER_MODE_DEMO_ENABLED 1
#endif

void user_mode_start_demo(void);
void user_mode_prepare_process(struct user_process *proc,
							   unsigned int entry_point,
							   unsigned int user_stack_top);

#endif /* INCLUDE_USER_MODE_H */
