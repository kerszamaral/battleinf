#include "core.h"

//Function for saving the game
void saving(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Rectangle terrainarray[] )
{
    //? ***************************** VARIABLES *****************************/
    int savescore = 0, lives = 0; //Initialize variables for storing score and lives
    char savegame[ MAPSIZE + 20], terrainspace2[MAPSIZE]; //Initialize arrays for map
    Rectangle terrainarraycomp[MAPSIZE]; //Initialize array for terrain comparassion
    strcpy(terrainspace2, terrainspace); //Copy map onto new array for modifications
    
    float terrainx = 5*scaleX(), terrainy = 50*scaleY(); //Sets the starting x and y position of the terrain
    for (int i = 0; i < MAPSIZE; i++)
    {
        if ( terrainspace[i] == '#' ) //If the char is a wall add a rectangle
            terrainarraycomp[i] = (Rectangle){ terrainx, terrainy, 25*scaleX(), 40*scaleY() };
        
        terrainx += 25*scaleX(); //Increments the x position of the terrain
        
        if (terrainspace[i] == '\n') //If the char is a new line
        {
            terrainx = 5*scaleX(); //Resets the x position of the terrain
            terrainy += 40*scaleY(); //Increments the y position of the terrain
        }
    }
    
    //? ***************************** MAP DESTRUCTION *****************************/
    for (int i = 0; i < MAPSIZE; i++) //Loop for every space in the map
    {
        if (terrainspace[i] == '#') //If the char is a wall
        {
            if (terrainarray[i].x == GetScreenWidth()) //Checks for complete destruction on map
                terrainspace2[i] = '-'; //If complete destruction, change char to a '-'
            else
            {
                //!ENCODING
                int destruction = 0; //Variabel for knowing how destroyed the terrain is
                //? How it's encoded: Destruction on Y, Destruction on X, Destruction Height, Destruction Width
                switch ( (int)( terrainarray[i].width / ( 6.25 * scaleX() ) ) ) //Destruction level on width
                {
                case 3:
                    destruction += 1;
                    break;
                case 2:
                    destruction += 2;
                    break;
                case 1:
                    destruction += 3;
                    break;
                default:
                    break;
                }
                switch ( (int)( terrainarray[i].height / ( 10 * scaleY() ) ) ) //Destruction level on height
                {
                case 3:
                    destruction += 10;
                    break;
                case 2:
                    destruction += 20;
                    break;
                case 1:
                    destruction += 30;
                    break;
                default:
                    break;
                }
                switch ( (int)floor( terrainarray[i].x - terrainarraycomp[i].x )) //Destruction level on X
                {
                case 6:
                    destruction += 100;
                    break;
                case 12:
                    destruction += 200;
                    break;
                case 18:
                    destruction += 300;
                    break;
                default:
                    break;
                }
                switch ( (int)( terrainarray[i].y - terrainarraycomp[i].y )) //Destruction level on Y
                {
                case 10:
                    destruction += 1000;
                    break;
                case 20:
                    destruction += 2000;
                    break;
                case 30:
                    destruction += 3000;
                    break;
                default:
                    break;
                }
                //!SAVING ON ARRAY
                switch (destruction) //Encoded destruction on the terrain, each case is a different level of destruction correspoding to a different letter
                {
                case 1:
                    terrainspace2[i] = 'A';
                    break;
                case 2:
                    terrainspace2[i] = 'B';
                    break;
                case 3:
                    terrainspace2[i] = 'C';
                    break;
                case 10:
                    terrainspace2[i] = 'D';
                    break;
                case 20:
                    terrainspace2[i] = 'F';
                    break;
                case 30:
                    terrainspace2[i] = 'G';
                    break;
                case 101:
                    terrainspace2[i] = 'H';
                    break;
                case 202:
                    terrainspace2[i] = 'I';
                    break;
                case 303:
                    terrainspace2[i] = 'J';
                    break;
                case 1010:
                    terrainspace2[i] = 'K';
                    break;
                case 2020:
                    terrainspace2[i] = 'L';
                    break;
                case 3030:
                    terrainspace2[i] = 'M';
                    break;
                case 11:
                    terrainspace2[i] = 'N';
                    break;
                case 12:
                    terrainspace2[i] = 'O';
                    break;
                case 13:
                    terrainspace2[i] = 'Q';
                    break;
                case 111:
                    terrainspace2[i] = 'R';
                    break;
                case 212:
                    terrainspace2[i] = 'S';
                    break;
                case 313:
                    terrainspace2[i] = 'U';
                    break;
                case 21:
                    terrainspace2[i] = 'W';
                    break;
                case 22:
                    terrainspace2[i] = 'X';
                    break;
                case 23:
                    terrainspace2[i] = 'Y';
                    break;
                case 121:
                    terrainspace2[i] = 'Z';
                    break;
                case 222:
                    terrainspace2[i] = 'a';
                    break;
                case 323:
                    terrainspace2[i] = 'b';
                    break;
                case 31:
                    terrainspace2[i] = 'c';
                    break;
                case 32:
                    terrainspace2[i] = 'd';
                    break;
                case 33:
                    terrainspace2[i] = 'e';
                    break;
                case 131:
                    terrainspace2[i] = 'f';
                    break;
                case 232:
                    terrainspace2[i] = 'g';
                    break;
                case 333:
                    terrainspace2[i] = 'h';
                    break;
                case 1011:
                    terrainspace2[i] = 'i';
                    break;
                case 1012:
                    terrainspace2[i] = 'j';
                    break;
                case 1013:
                    terrainspace2[i] = 'k';
                    break;
                case 1111:
                    terrainspace2[i] = 'l';
                    break;
                case 1212:
                    terrainspace2[i] = 'm';
                    break;
                case 1313:
                    terrainspace2[i] = 'n';
                    break;
                case 2021:
                    terrainspace2[i] = 'o';
                    break;
                case 2022:
                    terrainspace2[i] = 'p';
                    break;
                case 2023:
                    terrainspace2[i] = 'q';
                    break;
                case 2121:
                    terrainspace2[i] = 'r';
                    break;
                case 2222:
                    terrainspace2[i] = 's';
                    break;
                case 2323:
                    terrainspace2[i] = 't';
                    break;
                case 3031:
                    terrainspace2[i] = 'u';
                    break;
                case 3032:
                    terrainspace2[i] = 'v';
                    break;
                case 3033:
                    terrainspace2[i] = 'w';
                    break;
                case 3131:
                    terrainspace2[i] = 'x';
                    break;
                case 3232:
                    terrainspace2[i] = 'y';
                    break;
                case 3333:
                    terrainspace2[i] = 'z';
                    break;
                default:
                    break;
                }
            }    
        }    
    }

    //? ***************************** OBJECT SAVING *****************************/
    //!For saving on different screensizes, the rounding gives an error based on the position of the object when translating to the text array, 
    //!if it's over half screen, in width or height, we need to use different methods, based on round or floor for saving the object
    //*Energy / encoded as "P"
    if (energy->health > 0) //If the energy is alive, it stores it's position
    {   
        if (energy->draw.x < GetScreenWidth()/2 )
        {
            if (energy->draw.y < GetScreenHeight()/2 )
                terrainspace2[ (int)( round( ( energy->draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( energy->draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'P';
            else
                terrainspace2[ (int)( round( ( energy->draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( energy->draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'P';
        }
        else if (energy->draw.y < GetScreenHeight()/2 )
            terrainspace2[ (int)( floor( ( energy->draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( energy->draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'P';
        else
            terrainspace2[ (int)( floor( ( energy->draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( energy->draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'P';
    }
    //*Enemies / encoded as "E" or "@"
    for (int e = 0; e < settings->level; e++) //Loop for every enemy
    {
        if ( enemy[e].health > 0 ) //For every enemy that's alive, store it's position
        {
            if (enemy[e].health == 1) //Store the enemy in different encoding depending on it's health, for different difficulties
            {
                if (enemy[e].draw.x < GetScreenWidth()/2 )
                {
                    if (enemy[e].draw.y < GetScreenHeight()/2 )
                        terrainspace2[ (int)( round( ( enemy[e].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( enemy[e].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'E';
                    else
                        terrainspace2[ (int)( round( ( enemy[e].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( enemy[e].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'E';
                }
                else if (enemy[e].draw.y < GetScreenHeight()/2 )
                    terrainspace2[ (int)( floor( ( enemy[e].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( enemy[e].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'E';
                else
                    terrainspace2[ (int)( floor( ( enemy[e].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( enemy[e].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'E';
            }
            else if (enemy[e].health == 2 )
            {
                if (enemy[e].draw.x < GetScreenWidth()/2 )
                {
                    if (enemy[e].draw.y < GetScreenHeight()/2 )
                        terrainspace2[ (int)( round( ( enemy[e].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( enemy[e].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = '@';
                    else
                        terrainspace2[ (int)( round( ( enemy[e].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( enemy[e].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = '@';
                }
                else if (enemy[e].draw.y < GetScreenHeight()/2 )
                    terrainspace2[ (int)( floor( ( enemy[e].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( enemy[e].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = '@';
                else
                    terrainspace2[ (int)( floor( ( enemy[e].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( enemy[e].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = '@';
            }
        }
    }
    //*Player / encoded as "T" or if player > 1 = "PlayerID"
    for (int p = 0; p < settings->players; p++)
    {
        savescore += player[p].score; //Adds the players score to the total score
        if ( player[p].health > 0 ) //If the player is alive, store it's position and lives remaining
        {
            lives += player[p].health*pow(10, (settings->players-1)-p); //Adds the players lives to the total lives, if the player is dead, it doesn't count

            if (player[p].id == 0) //If player is 1, encode as T
            {
                if (player[p].draw.x < GetScreenWidth()/2 )
                {
                    if (player[p].draw.y < GetScreenHeight()/2 )
                        terrainspace2[ (int)( round( ( player[p].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( player[p].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'T';
                    else
                        terrainspace2[ (int)( round( ( player[p].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( player[p].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'T';
                }
                else if (player[p].draw.y < GetScreenHeight()/2 )
                    terrainspace2[ (int)( floor( ( player[p].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( player[p].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'T';
                else
                    terrainspace2[ (int)( floor( ( player[p].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( player[p].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = 'T';
            }
            else //If player is any other number, encode as PlayerID number + '0' to turn into char
            {
                if (player[p].draw.x < GetScreenWidth()/2 )
                {
                    if (player[p].draw.y < GetScreenHeight()/2 )
                        terrainspace2[ (int)( round( ( player[p].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( player[p].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = p+'0';
                    else
                        terrainspace2[ (int)( round( ( player[p].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( player[p].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = p+'0';
                }
                else if (player[p].draw.y < GetScreenHeight()/2 )
                {
                    terrainspace2[ (int)( floor( ( player[p].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( round( ( player[p].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = p+'0';
                }
                else
                {
                    terrainspace2[ (int)( floor( ( player[p].draw.x / ( ( 970 * scaleX() ) / 40.0 ) ) - 1 ) + ( floor( ( player[p].draw.y / (38 * scaleY()) ) ) - 2 ) * 41 ) ] = p+'0';
                }
            }
        }
    }

    //Saves the map, score, and lives to a file named "savegame.txt"
    strcpy( savegame, TextFormat("%s\n%d\n%d\n%d", terrainspace2, settings->level, lives, savescore ) );
    SaveFileText("saves/savegame.txt", savegame);
}

void loading(char filename[100], Setti *settings, Obj player[], Obj enemy[] , Obj *energy, Rectangle terrainarray[], char terrainspace[], bool print )
{
    //? ***************************** VARIABLES *****************************/
    char savegame[ MAPSIZE + 20 ] = {0}, savevar[20] = {0}, savemap[MAPSIZE] = {0};
    //*Copies the savegame.txt file into the savegame variable
    strcpy( savegame, LoadFileText(TextFormat("%s.txt", filename)) );
    
    //? ********************STORED VARIABLES************************/
    if ( !strcmp( filename, "saves/savegame") ) //The only time we change the game variables is if it's a savegame
    {
        strcpy( savevar, &savegame[615] ); //Copies the savegame variable into savevar starting at the position 615
        int counter = 0; //Counter for the savegame variable spacing

        do //Skips the level variable because it's already been read on main
        {
            counter++;
        } while (savevar[counter] != '\n'); //When it finds the next line, it stops
        
        for (int i = 1; i < settings->level; i++) //Restores the minimum score 
            settings->score += 800*i; //For you to complete a level you get 800 points for each enemy, so we can add the score for each level to get the minimum score
        counter++;
        
        int tempcounter = counter; //Temporary counter for the health variable spacing, start at player 0
        do
        {
            player[counter-tempcounter].health = savevar[counter] - '0'; //Subtracts the '0' to turn the char into an int and store it into the right player
            counter++;
        } while (savevar[counter] != '\n'); //When it finds the next line, it stops
        
        player[0].score = atoi(&savevar[counter]); //The rest of the files is the score for the current level, so we store it in the player as the normal game would
    }
    //? ********************STORED MAP************************/
    strncpy( savemap, savegame, MAPSIZE ); //Copies the savegame variable into savemap but discards the level, score and lives variables 
    int e = 0; //Counter for the enemies already spawned

    float terrainx = 5*scaleX(), terrainy = 50*scaleY(); //Sets the starting x and y position of the terrain
    for (int i = 0; i < MAPSIZE; i++)
    {
        terrainspace[i] = '#'; //Sets the char terrain to be all walls
        if (savemap[i] == '-' || savemap[i] == 'T' || savemap[i] == '1' || savemap[i] == '2' || savemap[i] == '3' || savemap[i] == '4' || savemap[i] == 'E' || savemap[i] == '@' || savemap[i] == 'P' )
            terrainspace[i] = '-'; //If the char in the txt file is anything that's not a wall, overwrite it as nothing(-)
        if ( (i+1) % 41  == 0) //If the terrain is at the end of the line
            terrainspace[i] = '\n'; //Sets the char to be a new line
        if ( i == 614 ) //If the terrain is at the end of the map
            terrainspace[i] = '\0'; //Sets the char to be a null terminator
        
        switch (savemap[i]) //! Decodes the chairs in the savemap for their respective object or rectangle size and position
        {
        //*Terrain
        case '#':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, 25*scaleX(), 40*scaleY() };
            break;
        case 'A':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 6.25)*scaleX(), 40*scaleY() };
            break;
        case 'B':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 12.5) * scaleX(), 40*scaleY()};
            break;
        case 'C':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 18.75)*scaleX(), 40*scaleY() };
            break;
        case 'D':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, 25 * scaleX() , (40 - 10)*scaleY() };
            break;
        case 'F':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, 25 * scaleX() , (40 - 20)*scaleY() };
            break;
        case 'G':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, 25 * scaleX() , (40 - 30)*scaleY() };
            break;
        case 'H':
            terrainarray[i] = (Rectangle){ terrainx + (6.25*scaleX()), terrainy, (25 - 6.25)*scaleX(), 40*scaleY() };
            break;
        case 'I':
            terrainarray[i] = (Rectangle){ terrainx + (12.50*scaleX()), terrainy, (25 - 12.50)*scaleX(), 40*scaleY() };
            break;
        case 'J':
            terrainarray[i] = (Rectangle){ terrainx + (18.75*scaleX()), terrainy, (25 - 18.75)*scaleX(), 40*scaleY() };
            break;
        case 'K':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (10*scaleY()), 25 * scaleX() , (40 - 10)*scaleY() };
            break;
        case 'L':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (20*scaleY()), 25 * scaleX() , (40 - 20)*scaleY() };
            break;
        case 'M':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (30*scaleY()), 25 * scaleX() , (40 - 30)*scaleY() };
            break;
        case 'N':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 6.25)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'O':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 12.50)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'Q':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 18.75)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'R':
            terrainarray[i] = (Rectangle){ terrainx + (6.25*scaleX()), terrainy, (25 - 6.25)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'S':
            terrainarray[i] = (Rectangle){ terrainx + (12.50*scaleX()), terrainy, (25 - 12.50)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'U':
            terrainarray[i] = (Rectangle){ terrainx + (18.75*scaleX()), terrainy, (25 - 18.75)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'W':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 6.25)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'X':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 12.50)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'Y':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 18.75)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'Z':
            terrainarray[i] = (Rectangle){ terrainx + (6.25*scaleX()), terrainy, (25 - 6.25)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'a':
            terrainarray[i] = (Rectangle){ terrainx + (12.50*scaleX()), terrainy, (25 - 12.50)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'b':
            terrainarray[i] = (Rectangle){ terrainx + (18.75*scaleX()), terrainy, (25 - 18.75)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'c':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 6.25)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'd':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 12.50)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'e':
            terrainarray[i] = (Rectangle){ terrainx, terrainy, (25 - 18.75)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'f':
            terrainarray[i] = (Rectangle){ terrainx + (6.25*scaleX()), terrainy, (25 - 6.25)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'g':
            terrainarray[i] = (Rectangle){ terrainx + (12.50*scaleX()), terrainy, (25 - 12.50)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'h':
            terrainarray[i] = (Rectangle){ terrainx + (18.75*scaleX()), terrainy, (25 - 18.75)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'i':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (10*scaleY()), (25 - 6.25)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'j':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (10*scaleY()), (25 - 12.50)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'k':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (10*scaleY()), (25 - 18.75)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'l':
            terrainarray[i] = (Rectangle){ terrainx + (6.25*scaleX()), terrainy + (10*scaleY()), (25 - 6.25)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'm':
            terrainarray[i] = (Rectangle){ terrainx + (12.50*scaleX()), terrainy + (10*scaleY()), (25 - 12.50)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'n':
            terrainarray[i] = (Rectangle){ terrainx + (18.75*scaleX()), terrainy + (10*scaleY()), (25 - 18.75)*scaleX(), (40 - 10)*scaleY() };
            break;
        case 'o':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (20*scaleY()), (25 - 6.25)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'p':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (20*scaleY()), (25 - 12.50)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'q':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (20*scaleY()), (25 - 18.75)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'r':
            terrainarray[i] = (Rectangle){ terrainx + (6.25*scaleX()), terrainy + (20*scaleY()), (25 - 6.25)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 's':
            terrainarray[i] = (Rectangle){ terrainx + (12.50*scaleX()), terrainy + (20*scaleY()), (25 - 12.50)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 't':
            terrainarray[i] = (Rectangle){ terrainx + (18.75*scaleX()), terrainy + (20*scaleY()), (25 - 18.75)*scaleX(), (40 - 20)*scaleY() };
            break;
        case 'u':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (30*scaleY()), (25 - 6.25)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'v':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (30*scaleY()), (25 - 12.50)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'w':
            terrainarray[i] = (Rectangle){ terrainx, terrainy + (30*scaleY()), (25 - 18.75)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'x':
            terrainarray[i] = (Rectangle){ terrainx + (6.25*scaleX()), terrainy + (30*scaleY()), (25 - 6.25)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'y':
            terrainarray[i] = (Rectangle){ terrainx + (12.50*scaleX()), terrainy + (30*scaleY()), (25 - 12.50)*scaleX(), (40 - 30)*scaleY() };
            break;
        case 'z':
            terrainarray[i] = (Rectangle){ terrainx + (18.75*scaleX()), terrainy + (30*scaleY()), (25 - 18.75)*scaleX(), (40 - 30)*scaleY() };
            break;

        //*Energy
        case 'P':
            energy->pos = (Vector2){ terrainx, terrainy };
            energy->health = 1;
            break;

        //*Players
        case 'T':
            player[0].pos = (Vector2){ terrainx, terrainy };
            settings->foundplayerposition++;
            break;
        case '1':
            player[1].pos = (Vector2){ terrainx, terrainy };
            settings->foundplayerposition++;
            break;
        case '2':
            player[2].pos = (Vector2){ terrainx, terrainy };
            settings->foundplayerposition++;
            break;
        case '3':
            player[3].pos = (Vector2){ terrainx, terrainy };
            settings->foundplayerposition++;
            break;
        case '4':
            player[4].pos = (Vector2){ terrainx, terrainy };
            settings->foundplayerposition++;
            break;
            
        //*Enemies
        case 'E':
            enemy[e].pos = (Vector2){ terrainx, terrainy };
            enemy[e].health = 1;
            enemy[e].color = WHITE;
            e++;
            break;
        case '@':
            enemy[e].pos = (Vector2){ terrainx, terrainy };
            enemy[e].health = 2;
            enemy[e].color = RED;
            e++;
            break;
        }

        terrainx += 25*scaleX(); //Increments the x position of the terrain
        
        if (savemap[i] == '\n') //If the char is a new line
        {
            terrainx = 5*scaleX(); //Resets the x position of the terrain
            terrainy += 40*scaleY(); //Increments the y position of the terrain
        }
    }

    settings->enemiesremaining = e + (player[0].score/800); //Adds the number of enemies already spawned and the enemies killed by the player to the enemies remaining
    if (print) //If the map should be printed
        printf("\n%s\n\n",terrainspace); //Prints the map
}