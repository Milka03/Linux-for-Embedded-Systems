CC=$(CROSS_COMPILE)gcc
OBJS := timer.o
timer: $(OBJS)
	$(CC) -o timer $(CFLAGS) $(LDFLAGS) $(OBJS) -l gpiod
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

