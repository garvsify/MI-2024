/*NOTES ON THIS BUILD:


Issues:


Potential Issues:


*/


//INCLUDES
#include "main.h"

int main(void)
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



