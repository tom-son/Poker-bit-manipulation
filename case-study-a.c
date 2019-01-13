#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>

/* handy typedefs */
typedef unsigned char card;
typedef unsigned char pairs;

/* arrays for the names of things */
static char *suits[] = {"Hearts","Diamonds","Clubs","Spades"};
static char *values[]= {"Ace","Two","Three","Four","Five","Six",\
                        "Seven","Eight","Nine","Ten","Jack",\
                        "Queen","King"};
static char *colour[]= {"Black","Red"};

/* function prototypes */
void printcard(card c); /* Displays the value of a card*/

void printdeck(card deck[52]); /* prints an entire deck of cards*/

void filldeck(card deck[52]); /* Populates a deck of cards */

void shuffle(card deck[52]); /* Randomizes the order of cards */

int compareface(const void* c1,const void *c2); 
/* compares the face value of 2 cards, suitable to pass to qsort
   as the fourth argument */

void printBinary(int number, int num_digits);
/* Prints the number into binary form */

void printHands(card hands[5][5], int player);
/* Prints the hands in 5/5 matrix */

pairs findpairs(card *hand); /* finds any pairs in a hand */

void findWinner(int winner, int highest);


int main()
{
    card deck[52],*deckp;
    card hands[5][5],handssorted[5][5];
    pairs numpairs[5],highest;
    int hand,cd,winner = -1,index = 0;
    
    srand(time(NULL));       /* seed the random number generator */

    /*populate and shuffle the deck */
    filldeck(deck);
    printdeck(deck);
    shuffle(deck);
    printdeck(deck);
  
    for(cd=0;cd<5;cd++)
    {
        for(hand=0;hand<5;hand++)
        {
            /* deal the hands here */
            hands[hand][cd] = deck[index];
            handssorted[hand][cd] = deck[index];
            index++;
        }
    }

    for(hand=0;hand<5;hand++)
    {
        /* sort the hands here */
        qsort(handssorted[hand], 5, sizeof(card), compareface);

        numpairs[hand]=findpairs(handssorted[hand]);

        printf("Hand %i:",hand+1);
        /* print the hands here */
        printHands(hands, hand);
        /* print the number and value of any pairs here */
        char pairCounter = numpairs[hand] & 0x0f;
        char highestPair = numpairs[hand] & 0xf0;
        /* shift bits to lsb and remove all bits moved from the right shift */
        highestPair = highestPair >> 4;
        highestPair = highestPair & 0x0f;
        /* find the highest pair in the hand and determine if it is a winning pair */
        if(pairCounter > 0)
        {
            if(highestPair > highest)
            {
                winner = hand;
                highest = highestPair;
            }
            // else if (highestPair == highest)
            // {
            //     /* winner 10 when two players have the same highest pair */
            //     winner = 10;
            //     // return;
            // }
        }
        printf(" Number of pairs: %u\n", pairCounter);
        if(pairCounter > 0)
        {
            printf(" Highest pair is: %s\n\n", values[highestPair]);
        }
    }

    findWinner(winner, highest);
        
    return 0;
}


void findWinner(winner, highest)
{
    /* determine the winner and print it */
    if(winner == -1)
    {
        printf("\n--> Drawn game <--\n\n");
    }
    else if(winner == 10)
    {
        printf("\n--> Drawn game <--\n\n");
    } 
    else 
    {
        /* winner + 1 to account for index offset */
        printf("\n--> Winner is hand %d with pair of %s <--\n\n", winner+1, 
        values[highest]);
    }
    return;
}

void printBinary(int number, int num_digits) 
{
    int digit;
    /* go through all the digits and return 1 or 0 if number 
    has a number or not */
    for(digit = num_digits - 1; digit >= 0; digit--) 
    {
        printf("%c", number & (1 << digit) ? '1' : '0');
    }
    printf("\n");
}

