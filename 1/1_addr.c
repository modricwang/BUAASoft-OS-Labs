#include <stdio.h>

void show_addr(const char *name, const void *p) {
    printf("%s address = %016p\n", name, p);
}

void get_register_val() {
    void *rbp_val;
    __asm__ __volatile__ (
    "movq 0x8(%%rbp),%0":"=r"(rbp_val)
    );
    show_addr("Function return", rbp_val);

    void *begin_addr = &(get_register_val);
    show_addr("Function begin ", begin_addr);


    printf("\n\nStack Frame Begin:\n\n");
    for (long long *ll = rbp_val; ll >= begin_addr; ll--) {
        printf("%016llx\n", *ll);
    }
    printf("\nStack Frame End.\n\n\n");
}


const int p = 10;
int x;


int main() {
    int y;
    show_addr("Global var", &x);
    show_addr("Global const", &p);
    show_addr("Local var", &y);

    puts("");

    get_register_val();

    long long val = 0xfff;
    long long *l1_ptr = &val;
    long long **l2_ptr = &l1_ptr;
    if (**l2_ptr == val) {
        printf("Yes, L2 pointer works!\n");
    }
}
