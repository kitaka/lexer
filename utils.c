#include "utils.h"

char *find_string_between(char *source, char first, char last)
{
  	int i, j = 0, start_tracking = 0, buffer_len = 0;
  	int len = strlen(source);

	char buffer[1024];

	for (i = 0; i < len; i++) {
	  	if (start_tracking && source[i] != last) {
			buffer[j++] = source[i];		
			++buffer_len;
		}
		else if (source[i] == last) {
			break;
		}
		
	  	if (!start_tracking && source[i] == first) start_tracking = 1;
	}	  

	return strndup(buffer, buffer_len);
}

int find_pos_closing_character(char *source, char open, char close)
{
  	int i, brackets = 0;
	int len = strlen(source);

  	for (i = 0; i < len; i++) {
      		
	  	if (source[i] == open) {
			++brackets;
		}		  
		else if (source[i] == close) {
		  	--brackets;

	  		if (brackets == 0) 
			  	return i;
		}
	}

	return -1;	
}
