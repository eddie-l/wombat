#include <kipr/wombat.h>
char colour_with_a_u_sensor_for_pompoms();
void calibratecolor();
void sort();
void linefollow(int n, float multiplier,int adjustment,int max);
void linefollowbutbackwards(int n);
int absol(int num);
void slow_servo(int port, int maxmin,int speed, int interval);
void turn(char n,int modifier,int detectline);
void lnsqr (char n);
void path ();
void mall (int speed);
void rot(int n, int speed);
void reset();
void waitforlight();
//Global Ports
int tophat1 = 4;
int tophat2 = 0;
int motor1 = 0;
int motor2 = 2;
int frontsensor = 0;
int backsensor = 2;
int threshold = 0;
int lightsensor = 3;
float redpomthreshold = 0;
float greenpomthreshold = 0;
float redgreenpomthreshold = 0;
int main()
{
    //waitforlight();
    shut_down_in(118);
    printf("%i", camera_open());
    msleep(1000);
    enable_servos();
    threshold = (((analog(tophat1) + analog(tophat1) + analog(tophat1) + analog(tophat2) + analog(tophat2) + analog(tophat2))/4));
    //threshold = 850;
    //colour_with_a_u_sensor_for_pompoms();
    printf("%i \n",threshold);
    slow_servo(1,1460,1,2);
    //calibratecolor();
    printf("\n %f \n",redpomthreshold);
    printf("%f \n",greenpomthreshold);
    printf("%f \n",redgreenpomthreshold);
    path();
    //sort();
    msleep(100);
    camera_close();
    return 0;
}
char colour_with_a_u_sensor_for_pompoms()
{

    int r, c, i;
    camera_update();
    const unsigned char* img = get_camera_frame();
    int green = 0;
    int red = 0;
    int other = 0;
    for(r = 50; r < 70; r++) 
    {
        for(c = 70; c < 90; c++) 
        {
            i = (160*r + c); // index of pixel to paint into row r, column c
            //printf("Pixel #%i: ", i);
            i = i * 3;
            if (img[i + 2] > (img[i + 1] + 10))
            {
                //printf("r");
                red += 1;
            }
            else if (img[i + 1] > (img[i + 2] + 10))
            {
                //printf("g");
                green += 1;
            }
            else
            {
                //printf("o");
                other += 1;
            }
        }
    }
    if (other*redpomthreshold > red && other*greenpomthreshold > green)
    {
        printf("O");
        return 'E';
    }
    else if (red*redgreenpomthreshold > green)
    {
        printf("R");
        return 'r';
    }
    else if (red < green)
    {
        printf("G");
        return 'g';
    }
    else
    {
        return 'E';
    }
}
void sort()
{
    int green = 1350;
    int neutral = 975;
    int red = 600;
    int motorportformovingpompoms = 3;
    int servoport = 0;
    int servoport2 = 1;
    set_servo_position(servoport,neutral);
    set_servo_position(servoport,neutral);
    //int secondservoparameterlow = 300;
    //int secondservoparameterhi = 1460;
    mav(motorportformovingpompoms, 650);
    int j = 0;
    int counter = 0;
    printf("HERE");
    for (j = 0;j < 125;j++){
        counter = 0;
        //return;
        char data = 'E';
        char lastdata = 'E';
        int i = 0;
        while (data == 'E'){
            if (counter < 5){
                mav(motorportformovingpompoms, 500);
            }
            for (i = 0; i < 5; i++){
                data = colour_with_a_u_sensor_for_pompoms();
            }
            if (counter == 6){
                set_servo_position(servoport,neutral);
                set_servo_position(servoport,neutral);
            }
            counter = counter + 1;
        }
        if (data == 'r')
        {
            mav(motorportformovingpompoms, 600);
            if (lastdata == 'g'){
                msleep(250);
                set_servo_position(servoport, red);
                set_servo_position(servoport, red);
                msleep(375);
            }
            else{
                set_servo_position(servoport, red);
                set_servo_position(servoport, red);
                msleep(500);
            }
            lastdata = 'r';
        }
        else if (data == 'g')
        {
            mav(motorportformovingpompoms, 600);
            if (lastdata == 'r'){
                msleep(250);
                set_servo_position(servoport, green);
                set_servo_position(servoport, green);
                msleep(375);
            }
            else{
                set_servo_position(servoport, green);
                set_servo_position(servoport, green);
                msleep(500);
            }
            lastdata = 'g';
        }
        else{
            lastdata = 'E';
        }
        set_servo_position(servoport2,1300+(1460-(get_servo_position(servoport2))));
        msleep(50);
        set_servo_position(servoport2,1300+(1460-(get_servo_position(servoport2))));
        msleep(50);
        set_servo_position(servoport2,1300+(1460-(get_servo_position(servoport2))));
        msleep(50);
        set_servo_position(servoport2,1300+(1460-(get_servo_position(servoport2))));
        msleep(50);
        set_servo_position(servoport2,1460);
        msleep(10);
    }
    msleep(1600);
    set_servo_position(servoport, neutral);
    alloff();
    return;
}
int absol(int num) {
    if (num < 0) {
        return num*-1;
    }
    return num;
} 
void slow_servo(int port, int maxmin,int speed,int interval) {
    int initial = get_servo_position(port);
    int difference = absol(get_servo_position(port)-maxmin);
    if (difference == 0) {
        return;
    }
    int absolute = difference/(get_servo_position(port)-maxmin)*-1;
    int x = 0;
    for (x = 0; x <= difference; x += speed){
        set_servo_position(port, initial+(x*absolute));
        msleep(interval);
    }
}
void path()
{
    msleep(3000);
    printf("turning left...\n");
    turn('l',0,0);
    lnsqr('f');
    mall(1500);
    msleep(625);
    printf("turning right...\n");
    turn('r',-225,0);
    msleep(15000);
    printf("squaring up... \n");
    lnsqr('f');
    mall(1000);
    msleep(1250);
    alloff();
    slow_servo(1,300,1,2);
    turn('r',1000,1);
    alloff();
    printf("following line...\n");
    linefollow(6000,1.0,1,10);
    mall(500);
    msleep(150);
    alloff();
    slow_servo(1,800,1,2);
    mall(-500);
    msleep(1000);
    alloff();
    slow_servo(1,300,1,2);
    turn('l',-1135,0);
    linefollow(4000,1.0,1,10000);
    mall(-500);
    msleep(375);
    alloff();
    slow_servo(1,900,1,2);
    mall(-500);
    msleep(875);
    alloff();
    slow_servo(1,300,1,2);
    linefollow(1500,1,1,4000);
    mall(-375);
    msleep(375);
    alloff();
    slow_servo(1,1460,1,2);
    turn('l',0,0);
    turn('l',25,1);
    mav(motor2,-1000);
    mav(motor1,-1000);
    msleep(50);
    alloff();
    msleep(25);
    mall(-1500);
    msleep(1150);
    turn('l',-900,0);
    mall(-500);
    while (digital(backsensor) == 0)
    {
        msleep(50);
    }
    msleep(100);
    alloff();
    calibratecolor();
    sort();
}


