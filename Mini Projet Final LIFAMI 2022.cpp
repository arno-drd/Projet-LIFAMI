#include <Grapic.h>
using namespace grapic;
using namespace std;

const int DIMX = 1000;
const int DIMY = 600;
const float dt = 0.09f;
// JEU DE TENNIS GRAPIC

// **********************************************************************************************************************
//STRUCUTRES & OPERATIONS
// **********************************************************************************************************************

struct Vect2D{
    float x,y;
};                                                                                                                                            ;
struct Balle{
    Vect2D p,v,a; //position vitesse acceleration
    float m; //masse
};
struct Joueur_1{ //raquette gauche
    int x1,x2,y1,y2;
    int l;
    bool win_last_point; //si le joueur a gagné = true
    int jauge_smash, jauge_short, jauge_lob; //nombre de smash et d'amortis effectués par J1
};
struct Joueur_2{ //raquette droite
    int x1,x2,y1,y2;
    int l;
    bool win_last_point; //si le joueur a gagné = true
    int jauge_smash, jauge_short, jauge_lob; //nombres de smash et d'amortis et lobs effectués par J2
};
struct Filet{
    int x1,x2,y1,y2; //à voir plus tard si je reste sur un line
};
struct Jeu{
    Balle b;
    Filet f;
    Joueur_1 J1;
    Joueur_2 J2;
    int ssc1,ssc2; //score dans le jeu de j1 et 12, lorsqu'il atteint 40 +1jeu , jsc+=1
    int jsc1,jsc2; //nombre de jeux gagnés par j1 et j2
    bool J1Turn, J2Turn; // le dernier qui a frappé la balle
    int difficulty;
    bool endPoint; // si qqn a gagné, on recommence
    int nb_echanges, meilleur_nb_echanges;
    bool endGame;
    //int commands; //pour montrer/cacher les controles (valeur 2 pour cacher le menu ET les commandes)
};

Vect2D make_vect2d (float x, float y){
    Vect2D v;
    v.x=x;
    v.y=y;
    return v;
}
Vect2D operator+(Vect2D a, Vect2D b) {
    Vect2D c;
    return c = make_vect2d(a.x+b.x, a.y+b.y);
}
Vect2D operator-(Vect2D a, Vect2D b) {
    Vect2D c;
    return c = make_vect2d(a.x-b.x, a.y-b.y);
}
Vect2D operator*(float lambda, Vect2D a){
    Vect2D c;
    return c = make_vect2d( lambda*a.x, lambda*a.y);
}
Vect2D operator*(Vect2D a, float lambda){
    Vect2D c;
    return c = make_vect2d( lambda*a.x, lambda*a.y);
}
Vect2D operator*(Vect2D a, Vect2D b){
    Vect2D c;
    return c = make_vect2d( a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x );
}
Vect2D operator/(Vect2D b, float d) {
    Vect2D c;
    return c = make_vect2d( b.x/d, b.y/d );
}

// **********************************************************************************************************************
//FONCTIONS DE DEBUG
// **********************************************************************************************************************

void printVect2d(Vect2D v){
    cout<<v.x<<endl;
    cout<<v.y<<endl;
}
void printNbMoves(Jeu j){
    cout<<j.J1.jauge_smash<<" "<<j.J1.jauge_short<<" "<<j.J1.jauge_lob<<" "<<endl;
    cout<<j.J2.jauge_smash<<" "<<j.J2.jauge_short<<" "<<j.J2.jauge_lob<<" "<<endl;
}
void forceEndGame(Jeu& j){
    j.endGame==true;
}
void forceEndPoint (Jeu& j){
    j.endPoint==true;
}
// **********************************************************************************************************************
//CODE PRINCIPAL
// **********************************************************************************************************************

