/**************************************************************************************
* File : FP_gt521f32.c
* Description : This file contains the function definitions related to GT521F32 
                Fingerprint Sensor commands
***************************************************************************************/

#include "stm32f401rbt6.h"
#include "UART1.h"
#include "FP_gt521f32.h"
#include "km_lcd.h"
#include "stdio.h"
#include "systemticktimer.h"
#include "4x4_keypad.h"
#include "my_string.h"


int ID_MAPPING[MAX_USER_COUNT * 2];           //stores keypad ID and fingerprint ID
int attendance_log[MAX_ATTENDANCE_RECORDS];  //Stores fingerprint verification records
int attendance_index = 0;  //Keeps track of the next position to store attendance
char buffer[15];       //Buffer to store the converted integer as a string
unsigned char FP_Buffer[FP_BUFF_SIZE];
unsigned char packet[12]; 


unsigned short error_code;
int FP_value;
extern int touch_flag;
extern int ent_sw_flag;
extern char password[];
int userID;

extern volatile int timer_counter;    //Declare global variable


/************************************************************************************
*                          Fingerprint_Response()
**************************************************************************************/

void Fingerprint_Response(unsigned int Timeout_Value)
{
	int i;
	int received_data; 
			
	for(i=0;i<FP_BUFF_SIZE;i++)
	{
		received_data = UART1_InChar_TIMEOUT(Timeout_Value);
		if(received_data == TIMEOUT)
		{
			return;			
		}
		FP_Buffer[i] = (unsigned char)received_data;
	}
}



/*************************************************************************************
*                              PreparePacket()
***************************************************************************************/

void PreparePacket(unsigned char packet[],unsigned char command,unsigned char parameter)
{	
	int i;
	unsigned short int checksum=0;

	//Start Header (Fixed)
  packet[0] = 0x55;  
  packet[1] = 0xAA;  

  //Device ID (Default:0x0001)
  packet[2] = 0x01;  
  packet[3] = 0x00;  

  //Parameters
  packet[4] = (parameter & 0xFF);           //lower byte of parameter 
  packet[5] = ((parameter >> 8) & 0xFF);    //higher byte of parameter
  packet[6] = 0x00;  
  packet[7] = 0x00;  
  	
  //Command Byte
  packet[8] = command;  
  packet[9] = 0x00;  
 
  //Checksum (Sum of bytes from 0 to 9)
	for (i = 0; i <= 9; i++)
  {
		checksum += packet[i];
  }
  packet[10] = (checksum & 0xFF);              //lower byte of checksum 
  packet[11] = ((checksum >> 8) & 0xFF);       //higher byte of checksum
	
}




/************************************************************************************
*                              SendCommand()
*************************************************************************************/

void SendCommand(unsigned char command, unsigned short parameter,unsigned int Timeout_Value)
{
  PreparePacket(packet, command, parameter);  //Prepare the packet
  UART1_OutStr(packet, 12);                   //Send the packet via UART
	Fingerprint_Response(Timeout_Value);        //Read 12-byte response into FP_Buffer[]
}



/*************************************************************************************
*                                GetErrorCode()
**************************************************************************************/

int GetErrorCode(void)
{
	int error = (FP_Buffer[5] << 8) | FP_Buffer[4];  //Extract the 2-byte error code from positions 4 and 5
	return error;
}



/************************************************************************************
*                               IsFingerPlaced()
**************************************************************************************/

int IsFingerPlaced(void)
{
	SendCommand(ISPRESSFINGER, 0, 250000);     //Send command to check if the finger is present

  error_code = GetErrorCode();               //Get the error code

  //Handle errors first
  switch (error_code)
  {
		case NACK_COMM_ERR:
			return NACK_COMM_ERR;         //Communication error

    case NACK_DEV_ERR:
			return NACK_DEV_ERR;          //Device error

    case NACK_FINGER_IS_NOT_PRESSED:
			return NACK_FINGER_IS_NOT_PRESSED;   //No finger pressed

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;                 //Timeout error

    default:
			return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_NO_FINGER;    //If no error, check for finger presence
  }
}



/************************************************************************************
*                                GetEnrollCount()
*************************************************************************************/

int GetEnrollCount(void)
{
	SendCommand(GETENROLLCOUNT, 0, 100000);     //Send the GetEnrollCount command

  error_code = GetErrorCode();                //Get the error code

  //Check for specific error messages
  switch (error_code)
  {
		case NACK_TIMEOUT:
			return NACK_TIMEOUT;                    //Timeout occurred

    case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;                //No fingerprints are enrolled

    case NACK_COMM_ERR:
			return NACK_COMM_ERR;                   //Communication error

    default:
			return ((FP_Buffer[5] << 8) | FP_Buffer[4]);    //If no error, extract enroll count
  }
}



