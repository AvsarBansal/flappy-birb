#include <iostream>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>


const int screen_width = 1200;
const int screen_height = 800;
class bird {
public:
    Vector2 pos;
    Color color = WHITE;
    float velocityY;
    float gravity = 1000;
    float jump = 800;
    float size = 40;
    bird()
    {
        velocityY = 0;
        pos.x = 200;
        pos.y = screen_height / 2;
       
    }
    void update()
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            velocityY -= jump;
        }

        velocityY += gravity*GetFrameTime();
        pos.y += velocityY * GetFrameTime();

        if (pos.y >= screen_height - size)
        {
            pos.y = screen_height - size;
            velocityY = 0;
        }
        if (pos.y <= 0)
        {
            pos.y = 0;
            velocityY = 0;
        }
        
    }
    void Draw()
    {
        DrawRectangle(pos.x, pos.y, size, size, color);
    }

};

class pillar {
public:
    float x;
    Color color = WHITE;
    int top_piller_width = 150;
    int gap = 250;
    float speedX = 600;
    int piller_width = 150;
    int top_piller_height;
    

    bool passed = false;
    pillar()
    {
        
        reset();

    }
    void reset()
    {
        x = screen_width;
        top_piller_height = GetRandomValue(100, screen_height - gap - 100);
        passed = false;
    }
    
        void update()
        {
            x -= speedX * GetFrameTime();

            if (x + piller_width < 0)
            {
                reset();
            }
        }

    void Draw()
    {
        DrawRectangle((int)x, 0, piller_width,top_piller_height, color);
        int bottom_y = top_piller_height + gap;
        int bottom_height = screen_height - bottom_y;
        DrawRectangle((int)x, bottom_y,piller_width, bottom_height, color);
    }

};
enum  GameState
{
    PLAYING,
    PAUSED,
    GAME_OVER
};


// note i think we need to load image after we initwindow so gpu can get the image 

int main()
{
    // max score saving 
    int maxscore = 0;
    std::ifstream inputfile("highscore.txt");
    if (inputfile.is_open())
    {
        inputfile >> maxscore;
        inputfile.close();
    }
    // max score logic done
    int score = 0;
    InitWindow(screen_width, screen_height, "flappy birb");
    //texture start 
    Texture2D birdtex = LoadTexture("bird.png");
    Texture2D pipetex = LoadTexture("pipe.png");    



    // texture ends
    bird mybird;
    pillar mypiller;    
    SetTargetFPS(60);
    while (WindowShouldClose() == false)
    {
        mybird.update();
        mypiller.update();
        //score logic 
        if (mybird.pos.x > mypiller.x + mypiller.piller_width && !mypiller.passed)
        {
            score++;
            mypiller.passed = true;
        }
        // collison logic 
        Rectangle birdRec = { mybird.pos.x,mybird.pos.y,mybird.size,mybird.size };
        Rectangle topPillerRec = { mypiller.x,0,mypiller.piller_width,mypiller.top_piller_height };
        //extra defining work
        float bottomY = mypiller.top_piller_height + mypiller.gap;
        float bottomHeight = screen_height - bottomY;
        Rectangle bottomPillerRec = { mypiller.x,bottomY,mypiller.piller_width,bottomHeight};
        //collision logic finished 

        // checking for true false of collision 
        bool hitTopPiller = CheckCollisionRecs(birdRec, topPillerRec);
        bool hitBottomPiller = CheckCollisionRecs(birdRec, bottomPillerRec);
        // end of these 

        if ((hitBottomPiller || hitTopPiller) || mybird.pos.y <=0 || mybird.pos.y+mybird.size>=screen_height)
        {
            if (score > maxscore)
            {
                maxscore = score;
                std::ofstream outputFile("highscore.txt");
                if (outputFile.is_open())
                {
                    outputFile << maxscore;
                    outputFile.close();
                }
            }
            score = 0;
            mybird.pos.y = screen_height / 2;
            mybird.velocityY = 0;
            mypiller.reset();
         }

        // end of this logic 
        BeginDrawing();

        ClearBackground(SKYBLUE);
        mybird.Draw();
        mypiller.Draw();
        DrawText(TextFormat("current score= %i",score), screen_width/2, 50, 50, BLACK);
        
        DrawText(TextFormat("max score = %i", maxscore), 50, 50, 50, BLACK);


        
        EndDrawing();
    }

    UnloadTexture(birdtex);
    UnloadTexture(pipetex);


    CloseWindow();
    return 0;

}
