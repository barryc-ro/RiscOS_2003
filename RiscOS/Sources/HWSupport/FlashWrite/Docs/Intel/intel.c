/* 28F016SA "C" DRIVERS */

/************************************************************************/
/* INTEL CONFIDENTIAL: DELIVER TO CUSTOMERS ONLY UNDER NDA UNTIL	*/	
/* 28F016SA MARKET INTRODUCTION						*/
/* Copyright Intel Corporation, 1993					*/
/* Example C Routines for the 28F016SA Flash memory component		*/
/* Patrick Killelea, Intel Corporation					*/
/* Revision 1.0, 25 June 1993						*/
/*									*/
/* NOTE: BYTE# pin on the device determines whether addressing 		*/
/* refers to words or bytes. I assume word mode.			*/
/* NOTE: A 28F016SA command can be written to any address in the	*/
/* block or device to be affected by that command.			*/
/************************************************************************/

#include <stdio.h>

void set_pin(int pin, int level)
{
/* set_pin is an implementation-dependent function which sets a		*/
/* given pin on the standard 28F016SA pinout HIGH = 1 or LOW = 0.	*/
}

int *base(int *address)
{
/* base is an implementation-dependent function which takes an		*/
/* address in the flash array and returns a pointer to the base		*/
/* of that 64K byte block.						*/
}

 
int compatible_block_erase(int *address)
{
  /* This procedure erases a 64K byte block on the 28F016SA.		*/
  /* It also works with a pair of 28F008SAs.				*/

  /* CSR variable is used to return contents of CSR register.		*/
  int CSR;

  /* Single Block Erase command						*/
  *address = 0x2020;

  /* Confirm command							*/
  *address = 0xD0D0;

  /* Poll CSR until CSR.7 = 1 (WSM ready)				*/
  while(!(0x0080 & *address))
  {
    /* Erase may be suspended here to write to a different block.	*/
  };
  
  /* At this point, CSR.7 is 1, indicating WSM is not busy.		*/
  /* Note that we are still reading from CSR by default.		*/

  /* Save CSR before clearing it.					*/
  CSR = *address;

  /* Clear Status Registers command					*/
  *address = 0x5050;

  /* Return CSR to be checked for status of operation.			*/
  return(CSR);
}

 
int compatible_byte_write(int *address, int data)
{
  /* This procedure writes a byte to the 28F016SA.			*/
  /* It also works with a the 28F008SA.					*/

  /* CSR variable is used to return contents of CSR register.		*/
  int CSR;

  /* Word Write command 						*/
  *address = 0x1010;

  /* Actual data write to flash address.				*/
  *address = data;

  /* Poll CSR until CSR.7 = 1 (WSM ready)				*/
  while(!(0x0080 & *address));

  /* Save CSR before clearing it.					*/
  CSR = *address;

  /* Clear Status Registers command					*/  
  *address = 0x5050;

  /* Return CSR to be checked for status of operation.			*/
  return(CSR);
}

/* This procedure suspends an erase operation to do a read.		*/
/*  It also works with a pair of 28F008SAs.				*/
int compatible_suspend_to_read(int *read_address, int *erase_address, int *result)
{
  /* CSR variable is used to return contents of CSR register.		*/
  int CSR;

  /* Assume erase is underway in block beginning at erase_address.	*/

  /* Erase Suspend command						*/
  *erase_address = 0xB0B0;

  /* Poll CSR until CSR.7 = 1 (WSM ready)				*/
  while(!(0x0080 & *erase_address));

  /* Read Flash Array command						*/
  *erase_address = 0xFFFF;

  /* Do the actual read. Any number of reads can be done here.		*/
  *result = *read_address;

  /* If CSR.6 = 1 (erase incomplete)					*/
  if (0x0040 & *erase_address)
    /* Erase Resume command						*/
    *erase_address = 0xD0D0;

  /* Save CSR before clearing it.					*/
  CSR = *erase_address;
  
  /* Clear Status Registers command					*/
  *erase_address = 0x5050;

  /* Return CSR to be checked for status of operation.			*/
  return(CSR);
}
 
/* This procedure uploads the device revision number to the DSR.	*/
int device_information_upload(int *address)
{
  /* DSR variable is used to return device revision status.		*/
  int GSR_DSR;

  /* Find pointer to base of 32K word block.				*/
  int *block_base = base(address);

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll GSR until GSR.3 = 0 (queue available).			*/
  while (0x0008 & *(block_base + 2));

  /* Poll GSR until GSR.7 = 1 (WSM available).				*/
  while (!(0x0080 & *(block_base + 2)));

  /* Device Information Upload command					*/
  *address = 0x9999;

  /* Confirmation command						*/
  *address = 0xD0D0;

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll GSR until GSR.7 = 1 (WSM not busy)				*/
  while (!(0x0080 & *(block_base + 2)));

  /* Swap page buffer to bring buffer with status information to top.	*/
  *address = 0x7272;

  /* Read Page Buffer command						*/
  *address = 0x7575;

  /* Put device revision code in bottom byte of return value.		*/
  /* Note that device revision code was read from word 3 in page buffer.*/
  GSR_DSR = (*(block_base + 3) & 0x00FF);

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Put GSR in top byte of return value.				*/
  GSR_DSR += (*(block_base + 2) & 0xFF00);

  /* Clear Status Registers command					*/
  *address = 0x5050;

  return(GSR_DSR);
}

