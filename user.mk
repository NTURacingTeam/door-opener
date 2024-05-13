#######################################
# user added source files
#######################################
# this variable is appended to C_SOURCES in the main Makefile
USER_SOURCES = \
Core/Src/user_main.c \
Core/Src/nec.c \
Core/Src/ui_feedback.c \
Core/Src/play_note.c

USER_DEFS = 

# custom variables to output colored texts
RED=$(shell tput setaf 1)
RESET=$(shell tput sgr0)
BOLD=$(shell tput bold)
YELLOW=$(shell tput setaf 3)
CYAN=$(shell tput setaf 6)