/************************************************************************************
*                                  CheckEnrolled()
*************************************************************************************/

int CheckEnrolled(unsigned short int ID)
{
	SendCommand(CHECKENROLLED, ID, 100000);     //Send CheckEnrolled command
	
	error_code = GetErrorCode();               //Get the error code
	
  //Handle specific errors
	switch (error_code)
  {
		case NACK_IS_NOT_USED:
			return NACK_IS_NOT_USED;          //ID is not used (not enrolled)

    case NACK_IS_ALREADY_USED:
			return NACK_IS_ALREADY_USED;      //ID is already enrolled

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;              //Timeout error

    default:
			//If no error, check for success response
      return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_TIMEOUT;
  }
}



/************************************************************************************
*                                CaptureFinger()
*************************************************************************************/

int CaptureFinger(void)
{
	SendCommand(CAPTUREFINGER, 1, 500000);   //Send CaptureFinger command with a timeout          

  error_code = GetErrorCode();            //Get the error code

  //Handle specific error cases
  switch (error_code)
  {
		case NACK_BAD_FINGER:
			return NACK_BAD_FINGER;             //Poor fingerprint quality
		
		case NACK_FINGER_IS_NOT_PRESSED:
			return NACK_FINGER_IS_NOT_PRESSED;  //No finger detected

    case NACK_COMM_ERR:
      return NACK_COMM_ERR;              //Communication error

    case NACK_DEV_ERR:
      return NACK_DEV_ERR;               //Device error

    case NACK_TIMEOUT:
      return NACK_TIMEOUT;              //Timeout error

    default:
			 //If no error, check for success response
       return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_CAPTURE_FAIL;  
    }
}



/************************************************************************************
*                              EnrollStart()
*************************************************************************************/

int EnrollStart(unsigned short int ID)
{
	SendCommand(ENROLLSTART, ID, 100000);        //Send EnrollStart command

  error_code = GetErrorCode();                 //Get the error code
	
	//Handle specific errors
  switch (error_code)
  {
		case NACK_IS_ALREADY_USED:
			return NACK_IS_ALREADY_USED;     //ID is already in use

    case NACK_INVALID_POS:
			return NACK_INVALID_POS;        //Invalid ID position (out of range)

    case NACK_DB_IS_FULL:
			return NACK_DB_IS_FULL;        //Database is full

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//If no error, check for success response
      return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;
  }
}



/************************************************************************************
*                                  Enroll1()
**************************************************************************************/

int Enroll1(void)
{
	SendCommand(ENROLL1, 0, 700000);             //Send Enroll1 command with a timeout

  //Get the error code
	error_code = GetErrorCode();
	
  //Handle specific error cases
  switch (error_code)
  {
		case NACK_BAD_FINGER:
			return NACK_BAD_FINGER;         //Poor fingerprint quality

    case NACK_ENROLL_FAILED:
			return NACK_ENROLL_FAILED;      //Enrollment processing error

    case NACK_TURN_ERR:
			return NACK_TURN_ERR;           //Incorrect enrollment sequence

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//If no error, check for success response
			return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;  
  }
}



/************************************************************************************
*                                     Enroll2()
*************************************************************************************/

int Enroll2(void)
{
	SendCommand(ENROLL2, 0, 700000);             //Send Enroll1 command with a timeout

  //Get the error code
	error_code = GetErrorCode();
	
  //Handle specific error cases
  switch (error_code)
  {
		case NACK_BAD_FINGER:
			return NACK_BAD_FINGER;         //Poor fingerprint quality

    case NACK_ENROLL_FAILED:
			return NACK_ENROLL_FAILED;      //Enrollment processing error

    case NACK_TURN_ERR:
			return NACK_TURN_ERR;           //Incorrect enrollment sequence

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//If no error, check for success response
			return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;  
  }
}



/************************************************************************************
*                                    Enroll3()
*************************************************************************************/

int Enroll3(void)
{
	SendCommand(ENROLL3, 0, 700000);             //Send Enroll1 command with a timeout

  //Get the error code
	error_code = GetErrorCode();
	
  //Handle specific error cases
  switch(error_code)
  {
		case NACK_BAD_FINGER:
			return NACK_BAD_FINGER;         //Poor fingerprint quality

    case NACK_ENROLL_FAILED:
			return NACK_ENROLL_FAILED;      //Enrollment processing error

    case NACK_TURN_ERR:
			return NACK_TURN_ERR;           //Incorrect enrollment sequence

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//If no error, check for success response
			return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_ENROLLMENT_FAIL;  
  }
}