/* This procedure erases a block on the 28F016SA.			*/ 
int ESR_block_erase(int *erase_address)
{
  /* ESR variable is used to return contents of GSR and BSR.		*/
  int ESR;

  /* Find address of base of block being erased.			*/
  int *block_base = base(erase_address);

  /* Read Extended Status Registers command				*/
  *erase_address = 0x7171;

  /* Poll BSR until BSR.3 of erase_address = 0 (queue available).	*/
  /* BSR is 1 word above base of target block in ESR space.		*/
  while (0x0008 & *(block_base + 1));

  /* Single Block Erase command						*/
  *erase_address = 0x2020;

  /* Confirm command							*/
  *erase_address = 0xD0D0;

  /* Read Extended Status Registers command				*/  
  *erase_address = 0x7171;

  /* Poll BSR until BSR.7 of target erase_address = 1 (block ready).	*/
  while (!(0x0080 & *(block_base + 1)));

  /* Put GSR in top byte and BSR in bottom byte of return value.	*/
  ESR = (*(block_base + 2) & 0xFF00) + (*(block_base + 1) & 0x00FF);

  /* Clear Status Registers command					*/
  *erase_address = 0x5050;

  return(ESR);
}


/* This procedure suspends an erase on the 28F016SA.			*/ 
/* Address is assumed to point to location to be read.			*/
/* result is used to hold read value until procedure is complete.	*/
int ESR_suspend_to_read(int *address,int *result)
{

  /* ESR variable is used to return contents of GSR and BSR.		*/
  int ESR;

  int *block_base = base(address);

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll BSR until BSR.7 of target address = 1 (block ready).		*/
  /* BSR is 1 word above base of target block in ESR space.		*/
  while (!(0x0080 & *(block_base + 1)));

  /* Operation Suspend command						*/
  *address = 0xB0B0;

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll GSR until GSR.7 = 1 (WSM ready).				*/
  while (!(0x0080 & *(block_base + 2)));

  /* Read Flash Array command						*/
  *address = 0xFFFF;

  /* Read the data.							*/
  *result = *address;

  /* If GSR.6 indicates an operation was suspended on this device,	*/
  if (0x0040 & *(block_base + 2))
    /* then resume the operation.					*/
    *address = 0xD0D0;

  /* Put GSR in top byte and BSR in bottom byte of return value.	*/
  ESR = (*(block_base + 2) & 0xFF00) + (*(block_base + 1) & 0x00FF);

  /* Clear Status Registers command					*/
  *address = 0x5050;

  return(ESR);
}

/* This procedure writes a word to the 28F016SA.			*/ 
int ESR_word_write(int *write_address, int data)
{
  /* ESR variable is used to return contents of GSR and BSR.		*/
  int ESR;

  int *block_base = base(write_address);

  /* Read Extended Status Registers command				*/
  *write_address = 0x7171;

  /* Poll BSR until BSR.3 of target address = 0 (queue available).	*/
  /* BSR is 1 word above base of target block in status reg space.	*/
  while (0x0008 & *(block_base + 1));

  /* Write word command							*/
  *write_address = 0x1010;

  /* Write actual data.							*/
  *write_address = data;

  /* Read Extended Status Registers command				*/
  *write_address = 0x7171;

  /* Poll BSR until BSR.7 of target address = 1 (block ready).		*/
  while (!(0x0080 & *(block_base + 1)));

  /* Put GSR in top byte and BSR in bottom byte of return value.	*/
  ESR = (*(block_base + 2) & 0xFF00) + (*(block_base + 1) & 0x00FF);

  /* Clear Status Registers command					*/
  *write_address = 0x5050;

  return(ESR);
}


