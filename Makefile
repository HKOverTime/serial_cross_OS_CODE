DEFS = -D_DEBUG_PRINT_
DEBUG_FLAGS += \
	$(DEFS)

LIB_SRCS = Serial_Base_API.c
LIB_OBJS = $(LIB_SRCS:.c=.o)

MAIN_SRC = main.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)

SRCS   = $(LIB_SRCS) $(MAIN_SRC)
TARGET = $(LIB_OBJS) $(MAIN_OBJ)

$(TARGET) : $(SRCS)
	cc $(DEBUG_FLAGS) -c $(SRCS)
	cc $(DEBUG_FLAGS) -o main $(TARGET) 
	rm $(TARGET)
pro  : $(PRO_SRCS)
	cc -c $(PRO_SRCS)
	cc -o main $(PRO_TARGET) 
	rm $(TARGET)
clean:
	rm main
	rm main.exe
