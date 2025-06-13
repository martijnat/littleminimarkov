# Little Mini Markov (LLM)

Simple tiny markov chain implementation to generate text. I wanted to see how simple I could make it with half coherent ouput.

It will train on plaintext ascii input to learn to predict the next character.
For each 3 character input, it will generate a 4th input and then
continue from there.

It learns by reading every 4 bytes of data and recording how often it sees it. It will then compress the data to 2 bit precision, scaled by each 3 byte prefix. When generating it will use the relative probability of each 4th byte to randomly generate each succesive byte.

# sample output

```text
The have that have your grows her was to thing of the counded here signs of the said not this both the do this the most a fears but this and and with and heave man, the with a barks his to he so his mind strue sir, that is parties is dare ther here strese the his for my shall this in this and to be do not with that this for and her and stants that the be for his me ther had that so lear the place, that so lovery do not world, and that is that hear and so see to the straction the make more to the me that this so fathe come to have may have you must that I said thing a brotect and to the works he conter see there to the sir, and to thing of the come she were and her sir, said Monter that in there to him there work of this death thing and the more to that this long the she me think that stand the leave miness the with there a would him the lease and at works is who had and with a laster ther and and have your sir, which a made and here that and will his the here that he for him as that I can and strius, and to t
```

# Instructions
To build everything:

```bash
make
```

To run the model:

```bash
./minimarkov model.q2.bin
```

# Training set

The example training data is taken from project gutenburg. You can add
your own data in the /data folder and it will train on that. All
non-ascii characters are ignored, so unicode won't work.
