#include <iostream>
using namespace std;
int main()
{
    int turn=1; 
    char box1='1', box2='2', box3='3', box4='4', box5='5', box6='6', box7='7', box8='8', box9='9';
    int dashes, input=0, drawCheck = 1;

    // Displaying the whole grid initially

        cout << "\n| " << box1 << " | " << box2 << " | " << box3 << " |" << endl;
        cout << "-------------" << endl;
        cout << "| " << box4 << " | " << box5 << " | " << box6 << " |" << endl;
        cout << "-------------" << endl;
        cout << "| " << box7 << " | " << box8 << " | " << box9 << " |" << endl;
    do
    {
        cout << "\nPlayer "<< ((turn%2 == 0)? "X" : "O") <<"'s turn. Enter a number(1-9): ";
        cin >> input;  // Taking input from user

        switch(input)
        {
            case 1: if(box1 == 'X' || box1 == 'O')
                    {   cout << "Invalid! Try again!";
                        turn--; 
                        continue;
                    }
                    box1 = (turn%2 == 0)? 'X' : 'O'; break;

            case 2: if(box2 == 'X' || box2 == 'O')
                    {   cout << "Invalid! Try again!";
                        turn--; 
                        continue;
                    }
                    box2 = (turn%2 == 0)? 'X' : 'O'; break;

            case 3: if(box3 == 'X' || box3 == 'O')
                    {   cout << "Invalid! Try again!";
                        turn--; 
                        continue;
                    }
                    box3 = (turn%2 == 0)? 'X' : 'O'; break;

            case 4: if(box4 == 'X' || box4 == 'O')
                    {   cout << "Invalid! Try again!";
                        turn--; 
                        continue;
                    }
                    box4 = (turn%2 == 0)? 'X' : 'O'; break;

            case 5: if(box5 == 'X' || box5 == 'O')
                    {   cout << "Invalid! Try again!";
                        turn--; 
                        continue;
                    }
                    box5 = (turn%2 == 0)? 'X' : 'O'; break;

            case 6: if(box6 == 'X' || box6 == 'O')
                    {   cout << "Invalid! Try again!";
                        turn--; 
                        continue;
                    }
                    box6 = (turn%2 == 0)? 'X' : 'O'; break;
            case 7: if(box7 == 'X' || box7 == 'O')
                    {   cout << "Invalid! Try again!";
                        turn--; 
                        continue;
                    }
                    box7 = (turn%2 == 0)? 'X' : 'O'; break;

            case 8: if(box8 == 'X' || box8 == 'O')
                    {   cout << "Invalid! Try again!";
                        turn--; 
                        continue;
                    }
                    box8 = (turn%2 == 0)? 'X' : 'O'; break;

            case 9: if(box9 == 'X' || box9 == 'O')
                    {   cout << "Invalid! Try again!";
                        turn--; 
                        continue;
                    }
                    box9 = (turn%2 == 0)? 'X' : 'O'; break;

            default: cout << "\nInvalid input, try again!\n"; turn--;
        }

        // Displaying the updated grid after each move

        cout << "\n| " << box1 << " | " << box2 << " | " << box3 << " |" << endl;
        cout << "-------------" << endl;
        cout << "| " << box4 << " | " << box5 << " | " << box6 << " |" << endl;
        cout << "-------------" << endl;
        cout << "| " << box7 << " | " << box8 << " | " << box9 << " |" << endl;

        if(
        (box1 == 'X' && box2=='X' && box3 == 'X') || (box4 == 'X' && box5=='X' && box6 == 'X') || 
        (box7 == 'X' && box8=='X' && box9 == 'X') || (box1 == 'X' && box4=='X' && box7 == 'X') ||
        (box2 == 'X' && box5=='X' && box8 == 'X') || (box3 == 'X' && box6=='X' && box9 == 'X') ||
        (box1 == 'X' && box5=='X' && box9 == 'X') || (box3 == 'X' && box5=='X' && box7 == 'X')
        ) // Checking every possible combination with which player X can win
            {

                cout << "\nPlayer X won!" ;
                drawCheck = 0;
                break;
            }
        else if(
        (box1 == 'O' && box2=='O' && box3 == 'O') || (box4 == 'O' && box5=='O' && box6 == 'O') || 
        (box7 == 'O' && box8=='O' && box9 == 'O') || (box1 == 'O' && box4=='0' && box7 == 'O') ||
        (box2 == 'O' && box5=='O' && box8 == 'O') || (box3 == 'O' && box6=='O' && box9 == 'O') ||
        (box1 == 'O' && box5=='O' && box9 == 'O') || (box3 == 'O' && box5=='O' && box7 == 'O')
        ) // Checking every possible combination with which player O can win
            {
                cout << "\nPlayer O won!" ;
                drawCheck = 0;
                break;
            }
    } while (turn++ < 9);

    if(drawCheck)
        cout << "\nDraw!";
}