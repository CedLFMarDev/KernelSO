int main(int argc, char **argv)
{
    volatile unsigned short *vga = (volatile unsigned short *)0xB8000;
    const char *msg = "User mode OK";
    int i;

    (void)argc;
    (void)argv;

    /* White on blue text at line 2, column 0. */
    for (i = 0; msg[i] != '\0'; i++) {
        vga[80 + i] = (unsigned short)(0x1F00u | (unsigned char)msg[i]);
    }

    for (;;) {
        /* Stay in ring 3 so the demo remains visible. */
    }

    /* Unreachable */
    return 0;
}
