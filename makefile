all:
	cd src; make all
	cd test; make all

clean:
	cd test; make clean
	cd src; make clean
