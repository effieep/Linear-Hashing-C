#
# In order to execute this "Makefile" just type "make"
#	A. Delis (ad@di.uoa.gr)
#

OBJS 	= main.o LH_Implementation.o List_Implementation.o Initialize_Data.o App_Functions.o
SOURCE	= main.c LH_Implementation.c List_Implementation.c Initialize_Data.c App_Functions.o
HEADER  = linear_hashing.h inverted_index.h data.h functions.h
OUT  	= mvote
CC	= gcc
FLAGS   = -g -c 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

# create/compile the individual files >>separately<< 
main.o: main.c
	$(CC) $(FLAGS) main.c

List_Implementation.o: List_Implementation.c
	$(CC) $(FLAGS) List_Implementation.c

LH_Implementation.o: LH_Implementation.c
	$(CC) $(FLAGS) LH_Implementation.c

App_Functions.o: App_Functions.c
	$(CC) $(FLAGS) App_Functions.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)