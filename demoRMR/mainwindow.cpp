#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <math.h>
///TOTO JE DEMO PROGRAM...AK SI HO NASIEL NA PC V LABAKU NEPREPISUJ NIC,ALE SKOPIRUJ SI MA NIEKAM DO INEHO FOLDERA
/// AK HO MAS Z GITU A ROBIS NA LABAKOVOM PC, TAK SI HO VLOZ DO FOLDERA KTORY JE JASNE ODLISITELNY OD TVOJICH KOLEGOV
/// NASLEDNE V POLOZKE Projects SKONTROLUJ CI JE VYPNUTY shadow build...
/// POTOM MIESTO TYCHTO PAR RIADKOV NAPIS SVOJE MENO ALEBO NEJAKY INY LUKRATIVNY IDENTIFIKATOR
/// KED SA NAJBLIZSIE PUSTIS DO PRACE, SKONTROLUJ CI JE MIESTO TOHTO TEXTU TVOJ IDENTIFIKATOR
/// AZ POTOM ZACNI ROBIT... AK TO NESPRAVIS, POJDU BODY DOLE... A NIE JEDEN,ALEBO DVA ALE BUDES RAD
/// AK SA DOSTANES NA SKUSKU


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //tu je napevno nastavena ip. treba zmenit na to co ste si zadali do text boxu alebo nejaku inu pevnu. co bude spravna
    ipaddress="192.168.1.14";//192.168.1.11toto je na niektory realny robot.. na lokal budete davat "127.0.0.1"
  //  cap.open("http://192.168.1.11:8000/stream.mjpg");
   // ipaddress="127.0.0.1";
    ui->setupUi(this);
    datacounter=0;
  //  timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(getNewFrame()));
    actIndex=-1;
    useCamera1=false;




    datacounter=0;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QSize size = this->size();
    static int i = 0;
    static int iBack = 0;
    static int iForw = 0;
    static int previousWidth = size.width(), previousHeight = size.height();
    QPainter painter(this);
    static float f = 681.743;
    static float zd = -145.0;
    static float yd = 115.0;
    static float z = 210.0;
    float xobr = 0;
    float yobr = 0;

    ///prekreslujem obrazovku len vtedy, ked viem ze mam nove data. paintevent sa
    /// moze pochopitelne zavolat aj z inych dovodov, napriklad zmena velkosti okna
    painter.setBrush(Qt::black);//cierna farba pozadia(pouziva sa ako fill pre napriklad funkciu drawRect)
    static QPen pero;
    pero.setStyle(Qt::SolidLine);//styl pera - plna ciara
    pero.setWidth(3);//hrubka pera -3pixely

    static QPen pero2;
    pero2.setStyle(Qt::SolidLine);//styl pera - plna ciara
    pero2.setWidth(6);//hrubka pera -3pixely
    pero2.setColor(Qt::green);//farba je zelena

    static QRect rect;
    rect= ui->frame->geometry();//ziskate porametre stvorca,do ktoreho chcete kreslit
    rect.translate(0,15);





    float pomer = (float)size.width() / (float)size.height();

    if((previousHeight-size.height()) < 0 || (previousHeight-size.width()) < 0){
        if(!((1.75 > pomer) && (pomer > 1.79))){
            if(pomer < 1.75){
                int width = size.height()*1.77;
                resize(width,size.height());
            }
            if(pomer > 1.79){
                int height = size.width()/1.77;
                resize(size.width(),height);
            }
        }
         ui->pushButton_9->setGeometry((size.width()/2)-100,size.height()-90,201,31);
    }else if((previousHeight-size.height()) > 0 || (previousHeight-size.width()) > 0){
        if(!((1.75 > pomer) && (pomer > 1.79))){
            if(pomer < 1.75){
                int height = size.width()/1.77;
                resize(size.width(),height);
            }
            if(pomer > 1.79){
                int width = size.height()*1.77;
                resize(width,size.height());
            }
        }
         ui->pushButton_9->setGeometry((size.width()/2)-100,size.height()-90,201,31);
    }

    previousHeight = size.height();
    previousWidth = size.width();

    if(actIndex>-1)
    {

        painter.drawRect(rect);


        QImage image = QImage((uchar*)frame[actIndex].data, frame[actIndex].cols, frame[actIndex].rows, frame[actIndex].step, QImage::Format_RGB888  );//kopirovanie cvmat do qimage
        QPainter painter2(&image);
        QFont font = painter2.font();
        static QRect rectLidar(0,image.height()-175,175,175);
        static QColor lidarRectColor(0, 0, 0, 75);

        painter2.fillRect(rectLidar,lidarRectColor);
        painter2.setPen(pero2);
        painter2.drawEllipse(QPoint(88, image.height()-88),4,4);
        pero.setColor(Qt::blue);
        pero.setWidth(3);
        painter2.setPen(pero);
        painter2.drawLine(88,image.height()-88,88,image.height()-94);




        if(!move){

            font.setPixelSize(25);
            font.setWeight(QFont::Bold);
            painter2.setFont(font);
            pero.setColor(Qt::blue);
            painter2.setPen(pero);
            painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("SAFETY STOP"));

        }else if(idemRovno){
            font.setPixelSize(25);
            font.setWeight(QFont::Bold);
            painter2.setFont(font);
            pero.setColor(Qt::red);
            painter2.setPen(pero);
            painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("ROVNO"));
        }else if(idemVlavo){
            font.setPixelSize(25);
            font.setWeight(QFont::Bold);
            painter2.setFont(font);
            pero.setColor(Qt::red);
            painter2.setPen(pero);
            painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("DOLAVA"));
        }else if(idemVpravo){
            font.setPixelSize(25);
            font.setWeight(QFont::Bold);
            painter2.setFont(font);
            pero.setColor(Qt::red);
            painter2.setPen(pero);
            painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("DOPRAVA"));
        }else if(tocimVlavo){
            font.setPixelSize(25);
            font.setWeight(QFont::Bold);
            painter2.setFont(font);
            pero.setColor(Qt::red);
            painter2.setPen(pero);
            painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("TOCIM VLAVO"));
        }else if(tocimVpravo){
            font.setPixelSize(25);
            font.setWeight(QFont::Bold);
            painter2.setFont(font);
            pero.setColor(Qt::red);
            painter2.setPen(pero);
            painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("TOCIM VPRAVO"));
        }else if(cuvam){
            font.setPixelSize(25);
            font.setWeight(QFont::Bold);
            painter2.setFont(font);
            pero.setColor(Qt::red);
            painter2.setPen(pero);
            painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("DOZADU"));
        }else if(brzdim){
            font.setPixelSize(25);
            font.setWeight(QFont::Bold);
            painter2.setFont(font);
            pero.setColor(Qt::red);
            painter2.setPen(pero);
            if(translationG != 0){
                painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("BRZDENIE"));
            }else painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("STOJIM"));

        }else if(emergencyBrdza){
            font.setPixelSize(25);
            font.setWeight(QFont::Bold);
            painter2.setFont(font);
            pero.setColor(Qt::red);
            painter2.setPen(pero);
            painter2.drawText(5,image.height()-175-30, 460, 350, 0, tr("BRZDENIE"));
        }

        pero.setColor(Qt::cyan);
        painter2.setPen(pero);

        if(updateLaserPicture==1) ///ak mam nove data z lidaru
        {
            updateLaserPicture=0;

            painter2.setPen(pero);
            for(int k=0;k<copyOfLaserData.numberOfScans/*360*/;k++)
            {

                if(copyOfLaserData.Data[k].scanAngle<=32 || copyOfLaserData.Data[k].scanAngle>=328){
                    if((copyOfLaserData.Data[k].scanDistance < 280) && (copyOfLaserData.Data[k].scanDistance > 0)){
                        iForw++;
                    }

                    float d = copyOfLaserData.Data[k].scanDistance;
                    float alfa = ((360.0-copyOfLaserData.Data[k].scanAngle)*3.14159)/180.0;
                    float x = d*cos(alfa);
                    float y = d*sin(alfa);
                    xobr = ((float)frame[actIndex].cols/2.0)-((f*y)/(x+zd));
                    yobr = ((float)frame[actIndex].rows/2.0)+((f*(-z+yd))/(x+zd));

                    QPen pen;
                    QBrush brush;


                    if(xobr>=0&&xobr<=frame[actIndex].cols&&yobr>=0&&yobr<=frame[actIndex].rows){

                        if(copyOfLaserData.Data[k].scanDistance>2000){
                            QColor color(0, 0, 0, 2);
                            pen.setWidth(2);
                            pen.setColor(color);
                            painter2.setBrush(brush);
                            painter2.setPen(pen);
                            painter2.setBrush(QColor("#DA8633"));
                            painter2.drawEllipse(QPoint((int)xobr, (int)yobr), 5, 5);

                        }else if(copyOfLaserData.Data[k].scanDistance<=2000 && copyOfLaserData.Data[k].scanDistance>1500){
                            QColor color(0, 0, 0, 5);
                            pen.setWidth(3);
                            pen.setColor(color);
                            painter2.setPen(pen);
                            painter2.setBrush(QColor("#DA5F33"));
                            painter2.drawEllipse(QPoint((int)xobr, (int)yobr), 7, 7);

                        }else if(copyOfLaserData.Data[k].scanDistance<=1500 && copyOfLaserData.Data[k].scanDistance>1000){
                            QColor color(0, 0, 0, 7);
                            pen.setWidth(3);
                            pen.setColor(color);
                            brush.setColor(color);
                            painter2.setPen(pen);
                            painter2.setBrush(QColor("#DA3333"));
                            painter2.drawEllipse(QPoint((int)xobr, (int)yobr), 9, 9);

                        }else if(copyOfLaserData.Data[k].scanDistance<=1000 && copyOfLaserData.Data[k].scanDistance>300){
                            QColor color(0, 0, 0, 10);
                            pen.setWidth(4);
                            pen.setColor(color);
                            painter2.setPen(pen);
                            painter2.setBrush(QColor("#D10000"));
                            painter2.drawEllipse(QPoint((int)xobr, (int)yobr), 11, 11);

                        }
                    }
                }

                if((copyOfLaserData.Data[k].scanAngle > 148.0) && (copyOfLaserData.Data[k].scanAngle < 212.0) && (copyOfLaserData.Data[k].scanDistance < 280) && (copyOfLaserData.Data[k].scanDistance > 0)){
                    iBack++;
                }

                int dist=copyOfLaserData.Data[k].scanDistance/55; ///vzdialenost nahodne predelena 20 aby to nejako vyzeralo v okne.. zmen podla uvazenia
                int xp=rectLidar.width()-(rectLidar.width()/2+dist*2*sin((360.0-copyOfLaserData.Data[k].scanAngle)*3.14159/180.0)); //prepocet do obrazovky
                int yp=rectLidar.height()-(rectLidar.height()/2+dist*2*cos((360.0-copyOfLaserData.Data[k].scanAngle)*3.14159/180.0)) + image.height()-175;//prepocet do obrazovky
                if(rectLidar.contains(xp,yp)){
                    pero.setColor(Qt::yellow);
                    pero.setWidth(4);
                    painter2.setPen(pero);
                    painter2.drawPoint(xp,yp);
                }
                if(copyOfLaserData.Data[k].scanDistance > 0 && copyOfLaserData.Data[k].scanDistance < 350){
                    i++;
                }
//////////////////////////////////////////////////////////////////////////////
                if((copyOfLaserData.Data[k].scanDistance<600) && (copyOfLaserData.Data[k].scanDistance>130)){
                    float pi = 3.14159;
                    int pointX, pointY, numLines, centerX, centerY, radius, endX, endY, lastX = 0, lastY = 0;
                    pointX = frame[actIndex].cols-120;
                    pointY = frame[actIndex].rows-120;
                    numLines = 8;
                    centerX = pointX+25;
                    centerY = pointY+25;
                    QPen pen2, pen3, pen4;
                    QBrush brush2;

                    pen2.setWidth(5);
                    pen2.setColor(Qt::green);
                    pen3.setColor(QColor(0, 0, 0, 0));
                    pen3.setWidth(4);
                    //pen4.setColor(QColor(0, 0, 0,1));
                    pen4.setWidth(4);
                    brush2.setColor(Qt::black);
                    painter2.setBrush(brush2);
                    painter2.setPen(pen2);
                    painter2.drawEllipse(pointX,pointY,50,50);
                    painter2.drawLine(pointX+25,pointY,pointX+25,pointY+25);
                    float laserRadians = (((360-copyOfLaserData.Data[k].scanAngle)*3.14159)/180.0);
                    //najmensi kruh
                    radius = 35;
                    painter2.setPen(pen3);
                    //boolean b = false;
                    for (int i = 0; i < numLines+1; i++) {
                        //boolean b = false;
                        float angle = (i * 2 * pi / numLines);
                        endX = centerX + radius * cos(angle + pi);
                        endY = centerY + radius * sin(angle + pi);
                        QColor colorHigh(51,156,218);


                        if(i>0){
                            if(laserRadians>=0 && laserRadians<0.785 && copyOfLaserData.Data[k].scanDistance<300 && i==2){
                            //printf("\nlidarUhol %f\n",laserRadians);
                                pen3.setColor(colorHigh);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                                //b=true;
                            }else if(laserRadians>=0.785 && laserRadians<1.57 && copyOfLaserData.Data[k].scanDistance<300 && i==1){
                                pen3.setColor(colorHigh);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                                //b=true;
                            }else if(laserRadians>=1.57 && laserRadians<2.335 && copyOfLaserData.Data[k].scanDistance<300 && i==8){
                                pen3.setColor(colorHigh);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                                //b=true;
                            }else if(laserRadians>=2.335 && laserRadians<3.14 && copyOfLaserData.Data[k].scanDistance<300 && i==7){
                                pen3.setColor(colorHigh);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                                //b=true;
                            }else if(laserRadians>=3.14 && laserRadians<3.925 && copyOfLaserData.Data[k].scanDistance<300 && i==6){
                                pen3.setColor(colorHigh);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                                //b=true;
                            }else if(laserRadians>=3.925 && laserRadians<4.71 && copyOfLaserData.Data[k].scanDistance<300 && i==5){
                                pen3.setColor(colorHigh);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                                //b=true;
                            }else if(laserRadians>=4.71 && laserRadians<5.495 && copyOfLaserData.Data[k].scanDistance<300 && i==4){
                                pen3.setColor(colorHigh);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                                //b=true;
                            }else if(laserRadians>=5.495 && laserRadians<6.28 && copyOfLaserData.Data[k].scanDistance<300 && i==3){
                                pen3.setColor(colorHigh);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                                //b=true;
                            }

                        }

                        lastX=endX;
                        lastY=endY;
                    }

                    //stredny kruh
                    radius = 45;

                    for (int i = 0; i < numLines+1; i++) {

                        float angle = (i * 2 * pi / numLines);
                        endX = centerX + radius * cos(angle + pi);
                        endY = centerY + radius * sin(angle + pi);
                        QColor colorMiddle(51,218,206);


                        if(i>0){
                            if(laserRadians>=0 && laserRadians<0.785 && copyOfLaserData.Data[k].scanDistance<450 && i==2){
                            //printf("\nlidarUhol %f\n",laserRadians);
                                pen3.setColor(colorMiddle);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>=0.785 && laserRadians<1.57 && copyOfLaserData.Data[k].scanDistance<450 && i==1){
                                pen3.setColor(colorMiddle);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>=1.57 && laserRadians<2.335 && copyOfLaserData.Data[k].scanDistance<450 && i==8){
                                pen3.setColor(colorMiddle);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>=2.335 && laserRadians<3.14 && copyOfLaserData.Data[k].scanDistance<450 && i==7){
                                pen3.setColor(colorMiddle);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>=3.14 && laserRadians<3.925 && copyOfLaserData.Data[k].scanDistance<450 && i==6){
                                pen3.setColor(colorMiddle);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>=3.925 && laserRadians<4.71 && copyOfLaserData.Data[k].scanDistance<450 && i==5){
                                pen3.setColor(colorMiddle);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>=4.71 && laserRadians<5.495 && copyOfLaserData.Data[k].scanDistance<450 && i==4){
                                pen3.setColor(colorMiddle);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>=5.495 && laserRadians<=6.28 && copyOfLaserData.Data[k].scanDistance<450 && i==3){
                                pen3.setColor(colorMiddle);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }

                        }

                        lastX=endX;
                        lastY=endY;
                    }

                    //Najvatsi kruh
                    radius = 55;

                    for (int i = 0; i < numLines+1; i++) {

                        float angle = (i * 2 * pi / numLines);
                        endX = centerX + radius * cos(angle + pi);
                        endY = centerY + radius * sin(angle + pi);
                        QColor colorLow(51,218,67);

                        if(i>0){
                            //copyOfLaserData.Data[k].scanDistance<550 &&
                            if(laserRadians>=0 && laserRadians<=0.785 && copyOfLaserData.Data[k].scanDistance<599 && i==2){
                                pen3.setColor(colorLow);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>0.785 && laserRadians<=1.57 && copyOfLaserData.Data[k].scanDistance<599 && i==1){
                                pen3.setColor(colorLow);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>1.57 && laserRadians<=2.335 && copyOfLaserData.Data[k].scanDistance<599 && i==8){
                                pen3.setColor(colorLow);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>2.335 && laserRadians<=3.14 && copyOfLaserData.Data[k].scanDistance<599 && i==7){
                                pen3.setColor(colorLow);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>3.14 && laserRadians<=3.925 && copyOfLaserData.Data[k].scanDistance<599 && i==6){
                                pen3.setColor(colorLow);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>3.925 && laserRadians<=4.71 && copyOfLaserData.Data[k].scanDistance<599 && i==5){
                                pen3.setColor(colorLow);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>4.71 && laserRadians<=5.495 && copyOfLaserData.Data[k].scanDistance<599 && i==4){
                                pen3.setColor(colorLow);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>5.495 && laserRadians<=6.28 && copyOfLaserData.Data[k].scanDistance<599 && i==3){
                                pen3.setColor(colorLow);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else if(laserRadians>0 && laserRadians<=0.785 && copyOfLaserData.Data[k].scanDistance<599 && i==3){
                                pen3.setColor(colorLow);
                                painter2.setPen(pen3);
                                painter2.drawLine(lastX, lastY, endX, endY);
                            }else{
                                //pen3.setColor(QColor(0, 0, 0, 1));
                                //painter2.setPen(pen3);
                            }

                        }

                        lastX=endX;
                        lastY=endY;
                    }
                }
////////////////////////////////////////////////////////////////

            }

            if(iForw > 0){
                stopForw = true;
                iForw = 0;
            }else stopForw = false;


            if(iBack > 0){
                stopBack = true;
                iBack = 0;
            }else stopBack = false;

            if(i > 0){
                font.setPixelSize(80);
                painter2.setFont(font);
                pero.setColor(Qt::blue);
                painter2.setPen(pero);
                painter2.drawText(image.width()/3,image.height()/2.5,500,500, 0, tr("!POZOR!"));
                i = 0;
            }


            painter.drawImage(rect,image.rgbSwapped());
        }
    }



    /*if(updateSkeletonPicture==1 )
    {
        painter.setPen(Qt::red);
        for(int i=0;i<75;i++)
        {
            int xp=rect.width()-rect.width() * skeleJoints.joints[i].x+rect.topLeft().x();
            int yp= (rect.height() *skeleJoints.joints[i].y)+rect.topLeft().y();
            if(rect.contains(xp,yp))
                painter.drawEllipse(QPoint(xp, yp),2,2);
        }
    }*/
}


