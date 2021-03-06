
/*
    Abir Muhtasim
    1905066
    sec - B (B1)
*/

# include "iGraphics.h"
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<windows.h>
char allCommands[40][100] = {
    "p           : pause",
    "r           : resume",
    "+           : increase tracer speed",
    "-           : decrease tracer speed",
    "a           : increase amplitude",
    "A           : decrease amplitude",
    "f           : increase frequency",
    "F           : decrease frequency",
    "s           : show/hide all curves",
    "b           : show/hide all tracers",
    "1-9         : show/hide ith tracer",
    "x           : add/remove random Phase",
    "space       : start moving curves",
    "<>          : move curves left/right",
    "up arrow    : zoom in",
    "down arrow  : zoom out",
    "left arrow  : move screen left",
    "right arrow : move screen right",
    "n           : add new curve",
    "backspace   : delete last curve",
    "u           : undo Curve delete",
    "d           : drawing on/off",
    " ",
    "right click on a curve to select",
    "left click and drag to adjust",
    "right click again to deselect",
    " ",
    "Esc : Quit program",
};

int totalCommands = 28;

//1280x720  don't change this
//screen variables.
double totalWidth = GetSystemMetrics(SM_CXSCREEN);
double screenWidth = totalWidth;
double screenHeight = GetSystemMetrics(SM_CYSCREEN);
double middleHeight = screenHeight/2;
double smoothness = 1;   //less is smoother
double pi = acos(-1);
double myNameWidth = 190;//totalWidth*190/1280;
double commandsWidth = 275 + 100;//totalWidth*275/1280;

//curve roperties
double frequency[50] = {0.8,0.6,1}; //1,.6,2
double amplitude[50] = {120,50,150}; //100,50,150
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


//for moving left and right
double moveX = 0;

//ball properties
double ballX = 0;
double ballY = 0;
double ballRadius = 10;
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
double buttonLength = 50;
double buttonHeight = 20;
double buttonDistance = 0;
double buttonY = 0;
double adderButtonX = 0;
double adderButtonY = 0;
double cancelButtonX = 0;
double adderLength = 0;
double adderHeight = 0;
double menuHeight = 0;


//drawing new curve
double drawX[10000] = {};
double drawY[10000] = {0};
int drawN = -1;
int drawMode = 0;
int showDraw = 0;
double drawHighY = 0;
double drawHighX = 0;
double drawLowY = 100000;
double drawLowX = 0;
int drawXid =-1;
int drawOne = 0;
int drawCurveIdx = -1;
double graph = 20;



void calculateDraw(){  //calculating drawn curve properties

    //as it is printing in curveDraw i have to set the amp phase and frq in reverse. for zooming and movement

    int i;
    int possible = 0;
    double zero = middleHeight,amp,frq,phase,x;

    if(drawLowY < zero && drawHighY > zero){

        //amp = (abs(drawHighY - zero) + abs(drawLowY-zero))/2 / (ampMulUser*ampMulZoom); //avarage
        amp = abs(drawHighY - zero)/ (ampMulUser*ampMulZoom); //high point
        x = drawHighX - drawLowX;

        if(abs(x)>30){


            frq = 180/x;
            if(frq<0) frq *= -1;
            if(frq>4) frq = 4;


            phase = -currentPhase * pi/180 + pi/2 - frq * (drawHighX + moveX) * pi/180;
            if(phase > 2*pi) phase -= 2*pi;
            if(phase < -2*pi) phase += 2*pi;

            frq = frq / (frqMulUser*frqMulZoom);
            totalCurves++;
            maxTotalCurves = totalCurves;

            //adding new curve to the main curve list
            //it will be called by curveDraw

            amplitude[totalCurves-1] = amp;
            frequency[totalCurves-1] = frq;
            initialPhase[totalCurves-1] = phase;
            curveType[totalCurves-1] = 1;
            drawOne = 1;
            drawCurveIdx = totalCurves - 1;
            //printf("%lf %lf %lf %lf\n",x,amp,frq,phase);
            //printf("%lf %lf %lf %lf\n\n",drawHighX,drawHighY,drawLowX,drawLowY);


            //Initailizing for using again
            drawHighX = 0;
            drawHighY = 0;
            drawLowY = 10000;
            drawLowX = 0;

        }


    }

    //hopefully it works now
}