/* This procedure erases all the unlocked blocks on a 28F016SA.		*/ 
int erase_all_unlocked_blocks(int *device_address, long *failure_list)
{
  /* Return value will contain GSR in both top and bottom byte.		*/
  /* 32 bit long pointed to by failure_list is used to return map	*/
  /* of block failures, each bit representing one block's status.	*/
  /* device_address points to base of chip.				*/
  int GSR;

  /* block is used to hold block count for loop through blocks.		*/
  int block;

  long power = 1;

  /* Initialize all 32 bits of failure list long to 0.			*/
  *failure_list = 0; 

  /* Read Extended Status Registers					*/
  *device_address = 0x7171;

  /* Poll BSR until BSR.3 of target address = 0 (queue available).	*/
  while (0x0008 & *(device_address + 1));

  /* Full-chip erase command						*/
  *device_address = 0xA7A7;

  /* Confirm command							*/
  *device_address = 0xD0D0;

  /* Read Extended Status Registers command				*/
  *device_address = 0x7171;

  /* Poll GSR until GSR.7 = 1 (WSM ready)				*/
  while (!(0x0080 & *(device_address + 2)));

  /* Go through blocks, looking at each BSR.5 for operation failure	*/
  /* and setting appropriate bit in long pointed to by failure list.	*/
  for (block = 0; block < 0x0020; block++)
  {
    /* Multiply block by 32K words to get to the base of each block.	*/
    if (0x0020 & *(device_address + block * 0x8000 + 1))
      /* If the block failed, set that bit in the failure list.		*/
      *failure_list += power;

    /* Increment to next power of two to access next bit.		*/
    power *= 2;
  }

  /* Put GSR in both bytes of return value.				*/
  GSR = *(device_address + 2);

  /* Clear Status Registers command					*/
  *device_address = 0x5050;

  return(GSR);
}

/* This procedure locks a block on the 28F016SA.			*/ 
int lock_block(int *lock_address)
{
  /* ESR variable is used to return contents of GSR and BSR.		*/
  int ESR;

  /* Write Protect pin (active low) is pin number 56 on standard	*/
  /* pinout of 28F016SA.						*/
  #define WPB 56

  /* Vpp pin is pin number 15 on standard pinout of 28F016SA.		*/
  #define VPP 15

  #define HIGH 1
  #define LOW  0

  /* Find pointer to base of block being locked.			*/
  int *block_base = base(lock_address);

  /* Read Extended Status Registers command				*/
  *lock_address = 0x7171;

  /* Poll GSR until GSR.3 = 0 (queue available).			*/
  while (0x0008 & *(block_base + 2));

  /* Disable write protection by setting WPB high.			*/
  set_pin(WPB, HIGH);

  /* Enable Vpp, wait for ramp if necessary in this system.		*/
  set_pin(VPP, HIGH);

  /* Lock Block command							*/
  *lock_address = 0x7777;

  /* Confirmation command						*/
  *lock_address = 0xD0D0;

  /* Read Extended Status Registers command 				*/
  *lock_address = 0x7171;

  /* GSR is 2 words above 0; poll GSR until GSR.7 = 1 (WSM ready).	*/
  while (!(0x0080 & *(block_base + 2)));


  /* Put GSR in top byte and BSR in bottom byte of return value.	*/
  ESR = (*(block_base + 2) & 0xFF00) + (*(block_base + 1) & 0x00FF);

  /* Clear Status Registers command					*/
  *lock_address = 0X5050;

  return(ESR);
}

/* This procedure uploads status information into the ESR from		*/
/* non-volatile status bits.						*/ 
int status_upload(int *address)
{
  /* ESR variable is used to return contents of GSR and BSR.		*/
  int ESR;

  /* Find pointer to base of 32K word block.				*/
  int *block_base = base(address);

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll GSR until GSR.3 = 1 (queue available).			*/
  while (0x0008 & *(block_base + 2));

  /* Lock-status Upload command						*/
  *address = 0x9797;

  /* Confirmation command						*/
  *address = 0xD0D0;

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll GSR until GSR.7 = 1 (WSM not busy)				*/
  while (!(0x0080 & *(block_base + 2)));

  /* Put GSR in top byte and BSR in bottom byte of return value.	*/
  ESR = (*(block_base + 2) & 0xFF00) + (*(block_base + 1) & 0x00FF);

  /* Clear Status Registers command					*/
  *address = 0x5050;

  return(ESR);
}


/* This procedure writes from page buffer to flash.			*/
/*									*/ 
/* This routine assumes page buffer is already loaded.			*/
/* Address is where in flash array to begin writing.			*/
/* Low byte of byte_count must be 256 or fewer, high must be 0.		*/
/* High byte exists for future Page Buffer expandability.		*/
int pagebuffer_write_to_flash(int *address, int byte_count)
{

  /* ESR variable is used to return contents of GSR and BSR.		*/
  int ESR;

  /* Find pointer to base of block to be written.			*/
  int *block_base = base(address);

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll BSR until BSR.3 of target address = 0 (queue available).	*/
  while (0x0008 & *(block_base + 1));

  /* Page Buffer Write to Flash command					*/
  *address = 0x0C0C;

  /* Only A0 valid here; low or high byte loaded depending on A0.	*/
  *address = byte_count;

  /* A0 internally complemented; alternate byte loads; write starts	*/
  *address = byte_count;

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll BSR until BSR.7 of target address =1 (block ready).		*/
  while (!(0x0080 & *(block_base + 1)));

  /* Put GSR in top byte and BSR in bottom byte of return value.	*/
  ESR = (*(block_base + 2) & 0xFF00) + (*(block_base + 1) & 0x00FF);

  /* Clear Status Registers command					*/
  *address = 0x5050;

  return(ESR);
}