/// toto je slot. niekde v kode existuje signal, ktory je prepojeny. pouziva sa napriklad (v tomto pripade) ak chcete dostat data z jedneho vlakna (robot) do ineho (ui)
/// prepojenie signal slot je vo funkcii  on_pushButton_9_clicked
void  MainWindow::setUiValues(double robotX,double robotY,double robotFi)
{

}

///toto je calback na data z robota, ktory ste podhodili robotu vo funkcii on_pushButton_9_clicked
/// vola sa vzdy ked dojdu nove data z robota. nemusite nic riesit, proste sa to stane
int MainWindow::processThisRobot(TKobukiData robotdata)
{


    ///tu mozete robit s datami z robota
    /// ale nic vypoctovo narocne - to iste vlakno ktore cita data z robota
    ///teraz tu posielam rychlosti na zaklade toho co setne joystick a vypisujeme data z robota(kazdy 5ty krat. ale mozete skusit aj castejsie). vyratajte si polohu. a vypiste spravnu

///TU PISTE KOD... TOTO JE TO MIESTO KED NEVIETE KDE ZACAT,TAK JE TO NAOZAJ TU. AK AJ TAK NEVIETE, SPYTAJTE SA CVICIACEHO MA TU NATO STRING KTORY DA DO HLADANIA XXX


        emit uiValuesChanged(robotdata.EncoderLeft,11,12);



    return 0;

}

