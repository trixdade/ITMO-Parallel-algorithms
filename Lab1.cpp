#include <iostream>
#include <string>


using namespace std;

#define OUT 0
#define IN 1

unsigned countWords(char *str)
{
    int state = OUT;
    unsigned wc = 0; // word count
 
    while (*str)
    {
        if (*str == ' ' || *str == '\n' || *str == '\t')
            state = OUT;
 
        else if (state == OUT)
        {
            state = IN;
            ++wc;
        }
        ++str;
    }
 
    return wc;
}

int main(int argc, char *argv[])
{
    int wc = countWords(argv[1]);
    cout << wc << endl;
	return 0;
}
