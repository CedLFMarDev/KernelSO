global paging_load_directory
global paging_enable

; paging_load_directory(page_directory_phys)
paging_load_directory:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

; paging_enable()
paging_enable:
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret
