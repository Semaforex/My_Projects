#include <iostream>
#include <conio.h>
int main()
{
    unsigned char znak;
    std::cout << "klawisz [ESC] wychodzi z programu." << std::endl;
    do
    {
        znak = getch();
        std::cout << "znak: '" << znak << "' kod: " << static_cast < int >( znak ) << std::endl;
        while( kbhit() )
        {
            znak = getch();
            std::cout << "*znak: '" << znak << "' kod: " << static_cast < int >( znak ) << std::endl;
        }
        std::cout << std::endl;
    } while( znak != 27 ); //ESC

    return( 0 );
}
