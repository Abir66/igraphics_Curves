/*
    Abir Muhtasim
    1905066
    sec - B (B1)
*/

# include "iGraphics.h"
#include<stdio.h>
#include<math.h>
#include<string.h>
char allCommands[40][100] = {
    "p/p   : pause",
    "r/R   : resume",
    "+/-   : increase/decrease tracer speed",
    "a     : increase amplitude",
    "A     : decrease amplitude",
    "f     : increase frequency",
    "F     : decrease frequency",
    "s     : show/hide all curves",
    "b     : show/hide all tracers",
    "1-9   : show/hide ith tracer",
    "space : start moving curves",
    "<>    : move curves left/right",
    "upper arrow : zoom in",
    "down arrow  : zoom out",
    "left arrow  : move screen left",
    "right arrow : move screen right",
    "x           : add/remove random Phase",
    "n           : add new curve",
    "backspace   : delete last",
    "u           : Undo Curve delete",
    "Enter       : show/hide commands",
    " ",
    "right click on a curve to select",
    "left click and drag to adjust",
    "right click again to deselect",
    " ",
    "Esc : Quit programme",
};

int totalCommands = 27;

//1280x720 or 1366x768
//screen variables.
double totalWidth = 1280;
double screenWidth = totalWidth;
double screenHeight = 720;
double middleHeight = screenHeight/2;
double smoothness = 1;   //less is smoother
double pi = acos(-1);
double myNameWidth = totalWidth*190/1280;
double commandsWidth = totalWidth*320/1280; // commands width

//curve roperties
double frequency[50] = {1,0.6,2};
double amplitude[50] = {100,50,150};
int curveType[50] = {1,0,1};
double initialPhase[50] = {0,0,0};
int showBall[50] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int showResult = 1;
int totalCurves = 3;
int maxTotalCurves = totalCurves; //for deleting and undoing

// colors
int red[50] = {255,0,255,0,255,153,102,255};
int green[50] = {255,0,0,255,51,0,255,153};
int blue[50] = {0,255,0,0,153,153,255,51};
int colors = 8;


//multiplication variables
double ampMulZoom = 1;   //zoom
double frqMulZoom = 1;   //zoom
double ampMulUser = 1;   //increase or decrease amplitude
double frqMulUser = 1;   //increase or decrease frequency
double MulScale = 1.025;   //increase or decrease amplitude or frequency
double zoomScale = 1.05;  //zoom

double currentPhase = 0; //for velocity and manual movements, in degree
int dir = 1; // ball direction

//showing variables
int showPhase = 0;
int showCurve = 1;
int showAllBalls = 1;
int showVelocity = 0;
int showCommands = 0;
int showNewButSelected = 0;
int showBackground = 1;


double moveX = 0;

//ball properties
double ballX = 0;
double ballY = 0;
double ballRadius = 5;
double balldelX=5;
double ballSpeed = 0.75;

//dragging curves
int curveCaught = 0;
int caughtidx = -1;
double caughtInitialX = 0;
double caughtInitialY = 0;


//new Curve properties
int addingNewCurve = 0;
int newCurvetype = 1;
double newCurveAmp = 100;
double newCurvefrq = 1;
double newCurvePhase = 0;

//curve adder menu properties
double button1X = 0;
double button2X = 0;
double buttonLength = 0;
double buttonHeight = 0;
double buttonDistance = 0;
double buttonY = 0;
double adderButtonX = 0;
double adderButtonY = 0;
double cancelButtonX = 0;
double adderLength = 0;
double adderHeight = 0;
double menuHeight = 0;