void checkCaught(int mx, int my, int id){ // to check if user clicked on a curve


    //if clicked on a curve, saves the x,y in caughtInitialX and caughtInitialY

    //this was a terrible idea

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
    oldd = (middleHeight - caughtInitialY);
    newd = (middleHeight - my);


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
        double oldamp = amplitude[idx]/(ampMulUser*ampMulZoom);
        amplitude[idx] = amplitude[idx] * newd / oldd;
        if(amplitude[idx]>500) amplitude[idx] = 500;
        if(amplitude[idx]!=0){
                double oldfrq = frequency[idx];
                frequency[idx] = oldfrq * newd/ oldd; //squeez
                if(frequency[idx]>1.5) frequency[idx] = 1.5;
                double newfrq = frequency[idx];

                initialPhase[idx] += ((oldfrq*frqMulUser*frqMulZoom * (x+moveX) + - newfrq*frqMulUser*frqMulZoom*(mx+moveX))*pi/180 ); // f1x1 = f2x2 + phase difference
                if(initialPhase[idx] < - 2*pi) initialPhase[idx] += 2*pi;
                if(initialPhase[idx] > 2* pi ) initialPhase[idx] -= 2*pi;


        }
        caughtInitialX = mx;
        caughtInitialY = my;
    }



}


void adderMenu(){ //for displaying the editing menu of the new curve. used in editNewCurve




    //button names
    iSetColor(255,255,255);
    double x = totalWidth - 250+25 - 50;

    //button variables to check if its pressed
    buttonLength = 70;
    buttonHeight = 30;
    button1X = x+100;
    button2X = button1X+buttonLength +10;

    buttonDistance = 40;
    buttonY = 60-5 + 10;

    adderButtonX = x-30;
    adderButtonY = 40-25;
    cancelButtonX = adderButtonX+adderLength+20;
    adderLength = 130;
    adderHeight = 35;
    menuHeight = 150-5+20;


    int xx=75,dd = 42;

    iText(x-30,xx+dd*3,"Amplitude : ",GLUT_BITMAP_9_BY_15);
    iText(x-30,xx+dd*2,"Frequency : ",GLUT_BITMAP_9_BY_15);
    iText(x-30,xx+dd,"Phase     : ",GLUT_BITMAP_9_BY_15);
    iText(x-30,xx,"Type:     : ",GLUT_BITMAP_9_BY_15);


    //left buttons
    iSetColor(255,153,51);

    iFilledRectangle(button1X,buttonY,                buttonLength,buttonHeight);
    iFilledRectangle(button1X,buttonY + buttonDistance,buttonLength,buttonHeight);
    iFilledRectangle(button1X,buttonY + buttonDistance*2,buttonLength,buttonHeight);
    iFilledRectangle(button1X,buttonY + buttonDistance*3,buttonLength,buttonHeight);


    //right buttons
    iSetColor(51, 153,255);
    iFilledRectangle(button2X,buttonY,buttonLength,buttonHeight);
    iFilledRectangle(button2X,buttonY + buttonDistance,buttonLength,buttonHeight);
    iFilledRectangle(button2X,buttonY + buttonDistance*2,buttonLength,buttonHeight);
    iFilledRectangle(button2X,buttonY + buttonDistance*3,buttonLength,buttonHeight);
    //button texts
    //iSetColor(255,255,255);
    xx = x+100+30;
    int dx = 80, yy = 75,dy = 40;
    iSetColor(0,0,0);
    iText(xx-7,yy+2, "sin",GLUT_BITMAP_9_BY_15);
    iText(xx,yy+dy, "-",GLUT_BITMAP_9_BY_15);
    iText(xx,yy+dy*2, "-",GLUT_BITMAP_9_BY_15);
    iText(xx,yy+dy*3, "-",GLUT_BITMAP_9_BY_15);

    iText(xx+dx-7,yy+2, "cos",GLUT_BITMAP_9_BY_15);
    iText(xx+dx,yy+dy, "+",GLUT_BITMAP_9_BY_15);
    iText(xx+dx,yy+dy*2, "+",GLUT_BITMAP_9_BY_15);
    iText(xx+dx,yy+dy*3, "+",GLUT_BITMAP_9_BY_15);

    //add and cancel
    iSetColor(255,255,255);
    iRectangle(adderButtonX,adderButtonY,adderLength,adderHeight);
    iRectangle(adderButtonX+adderLength+20,adderButtonY,adderLength,adderHeight);

    iText(x+37-20,40-15+3, "ADD",GLUT_BITMAP_9_BY_15);
    iText(x+110+27+25,40-15+3, "CANCEL",GLUT_BITMAP_9_BY_15);


    /*
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
    */

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
        iCircle(ballX, resultY, ballRadius+5); //ring around the resultant circle
    }

}

