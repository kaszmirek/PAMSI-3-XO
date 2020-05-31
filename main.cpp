#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdio.h>
using namespace std;

class Field
{
private:
    int **field;
    int fieldSize;
    int lenghtToWin;
    int moveCounter;
    int last_move[2];
public:
public:
    Field();
    virtual ~Field();

    void createField(int r,int w);
    char sign(int i,int j);
    void display();
    void setSign(int i,int j,int znak);
    int checkWinner();
    int returnSize();
    int returnSign(int i, int j);
    int returnlenghtToWin();
};

Field::Field()
{
    //ctor
}

Field::~Field()
{
    delete [] field;
    field=NULL;
}

void Field::createField(int r,int w)
{
    fieldSize=r;
    lenghtToWin=w;
    moveCounter=0;
    last_move[0]=last_move[1]=fieldSize;

    field=new int *[fieldSize];
    for (int i=0;i<fieldSize;i++)
    {
        field[i]=new int[fieldSize];
        for (int j=0;j<fieldSize;j++)
        {
            field[i][j]=0;
        }
    }
}

/* Funkcja zwracajaca znak przypisany do wskazanego pola
    1 - pole wybrane przez gracza (x), -1 - pole wybrane przez komputer (o), 0 - pole puste */
char Field::sign(int i,int j)
{
    if(field[i][j]==1)
        return 'x';

    else if(field[i][j]==-1)
        return 'o';

    else
        return ' ';
}

/* Funkcja wyswietlajaca plansze */
void Field::display()
{
    system("cls");
    int i,j,k;
    cout<<"    Kolko i krzyzyk dla rozmiaru: "<<fieldSize<<" na "<<fieldSize<<"\n";
    cout <<"   Do zwyciestwa potrzeba: " << lenghtToWin << " w pionie,poziomie lub na skos\n";;
    cout<<"Czlowiek - x     |     Komputer - o \n\n";

    for (k=0;k<fieldSize;k++)
        cout<<"     "<<k;
    cout<<endl<<"   ";
    for (k=0;k<fieldSize;k++)
        cout<<"______";
    cout<<endl;
    for (i=0;i<fieldSize;i++){
        cout<<"  |";
        for (k=0;k<fieldSize;k++)
            cout<<"     |";
        cout<<endl<<i;
        if(i>9)
            cout<<"|";
        else
            cout<<" |";
        for (j=0;j<fieldSize;j++){
            cout<<"  "<<sign(i,j)<<"  |";
        }
        cout<<endl;
        cout<<"  |";
        for (k = 0; k < fieldSize; k++)
            cout << "_____|";
        cout<<endl;
    }
}

/* Funkcja ustawia znak na wybranym polu
    1 - pole wybrane przez gracza (x), -1 - pole wybrane przez komputer (o), 0 - pole puste
    Przy przypisywaniu pola dolicza ruch do licznika ruchow i zapisuje wspolrzedne tego ruchu do tablicy ostatni
    Przy zwalnianiu pola (znak=0) odlicza ruch od liczby wykonanych ruchow i resetuje wspolrzedne ostatniego ruchu
*/
void Field::setSign(int i,int j,int znak)
{
    field[i][j]=znak;
    if(znak==0)
    {
        moveCounter--;
        last_move[0]=last_move[1]=fieldSize;
    }
    else
    {
        moveCounter++;
        last_move[0]=i;
        last_move[1]=j;
    }
}