/************************************************************************************
*                              IdentifyFingerprint()
**************************************************************************************/

int IdentifyFingerprint(void)
{
	SendCommand(IDENTIFY, 0, 1300000);   //Send Identify command with timeout
	
	error_code = GetErrorCode();        //Get the error code

  switch(error_code)
  {
		case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;       //Database is Empty
		
		case NACK_IDENTIFY_FAILED:
			return NACK_IDENTIFY_FAILED;    //No match found

    case NACK_COMM_ERR:
			return NACK_COMM_ERR;           //Communication error

    case NACK_DEV_ERR:
			return NACK_DEV_ERR;           //Device error

    case NACK_TIMEOUT:
			return NACK_TIMEOUT;           //Timeout error

    default:
			//Check for success response
      return (FP_Buffer[8] == 0x30) ? ((FP_Buffer[5] << 8) | FP_Buffer[4]) : ERROR_IDENTIFICATION_FAIL;  
  }
}




/************************************************************************************
*                           IdentifyFingerprint_LCD()
*************************************************************************************/

void IdentifyFingerprint_LCD(void)
{
	int result=0;
	char buffer[20];

	KM_LCD_Write_Cmd(0x01);
	KM_LCD_Write_Str((unsigned char *)" IDENTIFICATION ");
	KM_LCD_Write_Cmd(0xC5);
	KM_LCD_Write_Str((unsigned char *)"[1:N]");
	KM_Delay_ms(2000);
	
	KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Str((unsigned char *)"PLACE FINGER TO IDENTIFY");
	
  while(!(touch_flag == 1 && IsFingerPlaced() == SUCCESS)){;}

  if(CaptureFinger() != SUCCESS)
  {
		KM_LCD_Write_Cmd(0x01);
	  KM_LCD_Write_Str((unsigned char *)" ERROR: FINGER  CAPTURE FAILED");
    return;
  }	
	
	//KM_Delay_ms(1000);
	
  result = IdentifyFingerprint();  //Call IdentifyFingerprint()

  KM_LCD_Write_Cmd(0x01);         //Clear LCD before displaying new message

  switch (result)
  {
		case NACK_DB_IS_EMPTY:
			KM_LCD_Write_Str((unsigned char *)"ERROR: DB IS EMPTY");
      break;

    case NACK_IDENTIFY_FAILED:
			KM_LCD_Write_Str((unsigned char *)"ERROR: NO MATCH");
      break;

    case NACK_COMM_ERR:
			KM_LCD_Write_Str((unsigned char *)"ERROR: COMM FAILURE");
      break;

    case NACK_DEV_ERR:
			KM_LCD_Write_Cmd(0x01);
			KM_LCD_Write_Str((unsigned char *)"ERROR: SENSOR FAILURE");
      break;

    case NACK_TIMEOUT:
			KM_LCD_Write_Str((unsigned char *)"ERROR: TIMEOUT");
      break;

    default:
			if(result >= 0)     //If a valid ID is received
      {
				sprintf(buffer, "ID MATCH: %04X", result);
        KM_LCD_Write_Str((unsigned char *)buffer);
      }
      else
      {
				KM_LCD_Write_Str((unsigned char *)"IDENTIFICATION  FAILED");
				KM_Delay_ms(1000);
				KM_LCD_Write_Cmd(0X01);
        KM_LCD_Write_Str((unsigned char *)"STARTING ENROLL");
        KM_Delay_ms(1000);				
      }
      break;
    }
	
    KM_Delay_ms(2000);  //Delay 
}




/************************************************************************************
*                              VerifyFingerprint()
*************************************************************************************/

int VerifyFingerprint(int ID)
{
	SendCommand(VERIFY, ID, 1000000);  //Send Verify command with timeout

  error_code = GetErrorCode();       //Get the error code

  switch(error_code)
  {
		case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;       //Database is Empty
		
		case NACK_VERIFY_FAILED:
			return NACK_VERIFY_FAILED;   //No match for the given ID

    case NACK_COMM_ERR:
      return NACK_COMM_ERR;       //Communication error

    case NACK_DEV_ERR:
      return NACK_DEV_ERR;        //Device error

    case NACK_TIMEOUT:
      return NACK_TIMEOUT;        //Timeout error

    default:
			//Check for success response
      return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_VERIFICATION_FAIL;  
  }
}




/************************************************************************************
*                           VerifyFingerprint_LCD()
*************************************************************************************/

