#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
int snake_ladder(int );
int main()  
{
    int p1=1, p2=1;
    int diceRoll;
    int turn=1;
    char sl;

    do
    {
        system("cls");
        int box = 100;
        for(int r = 10; r >= 1; r--)
        {
            sl = ' ';
            // Current Game Grid
            printf("\n---------------------------------------------------------------------------------");
            printf("\n|");
            // Snakes:- 99 to 10, 81 to 35, 65 to 40, 25 to 9, 47 to 26, 89 to 54      Ladder:- 70 to 90, 85 to 93, 60 to 83, 31 to 53, 13 to 42.
            if(r % 2 == 0)
                for(box = r*10; box >= (r-1)*10 +1; box--)
                {
                    if(box == 99 || box == 89 || box == 81 || box == 65 || box == 47 || box == 25)
                        sl = 'S';
                    else if(box == 85 || box == 70 || box == 60 || box == 31 || box == 13)
                        sl = 'L';
                    else   
                        sl = ' ';
                    printf("%d\033[1;31m%c\033[1;32m%s%s\033[1;0m\t|", box , sl , (box==p1)?"P1":"" , (box==p2)?"P2":"\b\b");
                }
            else
            {
                for(box = (r-1)*10 +1; box <= r*10; box++)
                {
                    if(box == 99 || box == 89 || box == 81 || box == 65 || box == 47 || box == 25)
                        sl = 'S';
                    else if(box == 85 || box == 70 || box == 60 || box == 31 || box == 13)
                        sl = 'L';
                    else 
                        sl = ' ';
                    printf("%d\033[1;31m%c\033[1;32m%s%s\033[1;0m\t|", box , sl , (box==p1)?"P1":"" , (box==p2)?"P2":"\b\b");
                }
            }
        }
        
        // Checking for end of game
        if(p1 == 100)
        {
            printf("\nGAME END, PLAYER 1 WINS!");
            break;
        }
        else if(p2 == 100)
        {
            printf("\nGAME END, PLAYER 2 WINS!");
            break;
        }
        
        printf("\n\nPlayer %d's turn, press any key to roll the dice: ",(turn%2==1)? 1 : 2);
        getch();

        srand(time(NULL)); // Seed the random number generator

        // Generate a random number between 1 and 6 
        diceRoll = rand() % 6 + 1;
        printf("\nYou got: %d", diceRoll);

        //Adding random value to p1 / p2
        if(turn % 2)
            p1 += diceRoll;
        else
            p2 += diceRoll;

        // Canceling out if p1 / p2 goes above 100
        if(p1 > 100)
            p1 -= diceRoll;
        if(p2 > 100)
            p2 -= diceRoll;

        // Adding snake or ladder effect
        p1 = snake_ladder(p1);
        p2 = snake_ladder(p2);

       
    turn++;
    }
    while(1);
}

int snake_ladder(int p) // Snakes:- 99 to 10, 81 to 35, 65 to 40, 25 to 9, 47 to 26, 89 to 54      Ladder:- 70 to 90, 85 to 93, 60 to 83, 31 to 53, 13 to 42.
{
    switch(p)
    {
        //Snakes
        case 99: p=10; printf("\nOh no, a snake!"); break;
        case 65: p=40; printf("\nOh no, a snake!"); break;
        case 25: p=9;  printf("\nOh no, a snake!"); break;
        case 81: p=35; printf("\nOh no, a snake!"); break;
        case 47: p=26; printf("\nOh no, a snake!"); break;
        case 89: p=54; printf("\nOh no, a snake!"); break;
        //Ladders
        case 70: p=90; printf("\nA ladder !!!"); break;
        case 60: p=83; printf("\nA ladder !!!"); break;
        case 13: p=42; printf("\nA ladder !!!"); break;
        case 31: p=53; printf("\nA ladder !!!"); break;
        case 85: p=93; printf("\nA ladder !!!"); break;
        default: break;
    }
    return p;
}