/* Funkcja sprawdzajaca czy gra zostala rozstrzygnieta
    2 - gra w toku, 1 - wygrana gracza , -1 - wygrana komputera, 0 - remis
    Jesli wspolrzedne ostatniego ruchu = szerokosci pola (reset) to zwraca 2.
    Kiedy znajdzie rzad wygrywajacy to zwraca -1 lub 1.
    Jezeli liczba ruchow rowna sie szerokosci pola do kwadratu to zwraca 0.
	Jezeli zaden z warunkow nie zostanie spelniony zwraca 2.
*/
int Field::checkWinner(){
    if(last_move[0]==fieldSize)
        return 2;

    int i,j,w,k,a,b,znak,counter;
    znak=counter=1;
    w=last_move[0];
    k=last_move[1];
    znak=field[w][k];

    for (i=-1;i<1;i++){
        for(j=-1;j<=1;j++){
            if(i>=0 && j>=0){
                if(moveCounter>=fieldSize*fieldSize)
                    return 0;
                else
                    return 2;
            }
            counter=1;
            a=i;
            b=j;
            while(w+a>=0 && w+a<fieldSize && k+b>=0 && k+b<fieldSize && field[w+a][k+b]==znak){
                counter++;
                a+=i;
                b+=j;
            }
            a=i;
            b=j;
            while(w-a>=0 && w-a<fieldSize && k-b>=0 && k-b<fieldSize && field[w-a][k-b]==znak){
                counter++;
                a+=i;
                b+=j;
            }
            if (counter>=lenghtToWin)
                return field[w][k];
        }
    }
    return 2;
}

int Field::returnSize()
{
    return fieldSize;
}

int Field::returnSign(int i, int j)
{
    return field[i][j];
}

int Field::returnlenghtToWin()
{
    return lenghtToWin;
}

struct Move{
    Move() : score(0){};
    Move(int s) : score(s){}
    int w;				//wiersz
    int k;				//kolumna
    int score;			//ocena ruchu
};


class Computer
{
private:
    int ComputerPlayer;               //identyfikator dla komputera (-1)
    int Player;					 //identyfikator dla gracza (1)
    int depth ;               //glebokosc rekurencji algorytmu minimax
    int fieldSize;                 //rozmiar planszy
    int lenghtToWin;              //dlugosc wygrywajacego rzedu

protected:

public:
    Computer();
    virtual ~Computer();

    void Beginning(Field& field);
    Move MiniMax(Field& field,int alpha,int beta,int Player,int depth,bool recursion);
    void ComputerMove(Field& field);
};

Computer::Computer()
{
    //ctor
}

Computer::~Computer()
{
    //dtor
}

/*Funkcja inicjalizuje gracza komputer.
Wczytuje rozmiar i wygrywajacy rzad.
*/
void Computer::Beginning(Field& field){
    ComputerPlayer=-1;
    Player=1;
    depth=0;
    fieldSize=field.returnSize();
    lenghtToWin=field.returnlenghtToWin();
}