void VerifyFingerprint_LCD(int ID)
{
	int result=0;
	
	KM_LCD_Write_Cmd(0x01);
	KM_LCD_Write_Str((unsigned char *)"PLACE FINGER....");
	
  while(!(touch_flag == 1 && IsFingerPlaced() == SUCCESS && CaptureFinger() == SUCCESS)){;}

//  if(CaptureFinger() != SUCCESS)
//  {
//		KM_LCD_Write_Cmd(0x01);
//	  KM_LCD_Write_Str((unsigned char *)" ERROR: FINGER   CAPTURE FAILED");
//    return;
//  }	
	
	result = VerifyFingerprint(ID);  //Call VerifyFingerprint() with ID

  KM_LCD_Write_Cmd(0x01);         //Clear LCD before displaying message

  switch (result)
  {
		case NACK_DB_IS_EMPTY:
			KM_LCD_Write_Str((unsigned char *)"ERROR: DB IS EMPTY");
      break;

    case NACK_VERIFY_FAILED:
      KM_LCD_Write_Str((unsigned char *)"ERROR: NO MATCH");
      break;

    case NACK_COMM_ERR:
      KM_LCD_Write_Str((unsigned char *)"ERROR: COMM FAILURE");
      break;

    case NACK_DEV_ERR:
      KM_LCD_Write_Str((unsigned char *)"ERROR: SENSOR FAILURE");
      break;

    case NACK_TIMEOUT:
      KM_LCD_Write_Str((unsigned char *)"ERROR: TIMEOUT");
      break;

    case SUCCESS:
			KM_LCD_Write_Cmd(0x83);
      KM_LCD_Write_Str((unsigned char *)"ATTENDANCE");
		  KM_LCD_Write_Cmd(0xC5);
		  KM_LCD_Write_Str((unsigned char *)"MARKED");

		  StoreAttendance(ID);        //Store attendance in a separate buffer
      break;

    default:
      KM_LCD_Write_Str((unsigned char *)"ERROR: VERIFY FAIL");
      break;
  }

	KM_Delay_ms(2000);
	return;
}



/*************************************************************************************
*                                  DeleteAll()
****************************************************************************************/

int DeleteAll(void)
{
	SendCommand(DELETEALL, 0, 100000);  // Send DELETE ALL command

  error_code = GetErrorCode();        //Get the error code

  switch(error_code)
  {
		case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;       //No fingerprints in the database

    case NACK_COMM_ERR:
      return NACK_COMM_ERR;          //Communication failure

    case NACK_DEV_ERR:
      return NACK_DEV_ERR;           //Sensor hardware failure

    case NACK_TIMEOUT:
      return NACK_TIMEOUT;          //Timeout error

    default:
			//Check for success response
      return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_DELETE_ALL_FAIL;
    }
}



/*************************************************************************************
*                                DeleteID()
***************************************************************************************/

int DeleteID(int ID)
{
	SendCommand(DELETEALL, ID, 100000);  // Send DELETE ALL command

  error_code = GetErrorCode();        //Get the error code

  switch(error_code)
  {
		case NACK_DB_IS_EMPTY:
			return NACK_DB_IS_EMPTY;       //No fingerprints in the database

    case NACK_COMM_ERR:
      return NACK_COMM_ERR;          //Communication failure

    case NACK_DEV_ERR:
      return NACK_DEV_ERR;           //Sensor hardware failure

    case NACK_TIMEOUT:
      return NACK_TIMEOUT;          //Timeout error

    default:
			//Check for success response
      return (FP_Buffer[8] == 0x30) ? SUCCESS : ERROR_DELETE_ID_FAIL;
    }
}



/************************************************************************************
*                                  UserMode()
*************************************************************************************/

void UserMode(void) 
{
  int index,fingerprintID=0;
    
  //Call function to take user ID input from keypad
  EnterUserID();
	
	if(ent_sw_flag)
	{
		return;
	}
    
  //Convert the entered keypad ID from 'password' buffer to integer
  userID = my_atoi(password);  

  //Search in the buffer array (even indices for keypad ID, odd indices for corresponding FP ID)
  for(index = 0; index < (MAX_USER_COUNT * 2); index += 2)  
  {
		if(ID_MAPPING[index] == userID)  //Check if keypad ID matches
    {
			fingerprintID = ID_MAPPING[index + 1];  //Get corresponding FP ID
            
      KM_LCD_Write_Cmd(0x01);
      KM_LCD_Write_Str((unsigned char *)"ID FOUND!");
      KM_Delay_ms(1000);

      VerifyFingerprint_LCD(fingerprintID);  //Call fingerprint verification
      return;
    }
  }

  //If no match is found
  KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Str((unsigned char *)"ID NOT FOUND");
  KM_Delay_ms(1500);
	return;
}



/************************************************************************************
*                                     AdminMode()
*************************************************************************************/

