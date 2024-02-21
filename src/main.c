#include "../dependencies/kerep/src/base/base.h"
#include "gui/gui.h"

void kt_initGraphCTypes(){

}

int main(const int argc, const char* const* argv){
    if(setlocale(LC_CTYPE, "C.UTF-8")!=0)
        kprintf("\e[93msetlocale failed!\n");

    kt_beginInit(true);
    kt_initKerepTypes();
    kt_initGraphCTypes();
    kt_endInit();

    kprintf("\e[37margs:");
    for(i32 i=0; i<argc; i++)
        kprintf(" %s", argv[i]);
    kprintf("\n");

    tryLast(main_window_open("GraphC"),_1, ;);
    tryLast(main_window_loop_start(),_2, ;);
    tryLast(main_window_close(),_3, ;);

    kt_free();
    return 0;
}