void checkCaught(int mx, int my, int id){ // to check if user clicked on a curve


    //if clicked on a curve, saves the x,y in caughtInitialX and caughtInitialY

    if(id==-1){ //if no curves is selected
        double caughtY = 0,addedPhase = 0;
        int i;


        addedPhase = currentPhase * pi /180;
        for(i=0;i<totalCurves;i++){

        //to disable movement remove moveX
            double c = (mx+moveX)*pi/180;

            double a = ampMulUser*ampMulZoom*amplitude[i];
            double f = frequency[i]*frqMulZoom*frqMulUser;
            double theta = initialPhase[i] + addedPhase;

            if(curveType[i])
                caughtY = middleHeight + a*sin(c*f + theta);
            else
                caughtY = middleHeight + a*cos(c*f + theta);

            //printf("%d %d %d %lf %lf %lf %lf %lf\n",i,mx,my,c,a,f,theta,caughtY);
            if(abs(caughtY - my) <=2) {

                curveCaught = 1;
                caughtidx = i;
                caughtInitialX = mx;
                caughtInitialY = caughtY;

                break;
            }
        }
    }


    else{ // if a curve is selected
        double caughtY = 0,addedPhase = 0;
        int i;


        addedPhase = currentPhase * pi /180;
        i = id;

        //to disable movement remove moveX
            double c = (mx+moveX)*pi/180;

            double a = ampMulUser*ampMulZoom*amplitude[i];
            double f = frequency[i]*frqMulZoom*frqMulUser;
            double theta = initialPhase[i] + addedPhase;

            if(curveType[i])
                caughtY = middleHeight + a*sin(c*f + theta);
            else
                caughtY = middleHeight + a*cos(c*f + theta);

            //printf("%d %d %d %lf %lf %lf %lf %lf\n",i,mx,my,c,a,f,theta,caughtY);
            if(abs(caughtY - my) <=1) {

                curveCaught = 1;
                caughtidx = i;
                caughtInitialX = mx;
                caughtInitialY = caughtY;

            }

    }

}


void changeCaught(int mx, int my){ //to edit the selected curve

    int idx = caughtidx;
    double oldd=0,newd=0,x = caughtInitialX;
    oldd = middleHeight - caughtInitialY;
    newd = middleHeight - my;


    // not sure if the first two will work
    if(amplitude[idx] == 0) {
        double theta = mx * pi / 180;
        if(curveType[idx]) amplitude[idx] = my/sin(theta);
        else amplitude[idx] = my/sin(theta);
    }

    else if(oldd==0){

        double curvex,addedPhase=0,y;

        addedPhase = currentPhase * pi/180;
        caughtInitialX--;
        curvex = (caughtInitialX)*pi/180;

        double a = ampMulUser*ampMulZoom*amplitude[idx];
        double f = frequency[idx]*frqMulZoom*frqMulUser;
        double theta = initialPhase[idx] + addedPhase;

        if(curveType[idx])
            y = middleHeight + a*sin(curvex*f + theta);
        else
            y = middleHeight + a*cos(curvex*f + theta);

        caughtInitialY= y;
        oldd = middleHeight - caughtInitialY;
        amplitude[idx] = amplitude[idx] * newd / oldd;
        caughtInitialX = mx;
        caughtInitialY = my;
    }

    else{
        double oldamp = amplitude[idx];
        amplitude[idx] = amplitude[idx] * newd / oldd;
        if(amplitude[idx]!=0){
                double oldfrq = frequency[idx];
                frequency[idx] = frequency[idx] * amplitude[idx] / oldamp; //squeez
                if(frequency[idx]>1.5) frequency[idx] = 1.5;
                double newfrq = frequency[idx];

                initialPhase[idx] += (oldfrq * x - newfrq*mx)*pi/180; // f1x1 = f2x2 + phase difference
                if(initialPhase[idx] < - 2*pi) initialPhase[idx] += 2*pi;
                if(initialPhase[idx] > 2* pi ) initialPhase[idx] -= 2*pi;


        }
        caughtInitialX = mx;
        caughtInitialY = my;
    }



}


void adderMenu(){ //for displaying the editing menu of the new curve. used in editNewCurve

    iSetColor(255,255,255);
    double x = totalWidth - 250+25;
    iText(x,150,"Amplitude : ");
    iText(x,120,"Frequency : ");
    iText(x,90,"Phase     : ");
    iText(x,60,"Type:     : ");


    //left buttons
    iSetColor(255,153,51);
    iFilledRectangle(x+100,60-5,50,20);
    iFilledRectangle(x+100,90-5,50,20);
    iFilledRectangle(x+100,120-5,50,20);
    iFilledRectangle(x+100,150-5,50,20);


    //right buttons
    iSetColor(51, 153,255);
    iFilledRectangle(x+100+60,60-5,50,20);
    iFilledRectangle(x+100+60,90-5,50,20);
    iFilledRectangle(x+100+60,120-5,50,20);
    iFilledRectangle(x+100+60,150-5,50,20);

    //button texts
    //iSetColor(255,255,255);
    iSetColor(0,0,0);
    iText(x+100+13,60+2, "sin");
    iText(x+100+20,90, "-");
    iText(x+100+20,120, "-");
    iText(x+100+20,150, "-");

    iText(x+100+60+13,60+2, "cos");
    iText(x+100+60+20,90, "+");
    iText(x+100+60+20,120, "+");
    iText(x+100+60+20,150, "+");

    //add and cancel
    iSetColor(255,255,255);
    iRectangle(x,40-15,100,25);
    iRectangle(x+110,40-15,100,25);

    iText(x+37,40-15+10, "ADD");
    iText(x+110+27,40-15+10, "CANCEL");


    //button variables to check if its pressed
    button1X = x+100;
    button2X = x+100+60;
    buttonLength = 50;
    buttonHeight = 20;
    buttonDistance = 30;
    buttonY = 60-5;

    adderButtonX = x;
    adderButtonY = 40-15;
    cancelButtonX = x+110;
    adderLength = 100;
    adderHeight = 25;
    menuHeight = 150-5+20;

}


