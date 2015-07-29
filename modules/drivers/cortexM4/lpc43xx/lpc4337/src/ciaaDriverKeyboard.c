/*==================[macros and definitions]=================================*/
#define KEYB_LINE_CTRL   11

#define SELECTED_LINE_0    0
#define SELECTED_LINE_1    1
#define SELECTED_LINE_2    2
#define SELECTED_LINE_3    3
#define TEC_FIL0           0
#define TEC_FIL1           1
#define TEC_FIL2           2
#define TEC_FIL3           3

#define ACTIVATE_LINE      0
#define DESACTIVATE_LINE   1

#define TEC_COL0_MASK      0x01
#define TEC_COL1_MASK      0x02
#define TEC_COL2_MASK      0x04
#define TEC_COL3_MASK      0x08



extern int32_t ciaaDriverKeyboard_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   uint8_t ret = -1;
   if(device == ciaaKeyboardDevices[0])
   {
	   switch(request){
	   /* CASE KEYBOARD LINE CONTROL
	    * This request changes the active output of the keyboard
	    * The keyboard works with negative logic, so a low level output
	    * is taken as active. It returns the active line:
	    * sLine == SELECTED_LINE_0  --> T_FIL0 = 0, T_FIL1 = 1, T_FIL2 = 1, T_FIL3 = 1
	    * sLine == SELECTED_LINE_1  --> T_FIL0 = 1, T_FIL1 = 0, T_FIL2 = 1, T_FIL3 = 1
	    * sLine == SELECTED_LINE_2  --> T_FIL0 = 1, T_FIL1 = 1, T_FIL2 = 0, T_FIL3 = 1
	    * sLine == SELECTED_LINE_3  --> T_FIL0 = 1, T_FIL1 = 1, T_FIL2 = 1, T_FIL3 = 0
	    */

	         case KEYB_LINE_CTRL:
	   	     switch(((int)param)){
	   	     case SELECTED_LINE_0:
	   	    	ciaa_lpc4337_writeKeyboard(TEC_FIL0,ACTIVATE_LINE);
	   	    	ciaa_lpc4337_writeKeyboard(TEC_FIL1,DESACTIVATE_LINE);
	   	    	ciaa_lpc4337_writeKeyboard(TEC_FIL2,DESACTIVATE_LINE);
	   	    	ciaa_lpc4337_writeKeyboard(TEC_FIL3,DESACTIVATE_LINE);
	   	    	break;
	   	     case SELECTED_LINE_1:
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL0,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL1,ACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL2,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL3,DESACTIVATE_LINE);
		   	    break;
	   	     case SELECTED_LINE_2:
		   	   	ciaa_lpc4337_writeKeyboard(TEC_FIL0,DESACTIVATE_LINE);
		   	   	ciaa_lpc4337_writeKeyboard(TEC_FIL1,DESACTIVATE_LINE);
		   	   	ciaa_lpc4337_writeKeyboard(TEC_FIL2,ACTIVATE_LINE);
		   	   	ciaa_lpc4337_writeKeyboard(TEC_FIL3,DESACTIVATE_LINE);
		   	   	break;
	   	     case SELECTED_LINE_3:
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL0,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL1,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL2,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL3,ACTIVATE_LINE);
		   	    break;
	   	     }
	   	     break;

	   	  default:
	   		  break;
	      }
      ret = 1;
   }
   return ret;
}

/** \brief keyboard read function
 *
 * This function read the columns of the keyboard
 * The keyboard works with negative logic, so a low level output
 * is taken as active.
 ** \param[in]  device   pointer to the device to be controlled
 ** \param[in]  buffer   pointer to variable where the read information will be saved
 ** \param[in]  size     size of the buffer
 ** At the end, buffer saves the column number which has been detected as pressed. If
 ** no switch has been pressed, buffer has 0xFF
 */

extern uint16_t ciaaDriverKeyboard_read(ciaaDevices_deviceType const * const device, uint16_t * buffer, size_t size)
{
   ssize_t ret = -1;

   /* Can't store read result in buffer. At least 1 byte required. */
   if(size != 0)
   {
      if(device == ciaaKeyboardDevices[0])
      {
#if(BOARD == edu_ciaa_nxp)
#if (CIAA_PNCH_USER_INTERFACE == CIAA_ENABLE)
         buffer[0]  = Chip_GPIO_GetPinState(LPC_GPIO_PORT, 1, 8)  ? 1 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 12) ? 2 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 13) ? 4 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 2, 8)  ? 8 : 0;
#endif
#endif
      if(!(TEC_COL0_MASK & buffer[0]))
      {
         buffer[0] = 0;
         return 1;
      }
      else
      {
      if(!(TEC_COL1_MASK & buffer[0]))
      {
              buffer[0] = 1;
              return 1;
      }
      else
      {
      if(!(TEC_COL2_MASK & buffer[0]))
      {
         buffer[0] = 2;
         return 1;
      }
      else
      {
      if(!(TEC_COL3_MASK & buffer[0]))
      {
         buffer[0] = 3;
         return 1;
      }
      }
      }
      }
         /* 1 byte read */
         ret = 1;
      }

      else {
    	  /* Put your code if you have another device */
      }
   }
   return ret;
}

extern uint16_t ciaaDriverKeyboard_write(ciaaDevices_deviceType const * const device, uint16_t const * const buffer, size_t const size)
{
	/* keyboard cannot be written */
   ssize_t ret = -1;

  return ret;
}

void ciaaDriverKeyboard_init(void)
{
   uint8_t loopi;

   ciaa_lpc4337_Keyboard_init();

   /* add dio driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverKeyboardConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaKeyboardDevices_addDriver(ciaaDriverKeyboardConst.devices[loopi]);
      /* init layer data for each device */
      *((ciaaDriverKeyboard_keybType *)ciaaDriverKeyboardConst.devices[loopi]->layer) = 0;
   }
}


/*==================[interrupt hanlders]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

