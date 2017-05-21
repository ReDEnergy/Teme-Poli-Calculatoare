GAMES=1000
H=4
L=4
DIST=0

server:
	rm -f s_server && ./bricks_server --games-no $(GAMES) --height $(H) --length $(L)

maker:
	pypy ./brickmaker.py $(DIST)

layer:
	pypy ./bricklayer.py


run44:
	rm -f s_server && ./bricks_server --games-no $(GAMES) --height 4 --length 4 &
	sleep 0.5
	./brickmaker --in-file ./distributions/dist1 &
	bash -c "time pypy ./bricklayer.py"

run85:
	rm -f s_server && ./bricks_server --games-no $(GAMES) --height 8 --length 5 &
	sleep 0.5
	./brickmaker --in-file ./distributions/dist2 &
	bash -c "time pypy ./bricklayer.py"

run86:
	rm -f s_server && ./bricks_server --games-no $(GAMES) --height 8 --length 6 &
	sleep 0.5
	./brickmaker --in-file ./distributions/dist3 &
	bash -c "time pypy ./bricklayer.py"

clean:
	rm -f s_server
	rm -f scores