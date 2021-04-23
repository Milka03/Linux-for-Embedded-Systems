#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>

struct gpiod_line_event event_without_bounce(struct gpiod_line *line, const struct timespec *timeout, struct gpiod_line_event oldevent)
{
    const struct timespec ts = { 0, 200000000 };
    struct gpiod_line_event event;
    int event_value, read_value;
    event_value = gpiod_line_event_wait(line, timeout);
    int new_value = 0;
    
    event = oldevent;
    if(event_value == 1)
    {
	    read_value = gpiod_line_event_read(line, &event);
	    event = event_without_bounce(line, &ts, event);
    }

    return event;
}

struct gpiod_line_event bulk_event_without_bounce(struct gpiod_line_bulk bulk, const struct timespec *timeout, struct gpiod_line_event oldevent)
{
    const struct timespec ts = { 0, 200000000 };
    struct gpiod_line_event event;
    int event_value, read_value;
    struct gpiod_line_bulk result;
    struct gpiod_line *line;
    event_value = gpiod_line_event_wait_bulk(&bulk, timeout, &result);
    line = result.lines[0];
    int new_value = 0;
    
    event = oldevent;
    if(event_value == 1)
    {
	    read_value = gpiod_line_event_read(line, &event);
	    event = event_without_bounce(line, &ts, event);
	    if(line == bulk.lines[0])
	        event.event_type = 3;
	    else if(line == bulk.lines[1])
	        event.event_type = 4;
	    else if(line == bulk.lines[2])
	        event.event_type = 5;
    }

    return event;
}

int main(int argc, char **argv)
{
    struct gpiod_chip *chip;
    struct gpiod_line_bulk buttons;
    struct gpiod_line *led24;
    struct gpiod_line *led25;
    struct timespec ts = { 2, 0 };
    struct gpiod_line_event event, start, lap;
    bool timer = false;
    bool isLap = false;
    int seconds, nanoseconds;

    unsigned int button_count = 3;
    unsigned int buttons_offset[] = {12, 13, 14};

    chip = gpiod_chip_open_by_name("gpiochip0");
    if(!chip){
	    printf("Error with gpiod_chip_open\n");
	    return -1;
    }

    if(!(led24 = gpiod_chip_get_line(chip, 24))){
	    printf("Error with gpiod_chip_get_line for led 24\n");
	    gpiod_chip_close(chip);
	    return -1;
    }

    if(!(led25 = gpiod_chip_get_line(chip, 25))){
	    printf("Error with gpiod_chip_get_line for led 25\n");
	    gpiod_chip_close(chip);
	    return -1;
    }

    if(gpiod_chip_get_lines(chip, buttons_offset, button_count, &buttons)){
	    printf("Error with gpiod_chip_get_lines for buttons\n");
	    gpiod_chip_close(chip);
	    return -1;
    }

    if(gpiod_line_request_bulk_falling_edge_events(&buttons, "foobar")){
	    printf("Error with gpiod_line_request_bulk_falling_edge_event\n");
	    gpiod_chip_close(chip);
	    return -1;
    }

    if(gpiod_line_request_output(led24, "foobar", 0)){
	    printf("Error with gpiod_line_request_output for led 24\n");
	    gpiod_chip_close(chip);
	    return -1;
    }

    if(gpiod_line_request_output(led25, "foobar", 0)){
	    printf("Error with gpiod_line_request_output for led 25\n");
	    gpiod_chip_close(chip);
	    return -1;
    }
    
    while(true){
        event.event_type = 0;   
        event = bulk_event_without_bounce(buttons, &ts, event);
        if(event.event_type == 3){
            if(!timer){
                gpiod_line_set_value(led24, 1);
                timer = true;
                start = event;
		        lap = event;
		        printf("*** TIMER STARTED ***\n");
            }else{
                gpiod_line_set_value(led24, 0);
                timer = false;
                seconds = event.ts.tv_sec - start.ts.tv_sec;
                nanoseconds = event.ts.tv_nsec - start.ts.tv_nsec;
                if(nanoseconds < 0){
                    nanoseconds += 1000000000;
                    seconds -= 1;
                }
                printf("Time elapsed: [%ld.%ld]\n\n",  seconds, nanoseconds);
            }
        }
	    else if(event.event_type == 4){
	        if(timer){
		        if(!isLap){
		            gpiod_line_set_value(led25, 1);
            	    seconds = event.ts.tv_sec - start.ts.tv_sec;
            	    nanoseconds = event.ts.tv_nsec - start.ts.tv_nsec;
            	    if(nanoseconds < 0){
                	    nanoseconds += 1000000000;
                	    seconds -= 1;
            	    }
            	    printf("Lap start time:  [%ld.%ld]\n",  seconds, nanoseconds);
		            lap = event;
		            isLap = true;
		        }else{
		            seconds = event.ts.tv_sec - start.ts.tv_sec;
            	    nanoseconds = event.ts.tv_nsec - start.ts.tv_nsec;
        	        if(nanoseconds < 0){
                	    nanoseconds += 1000000000;
                	    seconds -= 1;
            	    }
		            printf("Lap end time: [%ld.%ld]\n",  seconds, nanoseconds);
		            seconds = event.ts.tv_sec - lap.ts.tv_sec;
            	    nanoseconds = event.ts.tv_nsec - lap.ts.tv_nsec;
            	    if(nanoseconds < 0){
                	    nanoseconds += 1000000000;
                	    seconds -= 1;
            	    }
            	    printf("Lap duration:  [%ld.%ld]\n\n",  seconds, nanoseconds);
		            isLap = false;
		            gpiod_line_set_value(led25, 0);
		        }
	        }
	    }
	    else if(event.event_type == 5){
	        printf("Goodbye!\n");
	        gpiod_line_set_value(led24, 0);
	        gpiod_line_set_value(led25, 0);
	        break;
	    }
    }
    gpiod_chip_close(chip);
    return EXIT_SUCCESS;
}
