all: build do_training

clean:
	-rm train generate quant
	-rm model.q2.bin

build:
	gcc src/train.c -o train
	gcc src/quant.c -o quant
	gcc src/generate.c -o generate

do_training:
	cat data/*.txt > training_data.txt
	./train training_data.txt model.bin
	rm training_data.txt
	./quant model.bin model.q2.bin
	rm model.bin
	./generate model.q2.bin
