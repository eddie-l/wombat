#include <kipr/wombat.h>
char colour_with_a_u_sensor_for_pompoms();
void sort();
void linefollow(int n,int multiplier)
int absol(int num);
void slow_servo(int port, int maxmin,int speed, int interval);
void turn(char n);
void lnsqr (char n);
void path ();
void mall (int speed);
void rot(int n, int speed);
//Global Ports
int tophat1 = 0;
int tophat2 = 1;
int motor1 = 0;
int motor2 = 2;
int frontsensor = 0;
int backsensor = 2;
int threshold = 0;
int main()
{
    printf("%i", camera_open());
    enable_servos();
    slow_servo(1,1460,1,2);
    threshold = (((analog(tophat1)+analog(tophat1)+analog(tophat1))/2));
    printf("%i",threshold);
    path();
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
    printf("\nRed %i Green %i Other %i",red,green,other);
    if (other*2.5 > red && other*5 > green)
    {
        printf("O");
        return 'E';
    }
    else if (red < green)
    {
        printf("G");
        return 'g';
    }
    else if (red > green)
    {
        printf("R");
        return 'r';
    }
    else
    {
        return 'E';
    }
}
void sort()
{
    int green = 1300;
    int neutral = 950;
    int red = 600;
    int motorportformovingpompoms = 3;
    int servoport = 0;
    int servoport2 = 1;
    //int secondservoparameterlow = 300;
    //int secondservoparameterhi = 1460;
    mav(motorportformovingpompoms, 500);
    int j = 0;
    int counter = 0;
    printf("HERE");
    msleep(1000);
    for (j = 0;j < 100;j++){
    counter = 0;
    //return;
    char data = 'E';
    int i = 0;
    while (data == 'E'){
        for (i = 0; i < 2; i++){
        	set_servo_position(servoport2,1400+(1460-(get_servo_position(servoport2))));
        	msleep(175);
        	printf("%i",get_servo_position(servoport2));
    	}
    	for (i = 0; i < 5; i++){
        	data = colour_with_a_u_sensor_for_pompoms();
        if (counter == 10){
            set_servo_position(servoport,neutral);
        }
        counter = counter + 1;
    	}
    }
    printf("%c",data);
    printf("here");
    if (data == 'r')
    {
        printf("REDHEHE");
        set_servo_position(servoport, red);
        msleep(50);
    }
    else if (data == 'g')
    {
        printf("GREENHEHE");
        set_servo_position(servoport, green);
        msleep(50);
    }
    msleep(50);
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
    printf("turning left...\n");
    turn('l');
    lnsqr('f');
    mall(1500);
    msleep(750);
    printf("turning right...\n");
    turn('r');
    printf("squaring up... \n");
    lnsqr('f');
    mall(1000);
    msleep(1500);
    alloff();
    slow_servo(1,300,1,2);
    turn('r');
    mall(-1500);
    msleep(150);
    printf("following line...\n");
    linefollow(8000);
    mall(500);
    while (digital(frontsensor) == 0);
    {
        msleep(50);
    }
    alloff();
    slow_servo(1,800,1,2);
    mall(-500);
    msleep(1000);
    slow_servo(1,300,1,2);
    mall(500);
    while (digital(frontsensor) == 0);
    {
        msleep(50);
    }
    mall(-500);
    msleep(750);
    alloff();
    slow_servo(1,1460,1,2);
    turn('l');
    turn('l');
    lnsqr('b');
    mall(-1500);
    while (digital(backsensor) == 0)
    {
        msleep(50);
    }
    sort();
}


void mall(int speed)
{
    mav(motor1, speed);
    mav(motor2, -speed);
}
void linefollow(int n,int multiplier)
{
    //motor 0 is right, motor 1 is left

    mav(motor1, 1300*multiplier);
    mav(motor2, -1300*multiplier);
    int i;
    for(i = 0; i < n; i+=100)
    {
        printf("Millisecond %i: ", i);
        if (analog(tophat2) < threshold && analog(tophat1) >= threshold)
        {
            //turn right
            mav(motor1, 500*multiplier);
            mav(motor2, -1000*multiplier);
            //printf("scenario /////////////////\n");

        }
        else if (analog(tophat1) < threshold && analog(tophat2) >= threshold)
        {
            //turn left
            mav(motor1, -500*multiplier);
            mav(motor2, -1000*multiplier);
            //printf("scenario \\\\\\\\\\\\\\\\\n");
        }
        else if (analog(tophat2) >= threshold && analog(tophat1) >= threshold)
        {
            printf("LG, life's good\n");
                mall(1300*multiplier);
        }
        //printf("%i, ", direction);
        msleep(100);
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

void turn(char n)
{
    //motor 0 is right, motor 1 is left

    if (n == 'l')
    {
        int x = get_motor_position_counter(motor2);
    	x += 1150;
        alloff();
    	mav(motor2,1000);
        mav(motor1,1000);
    	while (get_motor_position_counter(motor2) < x)
    	{
        	msleep(10);
    	} 
    }
    else if (n == 'r')
    {
        int x = get_motor_position_counter(motor1);
        x -= 1150;
    	alloff();
        mav(motor2,-1000);
        mav(motor1,-1000);
    	while (get_motor_position_counter(motor1) > x)
    	{
        	msleep(10);
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
