#include "menu.h"
#include"kernel.h"

int main(void)
{
    Kernel::initDatabase();   
    Menu::showBasicMenu();
}