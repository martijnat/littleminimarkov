all: minimarkov do_training

clean:
	-rm minimarkov
	-rm model.q2.bin

minimarkov: minimarkov.c
	gcc minimarkov.c -Werror -Wunused -o minimarkov

do_training:
	cat data/pg*.txt > training_data.txt
	./minimarkov training_data.txt model.q2.bin
	rm training_data.txt
	./minimarkov model.q2.bin | head -n 8
