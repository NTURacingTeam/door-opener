#######################################
# user added source files
#######################################
# this variable is appended to C_SOURCES in the main Makefile
USER_SOURCES = \
Core/Src/user_main.c \
Core/Src/nec.c \
Core/Src/ui_feedback.c

USER_DEFS = 

ifdef GATE_PASSWORD
USER_DEFS += -DGATE_PASSWORD=\"$(GATE_PASSWORD)\"
endif

