#pragma once 
#include <iostream>
#include <unistd.h>
#include <termios.h>

#define Max_Width 512
#define Max_Height 512

namespace graphics{
    enum Direction{UP, DOWN, LEFT, RIGHT};

    char PixelTable[11] = {
            ' ',
            '.',
            ':',
            '|', 
            '-', // is equivilant to 3
            '=', // is equivilante to 4
            '+',
            '*',
            '#',
            '%',
            '@'
            };


    struct Point{
        int X;
        int Y;

        Point(int X = -1, int Y = -1) : X(X), Y(Y){

        }

        Point& operator=(const Point& P){
            this->X = P.X;
            this->Y = P.Y;
            return *this;
        }

        Point& operator+(const Point& P){
            this->X += P.X;
            this->Y += P.Y;
            return *this;
        }
        Point& operator-(const Point& P){
            this->X -= P.X;
            this->Y -= P.Y;
            return *this;
        }
    };

    struct Line{
        int Count;
        Point *Points;

        Line(int Count = -1, Point *Points = NULL) : Count(Count), Points(Points){

        }

        Line& operator=(const Line& L){
            this->Count = L.Count;
            this->Points = L.Points;
            return *this;
        }

    };


    struct Input{
      char KeyList[128];
      char SmartInputBuffer;
      struct termios oldSettings;
      struct termios newSettings;

      inline void Init(){
           tcgetattr(STDIN_FILENO, &this->oldSettings); // Save current terminal settings
           this->newSettings = this->oldSettings;
           newSettings.c_lflag &= ~(ICANON | ECHO); // Turn off canonical mode and echo
          tcsetattr(STDIN_FILENO, TCSANOW, &this->newSettings); // Apply new terminal settings

           }

      inline void ThreadSmartInput(){
         char Key = getchar();
        if (Key == 27) { // Check if it's an escape sequence
            if (getchar() == '[') { // Verify the next character
                switch (getchar()) { // Get the third character to determine the arrow key
                    case 'A':
                        this->SmartInputBuffer = 'W';
                        break;
                    case 'B':
                        this->SmartInputBuffer = 'S';
                        break;
                    case 'C':
                        this->SmartInputBuffer = 'D';
                        break;
                    case 'D':
                        this->SmartInputBuffer = 'A';
                        break;
                }
            }
        } else {
          this->SmartInputBuffer =  Key;
        }

 
      }

      inline char SmartInput(){
        char Key = getchar();
        if (Key == 27) { // Check if it's an escape sequence
            if (getchar() == '[') { // Verify the next character
                switch (getchar()) { // Get the third character to determine the arrow key
                    case 'A':
                        return 'W';
                        break;
                    case 'B':
                        return 'S';
                        break;
                    case 'C':
                        return 'D';
                        break;
                    case 'D':
                        return 'A';
                        break;
                }
            }
        }
         
       return Key;
 

      }

      inline char RawInput(){
        return getchar();
      }

      inline void Restore(){
        tcsetattr(STDIN_FILENO, TCSANOW, &this->oldSettings);

      }


    };

    struct Display{
        
        int Width;
        int Height;

        char Buffer[Max_Width][Max_Width];

         

        //set as defaults cause i like that size 
        Display(int Width = 79, int Height = 23) : Width(Width), Height(Height){
            for(int i = this->Height; i > 0; i--){
                for(int j = 0; j <= this->Width; j++){
                    this->Buffer[j][i] = ' ';
                    }
            }
        }

        inline void PutPixel(int X, int Y, char C){
            this->Buffer[X][Y] = C;
        }

        inline void ClearPixels(int x1 =0, int y1=0, int x2=Max_Width, int y2=Max_Height){
            for(int i = y1; i <=y2; i++){
                for(int j = x1; j <= x2; j++){
                    this->Buffer[j][i] = ' ';
                }
            }
        }

        inline void Line(int x1,int y1, int x2, int y2, char C){
            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? 1 : -1;
            int err = dx - dy;

            while (x1 != x2 || y1 != y2) {
                this->Buffer[x1][y1] = C;
                int e2 = err * 2;
                if (e2 > -dy) {
                    err -= dy;
                    x1 += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    y1 += sy;
                }
            this->Buffer[x1][y1] = C;
            }

        }

         inline struct Line GetLine(int x1,int y1, int x2, int y2){
            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? 1 : -1;
            int err = dx - dy;
            int i = 0;
            struct Line line(0,(Point*) std::malloc(sizeof(Point)*(dx+dy)));


            while (x1 != x2 || y1 != y2) {
                line.Points[i].X = x1;
                line.Points[i].Y = y1;
                int e2 = err * 2;
                if (e2 > -dy) {
                    err -= dy;
                    x1 += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    y1 += sy;
                }
            i++;
            line.Points[i].X = x1;
            line.Points[i].Y = y1;
            }
            line.Count = i;
            return line;

        }

        inline void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, char C){
            this->Line(x1,y1,x2,y2,C);
            this->Line(x2,y2,x3,y3,C);
            this->Line(x3,y3,x1,y1,C);
        }

        inline void Rect(int TopX, int TopY, int BottomX, int BottomY, char C, bool fill = false){
            if(fill){
                for(int i = TopY; i <= BottomY; i++){
                for(int j = TopX; j <= BottomX; j++){
                    this->Buffer[j][i] = C;
                    }
                }
            } else {
                for(int i = TopY; i <= BottomY; i++){
                        this->Buffer[TopX][i] = C;
                        this->Buffer[BottomX][i] = C;
                    }
                
                // used J just to keep come consistansy 
                for(int j = TopX; j <= BottomX; j++){
                        this->Buffer[j][TopY] = C;
                        this->Buffer[j][BottomY] = C;
                    }
                    
                }
        }

        inline void PutText(int X, int Y, char const *Text, Direction dir = LEFT, Direction wdir = DOWN, int WrapLength = 80){
            const char  * temp = Text; // first copy the pointer to not change the original
            int i = 0;
            int tX = X;
            int tY = Y;
            while(*temp != '\0'){
                i += 1;
                if(i == WrapLength){
                  i = 0;
                  tX = X - tX;
                  tY = Y - tY;

                  X = X - tX;
                  Y = Y - tY;
                  switch(wdir){
                    case UP:
                        Y++;
                        break;
                    case DOWN:
                        Y--;
                        break;
                    case LEFT:
                        X++;
                        break;
                    case RIGHT:
                        X--; 
                        break;

                  };
                }

                this->Buffer[X][Y] = *temp;

                switch(dir){
                    case UP:
                        Y++;
                        break;
                    case DOWN:
                        Y--;
                        break;
                    case LEFT:
                        X++;
                        break;
                    case RIGHT:
                        X--; 
                        break;

                };

                temp++;

            }
        }
        
        inline void AltShow(Display &screen){
            for(int i = screen.Height; i >= 0; i--){
                for(int j = 0; j <= screen.Width; j++){
                    printf("%c",screen.Buffer[j][i]);
                    }
                printf("\n\r");
            }
        }

        inline void Show(){
            for(int i = this->Height; i >= 0; i--){
                for(int j = 0; j <= this->Width; j++){
                    printf("%c",this->Buffer[j][i]);
                    }
                printf("\n\r");
            }
        }

    };

};
