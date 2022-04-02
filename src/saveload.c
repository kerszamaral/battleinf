#include "core.h"

void saving(Setti *settings, char terrainspace[], Obj player[], Obj enemy[] , Obj *energy, Obj bullet[], Rectangle terrainarray[] )
{
    //TODO Make savesfiles work with other resolutions
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
                terrainspace2[(int)(floor((bullet[b].draw.x/(970/40.0))-1) + (floor((bullet[b].draw.y/38))-2)*41)] = '^';
                break;
            case 90:
                terrainspace2[(int)(floor((bullet[b].draw.x/(970/40.0))-1) + (floor((bullet[b].draw.y/38))-2)*41)] = '>';
                break;
            case 180:
                terrainspace2[(int)(floor((bullet[b].draw.x/(970/40.0))-1) + (floor((bullet[b].draw.y/38))-2)*41)] = 'V';
                break;
            case 270:
                terrainspace2[(int)(floor((bullet[b].draw.x/(970/40.0))-1) + (floor((bullet[b].draw.y/38))-2)*41)] = '<';
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

                switch ((int)(terrainarray[i].width/6.25)) //Destruction on width
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
                switch ((int)(terrainarray[i].height/10)) //Destruction on height
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
                switch ((int)floor(terrainarray[i].x - terrainarraycomp[i].x)) //Destruction on x
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
                switch ((int)(terrainarray[i].y - terrainarraycomp[i].y)) //Destruction on y
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
        terrainspace2[(int)(floor((energy->draw.x/(970/40.0))-1) + (floor((energy->draw.y/38))-2)*41)] = 'P';

    for (int e = 0; e < settings->level; e++)
        if ( enemy[e].health > 0 ) //For every enemy that's alive, store it's position
        {
            if (enemy[e].health == 1)
                terrainspace2[(int)(floor((enemy[e].draw.x/(970/40.0))-1) + (floor((enemy[e].draw.y/38))-2)*41)] = 'E';
            else if (enemy[e].health == 2 )
                terrainspace2[(int)(floor((enemy[e].draw.x/(970/40.0))-1) + (floor((enemy[e].draw.y/38))-2)*41)] = '@';
        }

    for (int p = 0; p < settings->players; p++)
    {
        savescore += player[p].score; //Adds the players score to the total score
        if ( player[p].health > 0 )
        {  //If the player is alive, store it's position and lives remaining
            lives += player[p].health*pow(10,p);

            if (player[p].id == 0)
                terrainspace2[(int)(floor((player[p].draw.x/(970/40.0))-1) + (floor((player[p].draw.y/38))-2)*41)] = 'T';
            else
                terrainspace2[(int)(floor((player[p].draw.x/(970/40.0))-1) + (floor((player[p].draw.y/38))-2)*41)] = p+'0';
        }
    }

    //Saves the map, score, and lives to a file named "savegame.txt"
    strcpy( savegame, TextFormat("%s%d\n%d\n%d\n%d", terrainspace2, settings->level, settings->players, lives, savescore ) );
    SaveFileText("saves/savegame.txt", savegame);
}