void AdminMode(void) 
{
	int option;
	
	EXTI_IMR &= ~(0x01 << 10);   //Disable Enter Switch Interrupt
	EXTI_IMR &= ~(0x01 << 9);    //Disable Down Switch Interrupt
	
	KM_LCD_Write_Cmd(0x01);  //Clear LCD
	KM_LCD_Write_Cmd(0x80);
  KM_LCD_Write_Str((unsigned char *)"ENTERING ADMIN");
	KM_LCD_Write_Cmd(0xC4);
  KM_LCD_Write_Str((unsigned char *)"MODE....");	
  KM_Delay_ms(1000);

  //Verify Admin Password before proceeding
  if(VerifyAdminPassword() != SUCCESS) 
  {
		EXTI_IMR |= (0x01 << 10);     //Re-Enable Enter Switch Interrupt
	  EXTI_IMR |= (0x01 << 9);      //Re-Enable Down Switch Interrupt
		
		KM_LCD_Write_Cmd(0x01);
	  KM_LCD_Write_Cmd(0x84);
    KM_LCD_Write_Str((unsigned char *)"EXITING");
	  KM_LCD_Write_Cmd(0xC2);
		KM_LCD_Write_Str((unsigned char *)"ADMINMODE...");
    KM_Delay_ms(1500);
    return;  //Exit if authentication fails
  }
	
	while(1) 
  {
		KM_LCD_Write_Cmd(0x01);  //Clear LCD before displaying options
    KM_LCD_Write_Str((unsigned char *)"1:ENROLL 2:DLT");
    KM_LCD_Write_Cmd(0xC0);  //Move to second line
    KM_LCD_Write_Str((unsigned char *)"3:REPORT 4:EXIT");    
		
		//option = Keypad_Scan();
		
		//Wait until a valid key is pressed
    while((option = Keypad_Scan()) == -1) {;}
        
    switch (option) 
		{
			case '1':
				Enroll();
        break;
			
      case '2':
				DeleteUser();
			  break;
			
      case '3':
				ShowReport();
        break;
			
      case '4':
				EXTI_IMR |= (0x01 << 10);     //Re-Enable Enter Switch Interrupt
	      EXTI_IMR |= (0x01 << 9);      //Re-Enable Down Switch Interrupt
			  KM_LCD_Write_Cmd(0x01);
			  KM_LCD_Write_Cmd(0x84);
        KM_LCD_Write_Str((unsigned char *)"EXITING");
			  KM_LCD_Write_Cmd(0xC2);
			  KM_LCD_Write_Str((unsigned char *)"ADMINMODE...");
        KM_Delay_ms(1500);
				return; //Exit admin mode
			
      default:
				KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"INVALID OPTION");
        KM_Delay_ms(1500);
    }
  }
}



/***********************************************************************************
*                                Enroll()
*************************************************************************************/

void Enroll(void) 
{
	int option;
  while(1)
  {
		KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"1:ENROLLUSER");
    KM_LCD_Write_Cmd(0xC0);
    KM_LCD_Write_Str((unsigned char *)"2:CHECK 3:BACK");

    //Wait until a valid key is pressed
    while((option = Keypad_Scan()) == -1) {;}
        
    switch(option) 
    {
			case '1':
				EnrollUser();
        break;
			
      case '2':
				CheckUserEnrolled();
        break;
			
      case '3':
				return;    //Exit Enroll function
			
      default:
				KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"INVALID OPTION");
        KM_Delay_ms(1500);
        return; //Exit after showing invalid option
    }
  }
}


/************************************************************************************
*                                    EnrollUser()
**************************************************************************************/

