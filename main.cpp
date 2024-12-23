#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <math.h>

using namespace std;

enum Stackan {
    EMPTY=0,
    BORDER,
    BLOCK,
    WIDTH = 10 + 2,
    HEIGHT = 15 + 1
};

const double PI = 3.14159;

void GoToXY(int x, int y) {
    COORD p = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}


struct Point {
    int x;
    int y;
};
class Figure 
{
     vector<Point> vec;
     vector<Point> bvec;//вектор для сохранения и отката
    
public:

    int getX(int i)
    {
        return vec[i].x;
    } 
    int getY(int i)
    {
        return vec[i].y;
    }  
    int getVecSize()
    {
        return vec.size();
    } 
    
    void save()
    {
        bvec = vec;
    }

    void undo()
    {
        vec = bvec;
    }

    void push(int x, int y)
    {
        Point temp;
        temp.x = x;
        temp.y = y;
        vec.push_back(temp);
    }
    void show() {
        for (int i = 0; i < vec.size(); i++)
        {
            GoToXY(vec[i].x, vec[i].y);
            cout << '*';
        }
    }
    void hide() {
        for (int i = 0; i < vec.size(); i++)
        {
            GoToXY(vec[i].x, vec[i].y);
            cout << ' ';
        }
    }
    void move_down() {
        for (int i = 0; i < vec.size(); i++)
        {
           // int sec = 100;
             vec[i].y++;
            // Sleep(sec);
            
        }
    } 
    void move_up() {
        for (int i = 0; i < vec.size(); i++)
        {
             vec[i].y--;
            
        }
    }
    void move_left() {
        for (int i = 0; i < vec.size(); i++)
        {
            vec[i].x--;

        }
    }
    void move_right() {
        for (int i = 0; i < vec.size(); i++)
        {
            vec[i].x++;

        }
    }
                                                   
    void spawn()
    {
        vec.clear();
        int r = rand() % 4;
        switch (r)
        {
        case 0: 
            this->push(4, 0);
            this->push(5, 0);
            this->push(4, 1);
            this->push(5, 1);
            break;
        case 1:
            this->push(4, 0);
            this->push(5, 0);
            this->push(6, 0);
            this->push(7, 0);
            break; 
        case 2:
            this->push(4, 0);
            this->push(5, 0);
            this->push(5, 1);
            this->push(6, 1);
            break;
        case 3:
            this->push(4, 0);
            this->push(5, 0);
            this->push(6, 0);
            this->push(6, 1);
            break;
      
        }
        
      
    }

    void rotateR(){
       

        for (int i = 0; i < vec.size(); i++) {

   
            int x = - (vec[i].y - vec[1].y)  + vec[1].x;
            int y = (vec[i].x - vec[1].x)  + vec[1].y;
            vec[i].x = x;
            vec[i].y = y; 

       
            
        }
    }
    void rotateL(){
       

        for (int i = 0; i < vec.size(); i++) {

   
            int x =  (vec[i].y - vec[1].y) + vec[1].x;
            int y = -(vec[i].x - vec[1].x)  + vec[1].y;
            vec[i].x = x;
            vec[i].y = y; 

       
            
        }
    }
};

class Map {

    vector <vector<int>> vec;
    friend class Figure;
public:
    void init(int width, int height)
    {
        vector<int>temp;
        temp.push_back(BORDER);
        for (int i = 1; i < width - 1; i++)
            temp.push_back(EMPTY);
        temp.push_back(BORDER);
        for (int i = 0; i < height - 1; i++)
            vec.push_back(temp);
        temp.clear();
        for (int i = 0; i < width; i++)
            temp.push_back(BORDER);
        vec.push_back(temp);
    }
  
    
    bool isCollision(Figure&fig) {
        for (int i = 0; i < fig.getVecSize(); i++)
        {
            if (fig.getY(i) < 0)//столкновение с верхней границей
                return true;
            
            //if (fig.getX(i) < 1)//столкновение с левой границей
            //    return true;

            //if (fig.getX(i) < WIDTH)//столкновение с правой границей
            //    return true;

            if (vec[fig.getY(i)][fig.getX(i)] == BORDER)//столкновение с границей
                return true;
            else if (vec[fig.getY(i)][fig.getX(i)] == BLOCK)//столкновение с уже поставленной фигурой
                return true;
        }
        return false;
    }

    bool shiftLeft(Figure &fig) {
        if (isCollision(fig))
        {
            fig.move_left();
            if (isCollision(fig)) {
                fig.move_left();
                if (isCollision(fig)) {
                    return false;
                }
            }
        }
        return true;
    } 
    bool shiftRight(Figure &fig) {
        if (isCollision(fig))
        {
            fig.move_right();
            if (isCollision(fig)) {
                fig.move_right();
                if (isCollision(fig)) {
                    return false;
                }
            }
        }
        return true;
    }