void addScoreJ1 (Jeu& j){
    //if (j.jsc1==5 && j.ssc1==40) j.endGame=true;
    if (j.jsc1==0 && j.ssc1==40) j.endGame=true; //pour aller plus vite
    switch (j.ssc1){
        case 0: j.ssc1 = 15; break;
        case 15: j.ssc1 = 30; break;
        case 30: j.ssc1 = 40; break;
        case 40: j.ssc1 = 0 ; j.ssc2 = 0 ; j.jsc1+=1; break;

    }
}
void addScoreJ2 (Jeu& j){
    //if (j.jsc2==5 && j.ssc2==40) j.endGame=true;
    if (j.jsc2==0 && j.ssc2==40) j.endGame=true; //pour aller plus vite
    switch (j.ssc2){
        case 0: j.ssc2 = 15; break;
        case 15: j.ssc2 = 30; break;
        case 30: j.ssc2 = 40; break;
        case 40: j.ssc2 = 0 ; j.ssc1 = 0 ; j.jsc2+=1; break;

    }

}
void initBalle(Jeu& j){
    j.b.m=1.0f;
    j.b.p=make_vect2d(DIMX/2, DIMY/2);
    j.b.a=make_vect2d(0,0);
    j.b.v=make_vect2d(0,0);
}
void InitRaquettes(Jeu& j, int l){
    j.J1.l=j.J2.l=l;

    j.J1.x1=j.J1.x2= 10; //espace entre raquette et bordure
    j.J1.y1= DIMY/2+j.J1.l; //raquette centrée, dc y1,y2 = DIMY/2 +/- longueur de raquette
    j.J1.y2= DIMY/2-j.J1.l;

    j.J2.x1=j.J2.x2=DIMX-10;
    j.J2.y1=DIMY/2+j.J2.l;
    j.J2.y2=DIMY/2-j.J2.l;
}
void BalleAddF (Balle& b, Vect2D A) {
    b.a = b.a + A; //où A est l'acceleration initiale (à voir faire le service)
}
void CollisionBalleBordures (Jeu& j){ //procédure pour la balle mais obligé d'utiliser la struct Jeu
    float frict ;
    frict = 0.98f;
    switch (j.difficulty){
        case 1: frict = 0.98f;break;
        case 2: frict = 1.0f;break;
        case 3: frict = 1.04f;break;
        case 4: frict =1.0f;break;
    }

    if(j.b.p.y<0){
        j.b.p.y=-j.b.p.y;
        j.b.v.y=-j.b.v.y;
        j.b.v.y=j.b.v.y*frict;
    }
    if(j.b.p.y>DIMY && j.difficulty == 4 ){ //pas besoin de plafond tant qu'on est pas sur la lune
        j.b.p.y=DIMY-(j.b.p.y-DIMY);
        j.b.v.y=-j.b.v.y;
        j.b.v.y=j.b.v.y*frict;
    }
}
void CoupsSpeciaux (Jeu &j){

    if (j.J1Turn == true){
        //AMORTI - Pour ralentir la balle lorsqu'elle touche la raquette + Alt appuyé
        if (isKeyPressed(SDLK_LALT) && j.b.p.x >= j.J1.x1-20 && j.J1.jauge_short < 75){
            switch (j.difficulty){
                case 1: j.b.a = make_vect2d(-2,0); j.J1.jauge_short++; break;
                case 2: j.b.a = make_vect2d(-2.5,0); j.J1.jauge_short++; break;
                case 3: j.b.a = make_vect2d(-3,0); j.J1.jauge_short++; break;
            }
        }
        //BALLE RAPIDE - Pour accelerer la balle lorsqu'elle touche la raquette + Ctrl appuyé
        else if (isKeyPressed(SDLK_LCTRL) && j.b.p.x >= j.J1.x1-20 && j.J1.jauge_smash < 75){
            switch (j.difficulty){
                case 1: j.b.a = make_vect2d(5,-1);j.J1.jauge_smash++; break;
                case 2: j.b.a = make_vect2d(7.5,-1);j.J1.jauge_smash++; break;
                case 3: j.b.a = make_vect2d(10,-1);j.J1.jauge_smash++; break;
            }
        }
        //LOB - Trajectoire parabollique, contre la vollée - Lorsque la balle touche la raquette + Espace appuyé pour J1
        else if (isKeyPressed(SDLK_SPACE) && j.b.p.x >= j.J1.x1-20 && j.J1.jauge_lob < 200){
            switch (j.difficulty){
                case 1: j.b.a = make_vect2d(2,5); j.J1.jauge_lob++; break;
                case 2: j.b.a = make_vect2d(2.5,5); j.J1.jauge_lob++; break;
                case 3: j.b.a = make_vect2d(3,5); j.J1.jauge_lob++; break;
            }
        }
        else j.b.a = make_vect2d(0,0);
    }
    else if (j.J2Turn == true){
        //AMORTI - Pour ralentir la balle lorsqu'elle touche la raquette + Alt appuyé
        if (isKeyPressed(SDLK_RALT) && j.b.p.x <= j.J2.x1-20 && j.J2.jauge_short < 75){
            switch (j.difficulty){
                case 1: j.b.a = make_vect2d(2,0);j.J2.jauge_short++; break;
                case 2: j.b.a = make_vect2d(2.5,0); j.J2.jauge_short++; break;
                case 3: j.b.a = make_vect2d(3,0); j.J2.jauge_short++; break;
            }
        }
        //BALLE RAPIDE - Pour accelerer la balle lorsqu'elle touche la raquette + Ctrl appuyé
        else if (isKeyPressed(SDLK_RCTRL) && j.b.p.x <= j.J2.x1-20 && j.J2.jauge_smash < 75){
            switch (j.difficulty){
                case 1: j.b.a = make_vect2d(-5,-1); j.J2.jauge_smash++; break;
                case 2: j.b.a = make_vect2d(-7.5,-1); j.J2.jauge_smash++; break;
                case 3: j.b.a = make_vect2d(-10,-1); j.J2.jauge_smash++; break;
            }
        }
        //LOB - Trajectoire parabollique, contre la vollée - Lorsque la balle touche la raquette + Entrée appuyé pour J2
        else if (isKeyPressed(SDLK_RETURN) && j.b.p.x <= j.J2.x1-20 && j.J2.jauge_lob < 200){
            switch (j.difficulty){
                case 1: j.b.a = make_vect2d(-2,5); j.J2.jauge_lob++; break;
                case 2: j.b.a = make_vect2d(-2.5,5); j.J2.jauge_lob++; break;
                case 3: j.b.a = make_vect2d(-3,5); j.J2.jauge_lob++; break;
            }
        }
        else j.b.a = make_vect2d(0,0);
    }


}
void MonterVollee (Jeu &j){
    if (isKeyPressed(SDLK_LSHIFT)){
        j.J1.x1=j.J1.x2=DIMX/2-20;
    }
    else j.J1.x1=j.J1.x2=10;
    if (isKeyPressed(SDLK_RSHIFT)){
        j.J2.x1=j.J2.x2=DIMX/2+20;
    }
    else j.J2.x1=j.J2.x2=DIMX-10;

}
void CollisionRaquettes (Jeu& j) {
    if (j.J2Turn == true ){ // J2 a tapé la balle != c'est à J2 de taper, ici c'est à J1 de jouer
        if (j.b.p.x <= j.J1.x1+8 && j.b.p.y<=j.J1.y1 && j.b.p.y>=j.J1.y2) { //18 = 10 (espace raquette/bordure) + 8 (rayon balle)
            j.b.p.x=j.b.p.x;
            j.b.v.x=-j.b.v.x;
            j.b.v.y+=2;
            j.b.a.x=-j.b.a.x;
            j.J2Turn=false; j.J1Turn=true;
            j.nb_echanges+=1;
            if (j.nb_echanges > j.meilleur_nb_echanges)
                j.meilleur_nb_echanges = j.nb_echanges;
        }
    }
    else {
        if (j.b.p.x >= j.J2.x1-8 && j.b.p.y<=j.J2.y1 && j.b.p.y>=j.J2.y2) {
            j.b.p.x=j.b.p.x;
            j.b.v.x=-j.b.v.x;
            j.b.v.y+=2;
            j.b.a.x=-j.b.a.x;
            j.J1Turn=false; j.J2Turn=true;
            j.nb_echanges+=1;
            if (j.nb_echanges > j.meilleur_nb_echanges)
                j.meilleur_nb_echanges = j.nb_echanges;
        }
    }
}
void WinPoint(Jeu& j){
    //Point si J1 ne rattrape pas la balle
    if(j.b.p.x<0){
        //perdu Joueur1
        addScoreJ2(j);
        j.endPoint=true;
        j.J2.win_last_point = true;
        j.J1.win_last_point = false;
    }
    //Point si J2 ne rattrape pas la balle
    if(j.b.p.x>DIMX){
        //perdu Joueur2
        addScoreJ1(j);
        j.endPoint=true;
        j.J2.win_last_point = false;
        j.J1.win_last_point = true;
    }
    //Point si J1 ou J2 met la balle dans le filet
    if ( (j.b.p.x >= j.f.x1-8 && j.b.p.x<=j.f.x1 )  &&  j.b.p.y <= j.f.y2 ) { //on vérifie si x de balle = x du filet ET si la hauteur de la balle est en dessous du y le + haut du filet
        if (j.J1Turn == true){
            if(j.difficulty == 3){ //le filet ne comptera pas pour un point à plus basse difficulté
                addScoreJ2(j);
            }
            j.endPoint=true;
            j.J2.win_last_point = true;
            j.J1.win_last_point = false;
        }
        else {
            if(j.difficulty == 3){
                addScoreJ1(j);
            }

            j.endPoint=true;
            j.endPoint=true;
            j.J2.win_last_point = false;
            j.J1.win_last_point = true;
        }

    }
}
void mvtRaquettes(Joueur_1& J1, Joueur_2& J2){
    if(isKeyPressed(SDLK_z) && J1.y1 < DIMY){
        J1.y1=J1.y1+2.0;
        J1.y2=J1.y2+2.0;
    }
    if(isKeyPressed(SDLK_s) && J1.y2 > 0){
        J1.y1=J1.y1-2.0;
        J1.y2=J1.y2-2.0;
    }
    if(isKeyPressed(SDLK_UP) && J2.y1 < DIMY){
        J2.y1=J2.y1+2.0;
        J2.y2=J2.y2+2.0;
    }
    if(isKeyPressed(SDLK_DOWN) && J2.y2 > 0){
        J2.y1=J2.y1-2.0;
        J2.y2=J2.y2-2.0;
    }
}
void updateBalle (Jeu& j, int select){
    if (select == 1){
        if(j.nb_echanges == 0){
            switch (j.difficulty){
                case 1: j.b.a = make_vect2d(2,0); break;
                case 2: j.b.a = make_vect2d(2.5,0); break;
                case 3: j.b.a = make_vect2d(3,0); break;
                case 4: j.b.a = make_vect2d(1.5,0); break;
            }
        }
        else CoupsSpeciaux(j);
    }
    if (select == 2){
        if(j.nb_echanges == 0){
            switch (j.difficulty){
                case 1: j.b.a = make_vect2d(-2,0); break;
                case 2: j.b.a = make_vect2d(-2.5,0); break;
                case 3: j.b.a = make_vect2d(-3,0); break;
                case 4: j.b.a = make_vect2d(-1.5,0); break;
            }
        }
        else CoupsSpeciaux(j);
    }
    //pas obligé de faire un switch mais je l'ai fait pour tous les changements de difficulté
    switch (j.difficulty){
        case 1: BalleAddF(j.b, make_vect2d(0,-j.b.m*9.81));break;
        case 3: BalleAddF(j.b, make_vect2d(0,-j.b.m*9.81));break;
        case 2: BalleAddF(j.b, make_vect2d(0,-j.b.m*9.81));break;
        case 4: BalleAddF(j.b, make_vect2d(0,-j.b.m*1.62));break;
    }
    j.b.v = j.b.v + dt * (1/j.b.m) * j.b.a;
    j.b.p = j.b.p + j.b.v * dt;
    CollisionBalleBordures(j);





}
void init(Jeu& j){
    j.endPoint=false;
    j.endGame=false;
    j.nb_echanges=j.meilleur_nb_echanges=0;
    j.J1.jauge_short = j.J1.jauge_smash = j.J2.jauge_short = j.J2.jauge_smash =  j.J1.jauge_lob = j.J2.jauge_lob = 0;
    initBalle(j);
    InitRaquettes(j,50);
    switch(j.difficulty){
        case 1: InitRaquettes(j,50);break;
        case 2: InitRaquettes(j,45);break;
        case 3: InitRaquettes(j,40);break;
        case 4: InitRaquettes(j,80);break;
    }
    j.f.x1=j.f.x2=DIMX/2;
    j.f.y1=0;
    j.f.y2=DIMY/4-20;

    j.ssc1=j.ssc2=j.jsc1=j.jsc2=0;
    //premier coup de la partie (lancement du prog)
    j.J1Turn=true;
    j.J2Turn=false;
}
void draw (Jeu j){
    int i;
    color(240,235,87);
    circleFill(j.b.p.x, j.b.p.y, 8);
    color(50,50,50);
    for (i=-2; i<=2; i++) line(j.f.x1+i ,j.f.y1 ,j.f.x2+i ,j.f.y2);
    color(240,185,185);
    for (i=-2; i<=2; i++) line(j.J1.x1+i ,j.J1.y1, j.J1.x2 +i,j.J1.y2);
    print(20, DIMY-30,j.jsc1);
    print(40,DIMY-30,j.ssc1);
    color(185,185,240);
    for (i=-2; i<=2; i++) line(j.J2.x1+i ,j.J2.y1, j.J2.x2 +i,j.J2.y2);
    print(DIMX-50,DIMY-30,j.jsc2);
    print(DIMX-30,DIMY-30,j.ssc2);
    color(0,0,0);
    print(DIMX/2,DIMY-30,j.nb_echanges);
    print(DIMX/2-20,DIMY-50,"BEST =");
    print(DIMX/2+20,DIMY-50,j.meilleur_nb_echanges);
    //commandes J1
    print(20, DIMY-45,"VOLLEY - RSHIFT");
    print(20, DIMY-60,"SMASH - RCTRL");
    print(20, DIMY-75,"SHORT - RALT");
    print(20, DIMY-90,"LOB - ESPACE");
    //commandes J2
    print(DIMX-100, DIMY-45,"VOLLEY - LSHIFT");
    print(DIMX-100, DIMY-60,"SMASH - LCTRL");
    print(DIMX-100, DIMY-75,"SHORT - LALT");
    print(DIMX-100, DIMY-90,"LOB - ENTER");
}
void drawEnd(Jeu j){
    color(80,80,80);
    rectangleFill(0,0,DIMX,DIMX);
    if (j.J1.win_last_point==true){
        color(240,185,185);
        print(DIMX/2-50,DIMY/2, "J1 A GAGNE LA PARTIE");
        print(DIMX/2-76, DIMY/2-30,"APPUYEZ SUR Q POUR QUITTER");
    }
    else{
        color(185,185,240);
        print(DIMX/2-50,DIMY/2, "J2 A GAGNE LA PARTIE");
        print(DIMX/2-76, DIMY/2-30,"APPUYEZ SUR Q POUR QUITTER");
    }

}
void update (Jeu& j){
    if (j.J1Turn==true){
        updateBalle(j,1);
    }
    else updateBalle(j,2);
    mvtRaquettes(j.J1,j.J2);
    MonterVollee(j);
    CollisionRaquettes(j);
    WinPoint(j);
    //printNbMoves(j);
    //cout<<j.ssc1; //debug

}
void resetGame(Jeu& j){
    j.endPoint=false;
    j.nb_echanges=0;
    j.J1.jauge_short = j.J1.jauge_smash = j.J2.jauge_short = j.J2.jauge_smash = 0;
    initBalle(j);
    if (j.difficulty <= 3){
        j.f.x1=j.f.x2=DIMX/2;
        j.f.y1=0;
        j.f.y2=DIMY/4-20;
    }
    else j.f.x1=j.f.x2=j.f.y1=j.f.y2=0;

    //service du coté de celui qui a perdu
    if (j.J2.win_last_point == true) {
        j.J1Turn=false;
        j.J2Turn=true;
    }
    else {
        j.J1Turn=true;
        j.J2Turn=false;
    }

}

