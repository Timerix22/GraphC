#include "../kerep/src/base/base.h"

void kt_initGraphCTypes(){

}

int main(const int argc, const char* const* argv){
    if(setlocale(LC_CTYPE, "C.UTF-8")!=0)
        kprintf("\e[93msetlocale failed!\n");

    kt_beginInit();
    kt_initKerepTypes();
    kt_initGraphCTypes();
    kt_endInit();

    kprintf("\e[37margs:");
    for(i32 i=0; i<argc; i++)
        kprintf(" %s", argv[i]);
    kprintf("\n");

    kt_free();
    return 0;
}