    void fixed(Figure &fig) {
        for (int i = 0; i < fig.getVecSize(); i++)
        {
            vec[fig.getY(i)][fig.getX(i)] = BLOCK;

        }

    }
    void show_map()
    {
        
        //для отладки
        for (int i = 0; i < vec.size(); i++) {
            GoToXY(19, i);
            cout << i << " ";
            if (i < 10) {
                cout << " ";
            }
            for (int j = 0; j < vec[i].size(); j++)
            {
                cout << vec[i][j];
            }
            cout << endl;
        }

        GoToXY(0, 0);
        for (int i = 0; i < vec.size(); i++) {
            for (int j = 0; j < vec[i].size(); j++)
            {
                if (vec[i][j] == BORDER)
                    cout << '#';
                else if (vec[i][j] == BLOCK)
                    cout << '*';
                else if (vec[i][j] == EMPTY)
                    cout << ' ';
            }
            cout << endl;
        }


    }

    void animatedDelLine(const vector<int>&lines){

        if (lines.empty())
            return;

        for (size_t i = 1; i < WIDTH-2; i++)
        {
            for (size_t j = 0; j < lines.size(); j++)
            {
                GoToXY(i, lines[j]);
                cout << ' ';

            }
            Sleep(50);
        }
    }
    void del_Lines(int width) {
        vector<int>lines;
        //определение линий которые нужног удалить
        for (int i = vec.size() - 2; i >= 0; i--) {
            int counterLines = 0;//счетчик 
            for (int j = 1; j < vec[i].size() - 1; j++)
            {
                if (vec[i][j] == BLOCK)
                {
                    counterLines++;
                }
                else {
                    break;
                }
            }
            if (counterLines == WIDTH - 2 * BORDER) {
                lines.push_back(i);
            }
        }

        animatedDelLine(lines);
        //удвление рядов из вектора
        for (int i = 0; i < lines.size(); i++) {
            vec.erase(vec.begin() + lines[i]);
        }
        //добавление пустых рядов взамен удаленных
        vector<int>temp;
        temp.push_back(BORDER);
        for (int i = 1; i < width - 1; i++)
            temp.push_back(EMPTY);
        temp.push_back(BORDER);
        for (int i = 0; i < lines.size(); i++) {
            vec.insert(vec.begin(), temp);
        }
    }
};
class Game {
private:
   
    //vector <vector<int>> vec;
   
public:
    Game() {}
                                 
    int run() {
        Map map;
        map.init( WIDTH, HEIGHT);
       // initVec(vec, WIDTH, HEIGHT);
        map.show_map();
       // show_field(vec);
        Figure fig;
        fig.spawn();
 /*       fig.push(4,0);
        fig.push(5,0);
        fig.push(4,1);
        fig.push(5,1);*/

      


        int ch = 0;
        int i = 0;
        int x = 5, y = 5;

        while (true) {
            while (!_kbhit()) {

                fig.hide();
                
                fig.move_down(); 

                if (map.isCollision(fig))
                {
                    fig.move_up();
                    map.fixed(fig);
                    map.del_Lines(WIDTH);
                    map.show_map();

                   // del_Lines(WIDTH);
                  // show_field(vec);
                   fig.spawn();

                }

                


                fig.show();
              
                Sleep(300);
               
               
            }
            ch = _getch();
            if (ch == 224)
            {
                ch = _getch();
                GoToXY(20, 20);
                cout << ch;
            }//обработка нажатых клавиш
            switch (ch)
            {
            case  77://право
            
                fig.hide();
            fig.move_right();
            if (map.isCollision(fig))
            {
                fig.move_left();

            }
            
                break;
            case  80://вниз
                fig.hide();
                fig.move_down();
                if (map.isCollision(fig))
                {
                    fig.move_up();

                }


                break;
            case  75://лево
            
                fig.hide();
            fig.move_left();
            if (map.isCollision(fig))
            {
                fig.move_right();

            }

            
                break;
           
            case  72:

                fig.hide();
                fig.save();
                fig.rotateR();
                if (!map.shiftLeft(fig)) {
                    fig.undo();
                    fig.rotateR();
                        if (!map.shiftRight(fig))
                            {
                                fig.undo();
                                    
                            }
                }
                    
                break; 

            case  32:
                fig.hide();
                fig.save();
                fig.rotateL();
                if (!map.shiftLeft(fig)) {
                    fig.undo();
                    fig.rotateL();
                    if (!map.shiftRight(fig))
                    {
                        fig.undo();

                    }
                }
                break;
            }
        }
        return 0;
    }

};


    int main() {

        Game game;
        game.run();

        return 0;
    }