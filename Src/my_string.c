/**************************************************************************************
* File :my_string.c
* Description : This file contains the function definitions related to string operations.
***************************************************************************************/


/******************strcmp() function******************/

int my_strcmp(char str1[], char str2[]) 
{
	int i = 0;

  while(str1[i] != '\0' && str2[i] != '\0') 
	{
		if(str1[i] != str2[i]) 
	  {
			return str1[i] - str2[i];
    }
    i++;
  }
    
  return str1[i] - str2[i];
}


/********************atoi() function*******************/

int my_atoi(char str[]) 
{
	int result = 0;
  int sign = 1;
  int i = 0;

  while (str[i] == ' ')  //Skip starting whitespaces
	{
		i++;
  }

  //Check for '+' or '-' sign
  if (str[i] == '-' || str[i] == '+') 
	{
		if (str[i] == '-') 
		{
			sign = -1;
    }
    i++;
  }

  //Convert characters to integer
  while (str[i] >= '0' && str[i] <= '9') 
	{
		result = result * 10 + (str[i] - '0');
    i++;
  }

  return sign * result;
}
