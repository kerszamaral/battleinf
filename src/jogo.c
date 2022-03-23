#include "jogo.h"
#include "core.h"
#include "raymath.h"
#include "collision.h"
#include "playermove.h"
#include "enemy.h"

#define SCREENWIDTH 800 //Screen size x
#define SCREENHEIGHT 450 //Screen size y
const int TOPBORDER = SCREENHEIGHT/10; //Menu border NEED TO CHANGE IT TO CORE WHEN CHANGED FIX IN ENEMY.C
const int BORDER = SCREENHEIGHT/90; //Border around playspace NEED TO CHANGE IT TO CORE WHEN CHANGED FIX IN ENEMY.C

int jogo(void)
{
    int k = 1; //variable for testing

    /********************** MENU VARIABELS *******************************/
    int level = 1; //Number of level on display
    //Textures
    Texture2D healthimg = LoadTexture( "resources/images/health.png" ); //Load imagem da vida do player
    //Rectangles for collision and drawing (dest rectangles need to be in while loop)
    Rectangle Menu[4] = {
    (Rectangle){ 0 , 0 , SCREENWIDTH , TOPBORDER }, //Rectangle for the ingame menu
    (Rectangle){ 0 , SCREENHEIGHT - BORDER , SCREENWIDTH, BORDER}, //Rectangle for bottom border
    (Rectangle){ 0 , 0 , BORDER, SCREENHEIGHT }, //Rectangle for left border
    (Rectangle){ SCREENWIDTH - BORDER , 0 , SCREENWIDTH , SCREENHEIGHT}   //Rectangle for right border
    };

    /********************** PLAYER VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D tankplayer = LoadTexture( "resources/images/player.png" );//Load player image
    //Object
    Obj player = 
    {
        (Vector2){ 0 , 0 }, //Vector2 pos, x and y
        (float) tankplayer.width / tankplayer.height, //ratio
        (Vector2){ tankplayer.width / 20.0 , ( tankplayer.height *player.ratio ) / 20.0 }, //Vector2 center, x and y
        (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
        3, //Health
        0, //Object rotation
        0, //Score
        0, //Time
        0, //Death
        2, //Speed
        true, //Ammo
        (Rectangle){ 0 , 0 , tankplayer.width , tankplayer.height }, //sourceRec
        (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0 , 0 , 0 , 0 }, //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
    };

    /********************** ENEMY VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D tankenemy = LoadTexture( "resources/images/enemy.png" ); //Load enemy image
    //Object
    Obj enemy = 
    {
        (Vector2){ SCREENWIDTH , SCREENHEIGHT }, //Vector2 pos, x and y     Bullets spawn 0,0 conflict enemy spawn
        (float) tankenemy.width / tankenemy.height, //ratio
        (Vector2){ tankenemy.width / 20.0 , ( tankenemy.height * enemy.ratio ) / 20.0 }, //Vector2 center, x and y
        (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
        0, //Health
        0, //Object rotation
        0, //Score
        0, //Time
        0, //Death
        1, //Speed
        true, //Ammo
        (Rectangle){ 0 , 0 , tankenemy.width , tankenemy.height }, //sourceRec
        (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0 , 0 , 0 , 0 } //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
    };

    /********************** BULLET VARIABELS *******************************/
    //Textures (for getting width and height)
    Texture2D bullet = LoadTexture( "resources/images/bullet.png" ); //Load playbullet image
    //Objects
    Obj playbullet = 
    {
        (Vector2){ 0 , 0 }, //Vector2 pos, x and y
        (float) bullet.width / bullet.height, //ratio
        (Vector2){ bullet.width / 100.0 , ( bullet.height * playbullet.ratio ) / 100.0 }, //Vector2 center, x and y
        (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
        0, //Health
        0, //Object rotation
        0, //Score
        0, //Time
        0, //Death
        3, //Speed
        false, //Ammo
        (Rectangle){ 0 , 0 , bullet.width , bullet.height }, //sourceRec
        (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0 , 0 , 0 , 0 } //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
    };
    Obj enemybullet = 
    {
        (Vector2){ 0 , 0 }, //Vector2 pos, x and y
        (float) bullet.width / bullet.height, //ratio
        (Vector2){ bullet.width/100.0 , (bullet.height*playbullet.ratio)/100.0 }, //Vector2 center, x and y
        (Vector2){ 0 , 0 }, //Vector 2 for drawing position, has x and y
        0, //Health
        0, //Object rotation
        0, //Score
        0, //Time
        0, //Death
        2, //Speed
        false, //Ammo
        (Rectangle){ 0 , 0 , bullet.width , bullet.height }, //sourceRec
        (Rectangle){ 0 , 0 , 0 , 0 }, //colRec for object collision, created here, updated in loop
        (Rectangle){ 0 , 0 , 0 , 0 }, //drawRec for drawing and object rotation, created here, updated in loop
        (Vector4){ 0 , 0 , 0 , 0 } //colSide for collision detection algorithm, x = up, y = right, z = down, w = left
    };
    
    /********************** TERRAIN VARIABELS *******************************/
    //Playspace is 800x400 / starts at 0x45 /ends 800x445 / We devide those in 50 by 50 cubes
    //Textures
    Texture2D wall = LoadTexture( "resources/images/wall.png" );
    Rectangle sourceWall = { 0 , 0 , wall.width , wall.height }; //Rectangle with size of original image
    int mapx = 16, mapy = 8;
    ///Random Map Generator for testing, needs to be replaced by read file
    char terrainspace [ mapy ][ mapx ];
    for (int i = 0; i < mapy; i++)
    {
        for (int j = 0; j < mapx; j++)
        {
            switch (GetRandomValue(0,4))
            {
            case 0:
                terrainspace[i][j] = '*';
                break;
            default:
                terrainspace[i][j] = '-';
                break;
            }
        }
    }
    //Prints map to console to know if everything lines up, can be removed when changed
    for (int i = 0; i < mapy; i++)
    {
        for (int j = 0; j < mapx; j++)
            printf("%c",terrainspace[i][j]);
        printf("\n");
    }
    //Creates the Rectangles in the place it finds * in the array to display it in the game
    //Varibles to help find the coordinates the triangles should be placed (might be a better way to do it idk)
    int terrainx = 0, terrainy = 0;
    //We use an array to create 128 rectangles, they are all set to size and position 0 
    //When it finds the * in sets the position and size for the rectangle on that place
    Rectangle terrainarray[ 8 ][ 16 ] = { 0 };
    for ( int i = 0 ; i < 8 ; i++ )
    {
        for ( int j = 0 ; j < 16 ; j++ )
        {
            if ( terrainspace[ i ][ j ] == '*' )
                terrainarray[ i ][ j ] = (Rectangle){ terrainx , terrainy + 45 , 50 , 50 };
            terrainx += 50;
        }
        terrainx = 0;
        terrainy +=50;
    }
    //Random player starting position
    player = spawn( player , terrainspace , terrainarray );
    
    //Main game loop
    while( !WindowShouldClose() && player.health != 0 ) //End if you press esc or player.health gets to 0
    {
        BeginDrawing();

        ClearBackground( RAYWHITE );
        
        /********************** TERRAIN CREATION *******************************/
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 16; j++)
                if (terrainspace[i][j] == '*')
                    DrawTexturePro( wall , sourceWall , terrainarray[i][j] , (Vector2){ 0 , 0 } , 0 , WHITE );

        /********************** MENU CREATION *******************************/
        //Draws solid color rectangles
        for (int i = 0; i < 4; i++)
            DrawRectangleRec( Menu[i] , DARKGRAY ); //Creates grey bars
        //Text
        DrawText( TextFormat( "Fase %d" , level ) , SCREENWIDTH/2 - 6*10 , 5 , 40 , YELLOW );
        DrawText( TextFormat( "Score: %i", player.score ), SCREENWIDTH / 2 + 185 , 5 , 32 , RED );
        //Draws player health for health remaining            spacing from image size x * scaling
        for ( int i = 0, healthx = 5 ; i < player.health ; i++ , healthx += 35 )//
            DrawTextureEx( healthimg , (Vector2){ healthx , 5 } , 0 , 0.025 , WHITE );
        //                                           This image is too big, scaling factor needs to be very small

        /********************** PLAYER DRAWING *******************************/
        //Draw position and draw rectangle update
        //Sets player.draw to be player.pos + offset
        player.draw = (Vector2){ player.pos.x + player.cen.x , player.pos.y + player.cen.y };
        //Rectangle resized and offset for player drawing
        player.drawRec = (Rectangle){ player.draw.x, player.draw.y, player.cen.x*2 , player.cen.y*2 }; 
        //Player collision rectangle update
        player.colRec = (Rectangle){ player.pos.x , player.pos.y , player.cen.x*2 , player.cen.y*2 };
        //Because player cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size
        DrawTexturePro( tankplayer , player.sourceRec , player.drawRec , player.cen , player.rot , WHITE ); //Draws player tank

        /********************** PLAYER COLLISION/MOVEMENT *******************************/
        //Resets collision detection
        player.colSide = (Vector4){ 0 , 0 , 0 , 0 };
        //Tests collision with sides
        for (int i = 0; i < 4; i++)
            player = collision( player , Menu[i]);
        //Tests collision with enemy
        player = collision( player , enemy.colRec );
        //Tests collision with each rectangle of terrain
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 16; j++)
                if (terrainspace[i][j] == '*')
                    player = collision( player, terrainarray[i][j]);
        
        player = moveplayer(player);

        /********************** ENEMY HITBOX *******************************/
        //Draw position and draw rectangle update
        //Sets enemy.draw to be enemy.pos + offset
        enemy.draw = (Vector2){ enemy.pos.x + enemy.cen.x , enemy.pos.y + enemy.cen.y }; 
        //Rectangle resized and offset for enemy drawing
        enemy.drawRec = (Rectangle){ enemy.draw.x, enemy.draw.y, enemy.cen.x*2 , enemy.cen.y*2 };
        //Enemy collision rectangle
        enemy.colRec = (Rectangle){ enemy.pos.x , enemy.pos.y , enemy.cen.x*2 , enemy.cen.y*2 };
        //Because enemy cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size

        /********************** ENEMY SPAWNING *******************************/
        //Spawn logic
        enemy = enemyspawn( enemy, terrainspace , terrainarray );
        //Drawing needs to be done here else it causes a major bug
        if (enemy.health != 0)
            DrawTexturePro( tankenemy , enemy.sourceRec , enemy.drawRec , enemy.cen , enemy.rot , WHITE ); //Draws Enemy tank
        //Will be removed when bullet.c is done
        if ( CheckCollisionRecs( playbullet.colRec, enemy.colRec ) ) //if playbullet collides with enemy, kills enemy
        {   //Reverts the states change when enemy alive to neutral
            enemy.health--;
            playbullet.health--;
            player.ammo = true;
            enemy.pos = (Vector2){SCREENWIDTH,SCREENHEIGHT};
            playbullet.pos = (Vector2){0,0};
            player.score += 800;
        }

        /********************** ENEMY COLLISION/MOVEMENT *******************************/
        //Resets collision detection
        enemy.colSide = (Vector4){ 0 , 0 , 0 , 0 };
        //Tests collision with sides
        for (int i = 0; i < 4; i++)
            enemy = collision( enemy , Menu[i] );
        //Tests collision with player
        enemy = collision( enemy , player.colRec );
        //Tests collision with each rectangle of terrain
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 16; j++)
                if (terrainspace[i][j] == '*')
                    enemy = collision( enemy , terrainarray[i][j]);
        //When map destruction is done need to lower enemy sight distance, very easy
        if ( enemy.health >= 1 )
            enemy = enemymove(enemy, player);
        
        /********************** PLAYER BULLET SHOOTING *******************************/
        //When pointers are implemented should be moved to "Shooting.c" function and data referenced through pointers
        if (IsKeyReleased(KEY_SPACE) && player.ammo == true) //Verify if player has ammo
        {
            //Shooting setup
            player.ammo = false; //Sets ammo to false
            playbullet.health = 1; //Sets playbullet health to 1

            playbullet.rot = player.rot; //Stores the player rotation when playbullet fires for next steps
            playbullet.pos.x = player.draw.x; //Stores the player x postion when playbullet fires for next steps
            playbullet.pos.y = player.draw.y; //Stores the player y postion when playbullet fires for next steps

            //Offsets for each rotation to fire from center and end of barrel
            switch (playbullet.rot)
            {
            case 0:
                playbullet.pos.y -= player.cen.y;
                playbullet.pos.x -= playbullet.cen.x;
                break;
            case 90:
                playbullet.pos.x += player.cen.x;
                playbullet.pos.y -= playbullet.cen.y;
                break;
            case 180:
                playbullet.pos.y += player.cen.y;
                playbullet.pos.x -= playbullet.cen.x;
                break;
            case 270:
                playbullet.pos.x -= player.cen.x;
                playbullet.pos.y -= playbullet.cen.y;
                break;
            }
        }

        //Draw position and draw rectangle updates
        //Sets player.draw to be player.pos + offset
        playbullet.draw = (Vector2){ playbullet.pos.x + playbullet.cen.x , playbullet.pos.y + playbullet.cen.y };
        //Rectangle resized and offset for player drawing
        playbullet.drawRec = (Rectangle){ playbullet.draw.x, playbullet.draw.y, playbullet.cen.x*2 , playbullet.cen.y*2 };
        //Bullet collision rectangle
        playbullet.colRec = (Rectangle){playbullet.pos.x, playbullet.pos.y, playbullet.cen.x*2, playbullet.cen.y*2 };
        //Because playbullet cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size

        if (playbullet.health >= 1) //test to see if should draw playbullet
        {
            //Resets collision detection
            playbullet.colSide = (Vector4){ 0 , 0 , 0 , 0 };
            //Tests collision with sides
            for (int i = 0; i < 4; i++)
                playbullet = collision( playbullet , Menu[i] );
            //Tests collision against other bullets
            playbullet = collision( playbullet , enemybullet.colRec );
            //Tests collision with each rectangle of terrain
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 16; j++)
                    if (terrainspace[i][j] == '*')
                        playbullet = collision( playbullet, terrainarray[i][j]);
            //Kills playbullet if 1 sec passes or it collides with border
            if ( playbullet.time == 60*1 || playbullet.colSide.x || playbullet.colSide.y || playbullet.colSide.z || playbullet.colSide.w )
            {   //Reverts the states change when firing playbullet to neutral
                player.ammo = true;
                playbullet.pos = (Vector2){0,0};
                playbullet.health = 0;
                playbullet.time = 0;
            }

            //Moves playbullet based on position and speed
            switch (playbullet.rot)
            {
            case 0:
                playbullet.pos.y -= playbullet.speed;
                break;
            case 90:
                playbullet.pos.x += playbullet.speed;
                break;
            case 180:
                playbullet.pos.y += playbullet.speed;
                break;
            case 270:
                playbullet.pos.x -= playbullet.speed;
                break;
            }
            //Draws playbullet
            DrawTexturePro(bullet, playbullet.sourceRec, playbullet.drawRec, playbullet.cen, playbullet.rot, WHITE);
            //Stores the time the playbullet is alive based on fps, 1 second = 60 frames
            playbullet.time++;
        }

        /********************** ENEMY BULLET SHOOTING *******************************/
        if (GetRandomValue(0,15) == 0 && enemy.ammo == true && enemy.health >= 1) //Verify if player has ammo
        {
            //Shooting setup
            enemy.ammo = false; //Sets ammo to false
            enemybullet.health = 1; //Sets playbullet health to 1

            enemybullet.rot = enemy.rot; //Stores the enemy rotation when enemybullet fires for next steps
            enemybullet.pos.x = enemy.draw.x; //Stores the enemy x postion when enemybullet fires for next steps
            enemybullet.pos.y = enemy.draw.y; //Stores the enemy y postion when enemybullet fires for next steps

            //Offsets for each rotation to fire from center and end of barrel
            switch (enemybullet.rot)
            {
            case 0:
                enemybullet.pos.y -= enemy.cen.y;
                enemybullet.pos.x -= enemybullet.cen.x;
                break;
            case 90:
                enemybullet.pos.x += enemy.cen.x;
                enemybullet.pos.y -= enemybullet.cen.y;
                break;
            case 180:
                enemybullet.pos.y += enemy.cen.y;
                enemybullet.pos.x -= enemybullet.cen.x;
                break;
            case 270:
                enemybullet.pos.x -= enemy.cen.x;
                enemybullet.pos.y -= enemybullet.cen.y;
                break;
            }
        }

        //Draw position and draw rectangle updates
        //Sets player.draw to be player.pos + offset
        enemybullet.draw = (Vector2){ enemybullet.pos.x + enemybullet.cen.x , enemybullet.pos.y + enemybullet.cen.y };
        //Rectangle resized and offset for player drawing
        enemybullet.drawRec = (Rectangle){ enemybullet.draw.x, enemybullet.draw.y, enemybullet.cen.x*2 , enemybullet.cen.y*2 };
        //Bullet collision rectangle
        enemybullet.colRec = (Rectangle){ enemybullet.pos.x, enemybullet.pos.y, enemybullet.cen.x*2, enemybullet.cen.y*2 };
        //Because enemybullet cen is the center(1/2) of the image scaled, we can multiply by 2 to get the full size

        if (enemybullet.health >= 1) //test to see if should draw enemybullet
        {
            //Resets collision detection
            enemybullet.colSide = (Vector4){ 0 , 0 , 0 , 0 };
            //Tests collision with sides
            for (int i = 0; i < 4; i++)
                enemybullet = collision( enemybullet , Menu[i] );
            //Tests collision against other bullets
            enemybullet = collision( enemybullet , playbullet.colRec );
            //Tests collision with each rectangle of terrain
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 16; j++)
                    if (terrainspace[i][j] == '*')
                        enemybullet = collision( enemybullet, terrainarray[i][j]);
            //Kills enemybullet if 1 sec passes or it collides with border
            if (enemybullet.time == 60*1 || enemybullet.colSide.x || enemybullet.colSide.y || enemybullet.colSide.z || enemybullet.colSide.w )
            {   //Reverts the states change when firing enemybullet to neutral
                enemy.ammo = true;
                enemybullet.health = 0;
                enemybullet.pos = (Vector2){SCREENWIDTH,SCREENHEIGHT};
                enemybullet.time = 0;
            }

            //Moves enemybullet based on position and speed
            switch (enemybullet.rot)
            {
            case 0:
                enemybullet.pos.y -= enemybullet.speed;
                break;
            case 90:
                enemybullet.pos.x += enemybullet.speed;
                break;
            case 180:
                enemybullet.pos.y += enemybullet.speed;
                break;
            case 270:
                enemybullet.pos.x -= enemybullet.speed;
                break;
            }
            
            if (CheckCollisionRecs( player.colRec , enemybullet.colRec ))
            {
                enemy.ammo = true;
                enemybullet.health = 0;
                enemybullet.time = 0;
                enemybullet.pos = (Vector2){SCREENWIDTH,SCREENHEIGHT};
                player.health--;
            }
            //Draws enemybullet
            DrawTexturePro(bullet, enemybullet.sourceRec, enemybullet.drawRec, enemybullet.cen, enemybullet.rot, WHITE);
            //Stores the time the enemybullet is alive based on fps, 1 second = 60 frames
            enemybullet.time++;
        }
        
        /********************** TESTING VARIABLES *******************************/
        if (IsKeyPressed(KEY_K))
            k*=-1;
        if (k==-1)
            player.health = 3;
        EndDrawing();
    }
    /********************** UNLOADING AREA *******************************/
    UnloadTexture(healthimg);
    UnloadTexture(tankplayer);
    UnloadTexture(bullet);
    UnloadTexture(tankenemy);
    return player.score;
}