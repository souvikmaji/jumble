#include<iostream>
#include"aspell.h"
#include<cstring>
using namespace std;

AspellConfig *spell_config;
AspellCanHaveError *possible_err;
AspellSpeller *spell_checker;


void swap(char *x, char *y);
void permute(char *a, int l, int r);
void initAspell();
void closeAspell();

int main(int argc, char **argv)
{
	initAspell();

	for(int i = 1; i < argc; i++){
		char *word = argv[i];
		int n = strlen(word);
		permute(word, 0, n-1);
	}

	closeAspell();
	return 0;
}


void swap(char *x, char *y)
{
	char temp;
	temp =*x;
	*x = *y;
	*y = temp;
}

void permute(char *a, int l, int r)
{
	int i;
	if(l == r){
		int correct = aspell_speller_check(spell_checker, a, -1);
		if(correct)	{
			cout<<a<<": Correct\n";
		}
		else if(!correct){
		}
		else{
			cout << "Error: " << aspell_speller_error_message(spell_checker) <<'\n';
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


void initAspell()
{
	spell_config = new_aspell_config();	
	aspell_config_replace(spell_config, "lang", "en_US");	//set language
	
	possible_err = new_aspell_speller(spell_config);
	spell_checker = 0;
	if(aspell_error_number(possible_err) != 0)
		cerr << aspell_error_message(possible_err);
	else
		spell_checker = to_aspell_speller(possible_err);
}

void closeAspell()
{
	delete_aspell_config(spell_config);
	delete_aspell_speller(spell_checker);
}
