/*NOTES ON THIS BUILD:


Issues:


Potential Issues:


*/


//INCLUDES
#include "main.h"

int __attribute__((optimize("O0")))main(void)
{
	//SYSTEM INIT
	System_Init();

	//STARTUP
	Startup();

	while (1)
	{
		Check_Software_Timers();
	}

	return 1;
}



