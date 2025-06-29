/*NOTES ON THIS BUILD:


Issues:


Potential Issues:


*/


//Flash program/read of presets dump
/*
	//TEST
	uint64_t preset_packed = 0;
	Pack_Preset_Into_Doubleword((struct Preset*)&test_preset_0, &preset_packed);


	uint64_t *test_preset_val = (uint64_t*)0x0800F800;
	uint32_t errors = 0;
	FLASH_EraseInitTypeDef erase_config = {.TypeErase = FLASH_CR_PER, .Banks = FLASH_CR_MER1, .Page = 31, .NbPages = 1};
	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&erase_config, &errors); //all 0xF is no errors


	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, USER_PRESET_0_FLASH_MEMORY_ADDRESS, preset_packed);
	uint64_t value = *test_preset_val;

	Read_and_Interpret_Preset_From_Flash(USER_PRESET_0_FLASH_MEMORY_ADDRESS, &test_preset_1);

	struct Preset read_preset = test_preset_1;

	HAL_FLASH_Lock();
	//TEST
*/


//INCLUDES
#include "main.h"
#include "main_vars.h"

int main(void)
{
	//SYSTEM INIT
	System_Init();

	//STARTUP
	Startup();

	while (1)
	{
		if(Get_Status_Bit(&statuses, Input_Capture_Processing_Can_Be_Started) == YES){

			//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 1);

			Input_Capture_Processing(interrupt_period, &params_to_be_loaded);

			//HAL_GPIO_WritePin(MONITOR_GPIO_Port, MONITOR_Pin, 0);
		}

		if(Get_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running) == YES){

			if(idle_counter < IDLE_COUNT){

				if(IP_CAP_fsm.current_state != IDLE){

					Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
					Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
					idle_counter = 0;
				}
				else{

					idle_counter++;
				}
			}
			else{

				Clear_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Is_Running);
				Set_Status_Bit(&statuses, Software_IP_CAP_Idle_Timer_Has_Timed_Out);
				idle_counter = 0;
			}
		}

		if(Get_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running) == YES){

			if(midi_counter < MIDI_COUNT){

				midi_counter++;
			}
			else{

				Clear_Status_Bit(&statuses, Software_MIDI_Timer_Is_Running);
				Set_Status_Bit(&statuses, Software_MIDI_Timer_Has_Timed_Out);
				active_status_byte = 0;
				Clear_Data_Buffer(&MIDI_data);
				midi_counter = 0;
			}
		}
	}

	return 1;
}