void printHands(card hands[5][5], int player) 
{
    printf("\n");
    /* Go through all the cards in a players hand and print card */
    for(int j = 0; j < 5; j++) 
    {
        printcard(hands[player][j]);
    }
    printf("\n");
}
pairs findpairs(card *hand)
{
    pairs numpairs=0;  
    char value1, value2, highestPair;
    /* find the pairs here */  
    /* print the handsstored*/ 
    // for(int j = 0; j < 5; j++) 
    // {
    //   printcard(hand[j]);
    // }

    /* compare */
    // printf("\nComparing: \n");
    for(int j = 0; j < 4; j++) 
    {
        value1 = hand[j] & 0x78;
        value2 = hand[j + 1] & 0x78;
        /* to bring the value bits to lsb 0 */
        value1 = value1 >> 3;
        value2 = value2 >> 3;
        if(value1 == value2) 
        {
            highestPair = value1 << 4;
            // printf("\nMatch!\n");
            // printcard(hand[j]);
            // printcard(hand[j+1]);
            numpairs = numpairs + 1;
            j++;
            // printf("\nThe highest pair is:");
            // print_binary(highestPair, 8);
            // printf("\n Value1 = ");
            // print_binary(value1,8);
        }
    }
    
    numpairs = numpairs & 0x0f;
    numpairs = numpairs | highestPair;

    printf("\n");
    return numpairs;
}

void filldeck(card deck[52])
{
    /* populate the deck here */
    int suitBits = 0; /* 00 Heart, 01 Diamond, 10 Club, 11 Spade */
    int colorBit = 1;
    int valueBits = 0;
    int arrayIndex = 0;
    /* concating value, color and suit to makeCard */
    for ( ; suitBits <= 3; suitBits++)
    {
        for ( ; valueBits <= 12; valueBits++)
        {
            /* If suit is clubs or spades turn colorBit to black */
            if(suitBits > 1) 
            {
              colorBit = 0;
            }
            char suit = suitBits & 0x03;
            char color = colorBit & 0x01;
            char value = valueBits & 0x0f;
            /* VALUE + COLOR */
            card makeCard = value << 1;
            
            makeCard = makeCard & 0x1e;
            makeCard = makeCard | color;
            /* SUIT */
            makeCard = makeCard << 2; 
            makeCard = makeCard & 0x7f;
            makeCard = makeCard | suit;

            deck[arrayIndex] = makeCard;
            /* reset card and increase deckArrayIndex placeholder */
            makeCard = 0;
            arrayIndex++;
        }
        /* reset value bits to ace from king */
        valueBits = 0;
    }
    return;
}

void printdeck(card deck[52])
{
    int i;
    printf("The deck is..\n");
    for(i=0;i<52;i++)
    {
        printcard(deck[i]);
    }
    printf("\n");
    return;
}

void printcard(card c)
{
    /* print the value of the card here */
    int value, color, suit;
    /* suit set to the first 2 lsb */
    suit = c & 0x03;
    /* shift out suit bit */
    c = c >> 2;
    /* color set to the first lsb */
    color = c & 0x01;
    /* shift out color bit */
    c = c >> 1;
    /* value set to the 4 lsb */
    value = c & 0x0f;

    printf("%s of %s, is %s\n",values[value], suits[suit], colour[color]);
    return;
}

void shuffle(card deck[52])
{
    int i,rnd;
    card c;
    printf("\n\n");
    for(i=0;i<52;i++)
    {
        /* generate a random number between 0 & 51 */
        rnd=rand() * 52.0 / RAND_MAX; 
        c = deck[i];
        deck[i] = deck[rnd];
        deck[rnd] = c;

        /* finish shuffling the deck here */
    }

    return;
}


int compareface(const void* c1, const void *c2)
{
    /* 
      This function extracts the two cards face values
      and returns 1 if cd1 > cd2, 0 if cd1 == cd2, and
      -1 otherwise. The weird argument types are for
      compatibility with qsort(), the first two lines
      decode the arguments back into "card".
    */
    card cd1,cd2;
    
    cd1=*((card*) c1);
    cd2=*((card*) c2);
    
    cd1= (cd1&0x78)>>3;
    cd2= (cd2&0x78)>>3;
    
    if(cd1>cd2)
    {
        return 1;
    }
    if(cd1==cd2)
    {  
        return 0;
    } 
    return -1;
}