/* This procedure loads a multiple bytes to a page buffer.		*/
/*									*/
/* Low byte of word_count must be 128 or fewer, high must be 0.		*/
/* High byte exists for future Page Buffer expandability.		*/ 
void sequential_pagebuffer_load(int *device_address, char *start_address,
                                int word_count, int* data)
{
  /* counter is used to keep track of bytes written.			*/
  char counter;

  /* Read Extended Status Registers command				*/
  *device_address = 0x7171;

  /* Poll GSR until GSR.2 = 1 (page buffer available).			*/
  while (!(0x0004 & *(device_address + 2)));

  /* Sequential Page Buffer Load command				*/  
  *device_address = 0xE0E0;

  /* Loads high or low byte of count register, depending on A0		*/
  *start_address = word_count;

  /* Automatically loads alternate byte of count register		*/
  *start_address = word_count;

  /* Loop through data, writing to page buffer.				*/
  /* This routine does not affect status registers.			*/
  for (counter = 0; counter < word_count; counter++)
    *(start_address + counter) = data[counter];

}


/* This procedure loads a single byte or word to a page buffer.		*/ 
void single_pagebuffer_load(int *device_address, char *address, int data)
{
  /* Read Extended Status Registers command				*/
  *device_address = 0x7171;

  /* Poll GSR until GSR.2 = 1 (page buffer available)			*/
  while (!(0x0004 & *(device_address + 2)));

  /* Single Load to Page Buffer command					*/
  *device_address = 0x7474;

  /* Actual write to page buffer					*/
  /* This routine does not affect status registers.			*/
  *address = data;

}

/* This routine is used when BYTE# is low, i.e. the 28F016SA 		*/
/* is in byte mode, to emulate a word write.				*/
int two_byte_write(int *address, char data_low, char data_high)
{
  /* ESR variable is used to return contents of GSR and BSR.		*/
  int ESR;

  /* Find pointer to base of block.					*/
  int *block_base = base(address);

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll BSR until BSR.3 of target address = 0 (queue available).	*/
  while (0x0008 & *(block_base + 1));

  /* Two-byte Write command						*/
  *address = 0xFBFB;

  /* Load one byte of data register; A0 = 0 loads low byte, A1 high	*/
  *address = data_low;

  /* 28F016SA automatically loads alternate byte of data register	*/
  *address = data_high;

  /* Write is initiated. Now we poll for successful completion.		*/

  /* Read Extended Status Registers command				*/
  *address = 0x7171;

  /* Poll BSR until BSR.7 of target address = 1 (block ready).		*/
  /* BSR is 1 word above base of target block in status reg space.	*/
  while (!(0x0080 & *(block_base + 1)));

  /* Put GSR in top byte and BSR in bottom byte of return value.	*/
  ESR = (*(block_base + 2) & 0xFF00) + (*(block_base + 1) & 0x00FF);

  /* Clear Status Registers command					*/
  *address = 0x5050;

  return(ESR);
}


/* This procedure writes to one block while another is erasing.		*/ 
int write_during_erase(int *write_address, int *erase_address, int data)
{
  /* ESR variable is used to return contents of GSR and BSR.		*/
  int ESR;

  /* Find pointer to base of block being erased.			*/
  int *block_base = base(erase_address);

  /* Read Extended Status Register command				*/
  *erase_address = 0x7171;

  /* Poll BSR until BSR.3 of target address = 0 (queue available).	*/
  /* BSR is 1 word above base of target block in ESR space.		*/
  while (0x0008 & *(block_base + 1));

  /* Erase Block command						*/
  *erase_address = 0x2020;

  /* Confirm command							*/
  *erase_address = 0xD0D0;

  /* Word Write command							*/
  *write_address = 0x4040;

  /* Write actual data.							*/
  /* Erase suspends, write takes place, then erase resumes.		*/
  *write_address = data;

  /* Read Extended Status Registers command				*/
  *erase_address = 0x7171;

  /* Poll BSR until BSR.7 of erase address = 1 (block ready).		*/
  /* BSR is 1 word above base of target block in status reg space.	*/
  while (!(0x0080 & *(block_base + 1)));

  /* Put GSR in top byte and BSR in bottom byte of return value.	*/
  ESR = (*(block_base + 2) & 0xFF00) + (*(block_base + 1) & 0x00FF);

  /* Clear Status Registers command					*/
  *block_base = 0x5050;

  return(ESR);
}

