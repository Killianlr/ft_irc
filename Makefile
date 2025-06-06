
################################################################################
#                                   VARIABLES                                  #
################################################################################

NAME = ircserv
CC = c++

CFLAGS = -Werror -Wall -Wextra -std=c++98 -g3

SRCS_PATH = srcs/
INC_PATH = includes/
OBJS_PATH = .obj/

# Source files and obj files
SRCS = $(addprefix $(SRCS_PATH), $(SRCS_FILES))
OBJS = $(SRCS:$(SRCS_PATH)%.cpp=$(OBJS_PATH)%.o)
INCS = -I$(INC_PATH)

################################################################################
#                                   SRCS_FILES                                 #
################################################################################

SRCS_FILES =	main.cpp \
\
					commandhandler/channel/cmdJoin.cpp \
					commandhandler/channel/cmdKick.cpp \
					commandhandler/channel/cmdMode.cpp \
					commandhandler/channel/cmdTopic.cpp \
					commandhandler/channel/cmdPart.cpp \
				\
					commandhandler/communication/cmdNames.cpp \
					commandhandler/communication/cmdPing.cpp \
					commandhandler/communication/cmdPrivmsg.cpp \
				\
					commandhandler/registration/cmdInvite.cpp \
					commandhandler/registration/cmdNick.cpp \
					commandhandler/registration/cmdPass.cpp \
					commandhandler/registration/cmdUser.cpp \
				\
				commandhandler/CommandHandler.cpp \
\
				error/IRCException.cpp \
\
				server/Channel.cpp \
				server/Client.cpp \
				server/IRCServer.cpp \
				

################################################################################
#                                    COlORS                                    #
################################################################################

COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

define run_and_test
printf "%b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(@F)$(NO_COLOR)\r"; \
	$(1) 2> $@.log; \
	RESULT=$$?; \
	if [ $$RESULT -ne 0 ] || [ -s $@.log ]; then \
		printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"   ; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"   ; \
	else  \
		printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"   ; \
	fi; \
	cat $@.log; \
	rm -f $@.log 2> /dev/null; \
	exit $$RESULT
endef

OK_STRING    = "[OK]"
ERROR_STRING = "[ERROR]"
WARN_STRING  = "[WARNING]"
COM_STRING   = "Compiling"

################################################################################
#                                   COMMANDS                                   #
################################################################################

all: $(OBJS_PATH) $(NAME)

$(OBJS_PATH):
	@mkdir -p $(OBJS_PATH)
	@mkdir -p .obj/commandhandler/channel
	@mkdir -p .obj/commandhandler/registration
	@mkdir -p .obj/commandhandler/communication
	@mkdir -p .obj/error
	@mkdir -p .obj/server

$(NAME): $(OBJS_PATH) $(OBJS)
	@$(call run_and_test, $(CC) $(CFLAGS) -o $@ $(OBJS))

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	@$(call run_and_test, $(CC) $(CFLAGS) $(INCS) -c $< -o $@)

clean:
	@$(call run_and_test, rm -rf $(OBJS_PATH))

fclean: clean
	@$(call run_and_test, rm -f $(NAME))

re: fclean all

.PHONY: all clean fclean re