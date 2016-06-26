TGT := test
SRCS := test.c dlist.c stack.c calc.c 
CFLAGS := -g -Wall -D_DEBUG
DEP := .dep

all: $(TGT)

$(TGT) : ${SRCS:%.c=%.o}
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

.PHONY : clean all  

sinclude $(DEP)

$(DEP) : $(SRCS)
	$(CC) $(CFLAGS) -MM $^ > $@

clean: 
	$(RM) ${SRCS:%.c=%.o} $(TGT) $(DEP)
