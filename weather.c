#include <stdio.h>
#include <libsocket/libinetsocket.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    if (argv[1] == NULL)
    {
        printf("Please supply a valid zip code\n");
        exit(1);
    }
    
    char *request = malloc(250);
    //char *client_id = "kk1CU9UdOTJ050z63ZuDh";
    //char *client_secret = "1y2kRweAUVgYoanjcWypsGBfbHPezYTOyQMDARXK";
    //sprintf(request,"/observations/%s?client_id=%s&client_secret=%s", argv[1], client_id, client_secret);
   
   strcat(strcat(strcat(request, "/observations/"), argv[1]), "?client_id=wQhXMMnxoRV4HNKoRLZrL&client_secret=rUOW0GEyf5bT9JhUzro2WQAuUpj3A7nFHgVCRGEK");
   
   // connect to website
    int fd = create_inet_stream_socket("api.aerisapi.com", "80", LIBSOCKET_IPv4, 0);
    if (fd == 1)
    {
        printf("Can't connect to service");
        exit(1);
    }
   
   // open socket number (fd) as a FILE   
   FILE *f = fdopen(fd, "r+");
    if (!f)
    {
        printf("Invalid socket file\n");
        exit(1);
    }
   
    // send GET request to web server
    fprintf(f, "GET %s HTTP/1.0\n", request);
    fprintf(f, "Host: api.aerisapi.com\n");
    fprintf(f, "\n");
    
    char city[32];
    char humidity[32];
    char winddirection[32];
    char windspeed[32];
    char state[12];
    char temp[32];
    char obtime[32];
    
    char line[1700];
    while (fgets(line, 1700, f))
    {   
        char *strtwinddirection = strstr(line, "\"windDir\"");
        if (strtwinddirection)
        {
            strtwinddirection += 11;
            char *endwinddirection = strstr(strtwinddirection, "\"");
            *endwinddirection = '\0';
            strcpy(winddirection, strtwinddirection);
        }
        
        char *strtwindspeed = strstr(line, "\"windSpeedMPH\"");
        if (strtwindspeed)
        {
            strtwindspeed += 15;
            char *endwindspeed = strstr(strtwindspeed, ",");
            *endwindspeed = '\0';
            strcpy(windspeed, strtwindspeed);
        }
        
        char *strthumidity = strstr(line, "\"humidity\"");
        if (strthumidity)
        {
            strthumidity += 11;
            char *endhumidity = strstr(strthumidity, ",");
            *endhumidity = '\0';
            strcpy(humidity, strthumidity);
        }
        
        char *strttemp = strstr(line, "\"tempF\"");
        if (strttemp)
        {
            strttemp += 8;
            char *endtemp = strstr(strttemp, ",");
            *endtemp = '\0';
            strcpy(temp, strttemp);
        }
        
        char *strtobtime = strstr(line, "obDateTime");
        if (strtobtime)
        {
            strtobtime += 13;
            char *endobtime = strstr(strtobtime, "\"");
            *endobtime = '\0';
            strcpy(obtime, strtobtime);
        }
        
        char *strtstate = strstr(line, "\"state");
        if (strtstate)
        {
            strtstate += 9;
            char *endstate = strstr(strtstate, "\"");
            *endstate = '\0';
            strcpy(state, strtstate);
        }
        
        char *strtcity = strstr(line, "name\"");
        if (strtcity)
        {
            
            strtcity += 7;
            char *endcity = strstr(strtcity, "\"");
            *endcity = '\0';
            strcpy(city, strtcity);
        }
        
        
    
    }   
       city[0] = toupper(city[0]);
       state[0] = toupper(state[0]);
       state[1] = toupper(state[1]);
    
       printf("Observation time: %s\n", obtime);
       printf("Location: %s, %s\n", city, state);
       printf("Temperature: %s F\n", temp);
       printf("Humidity: %s\n", humidity);
       printf("Wind: %s %s mph\n", winddirection, windspeed); 
       
       fclose(f);
}