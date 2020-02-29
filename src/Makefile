SERV_DIR = server
CLI_DIR = client
SUB_DIRS = $(SERV_DIR) $(CLI_DIR)

all:
	$(Q) for dir in $(SUB_DIRS); do \
		$(MAKE) -C $$dir $@; \
	done

server:
	$(Q) $(MAKE) -C $(SERV_DIR) all;

client:
	$(Q) $(MAKE) -C $(CLI_DIR) all;