void mall(int speed)
{
    mav(motor1, speed);
    mav(motor2, -speed);
}
void linefollow(int n,float multiplier,int adjustment, int max)
{
    //motor 0 is right, motor 1 is left

    mav(motor1, 1300*multiplier);
    mav(motor2, -1300*multiplier);
    int i = 0;
    int y = 0;
    while (0 == 0)
    {
        while (analog(tophat2) < threshold && analog(tophat1) >= threshold)
        {
            //turn right
            mav(motor1, 500*multiplier);
            mav(motor2, -1000*multiplier);
            //printf("scenario /////////////////\n");
            msleep(10);
            i += 10;
            y = 1;

        }
        while (analog(tophat1) < threshold && analog(tophat2) >= threshold)
        {
            //turn left
            mav(motor1, 1000*multiplier);
            mav(motor2, -500*multiplier);
            //printf("scenario \\\\\\\\\\\\\\\\\n");
            msleep(10);
            i += 10;
            y = 1;
        }
        if (i >= n && adjustment == 1 && y == 1){
            printf("%i I\n",i);
            printf("%i N\n",n);
            return;
        }
        else if (analog(tophat2) >= threshold && analog(tophat1) >= threshold)
        {
            mall(1300*multiplier);
            y = 0;
        }
        if (i >= n && adjustment == 0){
            //printf("%i, ", direction);
            return;
        }
        if (i >= n && adjustment == 1 && i >= max){
            //printf("%i, ", direction);
            return;
        }
        if (digital(frontsensor) == 1){
            return;
        }
        msleep(10);
        i += 10;
        printf("I %i N %i", i, n);
    }
}
void linefollowbutbackwards(int n)
{
    mav(motor1, -1300);
    mav(motor2, 1300);
    char direction = 'l';
    int i;
    for(i = 0; i < n; i+=25)
    {
        printf("Millisecond %i: ", i);
        if (analog(tophat2) < threshold && analog(tophat1) >= threshold)
        {
            //turn right
            mav(motor1, -1000);
            mav(motor2, 500);
            //printf("scenario /////////////////\n");

        }
        else if (analog(tophat1) < threshold && analog(tophat2) >= threshold)
        {
            //turn left
            mav(motor1, -500);
            mav(motor2, 1000);
            //printf("scenario \\\\\\\\\\\\\\\\\n");
        }
        else if (analog(tophat2) >= threshold && analog(tophat1) >= threshold)
        {
            printf("LG, life's good\n");
            mav(motor1,-1300);
            mav(motor2,1300);

            if (direction == 'r')
            {
                mav(motor1, -1000);
                mav(motor2, 500);
                direction = 'l';
            }
            else if (direction == 'l'){
                mav(motor1, -500);
                mav(motor2, 1000);
                direction = 'r';
            }
        }
        //printf("%i, ", direction);
        msleep(25);
        if (digital(backsensor) == 1){
            alloff();
            return;
        }
    }
}
void lnsqr(char n)
{
    //motor 0 is right, motor 1 is left

    if (n == 'r')
    {
        mav(motor1, -500);
        mav(motor2, 1000);
    }
    else if (n == 'l')
    {
        mav(motor1, 500);
        mav(motor2, -1000);
    }
    else if (n == 'f')
    {
        mav(motor1, 1000);
        mav(motor2, -1000);
    }
    else if (n == 'b')
    {
        mav(motor1, -1000);
        mav(motor2, 1000);
    }
    else
    {
        return;
    }
    while(0 == 0)
    {
        if (analog(tophat1) >= threshold && analog(tophat2) >= threshold)
        {
            alloff();
            break;
        }
    }
}

