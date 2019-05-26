#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_MODE

int main(int argc, char* argv[])
{
    char xstring[100];
    char xcmd[200];
    time_t curr_time;
    struct tm *ltp;
    struct tm loc_time;

    int nread;
    int fp;
    char buf_s[20], bat_status[15];
    int bat_capacity, bat_present;

    while (1) {


        // TIME

        curr_time = time(NULL);
        ltp = localtime(&curr_time);
        loc_time = *ltp;

#ifdef DEBUG_MODE
        printf("%d:%d:%d\n", loc_time.tm_hour, loc_time.tm_min, loc_time.tm_sec);
#endif

        // BATTERY 

        // capacity
        fp = open("/sys/class/power_supply/BAT0/capacity", O_RDONLY);
        if (read(fp, buf_s, 10) < 0) {
        //if (fscanf(fp, "%d", &bat_capacity)) {
            printf("Error reading battery capacity\n");
        } else {
#ifdef DEBUG_MODE
            printf("Battery capacity: %s\n", buf_s);
            //printf("Battery capacity: %d\n", bat_capacity);
#endif        
            bat_capacity = atoi(buf_s);
        }
        close(fp);

        // status
        fp = open("/sys/class/power_supply/BAT0/status", O_RDONLY);
        nread = read(fp, bat_status, 14);
        if (nread < 0) {
            printf("Error reading battery status\n");
        } else {
            bat_status[nread] = '\0';
#ifdef DEBUG_MODE
            printf("Battery status: %s\n", bat_status);
#endif        
        }
        close(fp); 


        // FINAL STRING
        sprintf(xstring, "BAT: %d%% %c   %d:%d", bat_capacity, bat_status[0], loc_time.tm_hour, loc_time.tm_min);
#ifdef DEBUG_MODE
        printf("%s\n", xstring);
#endif        
        sprintf(xcmd, "xsetroot -name \"%s\"", xstring);
#ifndef DEBUG_MODE
        system(xcmd);
#endif        
        sleep(1);
    }
    
}