void EnrollUser(void)
{
	int step=0;
  int enroll_status =0;
  int enroll_start_status=0;
	int keypadID=0;
	int fingerprintID=0;
	char buffer[20];
	int i;
	

	//Take keypad ID input from admin
  EnterUserID();
  keypadID = my_atoi(password);  // Convert entered password to integer

	
	//Check if the keypad ID already exists in ID_Mapping buffer
  for(i = 0; i < (MAX_USER_COUNT * 2); i += 2) 
  {
		if(ID_MAPPING[i] == keypadID) 
    {
			KM_LCD_Write_Cmd(0x01);
      KM_LCD_Write_Str((unsigned char *)"ERROR: ID EXISTS");
      KM_Delay_ms(1000);
      return;
    }
  }
		
	
  //Get the next available fingerprint ID
  fingerprintID = GetEnrollCount();	
	
  if(fingerprintID < 0)  
  {
		switch(fingerprintID)
    {
			case NACK_TIMEOUT:	
				KM_LCD_Write_Cmd(0x01);
				KM_LCD_Write_Str((unsigned char *)"ERROR: TIMEOUT  IN ID FETCH");
        break;
			
      case NACK_DB_IS_EMPTY:
				KM_LCD_Write_Cmd(0x01);
				KM_LCD_Write_Str((unsigned char *)"ERROR: NO FINGERPRINTS IN DB");
        break;
			
      case NACK_COMM_ERR:
				KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"ERROR: COMMUNICATION FAILURE");
        break;
			
      default:
				KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"ERROR: UNKNOWN ID ISSUE");
    }
    return;
  }
	
	KM_Delay_ms(2000);

	
  KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Str((unsigned char *)"STARTING TO ENROLL FINGERPRINT");
  KM_Delay_ms(1000);
		
  //Start Enrollment Process
  enroll_start_status = EnrollStart(fingerprintID);
	
  if(enroll_start_status != SUCCESS)
  {
		KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"ERROR: ENROLL FAIL");
    KM_Delay_ms(1500);
    return;
  }
	KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Str((unsigned char *)"ENROLL STARTED........");
	KM_Delay_ms(500);
	
	
	//Capture and enroll fingerprints sequentially
  for(step=1;step <= 3;step++)
  {
		sprintf(buffer,"PLACE FINGER %d",(step));
		KM_LCD_Write_Cmd(0x01);
		KM_LCD_Write_Str((unsigned char *)buffer);
		
		if(step!=1)
		{
			KM_LCD_Write_Cmd(0xC0);
			KM_LCD_Write_Str((unsigned char *)"[SAME AS BEFORE]");
		}

    //Wait until both touch_flag == 1 (button pressed) and IsFingerPlaced() is successful
    while(!(touch_flag == 1 && IsFingerPlaced() == SUCCESS && CaptureFinger() == SUCCESS)){;}

//    if(CaptureFinger() != SUCCESS)
//    {
//			KM_LCD_Write_Cmd(0x01);
//			KM_LCD_Write_Str((unsigned char *)" ERROR: FINGER  CAPTURE FAILED");
//      return;
//    }

        
    switch(step)
    {
			case 1:
				enroll_status = Enroll1();
			  break;
			
      case 2:
        enroll_status = Enroll2();
			  
        break;
			
      case 3:
        enroll_status = Enroll3();
        break;
    }

    if(enroll_status != SUCCESS)
    {
			switch (enroll_status)
      {
				case NACK_BAD_FINGER:
					KM_LCD_Write_Cmd(0x01);
					KM_LCD_Write_Str((unsigned char *)"ERROR: POOR FINGERPRINT QUALITY");
          break;
				
        case NACK_ENROLL_FAILED:
					KM_LCD_Write_Cmd(0x01);
          KM_LCD_Write_Str((unsigned char *)"ERROR: ENROLLMENT FAILED");
          break;
				
        case NACK_TURN_ERR:
					KM_LCD_Write_Cmd(0x01);
          KM_LCD_Write_Str((unsigned char *)"ERROR: WRONG SEQUENCE");
          break;
				
        case NACK_TIMEOUT:
					KM_LCD_Write_Cmd(0x01);
          KM_LCD_Write_Str((unsigned char *)"ERROR: TIMEOUT IN ENROLLMENT");
          break;
				
        default:
					KM_LCD_Write_Cmd(0x01);
          KM_LCD_Write_Str((unsigned char *)"ERROR: UNKNOWN ENROLLMENT ISSUE");
      }
			KM_Delay_ms(1500);
      return;
    }

		KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"REMOVE FINGER");
		
    //Wait for button release (touch_flag == 0) (which also results in finger removal)
    while(touch_flag == 1);
		
		KM_LCD_Write_Cmd(0x01);
		sprintf(buffer,"ENROLL%d SUCCESS",step);
    KM_LCD_Write_Str((unsigned char *)buffer);
	  KM_Delay_ms(1000);
		
  }

	
	//Store Keypad ID (odd index) and Fingerprint ID (even index) of ID_Mapping buffer
  for(i = 0; i < (MAX_USER_COUNT * 2); i += 2) 
  {
		if(ID_MAPPING[i] == 0)  //Find a empty slot
    {
			ID_MAPPING[i] = keypadID;
      ID_MAPPING[i + 1] = fingerprintID;
      break;
    }
  }

	
	KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Str((unsigned char *)"ENROLL SUCCESS!");
	KM_Delay_ms(1000);
}



/************************************************************************************
*                              CheckUserEnrolled()
*************************************************************************************/

