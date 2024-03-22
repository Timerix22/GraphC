#include "gui/gui.hpp"
#include <iostream>

// SDL for some reason redefines main
#ifdef main 
    #undef main
#endif

int main(const int argc, const char* const* argv){
    if(setlocale(LC_CTYPE, "C.UTF-8")!=0)
        kprintf("\e[93msetlocale failed!\n");

    kt_beginInit(true);
    kt_initKerepTypes();
    kt_endInit();

    try {
        GraphC::gui::GUI gui;
        gui.init("GraphC");
        gui.startAndWait();
    }
    catch(const std::exception& e){
        std::cerr<<"Catched exception: "<<e.what()<<std::endl;
        return -1;
    }
    catch(const char* msg){
        std::cerr<<"Catched exception message: "<<msg<<std::endl;
        return -1;
    }
    catch(std::string& msg){
        std::cerr<<"Catched exception message: "<<msg<<std::endl;
        return -1;
    }
    catch(...){
        std::cerr<<"Catched unknown exception"<<std::endl;
        return -1;
    }
    
    kt_free();
    return 0;
}