void editNewCurve(int mx, int my){ //for editing the new temporary curve from the menu

    double x = mx, y=my;
    double d = buttonY;

    //cancel button
    if(x>= cancelButtonX && x<= cancelButtonX + adderLength && y>=adderButtonY && y<= adderButtonY+adderHeight){
        addingNewCurve = 0;
    }

    //add button
    if(x>= adderButtonX && x<= adderButtonX + adderLength && y>=adderButtonY && y<= adderButtonY+adderHeight){
        addingNewCurve = 0;
        totalCurves++;
        maxTotalCurves = totalCurves;

        int i = totalCurves-1;
        amplitude[i] = newCurveAmp;
        frequency[i] = newCurvefrq;
        curveType[i] = newCurvetype;
        initialPhase[i] = newCurvePhase*pi/180;

        if(initialPhase[i]<-2*pi) initialPhase[i] += 2*pi;
        if(initialPhase[i]>2*pi) initialPhase[i] -= 2*pi;
        showBall[i] = 1;


        newCurvetype = 1;
        newCurveAmp = 100;
        newCurvefrq = 1;
        newCurvePhase = 0;

    }


    //left buttons
    else if(x>= button1X && x<= button1X+buttonLength){

        if(y>=d && y<= d+buttonHeight){
            newCurvetype = 1;
        }
        d += buttonDistance;
        if(y>=d && y<= d+buttonHeight){

            newCurvePhase  = newCurvePhase - 10;
            if(newCurvePhase<-360) newCurvePhase += 360;
        }
        d += buttonDistance;
        if(y>=d && y<= d+buttonHeight){
            if(newCurvefrq - 0.2 >= 0){
                newCurvefrq -= 0.1;
            }
        }
        d += buttonDistance;
        if(y>=d && y<= d+buttonHeight){

            if(newCurveAmp - 10 >= 0){
                newCurveAmp -= 10;
            }
        }
    }

    //right buttons
    else if(x>= button2X && x<= button2X+buttonLength){

        if(y>=d && y<= d+buttonHeight){
            newCurvetype = 0;
        }
        d += buttonDistance;
        if(y>=d && y<= d+buttonHeight){

            newCurvePhase  = newCurvePhase + 10;
            if(newCurvePhase>360) newCurvePhase -= 360;
        }
        d += buttonDistance;
        if(y>=d && y<= d+buttonHeight){
            newCurvefrq += 0.1;
        }

        d += buttonDistance;
        if(y>=d && y<= d+buttonHeight){
            newCurveAmp += 10;

        }
    }


}


void curveAdder(){ //for showing the new temporary curve


    //same as curveDraw / ball
    iSetColor(0,255,0);
    double x,tempX,y,curvex,resultY,addedPhase = 0;
    int i;
    addedPhase = currentPhase * pi/180;
    for(x=0;x<=screenWidth;x=x+smoothness){

        resultY = 0;
        ///to disable movement remove moveX
        curvex = (x+moveX)*pi/180;
        for(i=0;i<totalCurves;i++){


            double a = ampMulUser*ampMulZoom*newCurveAmp;
            double f = newCurvefrq*frqMulZoom*frqMulUser;
            double theta = newCurvePhase*pi/180 + addedPhase;

            if(newCurvetype)
                y = middleHeight + a*sin(curvex*f + theta);
            else
                y = middleHeight + a*cos(curvex*f + theta);
            iPoint(x,y);

        }

    }

}


void ballChange(){ // for Calculating the x-coordinate of the balls/tracers


    ballX += dir*balldelX*ballSpeed;
    if(ballX>screenWidth) {
        dir*=-1;
        ballX = screenWidth; //so that the balls don't hide behind the commands menu
    }
    if(ballX<0) dir*=-1;
}


