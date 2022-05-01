/* Ian Kersz - Cartão UFRGS: 00338368
18.03.22
BattleInf Jogo de tanquezinho lá do NES
*/
/**** Libraries used on main ****/
#include "core.h"
#include "menu.h"
#include "game.h"

int main(void)
{
    //Initialize game most settings
    Setti settings = { 1 , 1 , 0 , 0 , false , false, BLACK, RAYWHITE, 0, false, 0, false, 0, 0, 0, 0 };
    strcpy( settings.error, " " ); //Initialize error string to nothing
    strcpy( settings.loadgamename, " " ); //Initialize loadgamename string to nothing
    InitWindow( SCREENWIDTH, SCREENHEIGHT, "BattleINF" ); //Initialize window with default size and name
    InitAudioDevice(); //Initialize audio device
    Image logo = LoadImage( "assets/player.png" ); //Load player image
    SetWindowIcon( logo ); //Set window icon
    SetTargetFPS( 60 ); //Set target FPS to 60
    SetWindowState( FLAG_WINDOW_RESIZABLE ); //Set window to be resizable
    
    //!Highscores crash fix
    highscoresCrashFix();

    //Opens the highscore file and reads it, to be used later for checking if the player has a highscore
    FILE *highscorescheck = fopen( "assets/highscores.bin", "rb" ); //Opens the highscore file in read mode
    char scores[15][100] = {0}; //Array for the highscores
    fread( scores, sizeof( scores ), 1, highscorescheck ); //Reads the highscores into the array
    fclose( highscorescheck ); //Closes the file
    settings.lowscore = atoi( scores[0] ); //Sets the lowest score to the first score in the array

    while ( !settings.quit ) //Main game loop, doesn't close the window until the player wants it to
    {
        SetExitKey( KEY_ESCAPE ); //restores the exit key to escape
        startscreen( &settings ); //Runs the startscreen function, always comes back here
        SetExitKey(0); //Set the exit key to 0
        switch ( settings.select ) //Switch for the selected function in the startscreen
        {
        case 0: //Start
            settings.loadgame = false; //Sets the loadgame variable to false, avoids a couple bugs with the loadgame function and exiting
            remove("saves/savegame.txt"); //Removes the savegame file, to avoid savescumming

            do{ //Starts the game loop
                game( &settings ); //Runs the game function
                settings.foundplayerposition = 0; //Resets the foundplayerposition variable, for reading multiple level files
                settings.enemiesremaining = 0; //Resets the enemiesremaining variable, for reading multiple level files
                settings.level++; //Increases the level variable
            }while ( settings.won ); //Ends if the player has lost
            
            if ( !IsWindowFullscreen() ) //If the window is not fullscreen
                SetWindowState( FLAG_WINDOW_RESIZABLE ); //Set the window to be resizable again, avoids having changing ingame size
            if ( settings.score > settings.lowscore && !settings.quit && !settings.exitgame ) //If the player has a highscore and is not exiting the game
                namescreen( &settings ); //Runs the namescreen function

            settings.exitgame = false; //Resets the exitgame variable
            settings.level = 1; //Resets the level variable
            settings.score = 0; //Resets the score variable
            break; //Ends the switch
        case 1: //Continue
            if ( FileExists( "saves/savegame.txt" ) ) //Checks if the file for the level exists
            {
                settings.loadgame = true; //Sets the loadgame variable to true

                char levelsave[5]; //Array for the level in the savefile
                strncpy( levelsave, &LoadFileText( "saves/savegame.txt" )[615], 4 ); //Copies the level from the savefile to the array
                settings.level = atoi( levelsave ); //Sets the level variable to the level in the savefile
                strcpy( settings.loadgamename, "savegame" ); //Sets the loadgamename variable to savegame

                do{ //Starts the game loop
                    game( &settings ); //Runs the game function
                    settings.foundplayerposition = 0; //Resets the foundplayerposition variable, for reading multiple level files
                    settings.enemiesremaining = 0; //Resets the enemiesremaining variable, for reading multiple level files
                    settings.loadgame = false; //Sets the loadgame variable to false, the game is now able to generate and load new level files
                    settings.level++; //Increases the level variable
                }while ( settings.won );
                
                if ( !IsWindowFullscreen() ) //If the window is not fullscreen
                    SetWindowState( FLAG_WINDOW_RESIZABLE ); //Set the window to be resizable again, avoids having changing ingame size
                
                if ( settings.score > settings.lowscore && !settings.quit && !settings.exitgame ) //If the player has a highscore and is not exiting the game
                    namescreen( &settings ); //Runs the namescreen function
                
                settings.exitgame = false; //Resets the exitgame variable
                settings.level = 1; //Resets the level variable
                settings.score = 0; //Resets the score variable
            }
            else //If the file doesn't exist
                strcpy( settings.error, "No saved game!" ); //Sets the error string to ""No saved game!" and return to startscreen
            break; //Ends the switch
        case 2: //Load
            loadscreen( &settings ); //Runs the loadscreen function
            if ( settings.filenamefound ) //If the function has found a file with that name
            {
                settings.loadgame = true; //Sets the loadgame variable to true
                do{ //Starts the game loop
                    game( &settings ); //Runs the game function
                    settings.foundplayerposition = 0; //Resets the foundplayerposition variable, for reading multiple level files
                    settings.enemiesremaining = 0; //Resets the enemiesremaining variable, for reading multiple level files
                    settings.level++; //Increases the level variable
                    //Doesn't reset loadgame, so the game can load the same level again forever
                }while ( settings.won );
                
                if ( !IsWindowFullscreen() ) //If the window is not fullscreen
                    SetWindowState(FLAG_WINDOW_RESIZABLE); //Set the window to be resizable again, avoids having changing ingame size
                SetExitKey(KEY_ESCAPE); //restores the exit key to escape

                //This function doesn't go to highscore, because the player is loading a level, could be cheating

                settings.loadgame = false; //Resets the loadgame variable
                settings.exitgame = false; //Resets the exitgame variable
                settings.level = 1; //Resets the level variable
                settings.score = 0; //Resets the score variable
            } //If it doesn't, the player just returns to the startscreen
            break; //Ends the switch
        case 3: //Highscores
            highscorescreen( &settings ); //Runs the highscorescreen function
            break; //Ends the switch
        case 4: //Settings
            settingscreen( &settings ); //Runs the settingscreen function
            break; //Ends the switch
        case 5: //Exit
            settings.quit = true; //Sets the quit variable to true, to close the window
            break; //Ends the switch
        default: //If the selected function is not 0, 1, 2, 3, 4 or 5
            break; //Ends the switch and doesn't do anything
        }
    }

    UnloadImage( logo ); //Unloads the logo image
    CloseAudioDevice(); //Closes the audio device
    CloseWindow(); //Closes the window

    return 0; //Returns 0, finishing the program
}