void velocity(){ //for moving curves

    currentPhase -= 5;
    if(currentPhase>360) currentPhase-= 360;
    else if(currentPhase<-360) currentPhase += 360;
}



void curveDraw(){ // drawing the curves from the array

    //with iPoint

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
            else if(drawOne==1 && i == drawCurveIdx) iSetColor(53,152,255); // drawing
            else iSetColor(255,255,255);
            y += middleHeight;
            iPoint(x,y);


        }


        iSetColor(255,255,255); //51,153,255
        resultY += middleHeight;
        iPoint(x,resultY);


    }



    /*
    //With iLine
    double x,tempX,tempY, y,curvex,resultY,addedPhase = 0;
    int i;
    addedPhase = currentPhase * pi/180;

    for(i=0;i<totalCurves;i++){

        double a = ampMulUser*ampMulZoom*amplitude[i]; //amplitude, ampMulUser is for just amp, ampMulZoom is for zoom
        double f = frqMulUser*frqMulZoom*frequency[i]; //frequency, frqMulUser is for just frq, frqMulZoom is for zoom
        double theta = initialPhase[i] + addedPhase;   // total phase.

        if(caughtidx==i) iSetColor(255,30,30);
        else if(drawOne==1 && i == drawCurveIdx) iSetColor(53,152,255); // drawing
        else iSetColor(255,255,255);

        tempX = 0;
        if(curveType[i])
            tempY = a*sin(0*f + theta);
        else
            tempY = a*cos(0*f + theta);
        tempY += middleHeight;

        for(x=smoothness;x<=screenWidth;x=x+smoothness){
            //to disable movement remove moveX , left and right
            curvex = (x+moveX)*pi/180; // converting to radian

            if(curveType[i])
                y = a*sin(curvex*f + theta);
            else
                y = a*cos(curvex*f + theta);


            y += middleHeight;
            iLine(tempX,tempY,x,y);
            tempX = x;
            tempY = y;
        }

    }


    //resultant
    tempX = 0;
    for(i=0;i<totalCurves;i++){


        double a = ampMulUser*ampMulZoom*amplitude[i]; //amplitude, ampMulUser is for just amp, ampMulZoom is for zoom
        double f = frqMulUser*frqMulZoom*frequency[i]; //frequency, frqMulUser is for just frq, frqMulZoom is for zoom
        double theta = initialPhase[i] + addedPhase;   // total phase.

        if(curveType[i])
            y = a*sin(theta);
        else
            y = a*cos(theta);

        resultY += y;


    }
    tempY = resultY + middleHeight;

    iSetColor(255,255,255); //51,153,255
    for(x=smoothness;x<=screenWidth;x=x+smoothness){

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


        }



        resultY += middleHeight;
        iLine(tempX,tempY,x,resultY);
        tempX = x;
        tempY = resultY;


    }

    */

}