int CheckUserEnrolled(void)
{
	int userID, fpID, checkStatus, found, confirm, i;

  EnterUserID(); // Function to take user ID input
  userID = my_atoi(password);

  //Find the corresponding fingerprint ID
  found = 0;
  for(i = 0; i < MAX_USER_COUNT; i += 2) 
  {
		if(ID_MAPPING[i] == userID) 
    {
			fpID = ID_MAPPING[i + 1];
      found = 1;
      break;
    }
  }

  if(!found)
  {
		KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"ID NOT FOUND");
    KM_Delay_ms(1500);
		
		//Ask admin if they want to enroll a new user
    KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"ENROLL NEW USER?");
    KM_LCD_Write_Cmd(0xC0);
    KM_LCD_Write_Str((unsigned char *)"1:YES  2:NO");

    //confirm = Keypad_Scan();
		//Wait until a valid key is pressed
    while((confirm = Keypad_Scan()) == -1) {;}
			
    if(confirm == '1')
    {
			EnrollUser();
    }
    return -1; // Return error
  }

  //Check if fingerprint is enrolled
  checkStatus = CheckEnrolled(fpID);

  KM_LCD_Write_Cmd(0x01);
  if(checkStatus == SUCCESS)
  {
		KM_LCD_Write_Str((unsigned char *)"ENROLLED");
  }
  else if(checkStatus == NACK_IS_NOT_USED)
  {
		KM_LCD_Write_Str((unsigned char *)"NOT ENROLLED");
  }
  else if(checkStatus == NACK_ENROLL_FAILED)
  {
		KM_LCD_Write_Str((unsigned char *)"ID FOUND BUT");
    KM_LCD_Write_Cmd(0xC0);
    KM_LCD_Write_Str((unsigned char *)"NO ENROLLMENT");
  }
  KM_Delay_ms(2000);

  //If not enrolled, ask for confirmation to enroll
  if(checkStatus == NACK_IS_NOT_USED || checkStatus == NACK_ENROLL_FAILED)
  {
		KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"ENROLL NOW?");
    KM_LCD_Write_Cmd(0xC0);
    KM_LCD_Write_Str((unsigned char *)"1:YES 2:NO");

    //confirm = Keypad_Scan();
		//Wait until a valid key is pressed
    while((confirm = Keypad_Scan()) == -1) {;}
			
    if (confirm == '1')
    {
			EnrollUser();
    }
  }
    
  return checkStatus;
}



/*************************************************************************************
*                                DeleteUser()
**************************************************************************************/

void DeleteUser(void)
{
	int option, userID, fpID, found = 0, deleteStatus, i;

  while (1)
  {
		KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"1:DLT_ID");
    KM_LCD_Write_Cmd(0xC0);
    KM_LCD_Write_Str((unsigned char *)"2:DLT_ALL 3:BACK");

    //Wait until a valid key is pressed
    while((option = Keypad_Scan()) == -1) {;}

    switch(option)
    {
			case '1': //Delete specific ID
        EnterUserID();
        userID = my_atoi(password);

        //Search for the fingerprint ID in ID_MAPPING
        for (i = (MAX_USER_COUNT * 2)-2; i >= 0 ; i -= 2)
        {
					if(ID_MAPPING[i] == userID)
          {
						fpID = ID_MAPPING[i + 1];
            found = 1;
            break;
          }
        }

        if(!found)
        {
					KM_LCD_Write_Cmd(0x01);
          KM_LCD_Write_Str((unsigned char *)"ID NOT FOUND");
          KM_Delay_ms(1500);
          return;           //Go back to Delete menu
        }

        //Confirm deletion
        KM_LCD_Write_Cmd(0x01);
        KM_LCD_Write_Str((unsigned char *)"CONFIRM DELETE?");
        KM_LCD_Write_Cmd(0xC0);
        KM_LCD_Write_Str((unsigned char *)"1:YES  2:NO");

        //Wait until a valid key is pressed
        while((option = Keypad_Scan()) == -1) {;}

        if(option == '1')
        {
					deleteStatus = DeleteID(fpID);

          KM_LCD_Write_Cmd(0x01);
          if(deleteStatus == SUCCESS)
          {
						KM_LCD_Write_Str((unsigned char *)"DELETED SUCCESSFULLY");
						
            //Remove from ID_MAPPING
            for(i = (MAX_USER_COUNT * 2)-2; i >= 0 ; i -= 2)
            {
							if(ID_MAPPING[i] == userID)
              {
								ID_MAPPING[i] = 0;
                ID_MAPPING[i + 1] = 0;
                break;
              }
            }
						
						//Remove from attendance_log
            for(i = 0; i < MAX_ATTENDANCE_RECORDS; i += 2)
            {
							if(attendance_log[i] == fpID)
              {
								attendance_log[i] = 0;
                attendance_log[i + 1] = 0;
                break;
              }
            }
          }
					
          else
          {
						KM_LCD_Write_Str((unsigned char *)"DELETE FAILED");
          }
          KM_Delay_ms(1500);
        }
        return;

				
     case '2': //Delete All
			 KM_LCD_Write_Cmd(0x01);
       KM_LCD_Write_Str((unsigned char *)"CONFIRM DLT ALL?");
       KM_LCD_Write_Cmd(0xC0);
       KM_LCD_Write_Str((unsigned char *)"1:YES  2:NO");

       //Wait until a valid key is pressed
       while((option = Keypad_Scan()) == -1) {;}

       if(option == '1')
       {
				 deleteStatus = DeleteAll();
         KM_LCD_Write_Cmd(0x01);
         if(deleteStatus == SUCCESS)
         {
           //Manually clear ID_MAPPING array
           for(i = (MAX_USER_COUNT*2)-1; i >= 0 ; i--) 
           {
						 ID_MAPPING[i] = 0;
           }
					 
           //Clear attendance_log
           for(i = 0; i < MAX_ATTENDANCE_RECORDS; i++)
           {
						 attendance_log[i] = 0;
           }
					 
           KM_LCD_Write_Str((unsigned char *)"ALL ID DELETED");
					 					 
				 }
         else
         {
					 KM_LCD_Write_Str((unsigned char *)"DATABASE EMPTY");
         }
         KM_Delay_ms(1500);
       }
       return;

			 
			 
     case '3': //Back
			 return;

     default:
			 KM_LCD_Write_Cmd(0x01);
       KM_LCD_Write_Str((unsigned char *)"INVALID OPTION");
       KM_Delay_ms(1500);
		   return;
   }
  }
}