/* Funkcja dla algorytmu minimax z ciêciami alfa i beta oraz ocen¹ uwzglêdniaj¹c¹ iloœæ ruchu
    do zamierzonego rezultatu oraz ocenê wybranego pola.
    Jezeli ruch konczy siê wygrana ktoregos z graczy lub remisem to zwracany jest ruch
    z ocena 1000-punkty za glebokosc dla zwyciestwa komputera i -1000+punkty za glebokosc
    dla zwyciestwa czlowieka.
    Jezeli ruch nie konczy gry to wykonywany jest kolejny ruch i rekurencyjnie wywolywana jest
    ta sama funkcja, tym razem dla drugiego gracza. Po zebraniu kazdego mozliwego ruchu wraz z ich
    ocenami (do oceny ruchu doliczana jest takze ocena wstepna dla wybranego pola) wybierany jest
    ten najkorzystniejszy: Z najnizsza ocena w przypadku ruchu czlowieka i z najwyzsza w przypadku
    ruchu komputera.
    Ciecia alfa i beta pozwalaja ominac symulowania kolejnych ruchow gdy nie maja one wplywu na wybor.
    Dla komputera: Po przeszukaniu jednej ze sciezek, pomija siê przeszukiwanie czesci kolejnej sciezki
    jezeli wiadomo jest ze przeciwnik wybierze w niej ruch dla nas najbardziej niekorzystny, ktory jest
    oceniany slabiej od poprzednio znalezionego. Dla przeciwnika ciecia dzialaja analogicznie.

*/
Move Computer::MiniMax(Field& field,int alpha,int beta,int player,int depth,bool recursion){
    int i,j,best;
    int iterator=0;
    int score=field.checkWinner();
    Move ruch;
    vector<Move> moves;

    if (recursion)
        depth++;
    if(score!=2)
        return (score==-1) ? Move(score*(-1000)-20*depth) : Move(score*(-1000)+20*depth);

    else if(player==ComputerPlayer){
        best=alpha;
        for(i=0;i<field.returnSize();i++){
            for(j=0;j<field.returnSize();j++){
                if(field.returnSign(i,j)==0){
                    field.setSign(i,j,Player);
                    if (!recursion) printf("|");
                    if (depth<6)
                        ruch.score = MiniMax(field,best,beta,Player,depth,1).score;
                    ruch.w=i;
                    ruch.k=j;
                    moves.push_back(ruch);
                    field.setSign(i,j,0);
                    if(ruch.score>best) best=ruch.score;
                    if(beta<=best) i=j=field.returnSize();

                }
            }
        }
        best=-10000;
        for(i=0;i<(int)moves.size();i++){
            if(moves[i].score>best){
                iterator=i;
                best=moves[i].score;
            }
        }
    }
    else{
        best=beta;
        for(i=0;i<field.returnSize();i++){
            for(j=0;j<field.returnSize();j++){
                if(field.returnSign(i,j)==0){
                    field.setSign(i,j,player);
                    ruch.score = MiniMax(field,alpha,best,ComputerPlayer,depth,1).score;
                    ruch.w=i;
                    ruch.k=j;
                    moves.push_back(ruch);
                    field.setSign(i,j,0);
                    if(ruch.score<best)
                        best=ruch.score;
                    if(alpha>=best)
                        i=j=field.returnSize();
                }
            }
        }
        best=10000;
        for(i=0;i<(int)moves.size();i++){
            if(moves[i].score<best){
                iterator=i;
                best=moves[i].score;
            }
        }
    }
    return moves[iterator];
}

/* Funkcja obslugujaca ruch komputera
    Do znalezienia najlepszego ruchu wykorzystywany jest algorytm minimax z cieciami alfa i beta.
*/
void Computer::ComputerMove(Field& field){
    Move best;
    best=MiniMax(field,-10000,10000,ComputerPlayer,0,0);
    field.setSign(best.w,best.k,ComputerPlayer);
}

class Game
{
private:
    Field field;
    Computer ComputerPlayer;
    int player;         //znacznik gracza: 1 (gracz) lub 2 (komputer)
    int state;          //-1 wygrana komputera, 1 wygrana gracza
    //0 - remis, 2 - gra w toku
protected:

public:
    Game();
    virtual ~Game();

    void GameStart();
    void Process();
    void PlayerMove();
    void ChangePlayer();
};

Game::Game()
{
    //ctor
}

Game::~Game()
{
    //dtor
}