void ball(){  // for displaying the balls/tracers


    // almost same as curveDraw. just that i don't need to iterate for all x's

    double resultY = 0,addedPhase = 0,c;
    int i;

    //to disable movement remove moveX
    c = (ballX+moveX)*pi/180; //x coordinate in radian
    addedPhase = currentPhase * pi /180;

    for(i=0;i<totalCurves;i++){

        iSetColor(red[i%colors],green[i%colors],blue[i%colors]);


        double a = ampMulUser*ampMulZoom*amplitude[i];
        double f = frequency[i]*frqMulZoom*frqMulUser;
        double theta = initialPhase[i] + addedPhase;

        if(curveType[i])
            ballY = a*sin(c*f + theta);
        else
            ballY = a*cos(c*f + theta);

        resultY+= ballY;
        ballY += middleHeight;
        if(showBall[i]) iFilledCircle(ballX,ballY,ballRadius); //show ball
    }



    if(showResult){
        iSetColor(255,255,255);
        resultY += middleHeight;
        iFilledCircle(ballX,resultY,ballRadius);
        iCircle(ballX, resultY, ballRadius+3); //ring around the resultant circle
    }

}

void velocity(){ //for moving curves

    currentPhase -= 5;
    if(currentPhase>360) currentPhase-= 360;
    else if(currentPhase<-360) currentPhase += 360;
}


void curveDraw(){ // drawing the curves from the array


    double x,tempX,y,curvex,resultY,addedPhase = 0;
    int i;
    addedPhase = currentPhase * pi/180;
    for(x=0;x<=screenWidth;x=x+smoothness){

        resultY = 0;
        //to disable movement remove moveX , left and right
        curvex = (x+moveX)*pi/180; // converting to radian

        for(i=0;i<totalCurves;i++){


            double a = ampMulUser*ampMulZoom*amplitude[i]; //amplitude, ampMulUser is for just amp, ampMulZoom is for zoom
            double f = frqMulUser*frqMulZoom*frequency[i]; //frequency, frqMulUser is for just frq, frqMulZoom is for zoom
            double theta = initialPhase[i] + addedPhase;   // total phase.

            if(curveType[i])
                y = a*sin(curvex*f + theta);
            else
                y = a*cos(curvex*f + theta);

            resultY += y;

            if(caughtidx==i) iSetColor(255,30,30);
            else iSetColor(255,255,255);
            y += middleHeight;
            iPoint(x,y);


        }


        iSetColor(255,255,255); //51,153,255
        resultY += middleHeight;
        iPoint(x,resultY);


    }
}


void change(){ // called by iSetTimer

    // for moving curves and velocity
    ballChange();
    if(showVelocity) velocity();

}

