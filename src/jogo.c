#include "raylib.h"
#include "jogo.h"

#define SCREENWIDTH 800 //Screen size x
#define SCREENHEIGHT 450 //Screen size y
#define TOPBORDER 50 //Menu border
#define BORDER 5 //Game border

typedef struct //struct for the interactable objects in the game
{
    Vector2 pos; //Vector2 for position, has x and y
    Vector2 cen; //Vector2 for center position, has x and y
    int health; //Int for health, for player
    int rot; //Int for object rotation
    int score; //int for game score, for player 
    int time; //Int for time, for bullets and enemys
    float speed; //int for speed, defines the speed objects move
    bool ammo; //bool for defining if the object can shoot or is alive
} Obj;

int jogo(void)
{
    /********************** PLAYER VARIABELS *******************************/
    //              pos      x   y              IMG x     IMG Y       health   score   speed 
    Obj player = {(Vector2){ 0 , 0 }, (Vector2){ 348/20.0 , 457/20.0 }, 3 , 0 , 0 , 0 , 5 , true };
    //                                   cen        x          y           rot     time     ammo
    //Random starting position player.cenx
    player.pos.x = GetRandomValue( BORDER*2 + player.cen.x , SCREENWIDTH - BORDER*2 - player.cen.x ); //To start player in random position inbounds
    player.pos.y = GetRandomValue( 40 + BORDER*2 + player.cen.y , SCREENHEIGHT - BORDER*2 - player.cen.y ); //To start player in random position inbounds
    //Textures
    Texture2D tankplayer = LoadTexture( "resources/images/player.png" );//Load imagem do tanque do player
    //Rectangles for collision and drawing (dest rectangles need to be in while loop)
    Rectangle sourcePlayer = { 0 , 0 , 348 , 457 }; //Retangulo do tamanho da image com posição 0

    /********************** BULLET VARIABELS *******************************/
    //              pos      x   y              IMG x       IMG Y       health   score   speed 
    Obj bullet = {(Vector2){ 0 , 0 }, (Vector2){ 394/100.0 , 500/100.0 }, 0 , 0 , 0 , 0 , 6 , false };
    //                                   cen        x          y             rot     time     ammo
    //Textures
    Texture2D bulletimg = LoadTexture( "resources/images/bullet.png" ); //Load imagem da vida
    //Rectangles for collision and drawing (dest rectangles need to be in while loop)
    Rectangle sourceBullet = { 0 , 0 , 394 , 500 }; //Retangulo do tamanho da image com posição 0

    /********************** MENU VARIABELS *******************************/
    int level = 1; //Number of level on display
    //Textures
    Texture2D healthimg = LoadTexture( "resources/images/health.png" ); //Load imagem da vida do player
    //Rectangles for collision and drawing (dest rectangles need to be in while loop)
    Rectangle playspaceSizerec = { 0 , 0 , SCREENWIDTH , SCREENHEIGHT }; //Retangulo para a borda do jog
    Rectangle topMenurec = { 0 , 0 , SCREENWIDTH , TOPBORDER }; //Retangle for the ingame manu

    //Main game loop
    while( !WindowShouldClose() && player.health != 0 ) //End if you press esc or player.health gets to 0
    {
        BeginDrawing();

        ClearBackground( RAYWHITE );

        /********************** MENU CREATION *******************************/
        //Draws solid color rectangles
        DrawRectangleRec( topMenurec , DARKGRAY ); //Creates the big grey bar for text
        DrawRectangleLinesEx( playspaceSizerec , 5 , DARKGRAY ); //Creates the border for the playspace
        //Text
        DrawText( TextFormat( "Fase %d" , level ) , SCREENWIDTH/2 - 6*10 , 0 , 40 , YELLOW );
        DrawText( TextFormat( "Score: %i", player.score ), SCREENWIDTH / 2 + 215 , 5 , 32 , RED );
        //Draws player health for health remaining            spacing from image size x * scaling
        for ( int i = 0, healthx = 5 ; i < player.health ; i++ , healthx += 35 )//
            DrawTextureEx( healthimg , (Vector2){ healthx , 5 } , 0 , 0.025 , WHITE );
        //                                           This image is too big, scaling factor needs to be very small

        /********************** PLAYER DRAWING *******************************/
        Rectangle destPlayer = { player.pos.x , player.pos.y , player.cen.x*2 , player.cen.y*2 }; 
        //Because player cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
        DrawTexturePro( tankplayer , sourcePlayer , destPlayer , player.cen , player.rot , WHITE ); //Draws player tank

        /********************** PLAYER MOVEMENT *******************************/
        if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) 
        {   //For Tank like controls
            if ( (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && player.pos.y > player.cen.y + TOPBORDER + 2) 
            {                                   //Checks if player center is not colliding with top of playspace + length + correction factor
                player.pos.y -= player.speed;
                player.rot = 0; //Sets players rotation to up
            }
            if ( (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && player.pos.y + player.cen.y < SCREENHEIGHT - BORDER - 2)
            {                                   //Checks if player center + length is not colliding with bottom of playspace and border + correction factor
                player.pos.y += player.speed;
                player.rot = 180; //Sets player rotation to down
            }
        }
        if (!IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)&& !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) //For Tank like controls
        {
            if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && player.pos.x > player.cen.x + BORDER*2.5 +2 )
            {                                   //Checks if player center is not colliding with left of playspace + lenght + correction factor
                player.pos.x -= player.speed;
                player.rot = 270; //Sets player rotation to left
            }
            if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && player.pos.x + player.cen.x < SCREENWIDTH - BORDER*2.5 - 2)
            {                                   //Checks if player center + lenght is not colliding with right of playspace and border + correction factor
                player.pos.x += player.speed;
                player.rot = 90; //Sets player rotation to right
            }
        }
        
        /********************** BULLET SHOOTING *******************************/
        if (IsKeyReleased(KEY_SPACE) && player.ammo == true) //Verify if player has ammo
        {
            //Shooting setup
            player.ammo = false; //Sets ammo to false
            bullet.ammo = true; //Sets bullet firing(ammo) to true

            bullet.rot = player.rot; //Stores the player rotation when bullet fires for next steps
            bullet.pos.x = player.pos.x; //Stores the player x postion when bullet fires for next steps
            bullet.pos.y = player.pos.y; //Stores the player y postion when bullet fires for next steps

            //Offsets for each rotation to fire from center of barrel
            if (bullet.rot == 0)
                bullet.pos.y -= player.cen.y;
            if (bullet.rot == 90)
                bullet.pos.x += player.cen.x;
            if (bullet.rot == 180)
                bullet.pos.y += player.cen.y;
            if (bullet.rot == 270)
                bullet.pos.x -= player.cen.x;
        }
        if (bullet.ammo == true) //test to see if should draw bullet
        {
            Rectangle destBullet = {bullet.pos.x, bullet.pos.y, bullet.cen.x*2, bullet.cen.y*2 };
            //Because bullet cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size

            if (bullet.time == 60*1 || CheckCollisionRecs(destBullet,topMenurec)) //Kills bullet if 1 sec passes or it collides 
            {   //Reverts the states change when firing bullet to neutral
                player.ammo = true;
                bullet.ammo = false;
                bullet.time = 0;
            }

            //Moves bullet based on position and speed
            if (bullet.rot == 0)
                bullet.pos.y -= bullet.speed;
            if (bullet.rot == 90)
                bullet.pos.x += bullet.speed;
            if (bullet.rot == 180)
                bullet.pos.y += bullet.speed;
            if (bullet.rot == 270)
                bullet.pos.x -= bullet.speed;

            //Draws bullet
            DrawTexturePro(bulletimg, sourceBullet, destBullet, bullet.cen, bullet.rot, WHITE);
            //Stores the time the bullet is alive based on fps, 1 second = 60
            bullet.time++;
        }
        

        /********************** TESTING VARIABLES *******************************/
        if (IsKeyPressed(KEY_H)) //Test lives counter
            player.health -= 1;
        if (IsKeyPressed(KEY_K)) //Kill player
            player.health = 0;
        if (IsKeyPressed(KEY_ENTER)) //Test score counter
            player.score += 100;

        EndDrawing();
    }

    /********************** UNLOADING AREA *******************************/
    UnloadTexture(healthimg);
    UnloadTexture(tankplayer);
    UnloadTexture(bulletimg);
    return player.score;
}