/**************************************************************************************
* File : FP_gt521f32.h
* Description : This file contains the Commands related to GT521F32 Fingerprint Sensor
***************************************************************************************/

#define FP_BUFF_SIZE           12
#define MAX_USER_COUNT         20
#define MAX_ATTENDANCE_RECORDS 100     //Maximum attendance entries
#define TIMEOUT_LIMIT          10000   //10sec              


#define SUCCESS                     1
#define ERROR_TIMEOUT               0
#define ERROR_ALREADY_ENROLLED      2
#define ERROR_ENROLLMENT_FAIL       3
#define ERROR_NO_FINGER             4
#define ERROR_CAPTURE_FAIL          5
#define ERROR_IDENTIFICATION_FAIL   6
#define ERROR_VERIFICATION_FAIL     7
#define ERROR_DELETE_ALL_FAIL       8
#define ERROR_DELETE_ID_FAIL        9


//INPUT COMMANDS
#define OPEN                0x01                 //initialization
#define CLOSE               0x02                 //termination
#define USBINTERNAL_CHECK   0x03                 //check if the connected usb device is valid
#define CHANGEBAUDRATE      0x04                 //change UART baud rste
#define SETIAPMODE          0x05                 //enter IAP mode inthis mode,FW upgrade is available
#define CMOSLED             0x12                 //control CMOS LED
#define GETENROLLCOUNT      0x20                 //get enrolled fingerprint count
#define CHECKENROLLED       0x21                 //check whether the specified id is already enrolled
#define ENROLLSTART         0x22                 //start an enrollment
#define ENROLL1             0x23                 //make 1st template for anenrollment
#define ENROLL2             0x24                 //make 2nd template for an enrollment
#define ENROLL3             0x25                 //make 3rd template for an enrollment,merge three templates into one template,save merged template to the database
#define ISPRESSFINGER       0x26                 //check if a finger is placed on the sensor
#define DELETEID            0x40                 //delete the fingerprint with the specified ID
#define DELETEALL           0x41                 //delete all fingerprints from the database
#define VERIFY              0x50                 //1:1 verification of the capturefingerprint image with the specified ID
#define IDENTIFY            0x51                 //1:N identification of the capture fingerprint image with the database
#define VERIFYTEMPLATE      0x52                 //1:1 verificatioin of a fingerprint template with the database
#define IDENTIFYTEMPLATE    0x53                 //1:N identification of a fingerprint template with the database
#define CAPTUREFINGER       0x60                 //capture a fingerprinyt image(256x256)from the sensor
#define MAKETEMPLATE        0x61                 //make template for transmission
#define GETIMAGE            0x62                 //download the captured fingerprint image(256x256)
#define GETRAWIMAGE         0x63                 //capture and download raw fingerprint image(320x240)
#define GETTEMPLATE         0x70                 //Download the template of the specified id
#define SETTEMPLATE         0x71                 //upload the template of the specified id
#define GETDATABASESTART    0x72                 //start database download,obsolete
#define GETDATABASEEND      0x73                 //end database download,obsolete
#define UPGRADE_FIRMWARE    0x80                 //not supported
#define UPGRADE_ISOCD_IMAGE 0x81                 //not supported
#define SETSECURITYLEVEL    0xF0                 //set security level
#define GETSECURITYLEVEL    0xF1                 //get security level
#define ACK                 0x30                 //ACKNOWLEDGE
#define NACK                0x31                 //NON-ACKNOWLEDGE



//ERROR COMMANDS
#define NACK_TIMEOUT                  0x1001                //obsolete,capture timeout
#define NACK_INVALID_BAUDRATE         0x1002                //obsolete,invalid serial baud rate
#define NACK_INVALID_POS              0x1003                //the specified ID is not in between 0-199
#define NACK_IS_NOT_USED              0x1004                //the specified ID is not used
#define NACK_IS_ALREADY_USED          0x1005                //the specified ID is already used
#define NACK_COMM_ERR                 0x1006                //Communication error
#define NACK_VERIFY_FAILED            0x1007                //1:1 Verification failure
#define NACK_IDENTIFY_FAILED          0x1008                //1:N Identifivation failure
#define NACK_DB_IS_FULL               0x1009                //The database is full
#define NACK_DB_IS_EMPTY              0x100A                //The database is empty
#define NACK_TURN_ERR                 0x100B                //OBSOLETE,Invalid order of the enrollment
#define NACK_BAD_FINGER               0x100C                //Too bad fingerprint
#define NACK_ENROLL_FAILED            0x100D                //Enrollment failure
#define NACK_IS_NOT_SUPPORTED         0x100E                //The specified command is not supported
#define NACK_DEV_ERR                  0x100F                //Device error,especially if crypto-chip is trouble
#define NACK_CAPTURE_CANCELLED        0x1010                //OBSOLETE,the capturing is cancelled
#define NACK_INVALID_PARAM            0x1011                //Invalid parameter
#define NACK_FINGER_IS_NOT_PRESSED    0x1012                //Finger is not pressed



void PreparePacket(unsigned char packet[],unsigned char command,unsigned char parameter);
void SendCommand(unsigned char command, unsigned short parameter,unsigned int Timeout_Value);
int IsFingerPlaced(void);
void EnrollUser(void);
int IdentifyFingerprint(void);
int VerifyFingerprint(int ID);
int DeleteAll(void);
int DeleteID(int ID);
int CaptureFinger(void);
void IdentifyFingerprint_LCD(void);
int CheckUserEnrolled(void);
void DeleteUser(void);
void StoreAttendance(int fingerprintID) ;
void AdminMode(void) ;
void VerifyFingerprint_LCD(int ID);
void UserMode(void) ;
void ShowReport(void);
void Enroll(void) ;