/************************************************************************************
*                              StoreAttendance()
*************************************************************************************/

void StoreAttendance(int fingerprintID) 
{
//	int index;
//	
//	//Search for the fingerprint ID in attendance_log[]
//  for(index = 0; index < MAX_ATTENDANCE_RECORDS; index ++)  
//  {
//		if(attendance_log[index] == fingerprintID)  //If fingerprint ID is found
//    {
//			attendance_log[index + 1]++;  //Increment the attendance count (stored in the next index)
//      return;
//    }
//  }

//  //If ID is not found, add a new record
//  if(attendance_index < fingerprintID - 1)  
//  {
//		attendance_log[attendance_index] = fingerprintID;  //Store fingerprint ID at even index
//    attendance_log[attendance_index + 1] = 1;  //Initialize attendance count at next index
//        
//    attendance_index += 2;  //Move to the next available slot
//		return;           //Exit function after adding a new entry
//  }
// 

	
	if(fingerprintID > MAX_ATTENDANCE_RECORDS)
	{
		KM_LCD_Write_Cmd(0x01);
    KM_LCD_Write_Str((unsigned char *)"ATTENDANCE FULL");
    KM_Delay_ms(1500);
    return;  //Exit function after handling full log
	}
	
	attendance_log[fingerprintID]++;
}



/************************************************************************************
*                                 ShowReport()
**************************************************************************************/

void ShowReport(void)
{
	int userID, fingerprintID, index,attendanceCount;
	
	
  //Take user ID input from keypad
  EnterUserID();
  userID = my_atoi(password);

  //Search for user ID in ID_MAPPING
  for(index = 0; index < MAX_USER_COUNT; index += 2)
  {
		if(ID_MAPPING[index] == userID)  //Check if keypad ID matches
    {
			fingerprintID = ID_MAPPING[index + 1];  //Get corresponding fingerprint ID

      //Search for fingerprint ID in attendance_log   
			attendanceCount = attendance_log[fingerprintID];  //Get attendance count

			KM_LCD_Write_Cmd(0x01);
			KM_LCD_Write_Str((unsigned char *)"ATTENDANCE: ");
			KM_LCD_Write_Cmd(0xC0);  

			sprintf(buffer, "%d", attendanceCount);  //Convert number to string
			KM_LCD_Write_Str((unsigned char *)buffer);  //Display count

			KM_Delay_ms(2000);
			return;
    }

//  If fingerprint ID found but no attendance record
//  KM_LCD_Write_Cmd(0x01);
//  KM_LCD_Write_Str((unsigned char *)"NO ATTENDANCE");
//	KM_Delay_ms(1500);
//	return;

  }
  //If user ID is not found in the system
  KM_LCD_Write_Cmd(0x01);
  KM_LCD_Write_Str((unsigned char *)"USER NOT FOUND");
  KM_Delay_ms(1500);
	return;
}



/****************************************************************************************/
