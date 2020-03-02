SERV_DIR = server
CLI_DIR = client
SUB_DIRS = $(SERV_DIR) $(CLI_DIR)

.PHONY = all
all:
	$(Q) for dir in $(SUB_DIRS); do \
		$(MAKE) -C $$dir $@; \
	done

.PHONY = server
server:
	$(Q) $(MAKE) -C $(SERV_DIR) all;

.PHONY = client
client:
	$(Q) $(MAKE) -C $(CLI_DIR) all;
