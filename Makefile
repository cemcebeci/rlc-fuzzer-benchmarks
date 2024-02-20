all:
	$(MAKE) -C rlc && cp rlc/build/* build
	$(MAKE) -C open_spiel && cp open_spiel/build/* build
clean:
	rm -f build/* && $(MAKE) -C open_spiel clean && $(MAKE) -C rlc clean