int main(int , char** ){
    Jeu j;
    Menu m1,m2;
    bool stop=false;
    winInit("Tennis C", DIMX, DIMY);
    setKeyRepeatMode(true);
    winSetPosition( DIMX,DIMY, -1, -1,false);
    init(j);
    menu_add(m1, "Clay");
    menu_add(m1, "Grass");
    menu_add(m1, "Asphalt");
    menu_add(m1, "Moon");
	while( !stop ) {
	    //(223,121,64) = clay   ; (136,209,98) = grass ;   (101,128,217) = asphalt
        winClear();
        switch(menu_select(m1)){
            case 0: j.difficulty = 1; break;
            case 1: j.difficulty = 2; break;
            case 2: j.difficulty = 3; break;
            case 3: j.difficulty = 4; break;
        }
        switch (j.difficulty){
            case 1: backgroundColor(223,121,64) ;break;
            case 2: backgroundColor(136,209,98) ;break;
            case 3: backgroundColor(101,128,217) ;break;
            case 4: backgroundColor(120, 120, 120);break;
        }
        draw(j);
        update(j);
        if (j.endPoint == true){
            if (j.endGame == false){
                resetGame(j);
                winClear();
                draw(j); //ces deux lignes permettent de replacer tous les éléments avant de recommencer un point
                pressSpace();
            }
            else{
                drawEnd(j);
                pressSpace();
            }
        }
        if (j.endGame == false) menu_draw(m1, 5, 5, 60, 90); // le menu ne sera pas affiché lorsqu'un joueur a gagné
        stop = winDisplay();
    }
    winQuit();
	return 0;
}
