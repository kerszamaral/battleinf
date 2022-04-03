#include "core.h"

void saving(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Obj bullet[], Rectangle terrainarray[] )
{
    /***************************** VARIABLES *****************************/
    int savescore = 0, lives = 0; //Initialize variables for storing

    char savegame[ 15*41 + 20], terrainspace2[15*41]; //Initialize arrays for map
    
    Rectangle terrainarraycomp[15*41]; //Initialize array for terrain comparassion

    strcpy(terrainspace2, terrainspace); //Copy map onto new array for modifications

    float terrainx = 5 * (GetScreenWidth()*(1.0/1010)), terrainy = 50 * (GetScreenHeight()*(1.0/655));
    //When it finds the * in sets the position and size for the rectangle on that place
    
    //Recreates the original map for comparassion
    for (int i = 0; i < 15 * 41; i++)
    {
        if ( terrainspace[i] == '#' )
            terrainarraycomp[i] = (Rectangle){ terrainx , terrainy , 25 * (GetScreenWidth()*(1.0/1010)) , 40 * (GetScreenHeight()*(1.0/655))};
        
        terrainx += 25 * (GetScreenWidth()*(1.0/1010));
        
        if (terrainspace[i] == '\n')
        {
            terrainx = 5 * (GetScreenWidth()*(1.0/1010));
            terrainy += 40 * (GetScreenHeight()*(1.0/655));
        }
    }

    /***************************** BULLETS *****************************/
    for (int b = 0; b < settings->players + settings->level; b++) //!Code for saving bullets, i think we shouldn't but we may need to
    {
        if ( bullet[b].health > 0 )
        {   //Checks if the bullet is alive
            switch (bullet[b].rot)
            {   //uses the bullets rotation to store it's rotation on the file with different characters
            case 0:
                if (bullet[b].draw.x < GetScreenWidth()/2 )
                {
                    if (bullet[b].draw.y < GetScreenHeight()/2 )
                    {
                        terrainspace2[ (int)( round( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '^';
                    }
                    else
                    {
                        terrainspace2[ (int)( round( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '^';
                    }
                }
                else if (bullet[b].draw.y < GetScreenHeight()/2 )
                {
                    terrainspace2[ (int)( floor( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '^';
                }
                else
                {
                    terrainspace2[ (int)( floor( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '^';
                }
                break;
            case 90:
                if (bullet[b].draw.x < GetScreenWidth()/2 )
                {
                    if (bullet[b].draw.y < GetScreenHeight()/2 )
                    {
                        terrainspace2[ (int)( round( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '>';
                    }
                    else
                    {
                        terrainspace2[ (int)( round( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '>';
                    }
                }
                else if (bullet[b].draw.y < GetScreenHeight()/2 )
                {
                    terrainspace2[ (int)( floor( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '>';
                }
                else
                {
                    terrainspace2[ (int)( floor( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '>';
                }
                break;
            case 180:
                if (bullet[b].draw.x < GetScreenWidth()/2 )
                {
                    if (bullet[b].draw.y < GetScreenHeight()/2 )
                    {
                        terrainspace2[ (int)( round( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'V';
                    }
                    else
                    {
                        terrainspace2[ (int)( round( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'V';
                    }
                }
                else if (bullet[b].draw.y < GetScreenHeight()/2 )
                {
                    terrainspace2[ (int)( floor( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'V';
                }
                else
                {
                    terrainspace2[ (int)( floor( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'V';
                }
                break;
            case 270:
                if (bullet[b].draw.x < GetScreenWidth()/2 )
                {
                    if (bullet[b].draw.y < GetScreenHeight()/2 )
                    {
                        terrainspace2[ (int)( round( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '<';
                    }
                    else
                    {
                        terrainspace2[ (int)( round( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '<';
                    }
                }
                else if (bullet[b].draw.y < GetScreenHeight()/2 )
                {
                    terrainspace2[ (int)( floor( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '<';
                }
                else
                {
                    terrainspace2[ (int)( floor( ( bullet[b].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( bullet[b].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '<';
                }
                break;
            }
        }
    }
    
    /***************************** MAP DESTRUCTION *****************************/
    for (int i = 0; i < 15 * 41; i++)
    {
        if (terrainspace[i] == '#')
        {
            if (terrainarray[i].x == GetScreenWidth())
                terrainspace2[i] = '-'; //Checks for complete destruction on map
            else
            {
                int destruction = 0;//Variabel for knowing how destroyed the terrain is

                switch ( (int)( terrainarray[i].width / ( 6.25 * (GetScreenWidth()*(1.0/1010)) ) ) ) //Destruction on width
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
                switch ( (int)( terrainarray[i].height / ( 10 * (GetScreenHeight()*(1.0/655)) ) ) ) //Destruction on height
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
                switch ( (int)floor( terrainarray[i].x - terrainarraycomp[i].x )) //Destruction on x
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
                switch ( (int)( terrainarray[i].y - terrainarraycomp[i].y )) //Destruction on y
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

                switch (destruction) //Destruction on the terrain
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

    /***************************** OBJECT SAVING *****************************/
    if (energy->health > 0) //If the energy is alive, it stores it's position
    {
        if (energy->draw.x < GetScreenWidth()/2 )
        {
            if (energy->draw.y < GetScreenHeight()/2 )
            {
                terrainspace2[ (int)( round( ( energy->draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( energy->draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'P';
            }
            else
            {
                terrainspace2[ (int)( round( ( energy->draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( energy->draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'P';
            }
        }
        else if (energy->draw.y < GetScreenHeight()/2 )
        {
            terrainspace2[ (int)( floor( ( energy->draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( energy->draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'P';
        }
        else
        {
            terrainspace2[ (int)( floor( ( energy->draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( energy->draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'P';
        }
    }

    for (int e = 0; e < settings->level; e++)
    {
        if ( enemy[e].health > 0 ) //For every enemy that's alive, store it's position
        {
            if (enemy[e].health == 1)
            {
                if (enemy[e].draw.x < GetScreenWidth()/2 )
                {
                    if (enemy[e].draw.y < GetScreenHeight()/2 )
                    {
                        terrainspace2[ (int)( round( ( enemy[e].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( enemy[e].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'E';
                    }
                    else
                    {
                        terrainspace2[ (int)( round( ( enemy[e].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( enemy[e].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'E';
                    }
                }
                else if (enemy[e].draw.y < GetScreenHeight()/2 )
                {
                    terrainspace2[ (int)( floor( ( enemy[e].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( enemy[e].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'E';
                }
                else
                {
                    terrainspace2[ (int)( floor( ( enemy[e].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( enemy[e].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'E';
                }
            }
            else if (enemy[e].health == 2 )
            {
                if (enemy[e].draw.x < GetScreenWidth()/2 )
                {
                    if (enemy[e].draw.y < GetScreenHeight()/2 )
                    {
                        terrainspace2[ (int)( round( ( enemy[e].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( enemy[e].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '@';
                    }
                    else
                    {
                        terrainspace2[ (int)( round( ( enemy[e].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( enemy[e].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '@';
                    }
                }
                else if (enemy[e].draw.y < GetScreenHeight()/2 )
                {
                    terrainspace2[ (int)( floor( ( enemy[e].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( enemy[e].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '@';
                }
                else
                {
                    terrainspace2[ (int)( floor( ( enemy[e].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( enemy[e].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = '@';
                }
            }
        }
    }
    for (int p = 0; p < settings->players; p++)
    {
        savescore += player[p].score; //Adds the players score to the total score
        if ( player[p].health > 0 )
        {  //If the player is alive, store it's position and lives remaining
            lives += player[p].health*pow(10, (settings->players-1)-p);

            if (player[p].id == 0)
            {
                if (player[p].draw.x < GetScreenWidth()/2 )
                {
                    if (player[p].draw.y < GetScreenHeight()/2 )
                    {
                        terrainspace2[ (int)( round( ( player[p].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( player[p].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'T';
                    }
                    else
                    {
                        terrainspace2[ (int)( round( ( player[p].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( player[p].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'T';
                    }
                }
                else if (player[p].draw.y < GetScreenHeight()/2 )
                {
                    terrainspace2[ (int)( floor( ( player[p].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( player[p].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'T';
                }
                else
                {
                    terrainspace2[ (int)( floor( ( player[p].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( player[p].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = 'T';
                }
            }
            else
            {
                if (player[p].draw.x < GetScreenWidth()/2 )
                {
                    if (player[p].draw.y < GetScreenHeight()/2 )
                    {
                        terrainspace2[ (int)( round( ( player[p].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( player[p].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = p+'0';
                    }
                    else
                    {
                        terrainspace2[ (int)( round( ( player[p].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( player[p].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = p+'0';
                    }
                }
                else if (player[p].draw.y < GetScreenHeight()/2 )
                {
                    terrainspace2[ (int)( floor( ( player[p].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( round( ( player[p].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = p+'0';
                }
                else
                {
                    terrainspace2[ (int)( floor( ( player[p].draw.x / ( ( 970 * (GetScreenWidth()*(1.0/1010)) ) / 40.0 ) ) - 1 ) + ( floor( ( player[p].draw.y / (38 * (GetScreenHeight()*(1.0/655))) ) ) - 2 ) * 41 ) ] = p+'0';
                }
            }
        }
    }

    //Saves the map, score, and lives to a file named "savegame.txt"
    strcpy( savegame, TextFormat("%s%d\n%d\n%d", terrainspace2, settings->level, lives, savescore ) );
    SaveFileText("saves/savegame.txt", savegame);
}

void loading(char filename[20], Setti *settings, Obj player[], Obj enemy[] , Obj *energy, Obj bullet[], Rectangle terrainarray[], char terrainspace[] )
{
    char savegame[ 15 * 41 + 20 ], savevar[20], savemap[15*41];
    strcpy( savegame, LoadFileText(TextFormat("saves/%s.txt", filename)) );
    
    /********************STORED VARIABLES************************/
    if ( !strcmp( filename, "savegame") )
    {
        strcpy( savevar, &savegame[615] );
        int counter = 0;

        do
        {
            counter++;
        } while (savevar[counter] != '\n');
        
        for (int i = 1; i < settings->level; i++)
            settings->score += 800*i;
        counter++;
        
        int tempcounter = counter;
        do
        {
            player[counter-tempcounter].health = savevar[counter] - '0';
            counter++;
        } while (savevar[counter] != '\n');
        
        player[0].score = atoi(&savevar[counter]);
    }
    /********************STORED MAP************************/
    strncpy( savemap, savegame, 15*41 );
    float terrainx = 5 * (GetScreenWidth()*(1.0/1010)), terrainy = 50 * (GetScreenHeight()*(1.0/655));
    int e = 0, b = 0;

    for (int i = 0; i < 15 * 41; i++)
    {
        terrainspace[i] = '#';
        if (savemap[i] == '-' || savemap[i] == 'T' || savemap[i] == '1' || savemap[i] == '2' || savemap[i] == '3' || savemap[i] == '4' || savemap[i] == 'E' || savemap[i] == '@' || savemap[i] == 'P' || savemap[i] == '^' || savemap[i] == '>' || savemap[i] == 'V' || savemap[i] == '<')
            terrainspace[i] = '-';
        if ( (i+1) % 41  == 0)
            terrainspace[i] = '\n';
        
        switch (savemap[i])
        {
        //! Terrain
        case '#':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , 25 * (GetScreenWidth()*(1.0/1010)) , 40 * (GetScreenHeight()*(1.0/655))};
            break;
        case 'A':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , 40 * (GetScreenHeight()*(1.0/655))};
            break;
        case 'B':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 12.5) * (GetScreenWidth()*(1.0/1010)) , 40 * (GetScreenHeight()*(1.0/655))};
            break;
        case 'C':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , 40 * (GetScreenHeight()*(1.0/655))};
            break;
        case 'D':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , 25 * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'F':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , 25 * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'G':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , 25 * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'H':
            terrainarray[i] = (Rectangle){ terrainx + (6.25 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , 40 * (GetScreenHeight()*(1.0/655))};
            break;
        case 'I':
            terrainarray[i] = (Rectangle){ terrainx + (12.50 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 12.5) * (GetScreenWidth()*(1.0/1010)) , 40 * (GetScreenHeight()*(1.0/655))};
            break;
        case 'J':
            terrainarray[i] = (Rectangle){ terrainx + (18.75 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , 40 * (GetScreenHeight()*(1.0/655))};
            break;
        case 'K':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (10 * (GetScreenHeight()*(1.0/655))), 25 * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'L':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (20 * (GetScreenHeight()*(1.0/655))), 25 * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'M':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (30 * (GetScreenHeight()*(1.0/655))), 25 * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'N':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'O':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'Q':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'R':
            terrainarray[i] = (Rectangle){ terrainx + (6.25 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'S':
            terrainarray[i] = (Rectangle){ terrainx + (12.50 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'U':
            terrainarray[i] = (Rectangle){ terrainx + (18.75 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'W':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'X':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'Y':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'Z':
            terrainarray[i] = (Rectangle){ terrainx + (6.25 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'a':
            terrainarray[i] = (Rectangle){ terrainx + (12.50 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'b':
            terrainarray[i] = (Rectangle){ terrainx + (18.75 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'c':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'd':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'e':
            terrainarray[i] = (Rectangle){ terrainx , terrainy , (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'f':
            terrainarray[i] = (Rectangle){ terrainx + (6.25 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'g':
            terrainarray[i] = (Rectangle){ terrainx + (12.50 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'h':
            terrainarray[i] = (Rectangle){ terrainx + (18.75 * (GetScreenWidth()*(1.0/1010))), terrainy , (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'i':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (10 * (GetScreenHeight()*(1.0/655))), (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'j':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (10 * (GetScreenHeight()*(1.0/655))), (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'k':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (10 * (GetScreenHeight()*(1.0/655))), (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'l':
            terrainarray[i] = (Rectangle){ terrainx + (6.25 * (GetScreenWidth()*(1.0/1010))), terrainy + (10 * (GetScreenHeight()*(1.0/655))), (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'm':
            terrainarray[i] = (Rectangle){ terrainx + (12.50 * (GetScreenWidth()*(1.0/1010))), terrainy + (10 * (GetScreenHeight()*(1.0/655))), (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'n':
            terrainarray[i] = (Rectangle){ terrainx + (18.75 * (GetScreenWidth()*(1.0/1010))), terrainy + (10 * (GetScreenHeight()*(1.0/655))), (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 10) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'o':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (20 * (GetScreenHeight()*(1.0/655))), (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'p':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (20 * (GetScreenHeight()*(1.0/655))), (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'q':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (20 * (GetScreenHeight()*(1.0/655))), (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'r':
            terrainarray[i] = (Rectangle){ terrainx + (6.25 * (GetScreenWidth()*(1.0/1010))), terrainy + (20 * (GetScreenHeight()*(1.0/655))), (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 's':
            terrainarray[i] = (Rectangle){ terrainx + (12.50 * (GetScreenWidth()*(1.0/1010))), terrainy + (20 * (GetScreenHeight()*(1.0/655))), (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 't':
            terrainarray[i] = (Rectangle){ terrainx + (18.75 * (GetScreenWidth()*(1.0/1010))), terrainy + (20 * (GetScreenHeight()*(1.0/655))), (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 20) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'u':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (30 * (GetScreenHeight()*(1.0/655))), (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'v':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (30 * (GetScreenHeight()*(1.0/655))), (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'w':
            terrainarray[i] = (Rectangle){ terrainx , terrainy + (30 * (GetScreenHeight()*(1.0/655))), (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'x':
            terrainarray[i] = (Rectangle){ terrainx + (6.25 * (GetScreenWidth()*(1.0/1010))), terrainy + (30 * (GetScreenHeight()*(1.0/655))), (25 - 6.25) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'y':
            terrainarray[i] = (Rectangle){ terrainx + (12.50 * (GetScreenWidth()*(1.0/1010))), terrainy + (30 * (GetScreenHeight()*(1.0/655))), (25 - 12.50) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;
        case 'z':
            terrainarray[i] = (Rectangle){ terrainx + (18.75 * (GetScreenWidth()*(1.0/1010))), terrainy + (30 * (GetScreenHeight()*(1.0/655))), (25 - 18.75) * (GetScreenWidth()*(1.0/1010)) , (40 - 30) * (GetScreenHeight()*(1.0/655))};
            break;

        //! PowerUps
        case 'P':
            energy->pos = (Vector2){ terrainx - energy->cen.x * (GetScreenWidth()*(1.0/1010)), terrainy - energy->cen.y * (GetScreenHeight()*(1.0/655)) };
            energy->health = 1;
            break;

        //! Players
        case 'T':
            player[0].pos = (Vector2){ terrainx - player[0].cen.x * (GetScreenWidth()*(1.0/1010)), terrainy - player[0].cen.y * (GetScreenHeight()*(1.0/655)) };
            settings->foundplayerposition++;
            break;
        case '1':
            player[1].pos = (Vector2){ terrainx - player[1].cen.x, terrainy - player[1].cen.y };
            settings->foundplayerposition++;
            break;
        case '2':
            player[2].pos = (Vector2){ terrainx - player[2].cen.x, terrainy - player[2].cen.y };
            settings->foundplayerposition++;
            break;
        case '3':
            player[3].pos = (Vector2){ terrainx - player[3].cen.x, terrainy - player[3].cen.y };
            settings->foundplayerposition++;
            break;
        case '4':
            player[4].pos = (Vector2){ terrainx - player[4].cen.x, terrainy - player[4].cen.y };
            settings->foundplayerposition++;
            break;
        //! Enemies
        case 'E':
            enemy[e].pos = (Vector2){ terrainx - enemy[e].cen.x, terrainy - enemy[e].cen.y };
            enemy[e].health = 1;
            e++;
            break;
        case '@':
            enemy[e].pos = (Vector2){ terrainx - enemy[e].cen.x, terrainy - enemy[e].cen.y };
            enemy[e].health = 2;
            enemy[e].color = RED;
            e++;
            break;
        }

        terrainx += 25 * (GetScreenWidth()*(1.0/1010));
        if (savemap[i] == '\n')
        {
            terrainx = 5 * (GetScreenWidth()*(1.0/1010));
            terrainy += 40 * (GetScreenHeight()*(1.0/655));
        }
    }
    settings->enemiesremaining = e + (player[0].score/800);
    printf("%d\n", settings->enemiesremaining);
    printf("\n%s\n",terrainspace);
}