///toto je calback na data z lidaru, ktory ste podhodili robotu vo funkcii on_pushButton_9_clicked
/// vola sa ked dojdu nove data z lidaru
int MainWindow::processThisLidar(LaserMeasurement laserData)
{


    memcpy( &copyOfLaserData,&laserData,sizeof(LaserMeasurement));
    //tu mozete robit s datami z lidaru.. napriklad najst prekazky, zapisat do mapy. naplanovat ako sa prekazke vyhnut.
    // ale nic vypoctovo narocne - to iste vlakno ktore cita data z lidaru
    updateLaserPicture=1;
    update();
   //tento prikaz prinuti prekreslit obrazovku.. zavola sa paintEvent funkcia


    return 0;

}

///toto je calback na data z kamery, ktory ste podhodili robotu vo funkcii on_pushButton_9_clicked
/// vola sa ked dojdu nove data z kamery
int MainWindow::processThisCamera(cv::Mat cameraData)
{

    cameraData.copyTo(frame[(actIndex+1)%3]);//kopirujem do nasej strukury
    actIndex=(actIndex+1)%3;//aktualizujem kde je nova fotka
    updateLaserPicture=1;
    ///update();
    return 0;
}

///toto je calback na data zo skeleton trackera, ktory ste podhodili robotu vo funkcii on_pushButton_9_clicked
/// vola sa ked dojdu nove data z trackera
int MainWindow::processThisSkeleton(skeleton skeledata)
{
    static int translation = 0;
    static boolean arc = false;
    emergencyBrdza = false;
    idemVlavo = false;
    idemVpravo = false;
    tocimVlavo = false;
    tocimVpravo = false;
    brzdim = false;
    cuvam = false;
    idemRovno = false;

    memcpy(&skeleJoints,&skeledata,sizeof(skeleton));

    if(move){
        ///("\n%f %f %f \n",(skeleJoints.joints[8].z - skeleJoints.joints[12].z), (skeleJoints.joints[8].z - skeleJoints.joints[16].z), (skeleJoints.joints[8].z - skeleJoints.joints[20].z));

        bool lavaRuka = (sqrt(pow((skeleJoints.joints[8].x - skeleJoints.joints[0].x),2)+pow((skeleJoints.joints[8].y - skeleJoints.joints[0].y),2)) > 2*(sqrt(pow((skeleJoints.joints[12].x - skeleJoints.joints[0].x),2)+pow((skeleJoints.joints[12].y - skeleJoints.joints[0].y),2)))) &&
                (sqrt(pow((skeleJoints.joints[8].x - skeleJoints.joints[0].x),2)+pow((skeleJoints.joints[8].y - skeleJoints.joints[0].y),2)) > 2*(sqrt(pow((skeleJoints.joints[16].x - skeleJoints.joints[0].x),2)+pow((skeleJoints.joints[16].y - skeleJoints.joints[0].y),2)))) &&
                (sqrt(pow((skeleJoints.joints[8].x - skeleJoints.joints[0].x),2)+pow((skeleJoints.joints[8].y - skeleJoints.joints[0].y),2)) > 2*(sqrt(pow((skeleJoints.joints[20].x - skeleJoints.joints[0].x),2)+pow((skeleJoints.joints[20].y - skeleJoints.joints[0].y),2))));

        bool pravaRuka =  (sqrt(pow((skeleJoints.joints[29].x - skeleJoints.joints[21].x),2)+pow((skeleJoints.joints[29].y - skeleJoints.joints[21].y),2)) > 2*(sqrt(pow((skeleJoints.joints[33].x - skeleJoints.joints[21].x),2)+pow((skeleJoints.joints[33].y - skeleJoints.joints[21].y),2)))) &&
                (sqrt(pow((skeleJoints.joints[29].x - skeleJoints.joints[21].x),2)+pow((skeleJoints.joints[29].y - skeleJoints.joints[21].y),2)) > 2*(sqrt(pow((skeleJoints.joints[37].x - skeleJoints.joints[21].x),2)+pow((skeleJoints.joints[37].y - skeleJoints.joints[21].y),2)))) &&
                (sqrt(pow((skeleJoints.joints[29].x - skeleJoints.joints[21].x),2)+pow((skeleJoints.joints[29].y - skeleJoints.joints[21].y),2)) > 2*(sqrt(pow((skeleJoints.joints[41].x - skeleJoints.joints[21].x),2)+pow((skeleJoints.joints[41].y - skeleJoints.joints[21].y),2))));
        bool palecLavy = (((sqrt(pow((skeleJoints.joints[4].x - skeleJoints.joints[0].x),2)+pow((skeleJoints.joints[4].y - skeleJoints.joints[0].y),2))) -(sqrt(pow((skeleJoints.joints[8].x - skeleJoints.joints[0].x),2)+pow((skeleJoints.joints[8].y - skeleJoints.joints[0].y),2)))) > 0.05);
        bool palecPravy = (((sqrt(pow((skeleJoints.joints[25].x - skeleJoints.joints[21].x),2)+pow((skeleJoints.joints[25].y - skeleJoints.joints[21].y),2))) -(sqrt(pow((skeleJoints.joints[29].x - skeleJoints.joints[21].x),2)+pow((skeleJoints.joints[29].y - skeleJoints.joints[21].y),2)))) > 0.05);

        bool stop = (sqrt(pow((skeleJoints.joints[8].x - skeleJoints.joints[29].x),2)+pow((skeleJoints.joints[8].y - skeleJoints.joints[29].y),2)) < 0.05);

        if(stop){
            emergencyBrdza = true;
            translation=0;
            robot.setTranslationSpeed(translation);
        }
        else if(lavaRuka && pravaRuka && !stopForw){
            arc = false;
            translation += 15;
            if(translation > 400) translation = 400;
            robot.setTranslationSpeed(translation);
            idemRovno = true;

        }else if(lavaRuka && !pravaRuka && palecPravy && !stopForw){

            arc = true;
            translation += 25;
            if(translation > 400) translation = 400;
            robot.setArcSpeed(translation,-400);
            idemVpravo = true;

        }else if(!lavaRuka && pravaRuka && palecLavy && !stopForw){

            arc = true;
            translation += 25;
            if(translation > 400) translation = 400;
            robot.setArcSpeed(translation,400);
            idemVlavo = true;


        }else if(lavaRuka && !pravaRuka && !palecPravy && (translation == 0)){

            robot.setRotationSpeed(-3.14159/3);
            tocimVpravo = true;
        }else if(!lavaRuka && pravaRuka && !palecLavy && (translation == 0)){

            robot.setRotationSpeed(3.14159/3);
            tocimVlavo = true;
        }
        else if(!stopBack && (!lavaRuka && !pravaRuka && palecLavy && palecPravy && (translation <= 0))){
            arc = false;
            translation -= 20;
            if(translation < -300) translation = -300;
            robot.setTranslationSpeed(translation);
            cuvam = true;
        }else {

            if(abs(translation) <= 50){
                translation=0;
                arc = false;
            }else if(translation > 50){
                translation -= 40;
            }else if(translation < -50){
                translation += 40;
            }
            brzdim = true;

            if(stopForw){
                brzdim = false;
                translation = 0;
                emergencyBrdza = true;
            }
            if(stopBack){
                brzdim = false;
                translation = 0;
                emergencyBrdza = true;
            }

            if(arc){
                robot.setArcSpeed(translation,400);
            }else robot.setTranslationSpeed(translation);

        }

        updateSkeletonPicture=1;
    }else robot.setTranslationSpeed(0);

    translationG = translation;
    return 0;
}
void MainWindow::on_pushButton_9_clicked() //start button
{

    static bool start = false, start_stop = false;

    if(!start){
        stopForw = false;
        stopBack = false;
        move = true;
        start = true;
        forwardspeed=0;
        rotationspeed=0;
        useCamera1 = true;

        //tu sa nastartuju vlakna ktore citaju data z lidaru a robota
        connect(this,SIGNAL(uiValuesChanged(double,double,double)),this,SLOT(setUiValues(double,double,double)));

        ///setovanie veci na komunikaciu s robotom/lidarom/kamerou.. su tam adresa porty a callback.. laser ma ze sa da dat callback aj ako lambda.
        /// lambdy su super, setria miesto a ak su rozumnej dlzky,tak aj prehladnost... ak ste o nich nic nepoculi poradte sa s vasim doktorom alebo lekarnikom...
        robot.setLaserParameters(ipaddress,52999,5299,/*[](LaserMeasurement dat)->int{std::cout<<"som z lambdy callback"<<std::endl;return 0;}*/std::bind(&MainWindow::processThisLidar,this,std::placeholders::_1));
        robot.setRobotParameters(ipaddress,53000,5300,std::bind(&MainWindow::processThisRobot,this,std::placeholders::_1));
        //---simulator ma port 8889, realny robot 8000
        robot.setCameraParameters("http://"+ipaddress+":8000/stream.mjpg",std::bind(&MainWindow::processThisCamera,this,std::placeholders::_1));
        robot.setSkeletonParameters("127.0.0.1",23432,23432,std::bind(&MainWindow::processThisSkeleton,this,std::placeholders::_1));
        ///ked je vsetko nasetovane tak to tento prikaz spusti (ak nieco nieje setnute,tak to normalne nenastavi.cize ak napr nechcete kameru,vklude vsetky info o nej vymazte)
        robot.robotStart();

        //ziskanie joystickov
        instance = QJoysticks::getInstance();


        /// prepojenie joysticku s jeho callbackom... zas cez lambdu. neviem ci som to niekde spominal,ale lambdy su super. okrem toho mam este rad ternarne operatory a spolocneske hry ale to tiez nikoho nezaujima
        /// co vas vlastne zaujima? citanie komentov asi nie, inak by ste citali toto a ze tu je blbosti
        connect(
            instance, &QJoysticks::axisChanged,
            [this]( const int js, const int axis, const qreal value) { if(/*js==0 &&*/ axis==1){forwardspeed=-value*300;}
                if(/*js==0 &&*/ axis==0){rotationspeed=-value*(3.14159/2.0);}}
        );

    }

    if(start_stop){
        ui->pushButton_9->setText("START");
        ui->pushButton_9->setStyleSheet("background-color: grey");
        start_stop = false;
        move = false;
        robot.setTranslationSpeed(0);

    }else {
        ui->pushButton_9->setText("STOP");
        ui->pushButton_9->setStyleSheet("background-color: white");
        start_stop = true;
        move = true;

    }




}

void MainWindow::on_pushButton_2_clicked() //forward
{
    //pohyb dopredu
    robot.setTranslationSpeed(500);

}

void MainWindow::on_pushButton_3_clicked() //back
{
    robot.setTranslationSpeed(-250);

}

void MainWindow::on_pushButton_6_clicked() //left
{
robot.setRotationSpeed(3.14159/2);

}

void MainWindow::on_pushButton_5_clicked()//right
{
robot.setRotationSpeed(-3.14159/2);

}

void MainWindow::on_pushButton_4_clicked() //stop
{
    robot.setTranslationSpeed(0);

}




void MainWindow::on_pushButton_clicked()
{
    if(useCamera1==true)
    {
        useCamera1=false;

        //ui->pushButton->setText("use camera");
    }
    else
    {
        useCamera1=true;

        //ui->pushButton->setText("use laser");
    }
}

void MainWindow::getNewFrame()
{

}