void change(){ // called by iSetTimer

    // for moving balls
    ballChange();

    // for moving curves
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


    iSetColor(25,25,25);

    iFilledRectangle(0,0,screenWidth,screenHeight);


    double d = graph, n = screenHeight/d;


    iSetColor(50,50,50);
    for(int i=0;i<=n;i++){
        iLine(0,i*d,screenWidth,i*d);
    }
    n = screenWidth/d;
    for(int i=0;i<=n;i++){
        iLine(i*d,0,i*d,screenHeight);
    }


    d = d*5, n = screenHeight/d;


    iSetColor(100,100,100);
    for(int i=0;i<=n;i++){
        iLine(0,i*d,screenWidth,i*d);
    }
    n = screenWidth/d;
    for(int i=0;i<=n;i++){
        iLine(i*d,0,i*d,screenHeight);
    }


     //show curves and balls
    if(showCurve) curveDraw();
    if(showAllBalls) ball();



     //commands background
    if(showCommands){
        iSetColor(50,50,50);
        iFilledRectangle(totalWidth-commandsWidth,0,commandsWidth,screenHeight);
    }



    // draw the middle line
    iSetColor(255,255,255);
    iLine(0,middleHeight,screenWidth,middleHeight);



    //drawing
    if(curveCaught==1) {
        drawMode = 0;
        drawOne = 0;
        drawCurveIdx = -1;
    }
    if(drawMode){
        // draws the new curve with big dots
        if(showDraw==1){
            int i;
            iSetColor(53,152,255);
            for(i=0;i<=drawN;i++){
                iFilledCircle(drawX[i],drawY[i],2);
            }
        }


    }

    //command and name message
    iSetColor(255,255,255);
    if(showCommands) iText(10,10,"Press Enter to hide commands",GLUT_BITMAP_9_BY_15);
    else iText(10,10,"Press Enter to view commands",GLUT_BITMAP_9_BY_15);
    //iText(totalWidth-myNameWidth - 30,10,"Abir Muhtasim - 1905066",GLUT_BITMAP_9_BY_15);


    //curve edit message
    if(curveCaught){
        iSetColor(255,30,30);
        iText(10,50,"Curve selected. Left Click on a point and drag to adjust.",GLUT_BITMAP_9_BY_15);
        iText(10,30,"Right Click to deselect",GLUT_BITMAP_9_BY_15);
    }


    //drawing mode message
    else if(drawMode){
        iSetColor(53,152,255);
        if(drawCurveIdx !=-1) iText(10,70,"Curve added. Press backspace to remove curves",GLUT_BITMAP_9_BY_15);
        iText(10,50,"Drawing: ON. Left click and drag to draw curves.",GLUT_BITMAP_9_BY_15);
        iText(10,30,"Press d to turn off",GLUT_BITMAP_9_BY_15);
    }








    //showing commands
    if(showCommands==1){
        iSetColor(255,255,255);
        int leftpos = totalWidth - commandsWidth + 20 ;
        double d = 20*screenHeight/750;
        double uppos = screenHeight - d;
        int i;
        for(i=0;i<totalCommands;i++){
            iText(leftpos,uppos,allCommands[i],GLUT_BITMAP_9_BY_15);
            uppos -= d;
        }

        iLine(leftpos,uppos,leftpos+335,uppos);



    }

     //adding new curve
    if(addingNewCurve==1){

        iSetColor(50,50,50);
        //iSetColor(25,250,25);
        iFilledRectangle(totalWidth-commandsWidth,0,commandsWidth,230);
        curveAdder();
        adderMenu();
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

    else if(drawMode==1){
        showDraw = 1;
        drawN++;
        if(drawN>=99998) drawN = 0;
        drawX[drawN] = mx;
        drawY[drawN] = my;

        if(drawHighY<my){
            drawHighY = my;
            drawHighX = mx;
            drawXid = drawN;
        }

        if(drawLowY>my){
            drawLowY = my;
            drawLowX = mx;
        }
    }




    //printf("x = %d, y= %d\n",mx,my);
    //place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/


void iPassiveMouseMove(int mx, int my)
{
	//place your code here
    //printf("%d %d\n",mx,my);
	/*mposx = mx;
	mposy = my;
	if(mx== 2){}        /*Something to do with mx*/
	//else if(my== 2){}   /*Something to do with my*/

}

void iMouse(int button, int state, int mx, int my)
{


    iPassiveMouseMove(mx,my);
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
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){

        if(drawMode){
            showDraw = 0;
            calculateDraw();
            drawN = -1;
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
        else {
            showAllBalls = 1;
            int i;
            for(i=0;i<totalCurves;i++) showBall[i] = 1;
            showResult = 1;
        }
    }
    else if(key == '\b'){ //delete last curve
       if(totalCurves>=1) totalCurves--;
       drawCurveIdx = -1;
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
    else if(key=='d'){
        if(drawMode==1) {
                drawMode = 0;
                drawOne = 0;
                drawCurveIdx = -1;
        }
        else if(curveCaught!=1){
                drawMode = 1;
                showDraw = 1;
        }

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
    iInitialize(totalWidth, screenHeight, "Curves 1905066");
    return 0;
}

