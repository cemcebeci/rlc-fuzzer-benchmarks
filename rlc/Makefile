all: blackjack crazy_eights tic_tac_toe
	$(MAKE) -C blackjack && cp blackjack/build/* build
	$(MAKE) -C crazy_eights && cp crazy_eights/build/* build
	$(MAKE) -C tic_tac_toe && cp tic_tac_toe/build/* build
	
clean:
	rm -f build/* && $(MAKE) -C blackjack clean && $(MAKE) -C crazy_eights clean && $(MAKE) -C tic_tac_toe clean