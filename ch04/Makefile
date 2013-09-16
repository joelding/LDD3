SUBDIRS = ex10 ex11 ex12 ex13 ex14 ex15 ex16

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
