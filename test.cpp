#include "tgraphics.hpp"


int main(){
  //graphics::Display Screen(248,70);
  graphics::Display Screen(79,22);

  Screen.Rect(0,0, Screen.Width,Screen.Height, '#', false);

  Screen.PutText(5,4, "X");
  Screen.PutText(10,4, "X");
  Screen.PutText(12,4, "X");
  Screen.PutText(17,4, "X");


  Screen.PutText(4,5, "Y");
  Screen.PutText(4,10, "Y");


  //Screen.Rect(5,5, 5,10, '|', false);

  //Screen.Rect(12,5, 12,10, '|', true);

  //Screen.Triangle(5,2, 60,20, 15,2, 'o');

  Screen.PutText(30,15, "This Text", graphics::DOWN);

  Screen.PutText(30,1, "This Text 2", graphics::LEFT);

  //Screen.Line(5,2, 70,20, 'o');

  Screen.PutPixel(30,17, '5');

  graphics::Line Temp = Screen.GetLine(0,0, Screen.Width,10);

  std::cout << Temp.Points[0].X << " | "  << Temp.Points[0].Y << std::endl;
  for(int i = 0; i < Temp.Count; i++){
    if(i < 10){
      Screen.PutPixel(Temp.Points[i].X, Temp.Points[i].Y, graphics::PixelTable[9]);
    }
    if(i > 10){
      Screen.PutPixel(Temp.Points[i].X, Temp.Points[i].Y, graphics::PixelTable[8]);
    }
    if(i > 20){
      Screen.PutPixel(Temp.Points[i].X, Temp.Points[i].Y, graphics::PixelTable[7]);
    }
    if(i > 30){
      Screen.PutPixel(Temp.Points[i].X, Temp.Points[i].Y, graphics::PixelTable[6]);
    }
    if(i > 40){
      Screen.PutPixel(Temp.Points[i].X, Temp.Points[i].Y, graphics::PixelTable[5]);
    }
  }

  Screen.PutText(0,5, "100");
  Screen.PutText(0,10, "200");
  Screen.PutText(0,15, "300");
  Screen.Show();

 graphics::Input input;
 input.Init();

while(true){

  if(input.SmartInput() == 'D'){
    Screen.PutText(0,5, "350");
    Screen.PutText(0,10, "450");
    Screen.PutText(0,15, "550");
    Screen.Show();
  }


  if(input.SmartInput() == 'A'){
    Screen.PutText(0,5, "100");
    Screen.PutText(0,10, "200");
    Screen.PutText(0,15, "300");
    Screen.Show();
  }
  

  if(input.SmartInput() == 'q'){
    input.Restore();
    break;

}
}


  return 0;
}
