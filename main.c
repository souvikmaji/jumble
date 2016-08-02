#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aspell.h"

AspellConfig* spell_config;
AspellCanHaveError* possible_err;
AspellSpeller* spell_checker;


void permute(char* a, int l, int r);
void swap(char *p, char *r);
void initAspell();
void closeAspell();

void permutef(char* original, int depth, char* permutation, int* used)
{
	int length = strlen(original);

	if (length == depth) {
		permutation[length] = '\0';
		printf("%s", permutation);
	}
	else {
		int i;

		for (i = 0; i < length; ++i) {
			if (!used[i]) {
				used[i] = 1;
				permutation[depth] = original[i];
				permutef(original, depth + 1, permutation, used);
				used[i] = 0;
			}
		}
	}
}

int main(int argc, char** argv)
{
	initAspell();

	if (argc < 2) {
		printf("Usage: ./jumble [word_list].\n");
		return -1;
	}

	for (int i = 1; i < argc; i++) {
		char* word = argv[i];
		int n = strlen(word);
		permute(word, 0, n-1);
		int* used = (int*) malloc(sizeof(int) * n);
		char* permutation = (char*) malloc(sizeof(char) * (n+1));
		//permutef(word, 0, permutation, used);
	}

	closeAspell();
	return 0;
}

void permute(char *a, int l, int r)
{
	int i;
	if(l == r){
		int correct = aspell_speller_check(spell_checker, a, -1);
		if(correct)	{
			printf("%s: Correct\n", a);
		}
		else if(!correct){
		}
		else{
			 fprintf(stderr, "Error: %s\n", 
			 		 aspell_speller_error_message(spell_checker));
		}
	}
	else{
		for(i = l; i <= r; i++){
			swap((a+l),(a+i));
			permute(a, l+1, r);
			swap((a+l),(a+i));	//backtrack
		}
	}
}

void swap(char *p, char *q)
{
	int temp = *p;
	*p = *q;
	*q = temp;
}


void initAspell()
{
	spell_config = new_aspell_config();
	aspell_config_replace(spell_config, "lang", "en_US");	//set language
	possible_err = new_aspell_speller(spell_config);
	spell_checker = 0;

	if (aspell_error_number(possible_err) != 0) {
		printf("%s ", aspell_error_message(possible_err));
	}
	else {
		spell_checker = to_aspell_speller(possible_err);
	}
}

void closeAspell()
{
	delete_aspell_config(spell_config);
	delete_aspell_speller(spell_checker);
}
