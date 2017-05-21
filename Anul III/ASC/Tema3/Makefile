DIRS=ppu spu

include $(CELL_TOP)/buildutils/make.footer

run:
	bash -c 'time ./ppu/tema3_ppu /tmp/images ./result $(F)'
	
runs:
	./serial /tmp/images ./result_serial $(F)
		