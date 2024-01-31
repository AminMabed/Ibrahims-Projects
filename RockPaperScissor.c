#include <stdio.h>
#include <stdlib.h>
int main()
{
    int turn, randInput;
    char userTurn, compTurn;
    int userScore=0, compScore=0;
    printf("\nLet's play Rock Paper Scissors. Best of 5!");
    for(turn = 1; turn <= 5; turn++)
    {
        printf("\nYour turn, enter 'r' for Rock, 'p' for paper or 's' for scissors: ");
        scanf(" %c",&userTurn);

        if(userTurn!='r' && userTurn!='p' && userTurn!='s' && userTurn!='R' && userTurn!='P' && userTurn!='S')
            {
                printf("\nInvalid, try again!");
                turn--;
                continue;
            }

        switch(userTurn)
        {
            case 'R': userTurn = 'r'; break;
            case 'P': userTurn = 'p'; break;
            case 'S': userTurn = 's'; break;
        }
        
        randInput = (rand()%3 + 1);

        switch(randInput)
        {
            case 1: compTurn = 'r'; break;
            case 2: compTurn = 'p'; break;
            case 3: compTurn = 's'; break;
        }
        printf("You entered: %s",(userTurn=='r')?"Rock":(userTurn=='p')?"Paper":"Scissors");
        printf("\tComputer: %s",(compTurn=='r')?"Rock":(compTurn=='p')?"Paper":"Scissors");

        if((userTurn == 'r' && compTurn == 's') || (userTurn == 'p' && compTurn == 'r') || (userTurn == 's' && compTurn == 'p'))
        {
            printf("\nYou won\n");
            userScore++;
        }
        else
        {
            printf("\nComputer won\n");
            compScore++;
        }
        if(userScore >= 3)
        {
            printf("\nGame end. Congrats, You won!");
            break;
        }

        if(compScore >= 3)
        {
            printf("\nGame end. You lost, Computer wins!");
            break;
        }
}
}
