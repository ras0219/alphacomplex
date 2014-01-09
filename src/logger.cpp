#include "logger.hpp"
#include "stdarg.h"

Logger::Logger(const char* filename)
{
	out_file=NULL;
	file_location.append(LOG_PREFIX);
	file_location.append(filename);
	fopen_s(&out_file, file_location.c_str(), "w"); //open for write-only
	if(out_file==NULL)
	{
		//print error
		printf("Error opening a log at file:%s",file_location.c_str());
		perror("Unable to open log file!");
	}

}

Logger::~Logger()
{
	if(out_file!=NULL)
	{
		fflush(out_file);
		fclose(out_file);
		out_file=NULL;
	}
	fflush(NULL); //flush everything as well
}

int Logger::Write(const char * format, ...)
{
	if(out_file==NULL) return -1;
	fprintf(out_file, "%f:", (float) clock());
	va_list args;
	va_start(args, format);
	int ret_val=vfprintf(out_file,format,args);
	va_end(args); //clean up va_args
	int ret_val2=fputc('\n',out_file);
	if(ret_val<0 || ret_val2<0)
	{
		printf("Error writing to log  at file:%s!",file_location.c_str());
		//attempt to recover
		clearerr(out_file);
		//return the error statement
		return EOF;
	}

	return ret_val+1;
}
