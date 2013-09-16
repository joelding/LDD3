SUBDIRS = ex01 ex02 

subdirs:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
		done

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
		done

.PHONY: subdirs clean $(SUBDIRS)

subdirs: $(SUBDIRS)