/* Funkcja inicjalizujaca gre
    Pozwala wprowadzic rozmiar pola, dlugosc rzedu wygrywajacego i rozpoczynajacego gracza.
    Inicjalizuje plansze i gracza komputer.
*/
void Game::GameStart()
{
    int fieldSize=0,lenghtToWin=0;
    state=2;

    cout<<"Podaj rozmiar planszy od 3 do 9:"<<endl;
    while(1)
    {
        while (!(cin>>fieldSize))
        {
            cout<<"Zly wybor"<<endl;
            cout<<"Podaj rozmiar planszy od 3 do 9."<<endl;
            cin.clear();
            cin.sync();
        }
        if (fieldSize>2 && fieldSize<10)
            break;
        else
            cout<<"Zly rozmiar."<<endl;
        cout<<"Podaj rozmiar planszy od 3 do 9."<<endl;
    }

    cout<<"Podaj dlugosc wygrywajacego rzedu od 3 do rozmiaru planszy:"<<endl;
    while(1)
    {
        while (!(cin>>lenghtToWin))
        {
            cout<<"Zly wybor."<<endl;
            cout<<"Podaj dlugosc wygrywajacego rzedu od 3 do rozmiaru planszy:"<<endl;
            cin.clear();
            cin.sync();
        }
        if(lenghtToWin>2 && lenghtToWin<=fieldSize)
            break;
        else
            cout<<"Zly rozmiar."<<endl;
        cout<<"Podaj dlugosc wygrywajacego rzedu od 3 do rozmiaru planszy:"<<endl;
    }
    field.createField(fieldSize,lenghtToWin);
    ComputerPlayer.Beginning(field);

    cout<<"Kto zaczyna? 1 - Gracz (x), 2 - Komputer (o)."<<endl;
    while(1) {
        while (!(cin>>player)) {
            cout<<"Zly wybor."<<endl;
            cout<<"Kto zaczyna? 1 - Gracz (x), 2 - Komputer (o)."<<endl;
            cin.clear();
            cin.sync();
        }
        if(player==1 || player==2)
            break;
        else
            cout<<"Zly wybor."<<endl;
        cout<<"Kto zaczyna? 1 - Gracz (x), 2 - Komputer (o)."<<endl;
    }
}

/* Funkcja kontrolujaca przebieg gry
    Wywoluje funkcje inicjalizujaca, wyswietla plansze, umozliwia wykonanie ruchu,
    sprawdza stan gry po kazdym ruchu i zmienia graczy.
    Po zakonczeniu gry umozliwia ponowna gre.
*/
void Game::Process()
{

    while(1){
        GameStart();
        while(state==2){
            field.display();
            if(player==1)
                PlayerMove();
            else
                ComputerPlayer.ComputerMove(field);
            state=field.checkWinner();
            ChangePlayer();
        }
        field.display();
        if(state==0)
            cout<<"Remis!\n";
        else if(state==-1)
            cout<<"Przegrales\n";
        else if(state==1)
            cout<<"Wygrales!\n";

        cout<<"Nowa gra? 1 - tak, 0 - koniec.\n";
        while(1) {
            while (!(cin>>state)){
                cout<<"Zly wybor. 1 - tak, 0 - koniec.\n";
                cin.clear();
                cin.sync();
            }
            if(state==1)
                break;
            if(state==0)
                return;
            else
                cout<<"Zly wybor. 1 - tak, 0 - koniec.\n";
        }
    }
}

/* Funkcja obslugujaca ruch gracza
    Pozwala na wybranie pola, sprawdza poprawnosc wprowadzonych danych.
*/
void Game::PlayerMove()
{
    int w,k;
    bool change=0;
    cout<<"Twoj ruch. Najpierw wpisz wiersz, nastepnie kolumne.";
    while (!change){
        while (!(cin>>w)){
            cout<<"Zly wybor. Najpierw wpisz wiersz, nastepnie kolumne.\n";
            cin.clear();
            cin.sync();
        }
        while (!(cin>>k)){
            cout<<"Zly wybor. Najpierw wpisz wiersz, nastepnie kolumne.\n";
            cin.clear();
            cin.sync();
        }
        if(w>=0 && w<field.returnSize() && k>=0 && k<field.returnSize())
            if(field.returnSign(w,k)==0) {
                field.setSign(w,k,1);
                change=1;
            }
            else
                cout<<"Pole zajete. Wybierz nowe. Najpierw wpisz wiersz, nastepnie kolumne.\n";
        else
            cout<<"Pole spoza planszy. Wybierz nowe. Najpierw wpisz wiersz, nastepnie kolumne.\n";
    }
}

void Game::ChangePlayer()
{
    if(player==1)
        player=2;
    else
        player=1;
}

int main()
{
    Game game;
    game.Process();
    return 0;
}

