all: micromarkov do_training

clean:
	-rm micromarkov
	-rm model.q2.bin

micromarkov: micromarkov.c
	gcc micromarkov.c -Werror -Wunused -o micromarkov

do_training:
	cat data/pg*.txt > training_data.txt
	./micromarkov training_data.txt model.q2.bin
	rm training_data.txt
	./micromarkov model.q2.bin | head -n 1 | head -c 1024
