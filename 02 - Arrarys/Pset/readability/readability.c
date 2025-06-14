#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Compute the Coleman-Liau index
    double L=1.0*letters/words*100;
    double S=1.0*sentences/words*100;
    double index=0.0588*L-0.296*S-15.8;

    int level=round(index);

    // Print the grade level
    if(level>=16)
        printf("Grade 16+\n");
    else if(level<1)
        printf("Before Grade 1\n");
    else
        printf("Grade %d\n",level);

    return 0;
}

int count_letters(string text)
{
    // Return the number of letters in text
    int LENGTH=strlen(text);
    int count=0;

    for(int i=0;i<LENGTH;i++)
        if(isalnum(text[i]))
            count++;

    return count;
}

int count_words(string text)
{
    // Return the number of words in text
    int LENGTH=strlen(text);
    int count=0;

    for(int i=0;i<LENGTH;i++)
        if(isblank(text[i]))
            count++;

    return count+1;
}

int count_sentences(string text)
{
    // Return the number of sentences in text
    int LENGTH=strlen(text);
    int count=0;

    for(int i=0;i<LENGTH;i++)
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
            count++;

    return count;
}