void turn(char n,int modifier,int detectline)
{
    //motor 0 is right, motor 1 is left

    if (n == 'l')
    {
        int x = get_motor_position_counter(motor2);
        x += 1150+modifier;
        alloff();
        mav(motor2,1000);
        mav(motor1,1000);
        while (get_motor_position_counter(motor2) < x)
        {
            msleep(10);
            if (analog(tophat1) >= threshold && analog(tophat2) >= threshold && detectline == 1)
            {
                printf("QUIT");
                alloff();
                break;
            }

        } 
    }
    else if (n == 'r')
    {
        int x = get_motor_position_counter(motor1);
        x -= (1150+modifier);
        alloff();
        mav(motor2,-1000);
        mav(motor1,-1000);
        while (get_motor_position_counter(motor1) > x)
        {
            msleep(10);
            if (analog(tophat1) >= threshold && analog(tophat2) >= threshold && detectline == 1)
            {
                printf("QUIT");
                alloff();
                break;
            }

        } 
    }
    else
    {
        return;
    }
    alloff();
}
void rot(int n, int speed)
{
    //n = 1/1400 of a rotation
    int x = get_motor_position_counter(1);
    x += n;
    mall(speed);
    while (get_motor_position_counter(1) < x)
    {
        msleep(10);
    }
    mall(-speed);
    msleep(10);
    mall(0);
    return;
}
void reset(){
    slow_servo(1,1460,1,2);
}
void calibratecolor()
{
    int y = 0;
    int green = 0;
    int red = 0;
    int other = 0;
    int r, c, i;
    for (y = 0;y < 5;y ++)
    {
        green = 0;
        red = 0;
        other = 0;
        printf("here");
        camera_update();
        r = 0;
        c = 0;
        i = 0;
        const unsigned char* img = get_camera_frame();
        for(r = 40; r < 80; r++) 
        {
            for(c = 60; c < 100; c++) 
            {
                i = (160*r + c); // index of pixel to paint into row r, column c
                //printf("Pixel #%i: ", i);
                i = i * 3;
                if (img[i + 2] > (img[i + 1] + 10))
                {
                    //printf("r");
                    red += 1;
                }
                else if (img[i + 1] > (img[i + 2] + 10))
                {
                    //printf("g");
                    green += 1;
                }
                else
                {
                    //printf("o");
                    other += 1;
                }
            }
        }
    }
    //printf("\nRed %i Green %i Other %i",red,green,other);
    if (other != 0){
        greenpomthreshold =((float)green/other)*1.5;
        redpomthreshold = ((float)red/other)*1.5;
    }
    if (red != 0){
        redgreenpomthreshold = ((float)green/red)*1.25;
    }
    if (redgreenpomthreshold == 0.0){
        redgreenpomthreshold = 1.375;
    }
    if (greenpomthreshold == 0.0){
        greenpomthreshold = 1.5;
    }
    if (redpomthreshold == 0.0){
        redpomthreshold = 1.5;
    }
    return;
}

void waitforlight() {
    int previous = analog(3);
    int threshold = absol(25*(previous-analog(3)));
    previous = analog(3);
    msleep(333);
    threshold += absol(25*(previous-analog(3)));
    previous = analog(3);
    msleep(333);
    threshold += absol(25*(previous-analog(3)));
    printf("%i\n",threshold);
    threshold = threshold/3;
    printf("%i", threshold);
    while (1) {
        msleep(500);
        if ((previous-analog(3)) > threshold) {
            printf("Previous %i Analog %i",previous,analog(3));
            break;
        }
        previous = analog(3);
    }
}
