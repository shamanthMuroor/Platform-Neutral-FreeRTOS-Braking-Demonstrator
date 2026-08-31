#include "SpeedEstimator.h"
#include "stdio.h"

int flWheelSpeed = 0;
int frWheelSpeed = 0;

int App_AverageFrontSpeed(void) {

    int avgFrontWheelSpeed = (flWheelSpeed + frWheelSpeed) / 2;

    printf("[Application] front-left speed = %d, front-right speed: %d\r\n", flWheelSpeed, frWheelSpeed);
    printf("[Application] Average front wheel speed = %d\r\n\n", avgFrontWheelSpeed);
	return avgFrontWheelSpeed;
}