/*
	function iDraw() is called again and again by the system.
*/
void iDraw()
{
    //place your drawing codes here
    iClear();

    //background
    iSetColor(22,22,22);
    iFilledRectangle(0,0,totalWidth,screenHeight);


    // draw the middle line
    iSetColor(255,255,255);
    iLine(0,middleHeight,screenWidth,middleHeight);

    //show curves and balls
    if(showCurve) curveDraw();
    if(showAllBalls) ball();

    //command and name message
    iSetColor(255,255,255);
    if(showCommands) iText(10,10,"press Enter to hide commands");
    else iText(10,10,"press Enter to view commands");
    iText(totalWidth-myNameWidth,10,"Abir Muhtasim - 1905066");


    //curve edit message
    if(curveCaught){
        iSetColor(255,30,30);
        iText(10,50,"Curve selected. Left Click on a point and drag to adjust.");
        iText(10,30,"Right Click to deselect");
    }


    //adding new curve
    if(addingNewCurve==1){
        curveAdder();
        adderMenu();
    }


    //showing commands
    if(showCommands==1){
        iSetColor(255,255,255);
        int leftpos = totalWidth - commandsWidth +5;
        double d = 20*screenHeight/750;
        double uppos = screenHeight - d;
        int i;
        for(i=0;i<totalCommands;i++){
            iText(leftpos,uppos,allCommands[i]);
            uppos -= d;
        }

    }

    //done
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{

    if(curveCaught==1) {
        if(addingNewCurve==1){
            if(!(mx>= adderButtonX && my<= menuHeight)){
                checkCaught(mx,my,caughtidx);
                changeCaught(mx,my);
            }
        }

        else{
            checkCaught(mx,my,caughtidx);
            changeCaught(mx,my);
        }
    }


    //printf("x = %d, y= %d\n",mx,my);
    //place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //place your codes here
        //printf("x = %d, y= %d\n",mx,my);
        if(addingNewCurve==1){

            editNewCurve(mx,my);
        }


        if(curveCaught==1) {   //to check if I pressing on another part of the selected curve
            if(addingNewCurve==1){
                if(!(mx>= adderButtonX && my<= menuHeight)){
                checkCaught(mx,my,caughtidx);
                }
            }

            else{
                checkCaught(mx,my,caughtidx);

            }
        }



    }

    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        //place your codes here
        if(curveCaught==0){
            checkCaught(mx,my,-1);
        }

        else if(curveCaught==1){
            curveCaught = 0;
            caughtidx = -1;
            caughtInitialX = 0;
            caughtInitialY = 0;
        }

    }
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if(key=='q' || key==27){
        exit(0);
    }
    else if(key == 'p' || key == 'P'){ //pause
        iPauseTimer(0);
    }
    else if(key == 'r' || key == 'R'){ //resume
        iResumeTimer(0);
    }
    else if(key=='+'){ //increase ball speed
        ballSpeed += 0.25;
    }
    else if(key == '-'){ //decrease ball speed
        ballSpeed -= 0.25;
        if(ballSpeed<0) ballSpeed = 0;
    }
    else if(key=='a'){ //increase amplitude
        ampMulUser *= MulScale;
    }
    else if(key=='A'){ //decrease amplitude
        ampMulUser /= MulScale;
    }
    else if(key=='f'){ //increase frequency
        frqMulUser *= MulScale;
    }
    else if(key=='F'){ //decrease frequency
        frqMulUser /= MulScale;
    }
    else if(key == 's'){ //show all curves
        if(showCurve) showCurve = 0;
        else showCurve = 1;
    }
    else if(key>='1' && key<='9'){ //show/hide a curve
        int idx = key-'0'-1;
        if(idx==totalCurves) showResult = (showResult+1)%2;
        else if(idx<totalCurves) showBall[idx] = (showBall[idx]+1) % 2;
    }
    // extras
    else if(key == ' '){ //start moving curves
        if(showVelocity) showVelocity = 0;
        else showVelocity = 1;
    }
    else if(key == 'b'){ //show all balls
        if(showAllBalls==1) showAllBalls = 0;
        else showAllBalls = 1;
    }
    else if(key == '\b'){ //delete last curve
       if(totalCurves>=1) totalCurves--;
    }
    else if(key=='u' || key == 'U'){ //undo delete
        if(maxTotalCurves > totalCurves) totalCurves++;
    }
    else if(key=='>'){ //move curves right
        currentPhase -= 5;
        if(currentPhase>360) currentPhase-= 360;
        else if(currentPhase<-360) currentPhase += 360;;
    }
    else if(key=='<'){ //move curves right
        currentPhase += 5;
        if(currentPhase>360) currentPhase-= 360;
        else if(currentPhase<-360) currentPhase += 360;;
    }

    else if(key == 'x'){ //random phase
        int i;
        if(showPhase==0){
            showPhase = 1;
            for(i=0;i<totalCurves;i++) initialPhase[i] = (rand() % 180) * pi/180;
        }

        else{
            showPhase = 0;
            for(i=0;i<totalCurves;i++) initialPhase[i] = 0;
        }

    }

    else if(key=='\r'){ //show/hide commands
        if(showCommands==0){
            showCommands = 1;
            screenWidth = totalWidth - commandsWidth;
        }
        else{
            showCommands = 0;
            screenWidth = totalWidth;
        }
    }
    else if(key=='n'|| key=='N'){ //add a new curve


        addingNewCurve = 1;

    }
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{


    //move Whole Thing Left/Right
    if(key==GLUT_KEY_RIGHT){
        moveX += 5;

    }
    else if(key==GLUT_KEY_LEFT){
        moveX -= 5;

    }
    //zoom
    else if(key==GLUT_KEY_UP){

        ampMulZoom *= zoomScale;
        frqMulZoom = 1/ampMulZoom;
    }
    else if(key==GLUT_KEY_DOWN){

        ampMulZoom /=zoomScale;
        frqMulZoom = 1/ampMulZoom;
    }
    //place your codes for other keys here
}

int main()
{
    //place your own initialization codes here.
    iSetTimer(25,change);
    iInitialize(screenWidth, screenHeight, "Curves 1905066");
    return 0;
}
